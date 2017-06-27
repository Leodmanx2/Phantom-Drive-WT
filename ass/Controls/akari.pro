:- multifile key_bind/4.
:- multifile schema_handleMouse/3.

key_bind(akari, 82, 1, 0) :- pd_translate(1, 0, 0). % R Pressed
key_bind(akari, 82, 2, 0) :- pd_translate(1, 0, 0). % R Held

schema_handleMouse(akari, _, _).
