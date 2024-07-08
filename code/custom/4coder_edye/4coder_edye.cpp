//~ @f4_readme Ryan Fleury's 4coder Custom Layer
//
// Hello! Thank you for checking out my custom layer. This layer is a collection
// of things I've found to be useful, funny, or interesting overtime. It includes
// a few things that makes 4coder feel a bit more like an IDE, a few things that
// are just for kicks and are ridiculous, and a few interesting editor experiments.
// Below is a list of features, along with more information about them and some
// options you can tweak to your liking. To get as much customization as possible,
// you'd obviously have to dig a bit more into the code and probably write your
// own custom layer and just use stuff from this custom layer as a library, but
// the below options are just to allow easy customization of the low-hanging
// fruit.

//~ @f4_features 4coder Fleury Feature/Option List

//- @f4_lang_intro Multi-Language Support
//
// This custom layer has been tweaked to allow for better multi-language support
// in 4coder. It isn't perfect and will not support all languages, but it works
// for more than C/C++ (which historically 4coder has been fairly coupled to). 
//
// List of supported languages (as of 2021/01/29):
// - C
// - C++, partially... if you do a lot of modern C++ (I hope not), nothing
//   me or 4coder do can help you much (in fact, maybe nothing can help you)
// - Jai
// - Metadesk
//
// Adding a language requires a few steps.
//
// 1. Adding a lexer (probably the hardest part)
// 
// 4coder comes bundled with some lexer generation things. They are hard for me
// to understand, but all the languages I've had to deal with have been fairly
// easy to adapt from C++'s lexer. You can take a look at 4coder_cpp_lexer_gen.cpp,
// and copy/paste it into a new file that can generate the lexer for whatever
// language. This is what I did for the Jai lexer. Take a look at build_lexers.bat
// to see what I do to generate the Jai lexing code.
//
// Alternatively, you can hand-roll a lexer. This is a lot simpler (but of course
// gets harder with more complex languages). You can see an example of this in
// the Metadesk language implementation (4coder_fleury_lang_metadesk.cpp).
//
// 2. Implement language hooks
//
// The language system has each language expose a few hooks in v-table-like
// fashion. This allows general editor code to "ask questions" about specific
// language concepts. Here is a list of the hooks:
//
// - IndexFile hook, implement w/ F4_LANGUAGE_INDEXFILE(name) { /* code */ }
//   Used to scan tokens of a file and do heuristic parses to pull out language
//   info, like function/type definitions and stuff.
//
// - LexInit hook, implement w/ F4_LANGUAGE_LEXINIT(name) { /* code */ }
//   A hook that lets you initialize any state you want for your lexer.
//   This is also one of the functions that can be generated for you with
//   the language lexer generator that comes with 4coder. For C++, the name
//   of the function is lex_full_input_cpp_init, so take that and adjust as
//   needed for your language.
//
// - LexFullInput hook, implement w/ F4_LANGUAGE_LEXFULLINPUT(name) { /* code */ }
//   A hook to lex the whole input of a file that is determined to be in a given
//   language. This is also a hook that can be generated with 4coder's lexer
//   generator stuff. For C++, the name of the function is lex_full_input_breaks,
//   so take that and adjust as needed for your language.
//
// - PosContext hook, implement using F4_LANGUAGE_POSCONTEXT(name) { /* code */ }
//   This hook is called into by some rendering features looking for contextual
//   information, usually about the cursor position. For example, figuring out
//   if the cursor is inside some arguments for a function, so a tooltip can be
//   displayed. You can just return 0 if you don't want to deal with this.
//
// - Highlight hook, implement using F4_LANGUAGE_HIGHLIGHT(name) { /* code */ }
//   This hook does any additional syntax highlighting that you want for the
//   language. You don't need to do anything here if you don't want to, in fact
//   you won't need this most of the time probably.
//
// 3. Add the info for your language to the 4coder_fleury_lang_list.h file.
// More information is in that file.

//- @f4_colors_intro Syntax Highlighting and Color Additions
//
// 4coder provides very basic syntax highlighting, but this layer implements
// some more features, particularly ones that rely on the code index. There
// are custom colors that this layer looks for from a theme file so that it
// can use good colors for these. Additionally, many other features (e.g.
// various rendering features that are mentioned elsewhere in here) also need
// custom colors. Here is a list of the colors this layer looks for:
//
// - fleury_color_syntax_crap: Used to change the color of textual syntax stuff
//   that is not really semantically important, but we have to type it anyways,
//   stuff like ;'s and parentheses and stuff.
//
// - fleury_color_operators: The color of operators.
//
// - fleury_color_inactive_pane_overlay: The color of a panel-sized rectangle
//   drawn over panels that are not active, to leave the active panel slightly
//   brighter.
//
// - fleury_color_inactive_pane_background: The color of a panel-sized rectangle
//   drawn *behind* panels that are not active, to leave the active panel slightly
//   brighter.
//
// - fleury_color_file_progress_bar: The color to render the progress bar, drawn
//   over the file bar in a panel.
//
// - fleury_color_brace_highlight: The color used for the brace highlight feature.
//
// - fleury_color_brace_line: The color used for the brace line feature.
//
// - fleury_color_brace_annotation: The color used for the brace annotation
//   feature.
//
// - fleury_color_index_sum_type: The color used for names of sum-types
//   (unions).
//
// - fleury_color_index_product_type: The color used for names of product-
//   types (structs, classes, whatever).
//
// - fleury_color_index_function: The color used for names of functions.
//
// - fleury_color_index_macro: The color used for names of macros.
//
// - fleury_color_index_constant: The color used for names of constants
//   (including enum values).
//
// - fleury_color_index_comment_tag: The color used for comment tags,
//   like this! @example_comment_tag
//
// - fleury_color_cursor_macro: The color used for the cursor when a macro
//   is being recorded.
//
// - fleury_color_cursor_power_mode: The color used for the cursor when
//   Power Mode is activated.
//
// - fleury_color_plot_cycle: The set of colors used for colors in plots.
//
// - fleury_color_token_highlight: The color used to underline/highlight
//   tokens that the cursor is over.
//
// - fleury_color_token_minor_highlight: The Color That is used for minor
//   highlights. i.e. cursor token occurance underlines.
//
// - fleury_color_comment_user_name: The color used to highlight the
//   username in comments.
// 
// - fleury_color_error_annotation: Text color used for the inline error
//   message string with F4_RenderErrorAnnotations


//- @f4_render_intro Rendering Features
//
// Many of the features in this layer are just rendering tweaks/improvements to
// 4coder's defaults. Below is a list of features and some options for them.
//
// - Brace Highlighting: When the cursor is inside of a pair of {}'s, highlight 
//   the brace symbols. Turn it off with "f4_disable_brace_highlight" in your
//   config file.
//
// - Close-Brace Annotations: When the cursor is inside a pair of {}'s, BUT
//   you cannot see the top of the scope, render a little annotation to the
//   right of the }, showing the code that was before the {. Turn it off with
//   "f4_disable_close_brace_annotation" in your config file.
//
// - Brace Lines: In many editors, they will draw a vertical line connecting
//   { and }'s in a file. This feature does that when the cursor inside the
//   scope. Turn it off with "f4_disable_brace_lines" in your config file.
//
// - Progress Bar: Colors the file bar (at the top of a 4coder panel) with
//   a "progress bar", showing you how far your cursor is in a file. Turn it
//   off with "f4_disable_progress_bar" in your config file.
//
// - Divider Comments: Any single-line comments starting with a ~ character
//   (e.g. //~) will render a horizontal line over them, acting as a divider
//   (used as an alternative to gigantic lines of -------'s or whatever).
//   Turn it off with "f4_disable_divider_comments" in your config file.
//
// - Error Annotations: When an error is parsed from compilation output in the
//   *compilation* window, it will display what that error is to the right of
//   the line. Turn it off with "f4_disable_error_annotations" in your config
//   file.
//
// - Cursor Identifier Highlight: The Identifier under the cursor is highlighted
//   with an underline (using the fleury_color_token_highlight color). It also
//   highlights all other occurances of the identifier (by string) that is
//   visible and syntax highlighted (occurances are highlighted using 
//   "fleury_color_token_minor_highlight". Turn the highlight of other occurances
//   off with "f4_disable_cursor_token_occurance" in your config file.

//- @f4_calc_intro Built-In Calculator/Plotting
//
// This custom layer includes a tiny little calculator language, which has
// plotting capabilities. It has a few features that comprise it all:
//
// - Calculator Comments: Any comments starting with a 'c', e.g. //c or /*c,
//   will be interpreted as calculator code, including inline plots. This
//   can be useful to do quick calculations, see a math function represented
//   visually, or plot data. You can check out examples/demos in the
//   4coder_fleury_plots_demo.cpp file. Turn it off with "f4_disable_calc_comments"
//   in your config file.
//
// - *calc* Buffer: On startup, a *calc* buffer is opened that you can switch
//   to. The entire buffer is interpreted as calculator code, and it will
//   do all the same things that calc comments do.

//- @f4_pos_context_intro Language Position-Context Tooltips
//
// This feature shows tooltips relating to the current cursor position, as it
// adheres to the programming language of the buffer in question. For example,
// if a buffer's language has implemented the PosContext hook, then when the
// cursor is inside a function call, it will show a tooltip of the arguments,
// and which one the cursor is over. Same thing with struct listers.
//
// Normally, this renders at the cursor location, but you can make it render
// in a perhaps less-distracting location (at the bottom of the buffer) with
// "f4_poscontext_draw_at_bottom_of_buffer" in your config file.
//
// Alternatively, you can turn the whole thing off with "f4_disable_poscontext"
// in your config file.

//- @f4_commands
//
// There are a lot of commands you can bind in your bindings.4coder file that
// have been helpful for me and others. Here is a list of the most useful ones.
// If you want to check out the code, go look at 4coder_fleury_base_commands.cpp.
//
// - f4_search: Will search forward in a buffer. When in notepad-like mode, will
//   use the current selection as the search query.
//
// - f4_reverse_search: Same thing as f4_search, just goes backwards.
//
// - f4_write_text_input: Use this instead of the default 4coder write_text_input
//   if you want Power Mode features.
//
// - f4_write_text_and_auto_indent: Same thing as above. Use over the default 4coder
//   equivalent if you want Power Mode features.
//
// - f4_write_zero_struct: 4coder provides a helper like this already, which writes
//   a C++-style '= {};' initializer, but this doesn't work for C (which uses '= {0}'),
//   yeah I know why the fuck don't we have a concise way to zero-initialize things
//   in both languages :) Anyways, bind this command to whatever you want if you want
//   to use it.
//
// - f4_home: 4coder obviously provides a default binding for the Home key, but it
//   doesn't move the view back over to the left margin, and that bothers me, so
//   I wrote a command that does exactly that.
//
// - f4_toggle_battery_saver: Some of the rendering features in this custom layer
//   include some animations, because I like animations. This will disable them,
//   so your laptop doesn't get hot. Set its default value with "f4_battery_saver"
//   in your config file.
//
// - f4_disable_cursor_trails: Turns off cursor animation.
//
// - f4_toggle_compilation_expand: Expands/shrinks the *compilation* panel at the
//   bottom of the screen. Bind this to a key that you want to press when you are
//   checking out the output of a build, and be sure to send your build commands
//   here!
//
// - f4_go_to_definition: Uses the identifier that the cursor is currently over
//   as a lookup into the code index. Will open its definition, if it finds one,
//   in the panel that the cursor is not in.
//
// - f4_go_to_definition_same_panel: Same as above except does it in the same
//   panel.
//
// - f4_search_for_definition__project_wide: Opens a lister with all definitions
//   from all the code that is open. You can search by typing into it, and then
//   navigating to the right item, and hitting enter. It will then take you to
//   the location of that definition.
//
// - f4_search_for_definition__current_file: Same as above, except restricts the
//   search to definitions in the file that your cursor is in.
//
// - f4_toggle_enclosure_side: When inside a scope or parentheses, moves the
//   cursor between the starting/ending position of that scope/parentheses.
//
// - f4_open_project: Opens a project file. Navigate to a project file on disk,
//   and when you select it, it will be loaded.
//
// - f4_setup_new_project: Makes a new project file at the location that you
//   specify.
//
// - f4_move_left: Move the cursor left. I have my own to correct for some
//   behavior issues with notepad-like mode. I've tried to converge to VS-like
//   behavior as much as possible with notepad-like mode, so use this if that's
//   what you want.
//
// - f4_move_right: Same as above, except right.
//
// - f4_move_right_token_boundary: Implements the equivalent of your VS-style
//   Ctrl+Right.
//
// - f4_move_left_token_boundary: Implements the equivalent of your VS-style
//   Ctrl+Left.
//
// - f4_backspace_token_boundary: Implements the equivalent of your VS-style
//   Ctrl+Backspace.
//
// - f4_delete_token_boundary: Implements the equivalent of your VS-style
//   Ctrl+Delete.
//
// - f4_backspace_alpha_numeric_or_camel_boundary: Does backspace, but restricts
//   it to camel-case and alphanumeric boundaries. Kind of like Ctrl+Backspace
//   except with different groupings.
//
// - f4_delete_alpha_numeric_or_camel_boundary: Same as above, just does it forward.
//
// - f4_home_first_non_whitespace: Implements VS-style Home key, where it first
//   goes to the first non-whitespace character on the line, then all the way to
//   the left margin, then back again. Bind this to Home if you want VS-like
//   behavior.
//
// - f4_autocomplete_or_indent: Autocompletes an incomplete word at the cursor, or
//   indents the line.
//
// - f4_unindent: Similar to Shift+Tab in VS.
//
// - f4_comment_selection: VS-style selection commenting.
//
// - f4_uncomment_selection: VS-style uncommenting.

