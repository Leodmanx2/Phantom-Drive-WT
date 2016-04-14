#version 330

// ----------------------------------------------------------------------------
//  Uniforms
// ----------------------------------------------------------------------------

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 normalMatrix;

uniform vec3 eyePos;

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;

uniform float ambience;

// TODO: Support more than one light
struct PointLight {
	vec3 position;
	vec3 color;
	float intensity;
	float radius;
};

#define POINT_LIGHT_COUNT 8
uniform PointLight lights[POINT_LIGHT_COUNT];

// ----------------------------------------------------------------------------
//  Input
// ----------------------------------------------------------------------------

in vec3 frag_position;
in vec3 frag_normal;
in vec2 frag_texCoord;

// ----------------------------------------------------------------------------
//  Output
// ----------------------------------------------------------------------------

out vec4 out_color;

// ----------------------------------------------------------------------------
//  Helper functions
// ----------------------------------------------------------------------------

vec3 pointLight(PointLight light) {
	vec3 normal = normalize(frag_normal);
	vec3 toLight = normalize(light.position - frag_position);
	vec3 halfVec = normalize(toLight + normalize(eyePos - frag_position));

	vec3 diffuseColor = texture2D(diffuseMap, frag_texCoord).rgb;
	vec3 ambientColor = ambience * diffuseColor;
	vec3 specularColor = texture2D(specularMap, frag_texCoord).rgb;
	float shine = 255.0 * texture2D(specularMap, frag_texCoord).a;

	float distanceToLight = abs(distance(light.position, frag_position));
	float falloff = 1.0 / (pow(light.radius, 2) * 0.05);
	float attenuation = 1.0 / (1.0 + falloff * pow(distanceToLight, 2));

	float diffuseIntensity = dot(normal, toLight);
	      diffuseIntensity = max(diffuseIntensity, 0.0);
	      diffuseIntensity *= attenuation;

	float specularIntensity = dot(normal, halfVec);
	      specularIntensity = max(specularIntensity, 0.0);
	      specularIntensity = pow(specularIntensity, shine);
	      specularIntensity *= attenuation;

	diffuseColor *= light.color * light.intensity;
	ambientColor *= light.color * light.intensity;
	specularColor *= light.color * light.intensity;

	vec3 color = 0.0 * ambientColor +
	             diffuseIntensity * diffuseColor +
	             specularIntensity * specularColor;

	return color;
}

// ----------------------------------------------------------------------------
//  Entry point
// ----------------------------------------------------------------------------

void main() {

	vec3 color = vec3(0);

	// TODO: Directional lights

	// Point lights
	for(int i=0; i<POINT_LIGHT_COUNT; ++i) {
		color += pointLight(lights[i]);
	}

	// TODO: Spot lights

	out_color = vec4(color, 1.0);
}
