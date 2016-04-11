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
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightIntensity;
uniform float lightRadius;

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
//  Entry point
// ----------------------------------------------------------------------------

void main() {
	vec3 diffuseColor = texture2D(diffuseMap, frag_texCoord).rgb;
	vec3 ambientColor = ambience * diffuseColor;
	vec3 specularColor = texture2D(specularMap, frag_texCoord).rgb;
	float shine = 255.0 * texture2D(specularMap, frag_texCoord).a;
	
	vec3 normal = normalize(frag_normal);
	vec3 toLight = normalize(lightPos - frag_position);
	vec3 toEye = normalize(eyePos - frag_position);
	vec3 halfVec = normalize(toLight + toEye);
	
	float distanceToLight = abs(distance(lightPos, frag_position));
	float falloff = 1.0 / (pow(lightRadius, 2) * 0.05);
	float attenuation = 1.0 / (1.0 + falloff * pow(distanceToLight, 2));
	
	diffuseColor *= lightColor * lightIntensity;
	ambientColor *= lightColor * lightIntensity;
	specularColor *= lightColor * lightIntensity;
	
	float diffuseIntensity = dot(normal, toLight);
	      diffuseIntensity = max(diffuseIntensity, 0.0);
	      diffuseIntensity *= attenuation;
	
	float specularIntensity = dot(normal, halfVec);
	      specularIntensity = max(specularIntensity, 0.0);
	      specularIntensity = pow(specularIntensity, shine);
	      specularIntensity *= attenuation;
	
	vec3 color = 0.0 * ambientColor +
	             diffuseIntensity * diffuseColor +
	             specularIntensity * specularColor;
	
	out_color = vec4(color, 1.0);
}