//~ TODO FOR SHAWN
//
// [ ] Project File Opener: Ignore directories, recursively list options.
// [X] Ctrl+Arrows: It's still weird... it skips punctuation weridly.
// [ ] Recent Files Menu: this isn't bad!  If it just defaulted to the first one
//     (but still allowed typing), and if the key that opened it continued to cycle
//     the items in the list (rather than closing the lister), that'd be pretty much
//     it!
// [ ] Tabbing: This is really close, the only problem is if the end of the selection
//     isn't at the beginning of a line it does something really weird -- I'm not sure 
//     what.  It shouldn't matter where the caret is, if multiple lines are selected
//     then tab should move the lines
// [ ] Comments: Very close!  The only problem is the multiple selection, single line 
//     comments: instead of putting the // at the start of each line after the whitespace, 
//     the // should be lined up with the shallowest whitespace..
// [ ] Pressing tab on an empty line inside a scope still doesn't do anything.
// [X] If the caret is at the start of the line and you press home, it doesn't take you to 
//     the first non-whitespace character... basically pressing home more than once should 
//     cycle between the first non-whitespace character and column 0
// [X] Token-like movement inside of comments.
// [ ] ctrl E centres the buffer on the current line,  vim has z. There's:
//     zz -> middle, zt-> top, zb->bottom, but also: z5t will centre it 5
//     lines from the top or whatever... is that something that's easy to add? 
//
// ORIGINAL REQUEST:
// [ ] Fix not showing .-lister when cursor is at end of query
// [ ] Cycle overloads, or list them all
// [ ] Ctrl+I but just for the project buffers
// [ ] Vertical edits
//
// DONE:
//

//~ TODO FOR CASEY
//
// [X] Active pane frame color/thickness control in Fleury config options, to allow turning it
//     off entirely or changing the color
//     done: f4_margin_size and f4_margin_use_mode_color
// [X] Inactive pane color cursor differences
// [X] Filenames to disambiguate indexed stuff with the same name
// [X] Way to get to forward decl for an indexed thing too
//     done: use go-to-definition again, at the definition site, and it should cycle between
//     all the definitions
// [ ] Fix clipboard stuff
// [ ] CPM counter shenanigans

//~ TODO PLOTS
// [ ] Un-Bust Histograms
// [ ] Fix plot clip rect bugs when the plot is not 100% in the visible range
//     (caused by 4coder laying out characters off the screen as being at 0, 0)
// [ ] Labels for histogram bins
// [ ] Plots and Calc are so coupled and I hate it, please rewrite or something

//~ TODO META
// [X] Project-wide todo list (by searching for TODO and DONE comments, lister for toggling)
// [ ] Project switcher
// [ ] Plan + do modal input scheme... Identifier-mode, text-mode, semantics mode, search mode...?

//~ NOTE(rjf): For DION team docs server stuff.
// {
#if OS_WINDOWS
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <windows.h>
typedef int socklen_t;
#pragma comment(lib, "Ws2_32.lib")
#endif
// }

//~ NOTE(rjf): Macros and pragmase stuff that have to be put here for various
// reasons
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "4coder_default_include.cpp"
#pragma warning(disable : 4706)
#pragma warning(disable : 4456)
#define COMMAND_SERVER_PORT 4041
#define COMMAND_SERVER_UPDATE_PERIOD_MS 200
#define COMMAND_SERVER_AUTO_LAUNCH_IF_FILE_PRESENT "project_namespaces.txt"

//~ NOTE(rjf): @f4_headers
#include "../4coder_fleury/4coder_fleury_ubiquitous.h"
#include "../4coder_fleury/4coder_fleury_audio.h"
#include "../4coder_fleury/4coder_fleury_lang.h"
#include "../4coder_fleury/4coder_fleury_index.h"
#include "../4coder_fleury/4coder_fleury_colors.h"
#include "../4coder_fleury/4coder_fleury_render_helpers.h"
#include "../4coder_fleury/4coder_fleury_brace.h"
#include "../4coder_fleury/4coder_fleury_error_annotations.h"
#include "../4coder_fleury/4coder_fleury_divider_comments.h"
#include "../4coder_fleury/4coder_fleury_power_mode.h"
#include "../4coder_fleury/4coder_fleury_cursor.h"
#include "../4coder_fleury/4coder_fleury_plot.h"
#include "../4coder_fleury/4coder_fleury_calc.h"
#include "../4coder_fleury/4coder_fleury_lego.h"
#include "../4coder_fleury/4coder_fleury_pos_context_tooltips.h"
#include "../4coder_fleury/4coder_fleury_code_peek.h"
#include "../4coder_fleury/4coder_fleury_recent_files.h"
// #include "../4coder_fleury/4coder_fleury_bindings.h" // NOTE(edye): im replacing these functions here
#include "../4coder_fleury/4coder_fleury_base_commands.h"
#if OS_WINDOWS
#include "../4coder_fleury/4coder_fleury_command_server.h"
#endif
#include "../4coder_fleury/4coder_fleury_hooks.h"

CUSTOM_ID(colors, defcolor_function);
CUSTOM_ID(colors, defcolor_type);
CUSTOM_ID(colors, defcolor_primitive);
CUSTOM_ID(colors, defcolor_macro);
CUSTOM_ID(colors, defcolor_control);
CUSTOM_ID(colors, defcolor_struct);
CUSTOM_ID(colors, defcolor_non_text);

// TODO(edye): experimenting with vim_lister
//#include "../4coder_byp/4coder_vim/4coder_vim_lister.cpp"
//#include "../4coder_byp/4coder_vim/4coder_vim_lists.cpp"
#include "../4coder_byp/4coder_vimrc.h"
#include "../4coder_byp/4coder_vim/4coder_vim_include.h"

#include "../4coder_byp/4coder_byp_helper.h"

#include "../4coder_byp/4coder_byp_token.h"
#include "../4coder_byp/4coder_byp_token.cpp"

#include "../4coder_byp/4coder_byp_build.cpp"
#include "../4coder_byp/4coder_byp_colors.cpp"
#include "../4coder_byp/4coder_byp_commands.cpp"

#include "../4coder_byp/4coder_byp_draw.cpp"
#include "../4coder_byp/4coder_byp_game.cpp"

#include "../4coder_byp/4coder_byp_bindings.cpp"
#include "../4coder_byp/4coder_byp_hooks.cpp"


//~ NOTE(rjf): @f4_src
#include "../4coder_fleury/4coder_fleury_ubiquitous.cpp"
#include "../4coder_fleury/4coder_fleury_audio.cpp"
#include "../4coder_fleury/4coder_fleury_lang.cpp"
#include "../4coder_fleury/4coder_fleury_index.cpp"
#include "../4coder_fleury/4coder_fleury_colors.cpp"
#include "../4coder_fleury/4coder_fleury_render_helpers.cpp"
#include "../4coder_fleury/4coder_fleury_brace.cpp"
#include "../4coder_fleury/4coder_fleury_error_annotations.cpp"
#include "../4coder_fleury/4coder_fleury_divider_comments.cpp"
#include "../4coder_fleury/4coder_fleury_power_mode.cpp"
#include "../4coder_fleury/4coder_fleury_cursor.cpp"
#include "../4coder_fleury/4coder_fleury_plot.cpp"
#include "../4coder_fleury/4coder_fleury_calc.cpp"
#include "../4coder_fleury/4coder_fleury_lego.cpp"
#include "../4coder_fleury/4coder_fleury_pos_context_tooltips.cpp"
#include "../4coder_fleury/4coder_fleury_code_peek.cpp"
#include "../4coder_fleury/4coder_fleury_recent_files.cpp"
// #include "../4coder_fleury/4coder_fleury_bindings.cpp" // NOTE(edye): im replacing these functions here
#include "../4coder_fleury/4coder_fleury_base_commands.cpp"
#if OS_WINDOWS
#include "../4coder_fleury/4coder_fleury_command_server.cpp"
#endif
#include "../4coder_fleury/4coder_fleury_casey.cpp"
#include "../4coder_fleury/4coder_fleury_hooks.cpp"

//~ NOTE(rjf): Plots Demo File
#include "../4coder_fleury/4coder_fleury_plots_demo.cpp"

//~ NOTE(rjf): 4coder Stuff
#include "generated/managed_id_metadata.cpp"


struct Command_Map_ID_Pair
{
	Command_Map_ID From;
	Command_Map_ID To;
};
static Command_Map_ID_Pair GlobalCommandMapReroute[4] = {};

CUSTOM_COMMAND_SIG(switch_to_keybinding_0)
CUSTOM_DOC("Switch the keybindings to mode 0.")
{
	GlobalKeybindingMode = KeyBindingMode_0;
}

CUSTOM_COMMAND_SIG(switch_to_keybinding_1)
CUSTOM_DOC("Switch the keybindings to mode 1.")
{
	GlobalKeybindingMode = KeyBindingMode_1;
}

CUSTOM_COMMAND_SIG(switch_to_keybinding_2)
CUSTOM_DOC("Switch the keybindings to mode 2.")
{
	GlobalKeybindingMode = KeyBindingMode_2;
}

CUSTOM_COMMAND_SIG(switch_to_keybinding_3)
CUSTOM_DOC("Switch the keybindings to mode 3.")
{
	GlobalKeybindingMode = KeyBindingMode_3;
}

function Implicit_Map_Result
F4_ImplicitMap(Application_Links *app, String_ID lang, String_ID mode, Input_Event *event)
{
    Implicit_Map_Result result = {};
    
    View_ID view = get_this_ctx_view(app, Access_Always);
    
	Command_Map_ID orig_id = default_get_map_id(app, view);
    Command_Map_ID map_id = orig_id;
	if(GlobalKeybindingMode == KeyBindingMode_1)
	{
		for(int PairIndex = 0;
			PairIndex < ArrayCount(GlobalCommandMapReroute);
			++PairIndex)
		{
			if(GlobalCommandMapReroute[PairIndex].From == map_id)
			{
				map_id = GlobalCommandMapReroute[PairIndex].To;
				break;
			}
		}
	}
    
	Command_Binding binding = map_get_binding_recursive(&framework_mapping, map_id, event);
	if(!binding.custom)
	{
		binding = map_get_binding_recursive(&framework_mapping, orig_id, event);
	}
    
    // TODO(allen): map_id <-> map name?
    result.map = 0;
    result.command = binding.custom;
    
    return(result);
}

function b32
combine_line_inner(Application_Links *app, View_ID view, Buffer_ID buffer, i64 line_num){
	if(!is_valid_line(app, buffer, line_num+1)){ return true; }
	i64 pos = get_line_end_pos(app, buffer, line_num);
	Range_i64 range = {};
	range.min = pos;
    
	i64 new_pos = pos + 1;
	String_Const_u8 delimiter = string_u8_litexpr(" ");
	if(!line_is_valid_and_blank(app, buffer, line_num+1)){
		if(character_is_whitespace(buffer_get_char(app, buffer, new_pos))){
			new_pos = buffer_seek_character_class_change_1_0(app, buffer, &character_predicate_whitespace, Scan_Forward, new_pos);
		}
	}else{
		new_pos = get_line_end_pos(app, buffer, line_num+1);
		delimiter.size = 0;
	}
	i64 end_pos = get_line_side_pos_from_pos(app, buffer, pos, Side_Max);
	view_set_cursor_and_preferred_x(app, view, seek_pos(end_pos));
	move_right(app);
    
	range.max = new_pos;
    
	buffer_replace_range(app, buffer, range, delimiter);
    
	return false;
}

CUSTOM_COMMAND_SIG(combine_line)
CUSTOM_DOC("Combine/join lines togther like in vim.")
{
    View_ID view = get_active_view(app, Access_ReadWriteVisible);
	Buffer_ID buffer = view_get_buffer(app, view, Access_ReadWriteVisible);
	if(buffer == 0){ return; }
	i64 pos = view_get_cursor_pos(app, view);
	i64 line = buffer_compute_cursor(app, buffer, seek_pos(pos)).line;
    
    Range_i64 range = get_view_range(app, view);
    i64 line_min = get_line_number_from_pos(app, buffer, range.min);
    i64 line_max = get_line_number_from_pos(app, buffer, range.max);
    i32 N = Max(1, i32(line_max-line_min));
    view_set_cursor_and_preferred_x(app, view, seek_pos(range.min));
    view_set_mark(app, view, seek_pos(range.max));
    line = line_min;
    
	History_Group history_group = history_group_begin(app, buffer);
	for(i32 i=0; i < N; i++){
		if(combine_line_inner(app, view, buffer, line)){
			break;
		}
	}
	if(N > 1){ history_group_end(history_group); }
}

