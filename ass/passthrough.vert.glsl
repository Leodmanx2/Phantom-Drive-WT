#version 330

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

in vec3 position;
in vec3 normal;
in vec2 texCoord;

out vec3 frag_position;
out vec3 frag_normal;
out vec2 frag_texCoord;

void main() {
	vec4 pos = vec4(position, 1.0f);
	vec4 norm = vec4(normal, 1.0f);

	pos = model * pos;
	norm = model * norm;

	frag_position = pos.xyz;
	frag_normal = normal.xyz;
	frag_texCoord = texCoord;

	gl_Position = projection * view * pos;
}
