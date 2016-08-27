/*

Copy Right FourTech LLC, 2016
All Rights Are Reserved

The OS agnostic file tracking API for applications
that want to interact with potentially many files on
the disk that could be changed by other applications.

Created on: 20.07.2016

*/

// TOP

#include "4tech_file_track.h"

#include "4tech_file_track_general.c"

#include <Windows.h>

typedef struct {
    char result[2048];
    OVERLAPPED overlapped;
    HANDLE dir;
    int32_t user_count;
} Win32_Directory_Listener;

typedef struct {
    DLL_Node node;
    Win32_Directory_Listener listener;
} Win32_Directory_Listener_Node;

typedef struct {
    HANDLE iocp;
    CRITICAL_SECTION table_lock;
    void *tables;
    DLL_Node free_sentinel;
} Win32_File_Track_Vars;

typedef struct {
    File_Index hash;
    HANDLE dir;
    Win32_Directory_Listener_Node *listener_node;
} Win32_File_Track_Entry;

#define to_vars(s) ((Win32_File_Track_Vars*)(s))
#define to_tables(v) ((File_Track_Tables*)(v->tables))

File_Track_Result
init_track_system(File_Track_System *system,
                  void *table_memory, int32_t table_memory_size,
                  void *listener_memory, int32_t listener_memory_size){
    File_Track_Result result = FileTrack_MemoryTooSmall;
    Win32_File_Track_Vars *vars = to_vars(system);
    
    Assert(sizeof(Win32_File_Track_Entry) <= sizeof(File_Track_Entry));
    
    if (enough_memory_to_init_table(table_memory_size) &&
        sizeof(Win32_Directory_Listener_Node) <= listener_memory_size){
        
        // NOTE(allen): Initialize main data tables
        vars->tables = table_memory;
        File_Track_Tables *tables = to_tables(vars);
        init_table_memory(tables, table_memory_size);
        
        // NOTE(allen): Initialize nodes of directory watching
        {
            init_sentinel_node(&vars->free_sentinel);
            
            Win32_Directory_Listener_Node *listener = (Win32_Directory_Listener_Node*)listener_memory;
            int32_t count = listener_memory_size / sizeof(Win32_Directory_Listener_Node);
            for (int32_t i = 0; i < count; ++i, ++listener){
                insert_node(&vars->free_sentinel, &listener->node);
            }
        }
        
        // NOTE(allen): Prepare the file tracking synchronization objects.
        {
            InitializeCriticalSection(&vars->table_lock);
            vars->iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 1);
        }
        
        result = FileTrack_Good;
    }
    
    return(result);
}

static int32_t
internal_get_parent_name(char *out, int32_t max, char *name){
    int32_t len, slash_i;
    
    char *ptr = name;
    for (; *ptr != 0; ++ptr);
    len = (int32_t)(ptr - name);
    
    // TODO(allen): make this system real
    Assert(len < max);
    
    for (slash_i = len-1;
         slash_i > 0 && name[slash_i] != '\\' && name[slash_i] != '/';
         --slash_i);
    
    for (int32_t i = 0; i < slash_i; ++i){
        out[i] = name[i];
    }
    out[slash_i] = 0;
    
    return(slash_i);
}

static File_Index
internal_get_file_index(BY_HANDLE_FILE_INFORMATION info){
    File_Index hash;
    hash.id[0] = info.nFileIndexLow;
    hash.id[1] = info.nFileIndexHigh;
    hash.id[2] = info.dwVolumeSerialNumber;
    hash.id[3] = 0;
    return(hash);
}

