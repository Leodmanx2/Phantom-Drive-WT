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

struct PointLight {
	vec3 position;
	vec3 color;
	float intensity;
	float radius;
};

struct SpotLight {
	vec3 position;
	vec3 direction;
	vec3 color;
	float intensity;
	float angle;
	float radius;
};

struct DirectionLight {
	vec3 direction;
	vec3 color;
	float intensity;
};

#define POINT_LIGHT_COUNT 8
#define SPOT_LIGHT_COUNT 8
#define DIRECTION_LIGHT_COUNT 2

uniform PointLight pointLights[POINT_LIGHT_COUNT];
uniform SpotLight spotLights[SPOT_LIGHT_COUNT];
uniform DirectionLight directionLights[DIRECTION_LIGHT_COUNT];

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
	specularColor *= light.color * light.intensity;

	vec3 color = diffuseIntensity * diffuseColor +
	             specularIntensity * specularColor;

	return color;
}

vec3 spotLight(SpotLight light) {
	vec3 toLight = normalize(light.position - frag_position);

	float angle = degrees(acos(dot(-toLight, normalize(light.direction))));
	if(angle > light.angle) return vec3(0);

	vec3 normal = normalize(frag_normal);
	vec3 halfVec = normalize(toLight + normalize(eyePos - frag_position));

	vec3 diffuseColor = texture2D(diffuseMap, frag_texCoord).rgb;
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
	specularColor *= light.color * light.intensity;

	vec3 color = diffuseIntensity * diffuseColor +
	             specularIntensity * specularColor;

	return color;
}

vec3 directionLight(DirectionLight light) {
	vec3 normal = normalize(frag_normal);
	vec3 toLight = normalize(-light.direction);
	vec3 halfVec = normalize(toLight + normalize(eyePos - frag_position));

	vec3 diffuseColor = texture2D(diffuseMap, frag_texCoord).rgb;
	vec3 specularColor = texture2D(specularMap, frag_texCoord).rgb;
	float shine = 255.0 * texture2D(specularMap, frag_texCoord).a;

	float diffuseIntensity = dot(normal, toLight);
	      diffuseIntensity = max(diffuseIntensity, 0.0);

	float specularIntensity = dot(normal, halfVec);
	      specularIntensity = max(specularIntensity, 0.0);
	      specularIntensity = pow(specularIntensity, shine);

	diffuseColor *= light.color * light.intensity;
	specularColor *= light.color * light.intensity;

	vec3 color = diffuseIntensity * diffuseColor +
	             specularIntensity * specularColor;

	return color;
}

// ----------------------------------------------------------------------------
//  Entry point
// ----------------------------------------------------------------------------

void main() {
	// TODO: Some of the code in the helper functions should be moved into main to
	//       prevent extraneous calculations


	vec3 color = vec3(0);

	// Directional lights
	for(int i=0; i<DIRECTION_LIGHT_COUNT; ++i) {
		color += directionLight(directionLights[i]);
	}

	// Point lights
	for(int i=0; i<POINT_LIGHT_COUNT; ++i) {
		color += pointLight(pointLights[i]);
	}

	// Spot lights
	for(int i=0; i<SPOT_LIGHT_COUNT; ++i) {
		color += spotLight(spotLights[i]);
	}

	color += ambience * texture2D(diffuseMap, frag_texCoord).rgb;

	out_color = vec4(color, 1.0);
}
