#version 330 core

in vec2 tex_coord;
in vec3 frag_coord;
in vec3 normal;

out vec4 FragColor;

uniform float u_time;

uniform vec3 camera_pos;

uniform sampler2D tex;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material model_material;

struct Light {
    vec3 position;

    Material material;

    float constant_term;
    float linear_term;
    float quad_term;
};
uniform Light light;

#define PI 3.14159265359

float almostIdentity(float x, float m, float n)
{
    if (x > m) return x;

    float a = 2.0 * n - m;
    float b = 2.0 * m - 3.0 * n;
    float t = x/m;

    return (a*t + b)*t*t + n;
}

float impulse( float k, float x )
{
    float h = k*x;
    return h*exp(1.0-h);
}

float cubicPulse( float c, float w, float x )
{
    x = abs(x - c);
    if( x>w ) return 0.0;
    x /= w;
    return 1.0 - x*x*(3.0-2.0*x);
}

float plot(vec2 st, float pct)
{
    return smoothstep(pct - 0.02, pct, st.y) -
           smoothstep(pct, pct + 0.02, st.y);
}

float ex1(float x, float ex)
{
    return 1.0 - pow(abs(x), ex);
}

float ex2(float x, float d, float ex)
{
    return pow(cos(PI * x / d), ex);
}

void main() {

    vec4 tex_color = texture(tex, tex_coord);

    vec4 ambient = vec4(model_material.ambient * light.material.ambient, 1.0);

    vec3 norm = normalize(normal);
    vec3 light_dir = normalize(light.position - frag_coord);

    vec3 light_ref = reflect(-light_dir, normal);
    vec3 view_dir = normalize(camera_pos - frag_coord);
    vec3 halfway_vec = normalize(light_dir + view_dir);

    float diff = max(dot(norm, light_dir), 0.0);
    vec4 diffuse = vec4((diff * model_material.diffuse) * light.material.diffuse, 1.0);

    float spec_val = pow(max(dot(norm, halfway_vec), 0.0), model_material.shininess);
    vec4 spec = vec4((spec_val * model_material.specular) * light.material.specular, 1.0);

    float dist = length(light.position - frag_coord);
    float constant_term = 1.0;
    float linear_term = 0.0014;
    float quad_term = 0.000007;

    float attenuation = 1.0 / (constant_term + (linear_term * dist) + (quad_term * pow(dist, 2)));

    vec4 lighting = (ambient * attenuation) + (diffuse * attenuation) + (spec * attenuation);

    vec4 color = lighting * tex_color;
    FragColor = color;

}