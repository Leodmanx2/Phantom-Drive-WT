cmake_minimum_required(VERSION 2.8)

# Create a list of source files
# and include headers on Windows (for Visual Studio)
set(SOURCE # List .cpp, .cc, .c, etc. files here

)
if(WIN32)
	set(SOURCE ${SOURCE}
		# List header files here
	)
endif(WIN32)

add_executable(game ${SOURCE})
install(TARGETS game DESTINATION bin)

