#version 330

// ----------------------------------------------------------------------------
//  Uniforms
// ----------------------------------------------------------------------------

uniform mat4 view;

uniform vec3 eyePos;

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;

uniform float ambience;

uniform int ID;

struct Light {
	vec3 position;
	vec3 direction;
	vec3 color;
	float intensity;
	float angle;
	float radius;
};

uniform Light light;

// ----------------------------------------------------------------------------
//  Input
// ----------------------------------------------------------------------------

in vec3 frag_position;
in vec3 frag_normal;
in vec2 frag_texCoord;

// ----------------------------------------------------------------------------
//  Output
// ----------------------------------------------------------------------------

layout(location = 0) out vec4 out_color;
layout(location = 1) out int out_id;

// ----------------------------------------------------------------------------
//  Entry point
// ----------------------------------------------------------------------------

void main() {
	vec3 color = vec3(0);

	vec3 lightPos = (view * vec4(light.position, 1.0)).xyz;

	vec3 normal = normalize(frag_normal);
	vec3 toLight = normalize(lightPos - frag_position);
	vec3 halfVec = normalize(toLight + normalize(eyePos - frag_position));

	vec3 diffuseColor = texture2D(diffuseMap, frag_texCoord).rgb;
	vec3 specularColor = texture2D(specularMap, frag_texCoord).rgb;
	float shine = 255.0 * texture2D(specularMap, frag_texCoord).a;

	float distanceToLight = abs(distance(lightPos, frag_position));
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
	specularColor *= light.color * light.intensity;

	color += diffuseIntensity * diffuseColor +
	             specularIntensity * specularColor;

	color += ambience * texture2D(diffuseMap, frag_texCoord).rgb;

	out_color = vec4(color, 1.0);
	out_id = ID;
}
