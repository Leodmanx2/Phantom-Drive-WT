#version 330

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

in vec3 position;
in vec3 normal;
in vec2 texCoord;

out vec3 out_normal;
out vec2 out_texCoord;

void main() {
	gl_Position = projection * view * model * vec4(position, 1.0f);
}
