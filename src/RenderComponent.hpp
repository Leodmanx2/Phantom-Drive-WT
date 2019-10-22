#ifndef PD_RENDERCOMPONENT_HPP
#define PD_RENDERCOMPONENT_HPP

#include <string>

// RenderComponent is a collection of all the resource names necessary to
// describe what a game entity looks like. It describes what to draw but
// not how to draw it. RenderComponent contains only resource handles.
// Resources themselves are managed by the renderer.
struct RenderComponent {
	std::string diffuse;
	std::string specular;
	std::string shader;
	std::string geometry;
};

#endif
