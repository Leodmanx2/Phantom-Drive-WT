% TODO: Move into input schema directory

% TODO: Avoid input schema conflicts
%
% Things are currently fine, but as soon as another schema is added these
% goals will overlap.
%
% One idea is simply to rename them for each schema and parse the appropriate
% schema name from the Actor description file

bindKeys :-
pd_bindKey(87, "pd_translate(1, 0, 0)."), % W
pd_bindKey(65, "pd_translate(0, 1, 0)."), % A
pd_bindKey(83, "pd_translate(-1, 0, 0)."), % S
pd_bindKey(68, "pd_translate(0, -1, 0)."), % D
pd_bindKey(67, "pd_translate(0, 0, -1)."), % C
pd_bindKey(32, "pd_translate(0, 0, 1)."). % Space

mouseHandle(X1, Y1, X2, Y2) :- Roll is 0.0,
                               Pitch is -((Y2 - Y1) / 128.0),
                               Yaw is -((X2 - X1) / 128.0),
                               pd_rotate(Roll, Pitch, Yaw).
