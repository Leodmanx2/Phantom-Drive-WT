#version 330

in vec3 frag_normal;
in vec2 frag_texCoord;

out vec4 out_color;

void main() {
	out_color = vec4(frag_normal, 1.0f);
}
