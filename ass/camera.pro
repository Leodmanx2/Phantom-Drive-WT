% TODO: Move into input schema directory

camera_bindKeys :-
pd_bindKey(87, "pd_translate(1, 0, 0)."), % W
pd_bindKey(65, "pd_translate(0, 1, 0)."), % A
pd_bindKey(83, "pd_translate(-1, 0, 0)."), % S
pd_bindKey(68, "pd_translate(0, -1, 0)."), % D
pd_bindKey(67, "pd_translate(0, 0, -1)."), % C
pd_bindKey(32, "pd_translate(0, 0, 1)."). % Space

camera_mouseHandle(X1, Y1, X2, Y2) :- Roll is 0.0,
                                      Pitch is -((Y2 - Y1) / 128.0),
                                      Yaw is -((X2 - X1) / 128.0),
                                      pd_rotate(Roll, Pitch, Yaw).