#define foreach(i,N) for(i32 i=0; i<N; i++)

// NOTE: byp_hex_color_preview
function void
hex_color_preview(Application_Links *app, Buffer_ID buffer_id, Text_Layout_ID text_layout_id){
	Scratch_Block scratch(app);
	Range_i64 visible_range = text_layout_get_visible_range(app, text_layout_id);
	u8 *text = push_buffer_range(app, scratch, buffer_id, visible_range).str;
	foreach(i, range_size(visible_range)-9){
		byp_hex_scan:
		if(text[i] != '0' || text[i+1] != 'x'){ continue; }
		foreach(j, 8){
			char c = text[i+2+j];
			if(!('0' <= c && c <= '9' || 'a' <= c && c <= 'f' || 'A' <= c && c <= 'F')){
				i+=9; goto byp_hex_scan;
			}
		}
		Range_i64 hex_range = Ii64_size(i+visible_range.min, 10);
		u32 c = u32(string_to_integer(SCu8(text+i+2, 8), 16));
		f32 avg = (((c >> 16) & 0xFF) + ((c >> 8) & 0xFF) + (c & 0xFF))/3.f;
        
		u32 contrast = 0xFF000000 | (i32(avg > 110.f)-1);
		paint_text_color(app, text_layout_id, hex_range, contrast);
		Rect_f32 rect_left  = text_layout_character_on_screen(app, text_layout_id, hex_range.min);
		Rect_f32 rect_right = text_layout_character_on_screen(app, text_layout_id, hex_range.max-1);
		Rect_f32 rect = rect_union(rect_left, rect_right);
		rect = rect_inner(rect, -1.f);
		draw_rectangle(app, rect, 8.f, c);
		draw_rectangle_outline(app, rect_inner(rect, -2.f), 8.f, 2.1f, contrast);
	}
}

// based on F4_RenderBuffer
function void
edye_render_buffer(Application_Links *app, View_ID view_id, Face_ID face_id,
                Buffer_ID buffer, Text_Layout_ID text_layout_id,
                Rect_f32 rect, Frame_Info frame_info)
{
    Scratch_Block scratch(app);
    ProfileScope(app, "[Fleury] Render Buffer");
    
    View_ID active_view = get_active_view(app, Access_Always);
    b32 is_active_view = (active_view == view_id);
    Rect_f32 prev_clip = draw_set_clip(app, rect);
    
    // NOTE(allen): Token colorizing
    Token_Array token_array = get_token_array_from_buffer(app, buffer);
    if(token_array.tokens != 0)
    {
        F4_SyntaxHighlight(app, text_layout_id, &token_array);
        
        // NOTE(allen): Scan for TODOs and NOTEs
        b32 use_comment_keywords = def_get_config_b32(vars_save_string_lit("use_comment_keywords"));
        if(use_comment_keywords)
        {
            Comment_Highlight_Pair pairs[] =
            {
                {str8_lit("NOTE"), finalize_color(defcolor_comment_pop, 0)},
                {str8_lit("TODO"), finalize_color(defcolor_comment_pop, 1)},
                {def_get_config_string(scratch, vars_save_string_lit("user_name")), finalize_color(fleury_color_comment_user_name, 0)},
            };
            draw_comment_highlights(app, buffer, text_layout_id,
                                    &token_array, pairs, ArrayCount(pairs));
        }
    }
    else
    {
        Range_i64 visible_range = text_layout_get_visible_range(app, text_layout_id);
        paint_text_color_fcolor(app, text_layout_id, visible_range, fcolor_id(defcolor_text_default));
    }
    
    i64 cursor_pos = view_correct_cursor(app, view_id);
    view_correct_mark(app, view_id);
    
    // NOTE(allen): Scope highlight
    b32 use_scope_highlight = def_get_config_b32(vars_save_string_lit("use_scope_highlight"));
    if (use_scope_highlight){
        Color_Array colors = finalize_color_array(defcolor_back_cycle);
        draw_scope_highlight(app, buffer, text_layout_id, cursor_pos, colors.vals, colors.count);
    }
    
    // NOTE(rjf): Brace highlight
    {
        Color_Array colors = finalize_color_array(fleury_color_brace_highlight);
        if(colors.count >= 1 && F4_ARGBIsValid(colors.vals[0]))
        {
            F4_Brace_RenderHighlight(app, buffer, text_layout_id, cursor_pos,
                                     colors.vals, colors.count);
        }
    }
    
    // NOTE(allen): Line highlight
    {
        b32 highlight_line_at_cursor = def_get_config_b32(vars_save_string_lit("highlight_line_at_cursor"));
        String_Const_u8 name = string_u8_litexpr("*compilation*");
        Buffer_ID compilation_buffer = get_buffer_by_name(app, name, Access_Always);
        if(highlight_line_at_cursor && (is_active_view || buffer == compilation_buffer))
        {
            i64 line_number = get_line_number_from_pos(app, buffer, cursor_pos);
            draw_line_highlight(app, text_layout_id, line_number,
                                fcolor_id(defcolor_highlight_cursor_line));
        }
    }
    
    // NOTE(rjf): Error/Search Highlight
    {
        b32 use_error_highlight = def_get_config_b32(vars_save_string_lit("use_error_highlight"));
        b32 use_jump_highlight = def_get_config_b32(vars_save_string_lit("use_jump_highlight"));
        if (use_error_highlight || use_jump_highlight){
            // NOTE(allen): Error highlight
            String_Const_u8 name = string_u8_litexpr("*compilation*");
            Buffer_ID compilation_buffer = get_buffer_by_name(app, name, Access_Always);
            if (use_error_highlight){
                draw_jump_highlights(app, buffer, text_layout_id, compilation_buffer,
                                     fcolor_id(defcolor_highlight_junk));
            }
            
            // NOTE(allen): Search highlight
            if (use_jump_highlight){
                Buffer_ID jump_buffer = get_locked_jump_buffer(app);
                if (jump_buffer != compilation_buffer){
                    draw_jump_highlights(app, buffer, text_layout_id, jump_buffer,
                                         fcolor_id(defcolor_highlight_white));
                }
            }
        }
    }
    
    // NOTE(rjf): Error annotations
    {
        String_Const_u8 name = string_u8_litexpr("*compilation*");
        Buffer_ID compilation_buffer = get_buffer_by_name(app, name, Access_Always);
        F4_RenderErrorAnnotations(app, buffer, text_layout_id, compilation_buffer);
    }
    
    // NOTE(jack): Token Occurance Highlight
    if (!def_get_config_b32(vars_save_string_lit("f4_disable_cursor_token_occurance"))) 
    {
        ProfileScope(app, "[Fleury] Token Occurance Highlight");
        
        // NOTE(jack): Get the active cursor's token string
        Buffer_ID active_cursor_buffer = view_get_buffer(app, active_view, Access_Always);
        i64 active_cursor_pos = view_get_cursor_pos(app, active_view);
        Token_Array active_cursor_buffer_tokens = get_token_array_from_buffer(app, active_cursor_buffer);
        Token_Iterator_Array active_cursor_it = token_iterator_pos(0, &active_cursor_buffer_tokens, active_cursor_pos);
        Token *active_cursor_token = token_it_read(&active_cursor_it);
        
        String_Const_u8 active_cursor_string = string_u8_litexpr("");
        if(active_cursor_token)
        {
            active_cursor_string = push_buffer_range(app, scratch, active_cursor_buffer, Ii64(active_cursor_token));
            
            // Loop the visible tokens
            Range_i64 visible_range = text_layout_get_visible_range(app, text_layout_id);
            i64 first_index = token_index_from_pos(&token_array, visible_range.first);
            Token_Iterator_Array it = token_iterator_index(0, &token_array, first_index);
            for (;;)
            {
                Token *token = token_it_read(&it);
                if(!token || token->pos >= visible_range.one_past_last)
                {
                    break;
                }
                
                if (token->kind == TokenBaseKind_Identifier)
                {
                    Range_i64 token_range = Ii64(token);
                    String_Const_u8 token_string = push_buffer_range(app, scratch, buffer, token_range);
                    
                    // NOTE(jack) If this is the buffers cursor token, highlight it with an Underline
                    if (range_contains(token_range, view_get_cursor_pos(app, view_id)))
                    {
                        F4_RenderRangeHighlight(app, view_id, text_layout_id,
                                                token_range, F4_RangeHighlightKind_Underline,
                                                fcolor_resolve(fcolor_id(fleury_color_token_highlight)));
                    }
                    // NOTE(jack): If the token matches the active buffer token. highlight it with a Minor Underline
                    else if(active_cursor_token->kind == TokenBaseKind_Identifier && 
                            string_match(token_string, active_cursor_string))
                    {
                        F4_RenderRangeHighlight(app, view_id, text_layout_id,
                                                token_range, F4_RangeHighlightKind_MinorUnderline,
                                                fcolor_resolve(fcolor_id(fleury_color_token_minor_highlight)));
                        
                    } 
                }
                
                if(!token_it_inc_non_whitespace(&it))
                {
                    break;
                }
            }
        }
    }
    // NOTE(jack): if "f4_disable_cursor_token_occurance" is set, just highlight the cusror 
    else
    {
        ProfileScope(app, "[Fleury] Token Highlight");
        
        Token_Iterator_Array it = token_iterator_pos(0, &token_array, cursor_pos);
        Token *token = token_it_read(&it);
        if(token && token->kind == TokenBaseKind_Identifier)
        {
            F4_RenderRangeHighlight(app, view_id, text_layout_id,
                                    Ii64(token->pos, token->pos + token->size),
                                    F4_RangeHighlightKind_Underline,
                                    fcolor_resolve(fcolor_id(fleury_color_token_highlight)));
        }
    }
    
    // NOTE(rjf): Flashes
    {
        F4_RenderFlashes(app, view_id, text_layout_id);
    }
    
    hex_color_preview(app, buffer, text_layout_id);
    
    // NOTE(allen): Color parens
    if(def_get_config_b32(vars_save_string_lit("use_paren_helper")))
    {
        Color_Array colors = finalize_color_array(defcolor_text_cycle);
        draw_paren_highlight(app, buffer, text_layout_id, cursor_pos, colors.vals, colors.count);
    }
    
    // NOTE(rjf): Divider Comments
    {
        F4_RenderDividerComments(app, buffer, view_id, text_layout_id);
    }
    
    // NOTE(rjf): Cursor Mark Range
    if(is_active_view && fcoder_mode == FCoderMode_Original)
    {
        F4_HighlightCursorMarkRange(app, view_id);
    }
    
    // NOTE(allen): Cursor shape
    Face_Metrics metrics = get_face_metrics(app, face_id);
    u64 cursor_roundness_100 = def_get_config_u64(app, vars_save_string_lit("cursor_roundness"));
    f32 cursor_roundness = metrics.normal_advance*cursor_roundness_100*0.01f;
    f32 mark_thickness = (f32)def_get_config_u64(app, vars_save_string_lit("mark_thickness"));
    
    // NOTE(rjf): Cursor
    switch (fcoder_mode)
    {
        case FCoderMode_Original:
        {
            F4_Cursor_RenderEmacsStyle(app, view_id, is_active_view, buffer, text_layout_id, cursor_roundness, mark_thickness, frame_info);
        }break;
        
        case FCoderMode_NotepadLike:
        {
            F4_Cursor_RenderNotepadStyle(app, view_id, is_active_view, buffer, text_layout_id, cursor_roundness,
                                         mark_thickness, frame_info);
            break;
        }
    }
    
    // NOTE(rjf): Brace annotations
    {
        F4_Brace_RenderCloseBraceAnnotation(app, buffer, text_layout_id, cursor_pos);
    }
    
    // NOTE(rjf): Brace lines
    {
        F4_Brace_RenderLines(app, buffer, view_id, text_layout_id, cursor_pos);
    }
    
    // NOTE(allen): put the actual text on the actual screen
    draw_text_layout_default(app, text_layout_id);
    
    // NOTE(rjf): Interpret buffer as calc code, if it's the calc buffer.
    {
        Buffer_ID calc_buffer_id = get_buffer_by_name(app, string_u8_litexpr("*calc*"), AccessFlag_Read);
        if(calc_buffer_id == buffer)
        {
            F4_CLC_RenderBuffer(app, buffer, view_id, text_layout_id, frame_info);
        }
    }
    
    // NOTE(rjf): Draw calc comments.
    {
        F4_CLC_RenderComments(app, buffer, view_id, text_layout_id, frame_info);
    }
    
    draw_set_clip(app, prev_clip);
    
    // NOTE(rjf): Draw tooltips and stuff.
    if(active_view == view_id)
    {
        // NOTE(rjf): Position context helper
        {
            F4_PosContext_Render(app, view_id, buffer, text_layout_id, cursor_pos);
        }
        
        // NOTE(rjf): Draw tooltip list.
        {
            Mouse_State mouse = get_mouse_state(app);
            
            Rect_f32 view_rect = view_get_screen_rect(app, view_id);
            
            Face_ID tooltip_face_id = global_small_code_face;
            Face_Metrics tooltip_face_metrics = get_face_metrics(app, tooltip_face_id);
            
            Rect_f32 tooltip_rect =
            {
                (f32)mouse.x + 16,
                (f32)mouse.y + 16,
                (f32)mouse.x + 16,
                (f32)mouse.y + 16 + tooltip_face_metrics.line_height + 8,
            };
            
            for(int i = 0; i < global_tooltip_count; ++i)
            {
                String_Const_u8 string = global_tooltips[i].string;
                tooltip_rect.x1 = tooltip_rect.x0;
                tooltip_rect.x1 += get_string_advance(app, tooltip_face_id, string) + 4;
                
                if(tooltip_rect.x1 > view_rect.x1)
                {
                    f32 difference = tooltip_rect.x1 - view_rect.x1;
                    tooltip_rect.x1 = (float)(int)(tooltip_rect.x1 - difference);
                    tooltip_rect.x0 = (float)(int)(tooltip_rect.x0 - difference);
                }
                
                F4_DrawTooltipRect(app, tooltip_rect);
                
                draw_string(app, tooltip_face_id, string,
                            V2f32(tooltip_rect.x0 + 4,
                                  tooltip_rect.y0 + 4),
                            global_tooltips[i].color);
            }
        }
    }
    
    // NOTE(rjf): Draw inactive rectangle
    if(is_active_view == 0)
    {
        Rect_f32 view_rect = view_get_screen_rect(app, view_id);
        ARGB_Color color = fcolor_resolve(fcolor_id(fleury_color_inactive_pane_overlay));
        if(F4_ARGBIsValid(color))
        {
            draw_rectangle(app, view_rect, 0.f, color);
        }
    }
    
    // NOTE(rjf): Render code peek.
    {
        if(!view_get_is_passive(app, view_id) &&
           !is_active_view)
        {
            F4_CodePeek_Render(app, view_id, face_id, buffer, frame_info);
        }
    }
    
    // NOTE(rjf): Draw power mode.
    {
        F4_PowerMode_RenderBuffer(app, view_id, face_id, frame_info);
    }
    
}

