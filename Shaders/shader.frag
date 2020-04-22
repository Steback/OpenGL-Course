#version 450

in vec4 vCol;
in vec2 texCoord;

out vec4 colour;

struct DirectionalLight {
    vec3 colour;
    float ambientIntensity;
};

uniform sampler2D tex;
uniform DirectionalLight directionalLight;

void main() {
    vec4 ambientColour = vec4(directionalLight.colour, 1.0f) * directionalLight.ambientIntensity;
    colour = texture(tex, texCoord) * ambientColour;
}
