#version 110

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

attribute vec3 position;
attribute vec3 normal;
attribute vec2 texCoord;

varying vec3 frag_normal;
varying vec2 frag_texCoord;

void main() {
	frag_normal = normal;
	frag_texCoord = texCoord;
	gl_Position = projection * view * model * vec4(position, 1.0f);
}
