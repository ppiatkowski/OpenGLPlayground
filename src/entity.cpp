#include "entity.h"

#include "model.h"

#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

Entity::Entity(const char *nam, std::shared_ptr<Model> m) : 
    name(nam),
    model(m), 
    transform(glm::mat4(1)),
    position(glm::vec3(0)),
    rotation(glm::vec3(0)),
    scale(glm::vec3(1))

{
    assert(model && "Entity requires a model");
    assert(model->IsLoaded() && "Model must be loaded");
}

Entity::~Entity()
{
}
        

void Entity::Render(const glm::mat4 &VP) const
{
    assert(model && "Entity without a model cannot be rendered");

    model->Render(VP, transform);
}

void Entity::CalculateTransform()
{
    if (!matrixDirty)
        return;

    transform = glm::translate(mat4(1), position);
    
    // TODO do not use Euler angles, use quaternions instead
    transform = glm::rotate(transform, rotation.x, glm::vec3(1.0, 0.0, 0.0));
    transform = glm::rotate(transform, rotation.y, glm::vec3(0.0, 1.0, 0.0));
    transform = glm::rotate(transform, rotation.z, glm::vec3(0.0, 0.0, 1.0));

    transform = glm::scale(transform, scale);
    matrixDirty = false;
}

void Entity::SetPosition(const glm::vec3 &pos)
{
    if (position != pos) {
        position = pos;

        matrixDirty = true;
        CalculateTransform();
    }
}

void Entity::SetRotation(const glm::vec3 &rot)
{
    if (rotation != rot) {
        rotation = rot;

        matrixDirty = true;
        CalculateTransform();
    }
}

void Entity::SetScale(const glm::vec3 &s)
{
    if (scale != s) {
        scale = s;

        matrixDirty = true;
        CalculateTransform();
    }
}

glm::vec3 Entity::Rotation() const
{
    return rotation;
}

std::string Entity::Name() const
{
    return name;
}