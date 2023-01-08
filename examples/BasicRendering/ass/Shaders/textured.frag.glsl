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

// TODO: Implement lighting details like direction and falloff
struct Light {
	vec3 position;
	vec3 direction;
	vec3 color;
	float intensity;
	float angle;
	float radius;
};

uniform Light light;

uniform bool useAmbient;
uniform bool useDiffuse;
uniform bool useSpecular;

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

	// Shared
	vec3 lightPos = (view * vec4(light.position, 1.0)).xyz;
	vec3 normal = normalize(frag_normal);
	vec3 toLight = normalize(lightPos - frag_position);
	vec3 halfVec = normalize(toLight + normalize(eyePos - frag_position));
	float distanceToLight = abs(distance(lightPos, frag_position));
	float falloff = 1.0 / (pow(light.radius, 2) * 0.05);
	float attenuation = 1.0 / (1.0 + falloff * pow(distanceToLight, 2));

	// Ambient only
	if(useAmbient) {
		color += ambience * texture2D(diffuseMap, frag_texCoord).rgb;;
	}

	// Diffuse only
	if(useDiffuse) {
		float diffuseIntensity = dot(normal, toLight);
		      diffuseIntensity = max(diffuseIntensity, 0.0);
		      diffuseIntensity *= attenuation;
		vec3 diffuseColor = texture2D(diffuseMap, frag_texCoord).rgb;
		diffuseColor *= light.color * light.intensity;
		color += diffuseIntensity * diffuseColor;
	}

	// Specular only
	if(useSpecular) {
		float shine = 255.0 * texture2D(specularMap, frag_texCoord).a;
		float specularIntensity = dot(normal, halfVec);
		      specularIntensity = max(specularIntensity, 0.0);
		      specularIntensity = pow(specularIntensity, shine);
		      specularIntensity *= attenuation;
		vec3 specularColor = texture2D(specularMap, frag_texCoord).rgb;
		specularColor *= light.color * light.intensity;
		color += specularIntensity * specularColor;
	}

	out_color = vec4(color, 1.0);
	out_id = ID;
}
