#version 330

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 normalMatrix;

in vec3 position;
in vec3 normal;
in vec2 texCoord;

out vec3 frag_position;
out vec3 frag_normal;
out vec2 frag_texCoord;

void main() {
	vec4 pos = vec4(position, 1.0f);
	vec4 norm = vec4(normal, 0.0f);

	pos = view * model * pos;
	norm = normalMatrix * norm;

	frag_position = pos.xyz;
	frag_normal = norm.xyz;
	frag_texCoord = texCoord;

	gl_Position = projection * pos;
}