File_Track_Result
add_listener(File_Track_System *system, char *filename){
    File_Track_Result result = FileTrack_Good;
    Win32_File_Track_Vars *vars = to_vars(system);
    
    EnterCriticalSection(&vars->table_lock);
    {
        File_Track_Tables *tables = to_tables(vars);
        
        // TODO(allen): make this real!
        char dir_name[1024];
        internal_get_parent_name(dir_name, sizeof(dir_name), filename);
        
        HANDLE dir = CreateFile(
            dir_name,
            FILE_LIST_DIRECTORY,
            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
            0,
            OPEN_EXISTING,
            FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
            0);
        
        if (dir != INVALID_HANDLE_VALUE){
            BY_HANDLE_FILE_INFORMATION dir_info = {0};
            DWORD getinfo_result = GetFileInformationByHandle(dir, &dir_info);
            
            if (getinfo_result){
                File_Index dir_hash = internal_get_file_index(dir_info);
                File_Track_Entry *dir_lookup = tracking_system_lookup_entry(tables, dir_hash);
                Win32_File_Track_Entry *win32_entry = (Win32_File_Track_Entry*)dir_lookup;
                
                if (entry_is_available(dir_lookup)){
                    if (tracking_system_has_space(tables, 1)){
                        Win32_Directory_Listener_Node *node = (Win32_Directory_Listener_Node*)
                            allocate_node(&vars->free_sentinel);
                        if (node){
                            if (CreateIoCompletionPort(dir, vars->iocp, (ULONG_PTR)node, 1)){
                                ZeroStruct(node->listener.overlapped);
                                if (ReadDirectoryChangesW(dir,
                                                          node->listener.result,
                                                          sizeof(node->listener.result),
                                                          0,
                                                          FILE_NOTIFY_CHANGE_LAST_WRITE,
                                                          0,
                                                          &node->listener.overlapped,
                                                          0)){
                                    node->listener.dir = dir;
                                    node->listener.user_count = 1;
                                    
                                    win32_entry->hash = dir_hash;
                                    win32_entry->dir = dir;
                                    win32_entry->listener_node = node;
                                    ++tables->tracked_count;
                                }
                                else{
                                    result = FileTrack_FileSystemError;
                                }
                            }
                            else{
                                result = FileTrack_FileSystemError;
                            }
                            
                            if (result != FileTrack_Good){
                                insert_node(&vars->free_sentinel, &node->node);
                            }
                        }
                        else{
                            result = FileTrack_OutOfListenerMemory;
                        }
                    }
                    else{
                        result = FileTrack_OutOfTableMemory;
                    }
                }
                else{
                    Win32_Directory_Listener_Node *node = win32_entry->listener_node;
                    ++node->listener.user_count;
                }
            }
            else{
                result = FileTrack_FileSystemError;
            }
        }
        else{
            result = FileTrack_FileSystemError;
        }
        
        if (result != FileTrack_Good && dir != 0 && dir != INVALID_HANDLE_VALUE){
            CloseHandle(dir);
        }
    }
    LeaveCriticalSection(&vars->table_lock);
    
    return(result);
}

File_Track_Result
remove_listener(File_Track_System *system, char *filename){
    File_Track_Result result = FileTrack_Good;
    Win32_File_Track_Vars *vars = to_vars(system);
    
    EnterCriticalSection(&vars->table_lock);
    
    {
        File_Track_Tables *tables = to_tables(vars);
        
        // TODO(allen): make this real!
        char dir_name[1024];
        internal_get_parent_name(dir_name, sizeof(dir_name), filename);
        
        HANDLE dir = CreateFile(
            dir_name,
            FILE_LIST_DIRECTORY,
            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
            0,
            OPEN_EXISTING,
            FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
            0);
        
        if (dir != INVALID_HANDLE_VALUE){
            BY_HANDLE_FILE_INFORMATION dir_info = {0};
            DWORD getinfo_result = GetFileInformationByHandle(dir, &dir_info);
            
            if (getinfo_result){
                File_Index dir_hash =  internal_get_file_index(dir_info);
                File_Track_Entry *dir_lookup = tracking_system_lookup_entry(tables, dir_hash);
                Win32_File_Track_Entry *win32_dir = (Win32_File_Track_Entry*)dir_lookup;
                
                Assert(!entry_is_available(dir_lookup));
                Win32_Directory_Listener_Node *node = win32_dir->listener_node;
                --node->listener.user_count;
                
                if (node->listener.user_count == 0){
                    insert_node(&vars->free_sentinel, &node->node);
                    CloseHandle(win32_dir->dir);
                    internal_free_slot(tables, dir_lookup);
                }
            }
            else{
                result = FileTrack_FileSystemError;
            }
            
            CloseHandle(dir);
        }
        else{
            result = FileTrack_FileSystemError;
        }
    }
    
    LeaveCriticalSection(&vars->table_lock);
    
    return(result);
}

File_Track_Result
move_track_system(File_Track_System *system, void *mem, int32_t size){
    File_Track_Result result = FileTrack_Good;
    Win32_File_Track_Vars *vars = to_vars(system);
    
    EnterCriticalSection(&vars->table_lock);
    {
        File_Track_Tables *original_tables = to_tables(vars);
        result = move_table_memory(original_tables, mem, size);
        if (result == FileTrack_Good){
            vars->tables = mem;
        }
    }
    LeaveCriticalSection(&vars->table_lock);
    
    return(result);
}

