#version 330

// ----------------------------------------------------------------------------
//  Uniforms
// ----------------------------------------------------------------------------

uniform mat4 model_transform;
uniform mat4 view_transfrom;
uniform mat4 projection_transform;
uniform mat4 normal_transform;

// ----------------------------------------------------------------------------
//  Input
// ----------------------------------------------------------------------------

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

// ----------------------------------------------------------------------------
//  Output
// ----------------------------------------------------------------------------

out vec3 frag_position;
out vec3 frag_normal;
out vec2 frag_uv;

// ----------------------------------------------------------------------------
//  Entry point
// ----------------------------------------------------------------------------

void main() {
	vec4 pos = vec4(position, 1.0f);
	vec4 norm = vec4(normal, 0.0f);

	pos = view * model * pos;
	norm = normalMatrix * norm;

	frag_position = pos.xyz;
	frag_normal = norm.xyz;
	frag_uv = uv;

	gl_Position = projection * pos;
}
