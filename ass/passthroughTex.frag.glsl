#version 330

uniform sampler2D textureSampler;

in vec3 frag_normal;
in vec2 frag_texCoord;

out vec4 out_color;

void main() {
	vec3 sample = texture2D(textureSampler, frag_texCoord).rgb;
	out_color = vec4(sample, 1.0f);
}
