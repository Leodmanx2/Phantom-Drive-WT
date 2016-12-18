:- multifile schema_key_binding/3.
:- multifile schema_handleMouse/5.

schema_key_binding(camera, 87, "pd_translate(1, 0, 0).").  % W
schema_key_binding(camera, 65, "pd_translate(0, 1, 0).").  % A
schema_key_binding(camera, 83, "pd_translate(-1, 0, 0)."). % S
schema_key_binding(camera, 68, "pd_translate(0, -1, 0)."). % D
schema_key_binding(camera, 67, "pd_translate(0, 0, -1)."). % C
schema_key_binding(camera, 32, "pd_translate(0, 0, 1).").  % Space

schema_handleMouse(camera, X1, Y1, X2, Y2) :- Roll is 0.0,
                                              Pitch is -((Y2 - Y1) / 128.0),
                                              Yaw is -((X2 - X1) / 128.0),
                                              pd_rotate(Roll, Pitch, Yaw).
