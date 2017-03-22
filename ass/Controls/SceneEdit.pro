:- [library(readutil), library(dcg/basics)].

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Standard Input
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

:- multifile schema_key_binding/3.
:- multifile schema_handleMouse/5.

schema_key_binding('SceneEdit', 87, "pd_translate(1, 0, 0).").  % W
schema_key_binding('SceneEdit', 65, "pd_translate(0, 1, 0).").  % A
schema_key_binding('SceneEdit', 83, "pd_translate(-1, 0, 0)."). % S
schema_key_binding('SceneEdit', 68, "pd_translate(0, -1, 0)."). % D
schema_key_binding('SceneEdit', 67, "pd_translate(0, 0, -1)."). % C
schema_key_binding('SceneEdit', 32, "pd_translate(0, 0, 1).").  % Space

schema_handleMouse('SceneEdit', X1, Y1, X2, Y2) :- Roll is 0.0,
                                              Pitch is -((Y2 - Y1) / 128.0),
                                              Yaw is -((X2 - X1) / 128.0),
                                              pd_rotate(Roll, Pitch, Yaw).

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

do_line :- read_line_to_codes(user_input, Line),
           phrase(command(C), Line),
           C.
