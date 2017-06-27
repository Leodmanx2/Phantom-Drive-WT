:- [library(readutil), library(dcg/basics)].

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Standard Input
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

:- multifile key_bind/4.
:- multifile schema_handleMouse/3.

% TODO: Handle movement commands including null case

key_bind('SceneEdit', 71, 1, 0) :- pd_select.  % G Pressed

key_bind('SceneEdit', 263, 1, 0) :- pd_move_by(0,-1,0).  % Left Arrow Pressed
key_bind('SceneEdit', 265, 1, 0) :- pd_move_by(0,0,1).  % Up Arrow Pressed
key_bind('SceneEdit', 262, 1, 0) :- pd_move_by(0,1,0).  % Right Arrow Pressed
key_bind('SceneEdit', 264, 1, 0) :- pd_move_by(0,0,-1).  % Down Arrow Pressed

key_bind('SceneEdit', 93, 1, 0) :- pd_rotate_by(1,0,0).  % ] Pressed
key_bind('SceneEdit', 39, 1, 0) :- pd_rotate_by(0,1,0).  % ' Pressed
key_bind('SceneEdit', 47, 1, 0) :- pd_rotate_by(0,0,1).  % / Pressed
key_bind('SceneEdit', 91, 1, 0) :- pd_rotate_by(-1,0,0).  % [ Pressed
key_bind('SceneEdit', 59, 1, 0) :- pd_rotate_by(0,-1,0).  % ; Pressed
key_bind('SceneEdit', 46, 1, 0) :- pd_rotate_by(0,0,-1).  % . Pressed

key_bind('SceneEdit', 263, 2, 0) :- pd_move_by(0,-1,0).  % Left Arrow Held
key_bind('SceneEdit', 265, 2, 0) :- pd_move_by(0,0,1).  % Up Arrow Held
key_bind('SceneEdit', 262, 2, 0) :- pd_move_by(0,1,0).  % Right Arrow Held
key_bind('SceneEdit', 264, 2, 0) :- pd_move_by(0,0,-1).  % Down Arrow Held

key_bind('SceneEdit', 93, 2, 0) :- pd_rotate_by(1,0,0).  % ] Held
key_bind('SceneEdit', 39, 2, 0) :- pd_rotate_by(0,1,0).  % ' Held
key_bind('SceneEdit', 47, 2, 0) :- pd_rotate_by(0,0,1).  % / Held
key_bind('SceneEdit', 91, 2, 0) :- pd_rotate_by(-1,0,0).  % [ Held
key_bind('SceneEdit', 59, 2, 0) :- pd_rotate_by(0,-1,0).  % ; Held
key_bind('SceneEdit', 46, 2, 0) :- pd_rotate_by(0,0,-1).  % . Held

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Console Input
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

command(pd_save) --> "save", whites.
command(pd_saveas(Name)) --> "save", whites, "as", white, whites, string(Name), whites.
command(pd_exit) --> "exit", whites.
command(pd_add_actor(Name)) --> "add", white, whites, string(Name), whites.
command(pd_select) --> "select", whites.
command(pd_deselect) --> "deselect", whites.
command(pd_remove_actor) --> "remove", whites.
command(pd_move_by(Longitude, Latitude, Altitude)) --> "move", whites, "by", white, whites, number(Longitude), ",", number(Latitude), ",", number(Altitude), whites.
command(pd_move_to(X, Y, Z)) --> "move", whites, "to", white, whites, number(X), ",", number(Y), ",", number(Z), whites.
command(pd_rotate_by(Roll, Pitch, Yaw)) --> "rotate", whites, "by", white, whites, number(Roll), ",", number(Pitch), ",", number(Yaw), whites.
command(pd_rotate_to(Roll, Pitch, Yaw)) --> "rotate", whites, "to", white, whites, number(Roll), ",", number(Pitch), ",", number(Yaw), whites.
command(pd_add_light) --> "light", whites, "add", whites.
command(pd_select_light(N)) --> "light", whites, "select", white, whites, integer(N), whites.
command(pd_remove_light) --> "light", whites, "remove", whites.
command(pd_edit_light) --> "light", whites, "edit", whites.
command(pd_load_assets) --> "load", whites, "assets", whites.

do_command(Line) :- atom_codes(Line, Codes), phrase(command(Command), Codes), Command.
