#include "model.h"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

using namespace glm;

Model::Model(const char *nam, GLint count, GLint start, GLenum primitive, 
             std::shared_ptr<ShaderProgram> shader, std::shared_ptr<Texture> tex) : 
        name(nam),
        drawCount(count),
        drawStart(start),
        drawPrimitive(primitive),
        program(shader),
        texture(tex),
        VBOs(0),
        VAO(0),
        loaded(false)
        //shininess(0.0f),
        //specularColor(1.0f, 1.0f, 1.0f)
{
}

Model::~Model()
{
    std::cout << "Unloading model " << name << std::endl;
    for (std::vector<VBOInfo>::iterator it = VBOs.begin(); it != VBOs.end(); ++it) {
        const VBOInfo &vbo = *it;
        glDeleteBuffers(1, &vbo.id);
    }
}

void Model::Render(const glm::mat4 &VP, const glm::mat4 &transform) const
{
    this->program->use();

    if (this->texture) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->texture->id());
        glUniform1i(this->shaderTextureID, 0);
    }

    glUniformMatrix4fv(this->shaderMatrixID, 1, GL_FALSE, &VP[0][0]);
    glUniformMatrix4fv(this->shaderTransformID, 1, GL_FALSE, &transform[0][0]);

    glBindVertexArray(this->VAO);
    glDrawArrays(this->drawPrimitive, this->drawStart, this->drawCount);

    if (this->texture) {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    glBindVertexArray(0);
    this->program->stopUsing();
}

void Model::AddVBO(const VBOInfo &vbo)
{
    VBOs.push_back(vbo);
}

void Model::ConnectToShader(const char *shaderAttrName, unsigned size, unsigned stride, unsigned offset)
{
    try {
        GLint attrib = program->attrib(shaderAttrName);
        glEnableVertexAttribArray(attrib);
        glVertexAttribPointer(attrib, size, GL_FLOAT, GL_FALSE, stride, (const GLvoid*) offset);
    } catch (const std::runtime_error& error) {
        std::cerr << "Model(" << name << "): runtime error " << error.what() << std::endl;
    }
}

void Model::Load()
{
    std::cout << "Loading model " << name << std::endl;
    assert(program && "Model has no shader program");

    if (texture) texture->load();
    program->load();

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    for (std::vector<VBOInfo>::iterator it = VBOs.begin(); it != VBOs.end(); ++it) {
        const VBOInfo &vbo = *it;
        assert(vbo.vertexData.size() && "No vertex data");

        glGenBuffers(1, (GLuint*)&vbo.id);
        glBindBuffer(GL_ARRAY_BUFFER, vbo.id);
        glBufferData(GL_ARRAY_BUFFER, vbo.vertexData.size() * sizeof(float), &(vbo.vertexData[0]), GL_STATIC_DRAW);
        switch(vbo.format) {
        case VertexFormat_XYZW_RGBA:
            ConnectToShader("vert", 4, 8*sizeof(float), 0);
            ConnectToShader("color", 4, 8*sizeof(float), 5 * sizeof(float));
            break;
        case VertexFormat_XYZ:
            ConnectToShader("vert", 3, 3*sizeof(float), 0);
            break;
        case VertexFormat_UV:
            ConnectToShader("vertexUV", 2, 2*sizeof(float), 0);
            break;
        case VertexFormat_NXYZ:
            ConnectToShader("vertexNormal", 3, 3*sizeof(float), 0);
            break;
        default:
            assert(false && "VertexFormat not supported");
            break;
        }
    }

    glBindVertexArray(0);

    GLuint shaderID = program->id();
    this->shaderMatrixID = glGetUniformLocation(shaderID, "VP");
    this->shaderTransformID = glGetUniformLocation(shaderID, "transform");
    if (this->texture) {
        this->shaderTextureID = glGetUniformLocation(shaderID, "myTextureSampler");
    }

    loaded = true;
}

bool Model::IsLoaded() const
{
    return loaded;
}