// based on F4_Render
function void
edye_render(Application_Links *app, Frame_Info frame_info, View_ID view_id)
{
    F4_RecentFiles_RefreshView(app, view_id);
    
    ProfileScope(app, "[Fleury] Render");
    Scratch_Block scratch(app);
    
    View_ID active_view = get_active_view(app, Access_Always);
    b32 is_active_view = (active_view == view_id);

	Face_ID face_id = get_face_id(app, 0);
	Face_Metrics face_metrics = get_face_metrics(app, face_id);
	f32 line_height = face_metrics.line_height;
	f32 digit_advance = face_metrics.decimal_digit_advance;
    
    f32 margin_size = (f32)def_get_config_u64(app, vars_save_string_lit("f4_margin_size"));
    Rect_f32 view_rect = view_get_screen_rect(app, view_id);
    Rect_f32 region = rect_inner(view_rect, margin_size);
    
    // TODO(edye): this is setting the drawing region so the lister can be drawn at the bottom of the screen
    Rect_f32 global_rect = global_get_screen_rectangle(app);
	f32 filebar_y = global_rect.y1 - 2.f*line_height - vim_cur_filebar_offset+1;
	if(vim_cur_filebar_offset > 0 && region.y1 >= filebar_y){ region.y1 = filebar_y; }
    
    Buffer_ID buffer = view_get_buffer(app, view_id, Access_Always);
    String_Const_u8 buffer_name = push_buffer_base_name(app, scratch, buffer);
    
    // TODO(edye)  what is this draw_set_clip?
    Rect_f32 prev_clip = draw_set_clip(app, region);

    //~ NOTE(rjf): Draw background.
    {
        ARGB_Color color = fcolor_resolve(fcolor_id(defcolor_back));
        if(string_match(buffer_name, string_u8_litexpr("*compilation*")))
        {
            color = color_blend(color, 0.5f, 0xff000000);
        }
        // NOTE(rjf): Inactive background color.
        else if(is_active_view == 0)
        {
            ARGB_Color inactive_bg_color = fcolor_resolve(fcolor_id(fleury_color_inactive_pane_background));
            if(F4_ARGBIsValid(inactive_bg_color))
            {
                color = inactive_bg_color;
            }
        }
        draw_rectangle(app, region, 0.f, color);
        draw_margin(app, view_rect, region, color);
    }
    
    //~ NOTE(rjf): Draw margin.
    {
        ARGB_Color color = fcolor_resolve(fcolor_id(defcolor_margin));
        if(def_get_config_b32(vars_save_string_lit("f4_margin_use_mode_color")) &&
        is_active_view)
        {
            color = F4_GetColor(app, ColorCtx_Cursor(power_mode.enabled ? ColorFlag_PowerMode : 0,
                                                    GlobalKeybindingMode));
        }
        draw_margin(app, view_rect, region, color);
    }
    
    // NOTE(allen): file bar
    b64 showing_file_bar = false;
    if(view_get_setting(app, view_id, ViewSetting_ShowFileBar, &showing_file_bar) && showing_file_bar)
    {
        Rect_f32_Pair pair = layout_file_bar_on_top(region, line_height);
        F4_DrawFileBar(app, view_id, buffer, face_id, pair.min);
        region = pair.max;
    }
    
    Buffer_Scroll scroll = view_get_buffer_scroll(app, view_id);
    Buffer_Point_Delta_Result delta = delta_apply(app, view_id, frame_info.animation_dt, scroll);
    
    if(!block_match_struct(&scroll.position, &delta.point))
    {
        block_copy_struct(&scroll.position, &delta.point);
        view_set_buffer_scroll(app, view_id, scroll, SetBufferScroll_NoCursorChange);
    }
    
    if(delta.still_animating)
    {
        animate_in_n_milliseconds(app, 0);
    }
    
    // NOTE(allen): query bars
    {
        Query_Bar *space[32];
        Query_Bar_Ptr_Array query_bars = {};
        query_bars.ptrs = space;
        if (get_active_query_bars(app, view_id, ArrayCount(space), &query_bars))
        {
            for (i32 i = 0; i < query_bars.count; i += 1)
            {
                Rect_f32_Pair pair = layout_query_bar_on_top(region, line_height, 1);
                draw_query_bar(app, query_bars.ptrs[i], face_id, pair.min);
                region = pair.max;
            }
        }
    }
    
    // NOTE(allen): FPS hud
    if(show_fps_hud)
    {
        Rect_f32_Pair pair = layout_fps_hud_on_bottom(region, line_height);
        draw_fps_hud(app, frame_info, face_id, pair.max);
        region = pair.min;
        animate_in_n_milliseconds(app, 1000);
    }
    
    // NOTE(allen): layout line numbers
    Rect_f32 line_number_rect = {};
    if(def_get_config_b32(vars_save_string_lit("show_line_number_margins")))
    {
        Rect_f32_Pair pair = layout_line_number_margin(app, buffer, region, digit_advance);
        line_number_rect = pair.min;
        line_number_rect.x1 += 4;
        region = pair.max;
    }
    
    // NOTE(allen): begin buffer render
    Buffer_Point buffer_point = scroll.position;
    if(is_active_view)
    {
        buffer_point.pixel_shift.y += F4_PowerMode_ScreenShake()*1.f;
    }
    Text_Layout_ID text_layout_id = text_layout_create(app, buffer, region, buffer_point);
    
    // NOTE(allen): draw line numbers
    if(def_get_config_b32(vars_save_string_lit("show_line_number_margins")))
    {
        draw_line_number_margin(app, view_id, buffer, face_id, text_layout_id, line_number_rect);
    }
    
    // NOTE(allen): draw the buffer
    edye_render_buffer(app, view_id, face_id, buffer, text_layout_id, region, frame_info);
    
    // NOTE(rjf): Render command server
#if OS_WINDOWS
    CS_render_caller(app, frame_info, view_id);
#endif
    
    text_layout_free(app, text_layout_id);
    draw_set_clip(app, prev_clip);
}

function void
edye_tick(Application_Links *app, Frame_Info frame_info)
{
    linalloc_clear(&global_frame_arena);
    global_tooltip_count = 0;

    vim_animate_filebar(app, frame_info);
	// vim_animate_cursor(app, frame_info);
    
    F4_TickColors(app, frame_info);
    F4_Index_Tick(app);
    F4_CLC_Tick(frame_info);
    F4_PowerMode_Tick(app, frame_info);
    F4_UpdateFlashes(app, frame_info);
    
    // NOTE(rjf): Default tick stuff from the 4th dimension:
    default_tick(app, frame_info);
}


#if 0

function void
byp_tick(Application_Links *app, Frame_Info frame_info){
	code_index_update_tick(app);
	if(tick_all_fade_ranges(app, frame_info.animation_dt)){
		animate_in_n_milliseconds(app, 0);
	}

	vim_animate_filebar(app, frame_info);
	vim_animate_cursor(app, frame_info);
	fold_tick(app, frame_info);
	byp_tick_colors(app, frame_info);

	vim_cursor_blink++;

	b32 enable_virtual_whitespace = def_get_config_b32(vars_save_string_lit("enable_virtual_whitespace"));
	if(enable_virtual_whitespace != def_enable_virtual_whitespace){
		def_enable_virtual_whitespace = enable_virtual_whitespace;
		clear_all_layouts(app);
	}
}

function void
F4_Tick(Application_Links *app, Frame_Info frame_info)
{
    linalloc_clear(&global_frame_arena);
    global_tooltip_count = 0;
    
    F4_TickColors(app, frame_info);
    F4_Index_Tick(app);
    F4_CLC_Tick(frame_info);
    F4_PowerMode_Tick(app, frame_info);
    F4_UpdateFlashes(app, frame_info);
    
    // NOTE(rjf): Default tick stuff from the 4th dimension:
    default_tick(app, frame_info);
}

// [0,1]
#ifndef VIM_USE_BOTTOM_LISTER
#define VIM_USE_BOTTOM_LISTER 1
#endif

// n,m : 0 < n <= m
#ifndef VIM_LISTER_RANGE
#define VIM_LISTER_RANGE 3,5
#endif

// (0.f, 1.f]
#ifndef VIM_LISTER_MAX_RATIO
#define VIM_LISTER_MAX_RATIO 0.55f
#endif

function f32 vim_lister_get_block_height(f32 line_height){ return 1.5f*line_height; }

