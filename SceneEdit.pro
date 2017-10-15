:- [library(readutil), library(dcg/basics)].

pd_save --> "save".
pd_save --> "save\r".

pd_exit --> "exit".

pd_add_actor(Name) --> "add", white, string(Name).

pd_select --> "select".

pd_deselect --> "deselect".

pd_remove_actor --> "remove".

pd_move_by(Longitude, Latitude, Altitude) --> "move", whites, "by", white, number(Longitude), ",", number(Latitude), ",", number(Altitude).

pd_move_to(X, Y, Z) --> "move", white, "to", white, number(X), ",", number(Y), ",", number(Z).

pd_rotate_by(Roll, Pitch, Yaw) --> "rotate", white, "by", white, number(Roll), ",", number(Pitch), ",", number(Yaw).

pd_rotate_to(Roll, Pitch, Yaw) --> "rotate", white, "to", white, number(Roll), ",", number(Pitch), ",", number(Yaw).

pd_add_light --> "light", whites, "add".

pd_select_light(N) --> "light", whites, "select", white, integer(N).

pd_remove_light --> "light", white, "remove".

pd_edit_light --> "light", white, "edit".
