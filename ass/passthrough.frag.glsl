#version 330

uniform sampler2DArray textureSampler;

in vec3 frag_normal;
in vec2 frag_texCoord;

out vec4 out_color;

void main() {
	vec3 texCoord3D = vec3(frag_texCoord, 0);
	vec3 sample = texture(textureSampler, texCoord3D).rgb;
	out_color = vec4(sample, 1.0f);
}
