#version 330

// ----------------------------------------------------------------------------
//  Uniforms
// ----------------------------------------------------------------------------

uniform vec3 eyePos;
uniform vec3 lightPos;

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
//  Function declarations
// ----------------------------------------------------------------------------

float ambient();
float diffuse();
float specular();

// ----------------------------------------------------------------------------
//  Entry point
// ----------------------------------------------------------------------------

void main() {
	// TODO: Colours and/or texture sampling
	vec3 frag_color = {0.5f, 0.5f, 0.5f};

	float lightIntensity = ambient() + diffuse() + specular();
	vec3 color = frag_color * lightIntensity;

	out_color = vec4(color, 1.0f);
}

// ----------------------------------------------------------------------------
//  Function definitions
// ----------------------------------------------------------------------------

float ambient() {
	// TODO: Determined by material properties
	return 0.2f;
}

float diffuse() {
	vec3 lightVec = normalize(lightPos - frag_position);
	
	vec3 normal = normalize(frag_normal);
	float diffuse = dot(normal, lightVec);
	diffuse = clamp(diffuse, 0.0f, 1.0f);
	return diffuse;
}

float specular() {
	vec3 lightVec = normalize(lightPos - frag_position);
	vec3 eyeVec = normalize(eyePos - frag_position);
	vec3 normal = normalize(frag_normal);
	
	vec3 halfVec = normalize(lightVec + eyeVec);
	
	float specular = dot(normal, halfVec);
	specular = max(specular, 0.0f);
	specular = pow(specular, 15);
	return specular;
}
