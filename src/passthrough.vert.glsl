in vec3 position;
in vec3 normal;
in vec3 texCoord;

out vec3 out_normal;
out vec3 out_texCoord;

void main() {
	gl_Position = vec4(position.x, position.y, position.z, 1.0f);
}
