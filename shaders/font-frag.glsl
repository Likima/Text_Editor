#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform float time; // Uniform variable to pass the time
uniform vec3 textColor;
uniform bool isKeyword; // Uniform variable to indicate if the text is a keyword
uniform float xpos;
uniform float ypos;

vec3 hsv2rgb(float h, float s, float v)
{
    float c = v * s;
    float x = c * (1.0 - abs(mod(h * 6.0, 2.0) - 1.0));
    float m = v - c;
    vec3 rgb;

    if (0.0 <= h && h < 1.0/6.0) {
        rgb = vec3(c, x, 0.0);
    } else if (1.0/6.0 <= h && h < 1.0/3.0) {
        rgb = vec3(x, c, 0.0);
    } else if (1.0/3.0 <= h && h < 1.0/2.0) {
        rgb = vec3(0.0, c, x);
    } else if (1.0/2.0 <= h && h < 2.0/3.0) {
        rgb = vec3(0.0, x, c);
    } else if (2.0/3.0 <= h && h < 5.0/6.0) {
        rgb = vec3(x, 0.0, c);
    } else {
        rgb = vec3(c, 0.0, x);
    }

    return rgb + m;
}

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    
    vec3 finalColor = textColor;
    
    if (isKeyword)
    {
        // Calculate hue based on time

        float hue = mod(time * 0.2, 1.0); // Adjust 0.1 to change the speed of the hue shift
        finalColor = hsv2rgb(sin(hue), 1.0, 1.0);
    }
    
    color = vec4(finalColor, 1.0) * sampled;
}
