:- multifile key_bind/4.
:- multifile schema_handleMouse/3.

key_bind(Spatial, camera, 87, 2, 0) :- pd_translate(Spatial, 1, 0, 0).  % W Held
key_bind(Spatial, camera, 65, 2, 0) :- pd_translate(Spatial, 0, 1, 0).  % A Held
key_bind(Spatial, camera, 83, 2, 0) :- pd_translate(Spatial, -1, 0, 0). % S Held
key_bind(Spatial, camera, 68, 2, 0) :- pd_translate(Spatial, 0, -1, 0). % D Held
key_bind(Spatial, camera, 67, 2, 0) :- pd_translate(Spatial, 0, 0, -1). % C Held
key_bind(Spatial, camera, 32, 2, 0) :- pd_translate(Spatial, 0, 0, 1).  % Space Held

schema_handleMouse(Spatial, camera, DX, DY) :- Pitch is -DY, 
                                      Yaw is -DX, 
                                      pd_rotate(Spatial, 0, Pitch, Yaw).
