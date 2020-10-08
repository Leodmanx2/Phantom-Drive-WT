#include "RenderComponent.hpp"

RenderComponent::RenderComponent(const std::string& diff,
                                 const std::string& spec,
                                 const std::string& shdr,
                                 const std::string& geo)
  : diffuse(diff), specular(spec), shader(shdr), geometry(geo) {}
