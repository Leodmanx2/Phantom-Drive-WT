#version 110

uniform sampler2D textureSampler;

varying vec3 frag_normal;
varying vec2 frag_texCoord;

void main() {
	vec3 sample = texture2D(textureSampler, frag_texCoord).rgb;
	gl_FragColor = vec4(sample, 1.0f);
}
