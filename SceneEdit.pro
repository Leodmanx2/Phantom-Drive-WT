:- [library(readutil), library(dcg/basics)].

pd_save --> "save\r".
pd_save --> "save".

pd_exit --> "exit", "\r".
pd_exit --> "exit".

pd_add_actor(Name) --> "add ", string(Name), "\r".
pd_add_actor(Name) --> "add ", string(Name).

pd_select --> "select", "\r".
pd_select --> "select".

pd_deselect --> "deselect", "\r".
pd_deselect --> "deselect".

pd_remove_actor --> "remove", "\r".
pd_remove_actor --> "remove".

pd_move_by(Longitude, Latitude, Altitude) --> "move by ", number(Longitude), ",", number(Latitude), ",", number(Altitude), "\r".
pd_move_by(Longitude, Latitude, Altitude) --> "move by ", number(Longitude), ",", number(Latitude), ",", number(Altitude).

pd_move_to(X, Y, Z) --> "move to ", number(X), ",", number(Y), ",", number(Z), "\r".
pd_move_to(X, Y, Z) --> "move to ", number(X), ",", number(Y), ",", number(Z).

pd_rotate_by(Roll, Pitch, Yaw) --> "rotate by ", number(Roll), ",", number(Pitch), ",", number(Yaw), "\r".
pd_rotate_by(Roll, Pitch, Yaw) --> "rotate by ", number(Roll), ",", number(Pitch), ",", number(Yaw).

pd_rotate_to(Roll, Pitch, Yaw) --> "rotate to ", number(Roll), ",", number(Pitch), ",", number(Yaw), "\r".
pd_rotate_to(Roll, Pitch, Yaw) --> "rotate to ", number(Roll), ",", number(Pitch), ",", number(Yaw).

pd_add_light --> "light add", "\r".
pd_add_light --> "light add".

pd_select_light(N) --> "light select ", integer(N), "\r".
pd_select_light(N) --> "light select ", integer(N).

pd_remove_light --> "light remove", "\r".
pd_remove_light --> "light remove".

pd_edit_light --> "light edit", "\r".
pd_edit_light --> "light edit".
