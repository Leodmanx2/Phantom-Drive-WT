% TODO: Find a way to express this on the C++ side.
consult_memory(File, Handle) :-
open_memory_file(Handle, read, Stream),
load_files(File, [stream(Stream)]),
close(Stream).
