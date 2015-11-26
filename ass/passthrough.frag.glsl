#version 330

// ----------------------------------------------------------------------------
//  Uniforms
// ----------------------------------------------------------------------------

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 normalMatrix;

const vec3 eyePos = {0.0f, 0.0f, -1000.0f};
const vec3 lightPos = {200.0f, 400.0f, 50.0f};

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
	vec3 normal = normalize(frag_normal);
	vec3 lightVec = normalize(lightPos - frag_position);
	
	// TODO: Colours and/or texture sampling
	vec3 frag_color = {0.5f, 0.5f, 0.5f};
	
	
	// TODO: Determined by material properties
	float ambient = 0.2f;
	
	
	float diffuse = dot(normal, lightVec);
	diffuse = clamp(diffuse, 0.0f, 1.0f);
	
	
	vec3 eyeVec = normalize(eyePos - frag_position);
	
	vec3 halfVec = normalize(lightVec + eyeVec);
	
	float specular = dot(normal, halfVec);
	specular = max(specular, 0.0f);
	specular = pow(specular, 15);
	

	float lightIntensity = ambient + diffuse + specular;
	vec3 color = frag_color * lightIntensity;

	out_color = vec4(color, 1.0f);
}
