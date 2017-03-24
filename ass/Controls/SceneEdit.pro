:- [library(readutil), library(dcg/basics)].

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Standard Input
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

:- multifile schema_key_binding/3.
:- multifile schema_handleMouse/5.

% TODO: Handle movement commands including null case

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
command(pd_rotate(Roll, Pitch, Yaw)) --> "move", whites, "to", white, whites, number(Roll), ",", number(Pitch), ",", number(Yaw), whites.
command(pd_add_light) --> "light", whites, "add", whites.
command(pd_select_light(N)) --> "light", whites, "select", white, whites, integer(N), whites.
command(pd_remove_light) --> "light", whites, "remove", whites.
command(pd_edit_light) --> "light", whites, "edit", whites.
command(pd_load_assets) --> "load", whites, "assets", whites.

do_command(Line) :- atom_codes(Line, Codes), phrase(command(Command), Codes), Command.
