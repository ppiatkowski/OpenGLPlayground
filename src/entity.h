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
    Entity(const char *name, std::shared_ptr<Model> m);
    virtual ~Entity();

    void SetPosition(const glm::vec3 &pos);
    void SetRotation(const glm::vec3 &vec);
    void SetScale(const glm::vec3 &scale);

    glm::vec3 Rotation() const;

    void Render(const glm::mat4 &mvp) const;
    std::string Name() const;

private:
    void CalculateTransform();

    std::string name;
    bool matrixDirty;
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation; // euler angles TODO convert to quaternions

    std::shared_ptr<Model> model;
    glm::mat4 transform;
};
#endif // _ENTITY_H_