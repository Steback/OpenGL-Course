#version 450

in vec4 vCol;
in vec2 texCoord;
in vec3 normal;

out vec4 colour;

struct DirectionalLight {
    vec3 colour;
    float ambientIntensity;
    vec3 direction;
    float diffuseIntensity;
};

uniform sampler2D tex;
uniform DirectionalLight directionalLight;

void main() {
    vec4 ambientColour = vec4(directionalLight.colour, 1.0f) * directionalLight.ambientIntensity;

    float diffuseFactor = max(dot(normalize(normal), normalize(directionalLight.direction)), 0.0f);
    vec4 diffuseColour = vec4(directionalLight.colour, 1.0f) * directionalLight.diffuseIntensity * diffuseFactor;

    colour = texture(tex, texCoord) * (ambientColour + diffuseColour);
}
