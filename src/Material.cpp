#include "Material.h"

Material::Material(GLfloat _sIntensity, GLfloat _shine) : specularIntesity(_sIntensity), shininess(_shine) {  }

Material::~Material() = default;

void Material::UseMateril(GLfloat _specularIntensityLocation, GLfloat _shininessLocation) {
    glUniform1f(_specularIntensityLocation, specularIntesity);
    glUniform1f(_shininessLocation, shininess);
}