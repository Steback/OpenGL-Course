#ifndef MATERIAL_H
#define MATERIAL_H

#include <GL/glew.h>

class Material {
    public:
        Material(GLfloat _sIntensity, GLfloat _shine);
        ~Material();
        void UseMateril(GLfloat _specularIntensityLocation, GLfloat _shininessLocation);

    private:
        GLfloat specularIntesity;
        GLfloat shininess;
};

#endif