function void
vim_lister_render(Application_Links *app, Frame_Info frame_info, View_ID view){
	Scratch_Block scratch(app);
	
	Lister *lister = view_get_lister(app, view);
	if(lister == 0){ return; }
	
	// Set up region for drawing
	Rect_f32 region = global_get_screen_rectangle(app);
	Rect_f32 prev_clip = draw_set_clip(app, region);
	
	Face_ID face_id = get_face_id(app, 0);
	Face_Metrics metrics = get_face_metrics(app, face_id);
	f32 line_height = metrics.line_height;
	f32 max_advance = metrics.max_advance;
	f32 block_height = vim_lister_get_block_height(line_height);
	f32 max_lister_height = rect_height(region)*VIM_LISTER_MAX_RATIO - 2.f*line_height;
	
	u64 max_name_size = 0;
	Range_i32 lister_range = Ii32(VIM_LISTER_RANGE);
	const i32 N = Min(lister_range.max*i32(block_height/max_lister_height), lister->filtered.count);
	foreach(i,N){
		if(lister->filtered.node_ptrs[i]->string.size > max_name_size){
			max_name_size = lister->filtered.node_ptrs[i]->string.size;
		}
	}
	
	i32 col_num = i32(rect_width(region)/((max_name_size+7)*max_advance));
	//i32 col_num = i32(rect_width(region)/((max_name_size)*max_advance));
	col_num = clamp(lister_range.min, col_num, lister_range.max);
	
	i32 max_row_num = 1 + (lister->filtered.count-1)/col_num;
	i32 row_num = (lister->filtered.count == 0 ? 0 : Min(i32(max_lister_height/block_height), max_row_num));
    
	lister->visible_count = Min(col_num*row_num, lister->filtered.count);
	
	// TODO(BYP) check exactly why row_num+2. Had to update when changing block_height
	region = rect_split_top_bottom_neg(region, (row_num+2)*block_height).b;
	region = rect_split_top_bottom_neg(region, 2.f*line_height).a;
	vim_nxt_filebar_offset = row_num*block_height + 0.1f;
	// non-zero so when lister displays no results, it still displays the cursor
	
	// Render the view
	Render_Caller_Function *render_caller = (Render_Caller_Function *)get_custom_hook(app, HookID_RenderCaller);
	render_caller(app, frame_info, view);
	draw_set_clip(app, region);
	
	Mouse_State mouse = get_mouse_state(app);
	Vec2_f32 m_p = V2f32(mouse.p);
	
	// NOTE(allen): auto scroll to the item if the flag is set.
	f32 scroll_y = lister->scroll.position.y;
	
	if(lister->set_vertical_focus_to_item){
		lister->set_vertical_focus_to_item = false;
		Range_f32 item_y = If32_size((lister->item_index/col_num)*block_height, block_height);
		f32 view_h = rect_height(region);
		Range_f32 view_y = If32_size(scroll_y, view_h);
		if(view_y.min > item_y.min || item_y.max > view_y.max){
			f32 item_center = (item_y.min + item_y.max)*0.5f;
			f32 view_center = (view_y.min + view_y.max)*0.5f;
			f32 margin = view_h*.3f;
			margin = clamp_top(margin, block_height*3.f);
			if(item_center < view_center){
				lister->scroll.target.y = item_y.min - margin;
			}
			else{
				f32 target_bot = item_y.max + margin;
				lister->scroll.target.y = target_bot - view_h;
			}
		}
	}
	
	// NOTE(allen): clamp scroll target and position; smooth scroll rule
	i32 count = lister->filtered.count;
	//Range_f32 scroll_range = If32(0.f, clamp_bot(0.f, (count/col_num)*block_height));
	Range_f32 scroll_range = If32(0.f, clamp_bot(0.f, (count/col_num - 1)*block_height));
	lister->scroll.target.y = clamp_range(scroll_range, lister->scroll.target.y);
	lister->scroll.target.x = 0.f;
	
	Vec2_f32_Delta_Result delta = delta_apply(app, view, frame_info.animation_dt, lister->scroll);
	lister->scroll.position = delta.p;
	if(delta.still_animating){ animate_in_n_milliseconds(app, 0); }
	
	lister->scroll.position.y = clamp_range(scroll_range, lister->scroll.position.y);
	lister->scroll.position.x = 0.f;
	
	scroll_y = lister->scroll.position.y;
	i32 first_index = (i32)(col_num*scroll_y/(block_height));
	
	f32 x_base = region.x0;
    
	f32 y_base = region.y1 - vim_cur_filebar_offset; 
	f32 block_width = rect_width(region)/col_num;
	Rect_f32 back_rect = region;
	back_rect.y0 = y_base;
	back_rect.y1 = y_base + rect_height(region);
	draw_rectangle_fcolor(app, back_rect, 0.f, fcolor_id(defcolor_back));
	
	Fancy_Block block = {};
	for(i32 i=first_index; i<count; i++){
		Lister_Node *node = lister->filtered.node_ptrs[i];
		
		f32 x0 = x_base + block_width*((i-first_index) % col_num);
		f32 y0 = y_base + block_height*((i-first_index) / col_num);
		if(y0 > region.y1){ break; }
		Rect_f32 item_rect = Rf32(x0, y0, x0+block_width, y0+block_height);
		Rect_f32 item_inner = rect_inner(item_rect, 3.f);
		
		b32 hovered = rect_contains_point(item_rect, m_p);
		if(hovered){
			Fancy_Line *line1 = push_fancy_line(scratch, &block, face_id);
			u64 index = string_find_last(node->status, '\n');
			
			if(index < node->status.size){
				push_fancy_string(scratch, line1, fcolor_id(defcolor_pop2), string_prefix(node->status, index));
				Fancy_Line *line2 = push_fancy_line(scratch, &block, face_id);
				push_fancy_string(scratch, line2, fcolor_id(defcolor_text_default), string_postfix(node->status, node->status.size-1 - index));
			}else{
				push_fancy_string(scratch, line1, fcolor_id(defcolor_pop2), node->status);
			}
		}
		UI_Highlight_Level highlight = UIHighlight_None;
		if(node == lister->highlighted_node){
			highlight = UIHighlight_Active;
		}else if(node->user_data == lister->hot_user_data){
			highlight = hovered ? UIHighlight_Active : UIHighlight_Hover;
		}else if(hovered){
			highlight = UIHighlight_Hover;
		}
		
		u64 lister_roundness_100 = def_get_config_u64(app, vars_save_string_lit("lister_roundness"));
		f32 roundness = block_height*lister_roundness_100*0.01f;
		draw_rectangle_fcolor(app, item_rect, roundness, get_item_margin_color(highlight));
		draw_rectangle_fcolor(app, item_inner, roundness, get_item_margin_color(highlight, 1));
		
		Fancy_Line line = {};
		push_fancy_string(scratch, &line, fcolor_id(defcolor_text_default), node->string);
		push_fancy_stringf(scratch, &line, " ");
		u64 index = string_find_last(node->status, '\n');
		push_fancy_string(scratch, &line, fcolor_id(defcolor_pop2), string_prefix(node->status, index));
		
		Vec2_f32 p = item_inner.p0 + V2f32(3.f, (block_height - line_height)*0.5f);
		draw_fancy_line(app, face_id, fcolor_zero(), &line, p);
	}
	f32 x_padding = metrics.normal_advance;
	f32 x_half_padding = x_padding*0.5f;
	draw_drop_down(app, face_id, &block, m_p, region, x_padding, x_half_padding, fcolor_id(defcolor_margin_hover), fcolor_id(defcolor_back));
	
	if(lister->visible_count != 0){
		Rect_f32 rect = Rect_f32{region.x0, region.y1 - 4.f, region.x1, region.y1};
		draw_rectangle_fcolor(app, rect, 0.f, get_item_margin_color(UIHighlight_Active));
	}
	
	draw_set_clip(app, prev_clip);
}

function Lister_Result
vim_run_lister(Application_Links *app, Lister *lister){
	Lister_Result ret;
	View_ID view = get_this_ctx_view(app, Access_Always);
	vim_lister_view_id = view;
#if VIM_USE_BOTTOM_LISTER
	Scratch_Block scratch(app);
	lister->filter_restore_point = begin_temp(lister->arena);
	lister_update_filtered_list(app, lister);
	
	vim_use_bottom_cursor = true;
	bool do_invalidate = true;
	vim_show_buffer_peek = false;
	
	View_Context ctx = view_current_context(app, view);
	Rect_f32 global_rect = global_get_screen_rectangle(app);
	
	ctx.render_caller = vim_lister_render;
	ctx.hides_buffer = false;
	View_Context_Block ctx_block(app, view, &ctx);
	
	u8 *begin, *dest;
	begin = dest = vim_bot_text.str + vim_bot_text.size;
	u64 base_size, after_size;
	base_size = after_size = vim_bot_text.size;
	
	User_Input in = {};
	for(;;){
		Vec2_i32 col_row = calc_col_row(app, lister);
		i32 col_num = col_row.x;
		i32 visible_count = col_row.x*col_row.y;
		block_copy(dest, lister->text_field.str, lister->text_field.size);
		vim_bot_text.size = after_size + lister->text_field.size;
		animate_in_n_milliseconds(app, 0);
		
		Lister_Activation_Code result = ListerActivation_Continue;
		b32 handled = true;
		
		in = get_next_input(app, EventPropertyGroup_Any, EventProperty_Escape);
		if(in.abort){
			block_zero_struct(&lister->out);
			lister->out.canceled = true;
			vim_reset_bottom_text();
			break;
		}
		
		if(in.event.kind == InputEventKind_KeyStroke){
			if(in.event.key.code == KeyCode_W && has_modifier(&in, KeyCode_Control) && lister->handlers.backspace){
				in.event.key.code = KeyCode_Backspace;
			}
			if(in.event.key.code == KeyCode_U && has_modifier(&in, KeyCode_Control) && lister->handlers.backspace){
				in.event.key.code = KeyCode_Backspace;
				Input_Modifier_Set *set = get_modifiers(&in.event);
				set->mods[set->count++] = KeyCode_Shift;
			}
		}
		
		switch(in.event.kind){
			
			case InputEventKind_TextInsert:{
				vim_cursor_blink = 0;
				if(lister->handlers.write_character != 0){
					result = lister->handlers.write_character(app);
				}
			} break;
			
			case InputEventKind_KeyStroke:{
				vim_cursor_blink = 0;
				
				switch(in.event.key.code){
					
					case KeyCode_Tab:{
						i32 delta = (has_modifier(&in.event, KeyCode_Shift) ? -1 : 1);
						if(lister->handlers.navigate != 0){
							lister->handlers.navigate(app, view, lister, delta);
						}else if(lister->handlers.key_stroke != 0){
							result = lister->handlers.key_stroke(app);
						}else{ handled = false; }
					} break;
					
					case KeyCode_Return:{
						void *user_data = 0;
						if(in_range(0, lister->raw_item_index, lister->options.count)){
							user_data = lister_get_user_data(lister, lister->raw_item_index);
							block_copy(dest, lister->highlighted_node->string.str, lister->highlighted_node->string.size);
							vim_bot_text.size = base_size + lister->highlighted_node->string.size;
						}
						lister_activate(app, lister, user_data, false);
						result = ListerActivation_Finished;
					} break;
					
					case KeyCode_Backspace:{
						if(lister->handlers.backspace != 0){
							lister->handlers.backspace(app);
						}else if(lister->handlers.key_stroke != 0){
							result = lister->handlers.key_stroke(app);
						}else{ handled = false; }
					} break;
					
					case KeyCode_Down:
					case KeyCode_Up:{
						if(lister->handlers.navigate != 0){
							i32 delta = (in.event.key.code == KeyCode_Up ? -1 : 1);
							lister->handlers.navigate(app, view, lister, delta*col_num);
						}else if(lister->handlers.key_stroke != 0){
							result = lister->handlers.key_stroke(app);
						}else{ handled = false; }
					} break;
					
					case KeyCode_Right:
					case KeyCode_Left:{
						i32 delta = (in.event.key.code == KeyCode_Left ? -1 : 1);
						if(lister->handlers.navigate != 0){
							lister->handlers.navigate(app, view, lister, delta);
						}else if(lister->handlers.key_stroke != 0){
							result = lister->handlers.key_stroke(app);
						}else{ handled = false; }
					} break;
					
					case KeyCode_PageDown:
					case KeyCode_PageUp:{
						if(lister->handlers.navigate != 0){
							i32 delta = (in.event.key.code == KeyCode_PageUp ? -1 : 1);
							lister->handlers.navigate(app, view, lister, delta*visible_count);
						}else if(lister->handlers.key_stroke != 0){
							result = lister->handlers.key_stroke(app);
						}else{ handled = false; }
					} break;
					
					default:{
						if(lister->handlers.key_stroke != 0){
							result = lister->handlers.key_stroke(app);
						}else{ handled = false; }
					} break;
				}
			} break;
			
			case InputEventKind_MouseWheel:{
				lister->scroll.target.y += get_mouse_state(app).wheel;
				lister_update_filtered_list(app, lister);
			} break;
			
			case InputEventKind_MouseMove:{ lister_update_filtered_list(app, lister); } break;
			
			case InputEventKind_Core:{
				switch(in.event.core.code){
					case CoreCode_Animate:{ lister_update_filtered_list(app, lister); } break;
					
					default:{ handled = false; } break;
				}
			} break;
			
			default:{ handled = false; } break;
		}
		
		// NOTE: This is a bit of a hack to make clicking the lister not change views
		Mouse_State mouse_state = get_mouse_state(app);
		Vec2_f32 mouse_pos = V2f32(mouse_state.p);
		if(mouse_state.press_l){
			void *clicked = vim_lister_user_data_at_p(app, view, lister, mouse_pos, col_num);
			if(clicked){
				lister_activate(app, lister, clicked, true);
				result = ListerActivation_Finished;
				do_invalidate = false;
				view_enqueue_command_function(app, view, vim_change_lister_view_back);
			}
		}
		
		if(result == ListerActivation_ContinueAndRefresh){
			lister_call_refresh_handler(app, lister);
		}
		else if(result == ListerActivation_Finished){ break; }
		
		if(handled){ continue; }
		Fallback_Dispatch_Result disp_result = fallback_command_dispatch(app, lister->mapping, lister->map, &in);
		if(disp_result.code == FallbackDispatch_DelayedUICall){
			call_after_ctx_shutdown(app, view, disp_result.func);
			break;
		}
		if(disp_result.code == FallbackDispatch_Unhandled){ leave_current_input_unhandled(app); }
		else{ lister_call_refresh_handler(app, lister); }
	}
	
	vim_use_bottom_cursor = false;
	vim_nxt_filebar_offset = 0.f;
	if(do_invalidate){ vim_lister_view_id = 0; }
	
	if(!in.abort){
		String_Const_u8 command_name = string_substring(vim_bot_text.string, Ii64(base_size,vim_bot_text.size));
		vim_register_copy(&vim_registers.command, command_name);
		vim_update_registers(app);
	}
	
	ret = lister->out;
#else
	ret = run_lister(app, lister);
#endif
	vim_lister_view_id = 0;
	return ret;
}


function Lister_Result
vim_run_lister_with_refresh_handler(Application_Links *app, Arena *arena, String_Const_u8 query, Lister_Handlers handlers){
	Lister_Result result = {};
	if(handlers.refresh != 0){
		Lister_Block lister(app, arena);
		lister_set_query(lister, query);
		lister_set_handlers(lister, &handlers);
		handlers.refresh(app, lister);
		result = vim_run_lister(app, lister);
	}
	else{
#define M "ERROR: No refresh handler specified for lister (query_string = \"%.*s\")\n"
		print_message(app, push_u8_stringf(arena, M, string_expand(query)));
#undef M
		result.canceled = true;
	}
	return(result);
}


