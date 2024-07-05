#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform float time; // Uniform variable to pass the time
uniform vec3 textColor;

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0/3.0, 1.0/3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    
    // Calculate hue based on time
    float hue = mod(time * 0.1, 1.0); // Adjust 0.1 to change the speed of the hue shift
    vec3 chromaColor = hsv2rgb(vec3(hue, 1.0, 1.0));
    
    color = vec4(chromaColor, 1.0) * sampled;
}
