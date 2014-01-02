#include "entity.h"

#include "model.h"

#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

Entity::Entity(std::shared_ptr<Model> m) : 
    model(m), 
    shaderMatrixID(0),
    transform(glm::mat4(1)),
    position(glm::vec3(0)),
    rotation(glm::vec3(0)),
    scale(glm::vec3(1))

{
    assert(model && "Entity requires a model");
    assert(model->IsLoaded() && "Model must be loaded");
    GLuint shaderID = model->program->id();
    this->shaderMatrixID = glGetUniformLocation(shaderID, "VP");
    this->shaderTransformID = glGetUniformLocation(shaderID, "transform");
    if (model->texture) {
        this->shaderTextureID = glGetUniformLocation(shaderID, "myTextureSampler");
    }
}

Entity::~Entity()
{
}
        

void Entity::Render(const glm::mat4 &VP) const
{
    assert(model && "Entity without a model cannot be rendered");
    model->program->use();

    if (model->texture) {
        GLuint textureId = model->texture->id();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glUniform1i(shaderTextureID, 0);
    }

    glUniformMatrix4fv(shaderMatrixID, 1, GL_FALSE, &VP[0][0]);
    glUniformMatrix4fv(shaderTransformID, 1, GL_FALSE, &transform[0][0]);

    glBindVertexArray(model->VAO);
    glDrawArrays(model->drawPrimitive, model->drawStart, model->drawCount);

    if (model->texture) {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    glBindVertexArray(0);
    model->program->stopUsing();
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