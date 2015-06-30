#include "Application.h"

int main(int argc, char *argv[]) {
	try {
		Application app;
		app.run();
	}
	catch(const std::exception exception) {
		// TODO: Log
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}