function void
byp_render_buffer(Application_Links *app, View_ID view_id, Face_ID face_id, Buffer_ID buffer, Text_Layout_ID text_layout_id, Rect_f32 rect){
	ProfileScope(app, "render buffer");
	b32 is_active_view = view_id == get_active_view(app, Access_Always);
	Rect_f32 prev_clip = draw_set_clip(app, rect);

	Range_i64 visible_range = text_layout_get_visible_range(app, text_layout_id);

	Face_Metrics metrics = get_face_metrics(app, face_id);
	u64 cursor_roundness_100 = def_get_config_u64(app, vars_save_string_lit("cursor_roundness"));
	f32 cursor_roundness = metrics.normal_advance*cursor_roundness_100*0.01f;
	f32 mark_thickness = (f32)def_get_config_u64(app, vars_save_string_lit("mark_thickness"));

	i64 cursor_pos = view_correct_cursor(app, view_id);
	view_correct_mark(app, view_id);

	b32 use_scope_highlight = def_get_config_b32(vars_save_string_lit("use_scope_highlight"));
	if(use_scope_highlight){
		Color_Array colors = finalize_color_array(defcolor_back_cycle);
		draw_scope_highlight(app, buffer, text_layout_id, cursor_pos, colors.vals, colors.count);
	}

	b32 highlight_line_at_cursor = def_get_config_b32(vars_save_string_lit("highlight_line_at_cursor"));
	if(highlight_line_at_cursor && is_active_view){
		i64 line_number = get_line_number_from_pos(app, buffer, cursor_pos);
		draw_line_highlight(app, text_layout_id, line_number, fcolor_id(defcolor_highlight_cursor_line));
	}

	if(is_active_view && byp_col_cursor.pos > 0){
		byp_col_cursor = buffer_compute_cursor(app, buffer, seek_line_col(byp_col_cursor.line, byp_col_cursor.col));

		i64 rel_pos = cursor_pos;
		Rect_f32 rel_rect = text_layout_character_on_screen(app, text_layout_id, rel_pos);

		i64 line = get_line_number_from_pos(app, buffer, rel_pos);
		Vec2_f32 col_rel = view_relative_xy_of_pos(app, view_id, line, byp_col_cursor.pos);
		Vec2_f32 rel_point = view_relative_xy_of_pos(app, view_id, line, rel_pos);
		Rect_f32 col_highlight_rect = {};
		col_highlight_rect.p0 = (rel_rect.p0 - rel_point) + col_rel;
		col_highlight_rect.p1 = (rel_rect.p1 - rel_point) + col_rel;
		col_highlight_rect.y0 = rect.y0;
		col_highlight_rect.y1 = rect.y1;

		draw_rectangle_fcolor(app, col_highlight_rect, 0.f, fcolor_id(defcolor_highlight_cursor_line));
	}

	Token_Array token_array = get_token_array_from_buffer(app, buffer);
	if(token_array.tokens == 0){
		paint_text_color_fcolor(app, text_layout_id, visible_range, fcolor_id(defcolor_text_default));
	}else{
		byp_draw_token_colors(app, view_id, buffer, text_layout_id);
		byp_draw_comments(app, buffer, text_layout_id, &token_array, rect);
	}

	b32 use_error_highlight = def_get_config_b32(vars_save_string_lit("use_error_highlight"));
	b32 use_jump_highlight  = def_get_config_b32(vars_save_string_lit("use_jump_highlight"));
	if(use_error_highlight || use_jump_highlight){
		Buffer_ID comp_buffer = get_buffer_by_name(app, string_u8_litexpr("*compilation*"), Access_Always);
		if(use_error_highlight){
			draw_jump_highlights(app, buffer, text_layout_id, comp_buffer, fcolor_id(defcolor_highlight_junk));
			byp_draw_compile_errors(app, buffer, text_layout_id, comp_buffer);
		}

		if(use_jump_highlight){
			Buffer_ID jump_buffer = get_locked_jump_buffer(app);
			if(jump_buffer != comp_buffer){
				draw_jump_highlights(app, buffer, text_layout_id, jump_buffer, fcolor_id(defcolor_highlight_white));
			}
		}
	}

	b32 use_paren_helper = def_get_config_b32(vars_save_string_lit("use_paren_helper"));
	if(use_paren_helper){
		Color_Array colors = finalize_color_array(defcolor_text_cycle);
		draw_paren_highlight(app, buffer, text_layout_id, cursor_pos, colors.vals, colors.count);
	}

	byp_draw_scope_brackets(app, view_id, buffer, text_layout_id, rect, cursor_pos);

	b64 show_whitespace = false;
	view_get_setting(app, view_id, ViewSetting_ShowWhitespace, &show_whitespace);
	if(show_whitespace){
		if(token_array.tokens == 0){
			draw_whitespace_highlight(app, buffer, text_layout_id, cursor_roundness);
		}else{
			draw_whitespace_highlight(app, text_layout_id, &token_array, cursor_roundness);
		}
	}

	if(byp_show_hex_colors){
		byp_hex_color_preview(app, buffer, text_layout_id);
	}

	if(is_active_view && vim_state.mode == VIM_Visual){
		vim_draw_visual_mode(app, view_id, buffer, face_id, text_layout_id);
	}

	fold_draw(app, buffer, text_layout_id);

	vim_draw_search_highlight(app, view_id, buffer, text_layout_id, cursor_roundness);

	switch(fcoder_mode){
		case FCoderMode_Original:
		vim_draw_cursor(app, view_id, is_active_view, buffer, text_layout_id, cursor_roundness, mark_thickness); break;
		case FCoderMode_NotepadLike:
		draw_notepad_style_cursor_highlight(app, view_id, buffer, text_layout_id, cursor_roundness); break;
	}


	paint_fade_ranges(app, text_layout_id, buffer);
	draw_text_layout_default(app, text_layout_id);

	vim_draw_after_text(app, view_id, is_active_view, buffer, text_layout_id, cursor_roundness, mark_thickness);
	if(is_active_view){
		byp_draw_function_preview(app, buffer, If32(rect.x0, rect.x1), cursor_pos);
	}

	draw_set_clip(app, prev_clip);
}

function void
byp_render_caller(Application_Links *app, Frame_Info frame_info, View_ID view_id){
	ProfileScope(app, "default render caller");

	Buffer_ID buffer = view_get_buffer(app, view_id, Access_Always);

	Face_ID face_id = get_face_id(app, 0);
	Face_Metrics face_metrics = get_face_metrics(app, face_id);
	f32 line_height = face_metrics.line_height;
	f32 digit_advance = face_metrics.decimal_digit_advance;

	Rect_f32 region = view_get_screen_rect(app, view_id);
	Rect_f32 prev_clip = draw_set_clip(app, region);

	Rect_f32 global_rect = global_get_screen_rectangle(app);
	f32 filebar_y = global_rect.y1 - 2.f*line_height - vim_cur_filebar_offset;
	if(region.y1 >= filebar_y){ region.y1 = filebar_y; }

	draw_rectangle_fcolor(app, region, 0.f, fcolor_id(defcolor_back));

	region = vim_draw_query_bars(app, region, view_id, face_id);

	{
		Rect_f32_Pair pair = layout_file_bar_on_bot(region, line_height);
		pair.b = rect_split_top_bottom(pair.b, line_height).a;
		vim_draw_filebar(app, view_id, buffer, frame_info, face_id, pair.b);
		region = pair.a;
	}

	Rect_f32_Pair scrollbar_pair = byp_layout_scrollbars(region, digit_advance);
	i64 show_scrollbar = false;
	view_get_setting(app, view_id, ViewSetting_ShowScrollbar, &show_scrollbar);
	show_scrollbar &= byp_show_scrollbars;
	if(show_scrollbar){
		region = scrollbar_pair.a;
	}
	draw_set_clip(app, region);

	// Draw borders
	if(region.x0 > global_rect.x0){
		Rect_f32_Pair border_pair = rect_split_left_right(region, 2.f);
		draw_rectangle_fcolor(app, border_pair.a, 0.f, fcolor_id(defcolor_margin));
		region = border_pair.b;
	}
	if(region.x1 < global_rect.x1){
		Rect_f32_Pair border_pair = rect_split_left_right_neg(region, 2.f);
		draw_rectangle_fcolor(app, border_pair.b, 0.f, fcolor_id(defcolor_margin));
		region = border_pair.a;
	}
	region.y0 += 3.f;


	if(show_fps_hud){
		Rect_f32_Pair pair = layout_fps_hud_on_bottom(region, line_height);
		draw_fps_hud(app, frame_info, face_id, pair.max);
		region = pair.min;
		animate_in_n_milliseconds(app, 1000);
	}

	// NOTE(allen): layout line numbers
	b32 show_line_number_margins = def_get_config_b32(vars_save_string_lit("show_line_number_margins"));
	Rect_f32_Pair pair = (show_line_number_margins ?
						  (byp_relative_numbers ?
						   vim_line_number_margin(app, buffer, region, digit_advance) :
						   layout_line_number_margin(app, buffer, region, digit_advance)) :
						  rect_split_left_right(region, 1.5f*digit_advance));
	Rect_f32 line_number_rect = pair.min;
	region = pair.max;

	Buffer_Scroll scroll = view_get_buffer_scroll(app, view_id);
	Buffer_Point_Delta_Result delta = delta_apply(app, view_id, frame_info.animation_dt, scroll);
	if(!block_match_struct(&scroll.position, &delta.point)){
		block_copy_struct(&scroll.position, &delta.point);
		view_set_buffer_scroll(app, view_id, scroll, SetBufferScroll_NoCursorChange);
	}
	if(delta.still_animating){ animate_in_n_milliseconds(app, 0); }
	Buffer_Point buffer_point = scroll.position;
	Text_Layout_ID text_layout_id = text_layout_create(app, buffer, region, buffer_point);

	if(show_line_number_margins){
		if(byp_relative_numbers)
			vim_draw_rel_line_number_margin(app, view_id, buffer, face_id, text_layout_id, line_number_rect);
		else
			vim_draw_line_number_margin(app, view_id, buffer, face_id, text_layout_id, line_number_rect);
	}else{
		draw_rectangle_fcolor(app, line_number_rect, 0.f, fcolor_id(defcolor_back));
	}

	if(show_scrollbar){
		byp_draw_scrollbars(app, view_id, buffer, text_layout_id, scrollbar_pair.b);
	}

	if(byp_drop_shadow){
		Buffer_Point shadow_point = buffer_point;
		Face_Description desc = get_face_description(app, face_id);
		shadow_point.pixel_shift -= Max((f32(desc.parameters.pt_size) / 8), 1.f)*V2f32(1.f, 1.f);
		Text_Layout_ID shadow_layout_id = text_layout_create(app, buffer, region, shadow_point);
		paint_text_color(app, shadow_layout_id, text_layout_get_visible_range(app, text_layout_id), 0xBB000000);
		draw_text_layout_default(app, shadow_layout_id);
		text_layout_free(app, shadow_layout_id);
	}

	byp_render_buffer(app, view_id, face_id, buffer, text_layout_id, region);

	/// NOTE(BYP): If 4coder gets even smaller fonts (smaller than u32=0) this *might* be viable
	if(false){
		Buffer_Point point = {};
		Rect_f32 prev_region = rect_split_left_right_neg(region, 190.f).b;
		prev_region.x1 -= 10.f;
		Face_ID prev_face = get_face_id(app, buffer);
		buffer_set_face(app, buffer, byp_minimal_face);
		Text_Layout_ID layout_id = text_layout_create(app, buffer, prev_region, point);

		draw_rectangle(app, rect_inner(prev_region, -10.f), 5.f, 0x44000000);

		i32 prev_mode = fcoder_mode;
		fcoder_mode = 20; // Just don't render/update the cursor
		byp_render_buffer(app, view_id, byp_minimal_face, buffer, layout_id, prev_region);
		fcoder_mode = prev_mode;

		buffer_set_face(app, buffer, prev_face);
		text_layout_free(app, layout_id);
	}


	text_layout_free(app, text_layout_id);
	draw_set_clip(app, prev_clip);
}
#endif

//~ NOTE(rjf): Bindings

