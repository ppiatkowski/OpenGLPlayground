#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <string>
#include <memory>

#include <GL/glew.h>
#include <glm/glm.hpp>

class Model;

class Entity
{
public:
    Entity(std::shared_ptr<Model> m);
    virtual ~Entity();

    void SetPosition(const glm::vec3 &pos);
    void SetRotation(const glm::vec3 &vec);
    void SetScale(const glm::vec3 &scale);

    void Render(const glm::mat4 &mvp) const;

private:
    void CalculateTransform();

    bool matrixDirty;
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation; // euler angles TODO convert to quaternions

    std::shared_ptr<Model> model;
    glm::mat4 transform;
    GLuint shaderMatrixID;
    GLuint shaderTransformID;
    GLuint shaderTextureID;
};
#endif // _ENTITY_H_