File_Track_Result
expand_track_system_listeners(File_Track_System *system, void *mem, int32_t size){
    File_Track_Result result = FileTrack_Good;
    Win32_File_Track_Vars *vars = to_vars(system);
    
    EnterCriticalSection(&vars->table_lock);
    
    if (sizeof(Win32_Directory_Listener_Node) <= size){
        Win32_Directory_Listener_Node *listener = (Win32_Directory_Listener_Node*)mem;
        int32_t count = size / sizeof(Win32_Directory_Listener_Node);
        for (int32_t i = 0; i < count; ++i, ++listener){
            insert_node(&vars->free_sentinel, &listener->node);
        }
    }
    else{
        result = FileTrack_MemoryTooSmall;
    }
    
    LeaveCriticalSection(&vars->table_lock);
    
    return(result);
}

File_Track_Result
get_change_event(File_Track_System *system, char *buffer, int32_t max, int32_t *size){
    File_Track_Result result = FileTrack_NoMoreEvents;
    Win32_File_Track_Vars *vars = to_vars(system);
    
    EnterCriticalSection(&vars->table_lock);
    
    {
        OVERLAPPED *overlapped = 0;
        DWORD length = 0;
        ULONG_PTR key = 0;
        
        if (GetQueuedCompletionStatus(vars->iocp,
                                      &length,
                                      &key,
                                      &overlapped,
                                      0)){
            
            Win32_Directory_Listener *listener_ptr = (Win32_Directory_Listener*)overlapped;
            
            // NOTE(allen): Get a copy of the state of this node so we can set the node
            // to work listening for changes again right away.
            Win32_Directory_Listener listener = *listener_ptr;
            
            ZeroStruct(listener_ptr->overlapped);
            ReadDirectoryChangesW(listener_ptr->dir,
                                  listener_ptr->result,
                                  sizeof(listener_ptr->result),
                                  0,
                                  FILE_NOTIFY_CHANGE_LAST_WRITE,
                                  0,
                                  &listener_ptr->overlapped,
                                  0);
            
            char *listener_buffer = listener.result;
            DWORD offset = 0;
            FILE_NOTIFY_INFORMATION *info = 0;
            
            for (;;){
                info = (FILE_NOTIFY_INFORMATION*)(listener_buffer + offset);
                
                int32_t len = info->FileNameLength / 2;
                int32_t dir_len = GetFinalPathNameByHandle(listener.dir, 0, 0,
                                                           FILE_NAME_NORMALIZED);
                int32_t req_size = dir_len + 1 + len;
                *size = req_size;
                if (req_size < max){
                    int32_t pos = 0;
                    
                    pos = GetFinalPathNameByHandle(listener.dir, buffer, max,
                                                   FILE_NAME_NORMALIZED);
                    buffer[pos++] = '\\';
                    
                    for (int32_t i = 0; i < len; ++i, ++pos){
                        buffer[pos] = (char)info->FileName[i];
                    }
                    
                    if (buffer[0] == '\\'){
                        for (int32_t i = 0; i+4 < pos; ++i){
                            buffer[i] = buffer[i+4];
                        }
                        *size -= 4;
                    }
                    
                    result = FileTrack_Good;
                }
                else{
                    // TODO(allen): Need some way to stash this result so that if the
                    // user comes back with more memory we can give them the change
                    // notification they missed.
                    result = FileTrack_MemoryTooSmall;
                }
                
                if (info->NextEntryOffset != 0){
                    // TODO(allen): We're not ready to handle this yet.
                    // For now I am breaking.  In the future, if there
                    // are more results we should stash them and return
                    // them in future calls.
                    offset += info->NextEntryOffset;
                    break;
                }
                else{
                    break;
                }
            }
        }
    }
    
    LeaveCriticalSection(&vars->table_lock);
    
    return(result);
}

File_Track_Result
shut_down_track_system(File_Track_System *system){
    File_Track_Result result = FileTrack_Good;
    Win32_File_Track_Vars *vars = to_vars(system);
    
    
    DWORD win32_result = 0;
    
    // NOTE(allen): Close all the handles stored in the table.
    {
        File_Track_Tables *tables = to_tables(vars);
        
        File_Track_Entry *entries = (File_Track_Entry*)to_ptr(tables, tables->file_table);
        uint32_t max = tables->max;
        
        for (uint32_t index = 0; index < max; ++index){
            File_Track_Entry *entry = entries + index;
            
            if (!entry_is_available(entry)){
                Win32_File_Track_Entry *win32_entry = (Win32_File_Track_Entry*)entry;
                if (!CloseHandle(win32_entry->dir)){
                    win32_result = 1;
                }
            }
        }
    }
    
    // NOTE(allen): Close all the global track system resources.
    {
        if (!CloseHandle(vars->iocp)){
            win32_result = 1;
        }
        DeleteCriticalSection(&vars->table_lock);
    }
    
    if (win32_result){
        result = FileTrack_FileSystemError;
    }
    
    return(result);
}

// BOTTOM