function void
F4_SetAbsolutelyNecessaryBindings(Mapping *mapping)
{
    String_ID global_map_id = vars_save_string_lit("keys_global");
    String_ID file_map_id = vars_save_string_lit("keys_file");
    String_ID code_map_id = vars_save_string_lit("keys_code");
    
	String_ID global_command_map_id = vars_save_string_lit("keys_global_1");
	String_ID file_command_map_id = vars_save_string_lit("keys_file_1");
    String_ID code_command_map_id = vars_save_string_lit("keys_code_1");
    
	implicit_map_function = F4_ImplicitMap;
    
	MappingScope();
    SelectMapping(mapping);
    
    SelectMap(global_map_id);
    BindCore(edye_startup, CoreCode_Startup);
    BindCore(default_try_exit, CoreCode_TryExit);
    Bind(exit_4coder,          KeyCode_F4, KeyCode_Alt);
    BindMouseWheel(mouse_wheel_scroll);
    BindMouseWheel(mouse_wheel_change_face_size, KeyCode_Control);
    
    SelectMap(file_map_id);
    ParentMap(global_map_id);
    BindTextInput(f4_write_text_input);
    BindMouse(click_set_cursor_and_mark, MouseCode_Left);
    BindMouseRelease(click_set_cursor, MouseCode_Left);
    BindCore(click_set_cursor_and_mark, CoreCode_ClickActivateView);
    BindMouseMove(click_set_cursor_if_lbutton);
    
    SelectMap(code_map_id);
    ParentMap(file_map_id);
    BindTextInput(f4_write_text_and_auto_indent);
    BindMouse(f4_lego_click_store_token_1, MouseCode_Right);
    BindMouse(f4_lego_click_store_token_2, MouseCode_Middle);
    
    SelectMap(global_command_map_id);
	ParentMap(global_map_id);
	GlobalCommandMapReroute[0].From = global_map_id;
	GlobalCommandMapReroute[0].To = global_command_map_id;
    
    SelectMap(file_command_map_id);
	ParentMap(global_command_map_id);
	GlobalCommandMapReroute[1].From = file_map_id;
	GlobalCommandMapReroute[1].To = file_command_map_id;
    
    SelectMap(code_command_map_id);
	ParentMap(file_command_map_id);
	GlobalCommandMapReroute[2].From = code_map_id;
	GlobalCommandMapReroute[2].To = code_command_map_id;
    
}

function void
F4_SetDefaultBindings(Mapping *mapping)
{
    String_ID global_map_id = vars_save_string_lit("keys_global");
    String_ID file_map_id = vars_save_string_lit("keys_file");
    String_ID code_map_id = vars_save_string_lit("keys_code");
    
    MappingScope();
    SelectMapping(mapping);
    SelectMap(global_map_id);
    Bind(keyboard_macro_start_recording , KeyCode_U, KeyCode_Control);
    Bind(keyboard_macro_finish_recording, KeyCode_U, KeyCode_Control, KeyCode_Shift);
    Bind(keyboard_macro_replay,           KeyCode_U, KeyCode_Alt);
    Bind(change_active_panel,           KeyCode_Comma, KeyCode_Control);
    Bind(change_active_panel_backwards, KeyCode_Comma, KeyCode_Control, KeyCode_Shift);
    Bind(interactive_new,               KeyCode_N, KeyCode_Control);
    Bind(interactive_open_or_new,       KeyCode_O, KeyCode_Control);
    Bind(open_in_other,                 KeyCode_O, KeyCode_Alt);
    Bind(interactive_kill_buffer,       KeyCode_K, KeyCode_Control);
    Bind(interactive_switch_buffer,     KeyCode_I, KeyCode_Control);
    Bind(project_go_to_root_directory,  KeyCode_H, KeyCode_Control);
    Bind(save_all_dirty_buffers,        KeyCode_S, KeyCode_Control, KeyCode_Shift);
    Bind(change_to_build_panel,         KeyCode_Period, KeyCode_Alt);
    Bind(close_build_panel,             KeyCode_Comma, KeyCode_Alt);
    Bind(goto_next_jump,                KeyCode_N, KeyCode_Alt);
    Bind(goto_prev_jump,                KeyCode_N, KeyCode_Alt, KeyCode_Shift);
    Bind(build_in_build_panel,          KeyCode_M, KeyCode_Alt);
    Bind(goto_first_jump,               KeyCode_M, KeyCode_Alt, KeyCode_Shift);
    Bind(toggle_filebar,                KeyCode_B, KeyCode_Alt);
    Bind(execute_any_cli,               KeyCode_Z, KeyCode_Alt);
    Bind(execute_previous_cli,          KeyCode_Z, KeyCode_Alt, KeyCode_Shift);
    Bind(command_lister,                KeyCode_X, KeyCode_Alt);
    Bind(project_command_lister,        KeyCode_X, KeyCode_Alt, KeyCode_Shift);
    Bind(list_all_functions_current_buffer_lister, KeyCode_I, KeyCode_Control, KeyCode_Shift);
    Bind(project_fkey_command, KeyCode_F1);
    Bind(project_fkey_command, KeyCode_F2);
    Bind(project_fkey_command, KeyCode_F3);
    Bind(project_fkey_command, KeyCode_F4);
    Bind(project_fkey_command, KeyCode_F5);
    Bind(project_fkey_command, KeyCode_F6);
    Bind(project_fkey_command, KeyCode_F7);
    Bind(project_fkey_command, KeyCode_F8);
    Bind(project_fkey_command, KeyCode_F9);
    Bind(project_fkey_command, KeyCode_F10);
    Bind(project_fkey_command, KeyCode_F11);
    Bind(project_fkey_command, KeyCode_F12);
    Bind(project_fkey_command, KeyCode_F13);
    Bind(project_fkey_command, KeyCode_F14);
    Bind(project_fkey_command, KeyCode_F15);
    Bind(project_fkey_command, KeyCode_F16);
    
    // NOTE(rjf): Custom bindings.
    {
        Bind(open_panel_vsplit, KeyCode_P, KeyCode_Control);
        Bind(open_panel_hsplit, KeyCode_Minus, KeyCode_Control);
        Bind(close_panel, KeyCode_P, KeyCode_Control, KeyCode_Shift);
        Bind(f4_search_for_definition__project_wide, KeyCode_J, KeyCode_Control);
        Bind(f4_search_for_definition__current_file, KeyCode_J, KeyCode_Control, KeyCode_Shift);
        Bind(fleury_toggle_battery_saver, KeyCode_Tick, KeyCode_Alt);
        Bind(move_right_token_boundary, KeyCode_Right, KeyCode_Shift, KeyCode_Control);
        Bind(move_left_token_boundary, KeyCode_Left, KeyCode_Shift, KeyCode_Control);
    }
    
    SelectMap(file_map_id);
    ParentMap(global_map_id);
    Bind(delete_char,            KeyCode_Delete);
    Bind(backspace_char,         KeyCode_Backspace);
    Bind(move_up,                KeyCode_Up);
    Bind(move_down,              KeyCode_Down);
    Bind(move_left,              KeyCode_Left);
    Bind(move_right,             KeyCode_Right);
    Bind(seek_end_of_line,       KeyCode_End);
    Bind(fleury_home,            KeyCode_Home);
    Bind(page_up,                KeyCode_PageUp);
    Bind(page_down,              KeyCode_PageDown);
    Bind(goto_beginning_of_file, KeyCode_PageUp, KeyCode_Control);
    Bind(goto_end_of_file,       KeyCode_PageDown, KeyCode_Control);
    Bind(move_up_to_blank_line_end,        KeyCode_Up, KeyCode_Control);
    Bind(move_down_to_blank_line_end,      KeyCode_Down, KeyCode_Control);
    Bind(move_left_whitespace_boundary,    KeyCode_Left, KeyCode_Control);
    Bind(move_right_whitespace_boundary,   KeyCode_Right, KeyCode_Control);
    Bind(move_line_up,                     KeyCode_Up, KeyCode_Alt);
    Bind(move_line_down,                   KeyCode_Down, KeyCode_Alt);
    Bind(backspace_alpha_numeric_boundary, KeyCode_Backspace, KeyCode_Control);
    Bind(delete_alpha_numeric_boundary,    KeyCode_Delete, KeyCode_Control);
    Bind(snipe_backward_whitespace_or_token_boundary, KeyCode_Backspace, KeyCode_Alt);
    Bind(snipe_forward_whitespace_or_token_boundary,  KeyCode_Delete, KeyCode_Alt);
    Bind(set_mark,                    KeyCode_Space, KeyCode_Control);
    Bind(replace_in_range,            KeyCode_A, KeyCode_Control);
    Bind(copy,                        KeyCode_C, KeyCode_Control);
    Bind(delete_range,                KeyCode_D, KeyCode_Control);
    Bind(delete_line,                 KeyCode_D, KeyCode_Control, KeyCode_Shift);
    Bind(center_view,                 KeyCode_E, KeyCode_Control);
    Bind(left_adjust_view,            KeyCode_E, KeyCode_Control, KeyCode_Shift);
    Bind(search,                      KeyCode_F, KeyCode_Control);
    Bind(list_all_locations,          KeyCode_F, KeyCode_Control, KeyCode_Shift);
    Bind(list_all_substring_locations_case_insensitive, KeyCode_F, KeyCode_Alt);
    Bind(goto_line,                   KeyCode_G, KeyCode_Control);
    Bind(list_all_locations_of_selection,  KeyCode_G, KeyCode_Control, KeyCode_Shift);
    Bind(kill_buffer,                 KeyCode_K, KeyCode_Control, KeyCode_Shift);
    Bind(duplicate_line,              KeyCode_L, KeyCode_Control);
    Bind(cursor_mark_swap,            KeyCode_M, KeyCode_Control);
    Bind(reopen,                      KeyCode_O, KeyCode_Control, KeyCode_Shift);
    Bind(query_replace,               KeyCode_Q, KeyCode_Control);
    Bind(query_replace_identifier,    KeyCode_Q, KeyCode_Control, KeyCode_Shift);
    Bind(query_replace_selection,     KeyCode_Q, KeyCode_Alt);
    Bind(reverse_search,              KeyCode_R, KeyCode_Control);
    Bind(save,                        KeyCode_S, KeyCode_Control);
    Bind(save_all_dirty_buffers,      KeyCode_S, KeyCode_Control, KeyCode_Shift);
    Bind(search_identifier,           KeyCode_T, KeyCode_Control);
    Bind(list_all_locations_of_identifier, KeyCode_T, KeyCode_Control, KeyCode_Shift);
    Bind(paste_and_indent,            KeyCode_V, KeyCode_Control);
    Bind(paste_next_and_indent,       KeyCode_V, KeyCode_Control, KeyCode_Shift);
    Bind(cut,                         KeyCode_X, KeyCode_Control);
    Bind(redo,                        KeyCode_Y, KeyCode_Control);
    Bind(undo,                        KeyCode_Z, KeyCode_Control);
    Bind(view_buffer_other_panel,     KeyCode_1, KeyCode_Control);
    Bind(swap_panels,                 KeyCode_2, KeyCode_Control);
    Bind(if_read_only_goto_position,  KeyCode_Return);
    Bind(if_read_only_goto_position_same_panel, KeyCode_Return, KeyCode_Shift);
    Bind(view_jump_list_with_lister,  KeyCode_Period, KeyCode_Control, KeyCode_Shift);
    
    // NOTE(rjf): Custom bindings.
    {
        Bind(fleury_write_zero_struct,  KeyCode_0, KeyCode_Control);
        Bind(move_right_token_boundary, KeyCode_Right, KeyCode_Shift, KeyCode_Control);
        Bind(move_left_token_boundary, KeyCode_Left, KeyCode_Shift, KeyCode_Control);
    }
    
    SelectMap(code_map_id);
    ParentMap(file_map_id);
    BindTextInput(f4_write_text_and_auto_indent);
    Bind(move_left_alpha_numeric_boundary,           KeyCode_Left, KeyCode_Control);
    Bind(move_right_alpha_numeric_boundary,          KeyCode_Right, KeyCode_Control);
    Bind(move_left_alpha_numeric_or_camel_boundary,  KeyCode_Left, KeyCode_Alt);
    Bind(move_right_alpha_numeric_or_camel_boundary, KeyCode_Right, KeyCode_Alt);
    Bind(comment_line_toggle,        KeyCode_Semicolon, KeyCode_Control);
    Bind(word_complete,              KeyCode_Tab);
    Bind(auto_indent_range,          KeyCode_Tab, KeyCode_Control);
    Bind(auto_indent_line_at_cursor, KeyCode_Tab, KeyCode_Shift);
    Bind(word_complete_drop_down,    KeyCode_Tab, KeyCode_Shift, KeyCode_Control);
    Bind(write_block,                KeyCode_R, KeyCode_Alt);
    Bind(write_todo,                 KeyCode_T, KeyCode_Alt);
    Bind(write_note,                 KeyCode_Y, KeyCode_Alt);
    Bind(list_all_locations_of_type_definition,               KeyCode_D, KeyCode_Alt);
    Bind(list_all_locations_of_type_definition_of_identifier, KeyCode_T, KeyCode_Alt, KeyCode_Shift);
    Bind(open_long_braces,           KeyCode_LeftBracket, KeyCode_Control);
    Bind(open_long_braces_semicolon, KeyCode_LeftBracket, KeyCode_Control, KeyCode_Shift);
    Bind(open_long_braces_break,     KeyCode_RightBracket, KeyCode_Control, KeyCode_Shift);
    Bind(select_surrounding_scope,   KeyCode_LeftBracket, KeyCode_Alt);
    Bind(select_surrounding_scope_maximal, KeyCode_LeftBracket, KeyCode_Alt, KeyCode_Shift);
    Bind(select_prev_scope_absolute, KeyCode_RightBracket, KeyCode_Alt);
    Bind(select_prev_top_most_scope, KeyCode_RightBracket, KeyCode_Alt, KeyCode_Shift);
    Bind(select_next_scope_absolute, KeyCode_Quote, KeyCode_Alt);
    Bind(select_next_scope_after_current, KeyCode_Quote, KeyCode_Alt, KeyCode_Shift);
    Bind(place_in_scope,             KeyCode_ForwardSlash, KeyCode_Alt);
    Bind(delete_current_scope,       KeyCode_Minus, KeyCode_Alt);
    Bind(if0_off,                    KeyCode_I, KeyCode_Alt);
    Bind(open_file_in_quotes,        KeyCode_1, KeyCode_Alt);
    Bind(open_matching_file_cpp,     KeyCode_2, KeyCode_Alt);
    
}

