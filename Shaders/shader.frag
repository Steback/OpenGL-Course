#version 330

in vec4 vCol;
in vec2 texCoord;
in vec3 normal;
in vec3 FragPos;

out vec4 colour;

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

struct Light {
    vec3 colour;
    float ambientIntensity;
    float diffuseIntensity;
};

struct DirectionalLight {
    Light base;
    vec3 direction;
};

struct PointLight {
    Light base;
    vec3 position;
    float constant;
    float linear;
    float exponent;
};

struct SpotLight {
    PointLight base;
    vec3 direction;
    float edge;
};

struct Material {
    float specularIntensity;
    float shininess;
};

uniform int pointLightCount;
uniform int spotLightCount;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform sampler2D tex;
uniform Material material;

uniform vec3 eyePosition;

vec4 CalcLightByDirection(Light light, vec3 direction) {
    vec4 ambientColour = vec4(light.colour, 1.0f) * light.ambientIntensity;

    float diffuseFactor = max(dot(normalize(normal), normalize(direction)), 0.0f);
    vec4 diffuseColour = vec4(light.colour * light.diffuseIntensity * diffuseFactor, 1.0f);

    vec4 specularColour = vec4(0, 0, 0, 0);

    if( diffuseFactor > 0.0f ) {
        vec3 fragToEye = normalize(eyePosition - FragPos);
        vec3 reflectedVertex = normalize(reflect(direction, normalize(normal)));

        float specularFactor = dot(fragToEye, reflectedVertex);

        if( specularFactor > 0.0f ) {
            specularFactor = pow(specularFactor, material.shininess);
            specularColour = vec4(light.colour * material.specularIntensity * specularFactor, 1.0f);
        }
    }

    return (ambientColour + diffuseColour + specularColour);
}

vec4 CalcPointLight(PointLight _pLight) {
    vec3 direction = FragPos - _pLight.position;
    float distance = length(direction);
    direction = normalize(direction);

    vec4 colour = CalcLightByDirection(_pLight.base, direction);
    float attenuation = _pLight.exponent * distance * distance +
    _pLight.linear * distance +
    _pLight.constant;

    return (colour / attenuation);
}


vec4 CalcPointLights() {
    vec4 totalColour = vec4(0, 0, 0, 0);

    for( int i = 0; i < pointLightCount; i++ ) {
        totalColour += CalcPointLight(pointLights[i]);
    }

    return totalColour;
}

vec4 CalcSpotLight(SpotLight _sLight) {
    vec3 rayDirection = normalize(FragPos - _sLight.base.position);
    float slFactor = dot(rayDirection, _sLight.direction);

    if ( slFactor > _sLight.edge ) {
        return CalcPointLight(_sLight.base);
    } else {
        return vec4(0, 0, 0, 0);
    }
}

vec4 CalcSpotLights() {
    vec4 totalColour = vec4(0, 0, 0, 0);

    for( int i = 0; i < spotLightCount; i++ ) {
        totalColour += CalcSpotLight(spotLights[i]);
    }

    return totalColour;
}

void main() {
    vec4 finalColour = CalcLightByDirection(directionalLight.base, directionalLight.direction);
    finalColour += CalcPointLights();
    finalColour += CalcSpotLights();

    colour = texture(tex, texCoord) * finalColour;
}
