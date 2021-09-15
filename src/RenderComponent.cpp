#include "RenderComponent.hpp"

RenderComponent::RenderComponent(const std::string& diff,
                                 const std::string& spec,
                                 const std::string& vs,
                                 const std::string& fs,
                                 const std::string& geo)
  : diffuse(diff)
  , specular(spec)
  , vertexShader(vs)
  , fragmentShader(fs)
  , geometry(geo) {}
