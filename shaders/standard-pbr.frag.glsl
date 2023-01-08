#version 330

#define PI 3.1415926535897932384626433832795028

// ----------------------------------------------------------------------------
//  Uniforms
// ----------------------------------------------------------------------------

uniform mat4 view;

uniform vec3 eye_position;

uniform sampler2D albedo_map;
uniform sampler2D roughness_map;
uniform sampler2D metalness_map;
uniform sampler2D occlusion_map;
uniform sampler2D emission_map;

uniform float ambience;

uniform int ID;

struct Light {
	vec3 position;
	vec3 direction;
	vec3 color;
	float intensity;
	float angle;
	float radius;
};

uniform Light light;

// ----------------------------------------------------------------------------
//  Input
// ----------------------------------------------------------------------------

in vec3 frag_position;
in vec3 frag_normal;
in vec2 frag_uv;

// ----------------------------------------------------------------------------
//  Output
// ----------------------------------------------------------------------------

layout(location = 0) out vec4 out_color;
layout(location = 1) out int out_id;

// ----------------------------------------------------------------------------
//  Entry point
// ----------------------------------------------------------------------------

float normal_distribution(vec3 normal, vec3 halway, float roughness) {
	float numerator = roughness*roughness;

	float 
}

// Estimates the probability of microfacet normal vectors (which the parameter 
// 'normal' is an average of) being aligned to the halfway vector given a 
// measure of the surface's roughness. Uses the Trowbridge-Reitz method.
float normal_distribution(vec3 normal, vec3 halway, float roughness)
{
    float roughness_pow2     = pow(roughness, 2);
    float dot_product  = max(dot(normal, halfway), 0.0);
    float denominator  = PI * (pow(dot_product, 2) * (roughness_pow2 - 1.0) + 1.0);
    return roughness_pow2 / denominator;
}

void main() {
	float specular_portion = texture(metalness_map, frag_uv).r;
	float diffuse_portion = 1.0 - specular_portion;

	// Diffuse: Oren-Nayar
	// NOTE: Using Lambertian diffuse for now
	vec3 diffuse_colour = texture(albedo_map, frag_uv).rgb / PI;

	// Specular: Cook-Torrence
	// - Normal distribution function: Trowbridge-Reitz
	float roughness = texture(roughness_map, frag_uv).r;

	vec3 normal = normalize(frag_normal);

	vec3 light_position = (view * vec4(light.position, 1.0)).xyz;
	vec3 to_light = normalize(lightPos - frag_position);
	vec3 to_eye = normalize(eye_position - frag_position);
	vec3 halfway = normalize(to_light + to_eye);

	vec3 distribution = normal_distribution(normal, halfway, roughness);

	// - Geometry function: Schlick-GGX


	// - Fresnel equation: Schlick
}
