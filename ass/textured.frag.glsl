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

// TODO: Lights and their properties, to be passed in from scene
const vec3 lightPos = vec3(0.0, 0.0, 1000.0);

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
	
	float diffuseIntensity = dot(normal, toLight);
	      diffuseIntensity = max(diffuseIntensity, 0.0);
	
	float specularIntensity = dot(normal, halfVec);
	      specularIntensity = max(specularIntensity, 0.0);
	      specularIntensity = pow(specularIntensity, shine);
	
	vec3 color = ambientColor +
	             diffuseIntensity * diffuseColor +
	             specularIntensity * specularColor;
	
	out_color = vec4(color, 1.0);
}
