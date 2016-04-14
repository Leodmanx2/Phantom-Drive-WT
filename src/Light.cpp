#include "Light.h"

// ----------------------------------------------------------------------------
//  PointLight
// ----------------------------------------------------------------------------

PointLight::PointLight(glm::vec3 pos,
                       glm::vec3 clr,
                       float its,
                       float rad) : position(pos),
                                    color(clr),
                                    intensity(its),
                                    radius(rad) {

}

PointLight::PointLight(const PointLight& original) : position(original.position),
                                               color(original.color),
                                               intensity(original.intensity),
                                               radius(original.radius) {

}

PointLight::~PointLight() {}

// ----------------------------------------------------------------------------
//  SpotLight
// ----------------------------------------------------------------------------

SpotLight::SpotLight(glm::vec3 pos,
                     glm::vec3 dir,
                     glm::vec3 clr,
                     float its,
                     float agl) : position(pos),
                                  direction(dir),
                                  color(clr),
                                  intensity(its),
                                  angle(agl) {

}

SpotLight::SpotLight(const SpotLight& original) : position(original.position),
                                                  direction(original.direction),
                                                  color(original.color),
                                                  intensity(original.intensity),
                                                  angle(original.angle) {

}

SpotLight::~SpotLight() {}

// ----------------------------------------------------------------------------
//  DirectionLight
// ----------------------------------------------------------------------------

DirectionLight::DirectionLight(glm::vec3 dir,
                               glm::vec3 clr,
                               float its) : direction(dir),
                                            color(clr),
                                            intensity(its){

}

DirectionLight::DirectionLight(const DirectionLight& original) : direction(original.direction),
                                                                 color(original.color),
                                                                 intensity(original.intensity) {

}

DirectionLight::~DirectionLight() {}
