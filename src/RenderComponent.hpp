#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H

#include <string>

// RenderComponent contains all the information necessary to load and render
struct RenderComponent {
	std::string diffuse;
	std::string specular;
	std::string shader;
	std::string geometry;
};

#endif