//~ NOTE(rjf): @f4_custom_layer_initialization

void custom_layer_init(Application_Links *app)
{
    
    default_framework_init(app);
    global_frame_arena = make_arena(get_base_allocator_system());
    permanent_arena = make_arena(get_base_allocator_system());
    
    // NOTE(rjf): Set up hooks.
    {
        set_all_default_hooks(app);

// TODO remove

	vim_buffer_peek_list[ArrayCount(vim_default_peek_list) + 0] = BYP_peek_list[0];
	vim_buffer_peek_list[ArrayCount(vim_default_peek_list) + 1] = BYP_peek_list[1];
	vim_request_vtable[VIM_REQUEST_COUNT + BYP_REQUEST_Title]   = byp_apply_title;
	vim_request_vtable[VIM_REQUEST_COUNT + BYP_REQUEST_Comment] = byp_apply_comment;
	vim_request_vtable[VIM_REQUEST_COUNT + BYP_REQUEST_UnComment] = byp_apply_uncomment;

	vim_text_object_vtable[VIM_TEXT_OBJECT_COUNT + BYP_OBJECT_param0] = {',', (Vim_Text_Object_Func *)byp_object_param};
	vim_text_object_vtable[VIM_TEXT_OBJECT_COUNT + BYP_OBJECT_param1] = {';', (Vim_Text_Object_Func *)byp_object_param};
	vim_text_object_vtable[VIM_TEXT_OBJECT_COUNT + BYP_OBJECT_camel0] = {'_', (Vim_Text_Object_Func *)byp_object_camel};
	vim_text_object_vtable[VIM_TEXT_OBJECT_COUNT + BYP_OBJECT_camel1] = {'-', (Vim_Text_Object_Func *)byp_object_camel};
	vim_init(app);
    


        //t $          ($  , $                             , $                     );
        set_custom_hook(app, HookID_Tick,                    edye_tick);
        set_custom_hook(app, HookID_BufferRegion,            byp_buffer_region);
        set_custom_hook(app, HookID_RenderCaller,            edye_render);
        set_custom_hook(app, HookID_BeginBuffer,             F4_BeginBuffer);
        set_custom_hook(app, HookID_Layout,                  F4_Layout);
        //set_custom_hook(app, HookID_WholeScreenRenderCaller, F4_WholeScreenRender);
        set_custom_hook(app, HookID_WholeScreenRenderCaller, byp_whole_screen_render_caller);
        set_custom_hook(app, HookID_DeltaRule,               F4_DeltaRule);
        set_custom_hook(app, HookID_BufferEditRange,         F4_BufferEditRange);
        set_custom_hook_memory_size(app, HookID_DeltaRule, delta_ctx_size(sizeof(Vec2_f32)));

        	set_custom_hook(app, HookID_BufferEditRange,          vim_buffer_edit_range);
	set_custom_hook(app, HookID_ViewChangeBuffer,         vim_view_change_buffer);
	//set_custom_hook(app, HookID_ViewEventHandler,         vim_view_input_handler);
    }
    
    // NOTE(rjf): Set up mapping.
    {
        Thread_Context *tctx = get_thread_context(app);
        mapping_init(tctx, &framework_mapping);
        String_Const_u8 bindings_file = string_u8_litexpr("bindings.4coder");
        F4_SetAbsolutelyNecessaryBindings(&framework_mapping);
        if(!dynamic_binding_load_from_file(app, &framework_mapping, bindings_file))
        {
            F4_SetDefaultBindings(&framework_mapping);
        }
        F4_SetAbsolutelyNecessaryBindings(&framework_mapping);
    }
    
    // NOTE(rjf): Set up custom code index.
    {
        F4_Index_Initialize();
    }
    
    // NOTE(rjf): Register languages.
    {
        F4_RegisterLanguages();
    }
}

// TODO(rjf): This is only being used to check if a font file exists because
// there's a bug in try_create_new_face that crashes the program if a font is
// not found. This function is only necessary until that is fixed.
function b32
IsFileReadable(String_Const_u8 path)
{
    b32 result = 0;
    FILE *file = fopen((char *)path.str, "r");
    if(file)
    {
        result = 1;
        fclose(file);
    }
    return result;
}


//~ NOTE(edye): @edye_startup called hen 4coder's core is ready for the custom layer to start up
CUSTOM_COMMAND_SIG(edye_startup)
CUSTOM_DOC("edye startup event")
{
    ProfileScope(app, "default startup");
    
    User_Input input = get_current_input(app);
    if(!match_core_code(&input, CoreCode_Startup))
    {
        return;
    }
    
    //~ NOTE(rjf): Default 4coder initialization.
    String_Const_u8_Array file_names = input.event.core.file_names;
    load_themes_default_folder(app);
    default_4coder_initialize(app, file_names);

    //~ NOTE(rjf): Open special buffers.
    {
        // NOTE(rjf): Open compilation buffer.
        {
            Buffer_ID buffer = create_buffer(app, string_u8_litexpr("*compilation*"),
                                             BufferCreate_NeverAttachToFile |
                                             BufferCreate_AlwaysNew);
            buffer_set_setting(app, buffer, BufferSetting_Unimportant, true);
            buffer_set_setting(app, buffer, BufferSetting_ReadOnly, true);
        }

        // NOTE(rjf): Open lego buffer.
        {
            Buffer_ID buffer = create_buffer(app, string_u8_litexpr("*lego*"),
                                             BufferCreate_NeverAttachToFile |
                                             BufferCreate_AlwaysNew);
            buffer_set_setting(app, buffer, BufferSetting_Unimportant, true);
            buffer_set_setting(app, buffer, BufferSetting_ReadOnly, true);
        }

        // NOTE(rjf): Open calc buffer.
        {
            Buffer_ID buffer = create_buffer(app, string_u8_litexpr("*calc*"),
                                             BufferCreate_NeverAttachToFile |
                                             BufferCreate_AlwaysNew);
            buffer_set_setting(app, buffer, BufferSetting_Unimportant, true);
        }

        // NOTE(rjf): Open peek buffer.
        {
            Buffer_ID buffer = create_buffer(app, string_u8_litexpr("*peek*"),
                                             BufferCreate_NeverAttachToFile |
                                             BufferCreate_AlwaysNew);
            buffer_set_setting(app, buffer, BufferSetting_Unimportant, true);
        }

        // NOTE(rjf): Open LOC buffer.
        {
            Buffer_ID buffer = create_buffer(app, string_u8_litexpr("*loc*"),
                                             BufferCreate_NeverAttachToFile |
                                             BufferCreate_AlwaysNew);
            buffer_set_setting(app, buffer, BufferSetting_Unimportant, true);
        }
    }
    
    
    //~ NOTE(rjf): Initialize panels
    {
        Buffer_Identifier comp = buffer_identifier(string_u8_litexpr("*compilation*"));
        Buffer_Identifier left  = buffer_identifier(string_u8_litexpr("*calc*"));
        Buffer_Identifier right = buffer_identifier(string_u8_litexpr("*messages*"));
        Buffer_ID comp_id = buffer_identifier_to_id(app, comp);
        Buffer_ID left_id = buffer_identifier_to_id(app, left);
        Buffer_ID right_id = buffer_identifier_to_id(app, right);
        
        View_ID view = get_active_view(app, Access_Always);
        new_view_settings(app, view);
        
        // NOTE(rjf): Left Panel
        view_set_buffer(app, view, left_id, 0);
        
        // NOTE(rjf): Bottom panel
        View_ID compilation_view = 0;
        {
            compilation_view = open_view(app, view, ViewSplit_Bottom);
            new_view_settings(app, compilation_view);
            Buffer_ID buffer = view_get_buffer(app, compilation_view, Access_Always);
            Face_ID face_id = get_face_id(app, buffer);
            Face_Metrics metrics = get_face_metrics(app, face_id);
            view_set_split_pixel_size(app, compilation_view, (i32)(metrics.line_height*4.f));
            view_set_passive(app, compilation_view, true);
            global_compilation_view = compilation_view;
            view_set_buffer(app, compilation_view, 
                            comp_id,
                            0);
        }
        
        view_set_active(app, view);
        
        // NOTE(rjf): Right Panel
        open_panel_vsplit(app);
        
        View_ID right_view = get_active_view(app, Access_Always);
        view_set_buffer(app, right_view, right_id, 0);
        
        // NOTE(rjf): Restore Active to Left
        view_set_active(app, view);
        
    }

    //~ NOTE(rjf): Auto-Load Project.
    {
        b32 auto_load = def_get_config_b32(vars_save_string_lit("automatically_load_project"));
        if (auto_load)
        {
            load_project(app);
        }
    }

    //~ NOTE(rjf): Set misc options.
    {
        global_battery_saver = def_get_config_b32(vars_save_string_lit("f4_battery_saver"));
    }
    
    //~ NOTE(rjf): Initialize audio.
    {
        def_audio_init();
    }
    
    //~ NOTE(rjf): Initialize stylish fonts.
    {
        Scratch_Block scratch(app);
        String_Const_u8 bin_path = system_get_path(scratch, SystemPath_Binary);
        
        // NOTE(rjf): Fallback font.
        Face_ID face_that_should_totally_be_there = get_face_id(app, 0);
        
        // NOTE(rjf): Title font.
        {
            Face_Description desc = {0};
            {
                desc.font.file_name =  push_u8_stringf(scratch, "%.*sfonts/RobotoCondensed-Regular.ttf", string_expand(bin_path));
                desc.parameters.pt_size = 18;
                desc.parameters.bold = 0;
                desc.parameters.italic = 0;
                desc.parameters.hinting = 0;
            }
            
            if(IsFileReadable(desc.font.file_name))
            {
                global_styled_title_face = try_create_new_face(app, &desc);
            }
            else
            {
                global_styled_title_face = face_that_should_totally_be_there;
            }
        }
        
        // NOTE(rjf): Label font.
        {
            Face_Description desc = {0};
            {
                desc.font.file_name =  push_u8_stringf(scratch, "%.*sfonts/RobotoCondensed-Regular.ttf", string_expand(bin_path));
                desc.parameters.pt_size = 10;
                desc.parameters.bold = 1;
                desc.parameters.italic = 1;
                desc.parameters.hinting = 0;
            }
            
            if(IsFileReadable(desc.font.file_name))
            {
                global_styled_label_face = try_create_new_face(app, &desc);
            }
            else
            {
                global_styled_label_face = face_that_should_totally_be_there;
            }
        }
        
        // NOTE(rjf): Small code font.
        {
            Face_Description normal_code_desc = get_face_description(app, get_face_id(app, 0));
            
            Face_Description desc = {0};
            {
                desc.font.file_name =  push_u8_stringf(scratch, "%.*sfonts/Inconsolata-Regular.ttf", string_expand(bin_path));
                desc.parameters.pt_size = normal_code_desc.parameters.pt_size - 1;
                desc.parameters.bold = 1;
                desc.parameters.italic = 1;
                desc.parameters.hinting = 0;
            }
            
            if(IsFileReadable(desc.font.file_name))
            {
                global_small_code_face = try_create_new_face(app, &desc);
            }
            else
            {
                global_small_code_face = face_that_should_totally_be_there;
            }
        }
    }
    
    //~ NOTE(rjf): Prep virtual whitespace.
    {
        def_enable_virtual_whitespace = def_get_config_b32(vars_save_string_lit("enable_virtual_whitespace"));
        clear_all_layouts(app);
    }
}

/*
void
custom_layer_init(Application_Links *app){
    Thread_Context *tctx = get_thread_context(app);
    
    // NOTE(allen): setup for default framework
    default_framework_init(app);
    
    // NOTE(allen): default hooks and command maps
    set_all_default_hooks(app);
    mapping_init(tctx, &framework_mapping);
    String_ID global_map_id = vars_save_string_lit("keys_global");
    String_ID file_map_id = vars_save_string_lit("keys_file");
    String_ID code_map_id = vars_save_string_lit("keys_code");
#if OS_MAC
    setup_mac_mapping(&framework_mapping, global_map_id, file_map_id, code_map_id);
#else
    setup_default_mapping(&framework_mapping, global_map_id, file_map_id, code_map_id);
#endif
	setup_essential_mapping(&framework_mapping, global_map_id, file_map_id, code_map_id);
}
*/

