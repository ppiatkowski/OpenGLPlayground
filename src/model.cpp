#include "model.h"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

using namespace glm;

Model::Model(const char *nam) : 
        name(nam),
        texture(NULL),
        program(NULL),
        VBOs(0),
        VAO(0),
        drawPrimitive(GL_TRIANGLES),
        drawStart(0),
        drawCount(0),
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

void Model::AddVBO(const VBOInfo &vbo)
{
    VBOs.push_back(vbo);
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
        GLint attrib;
        switch(vbo.format) {
        case VertexFormat_XYZW_RGBA:
            attrib = program->attrib("vert");
            glEnableVertexAttribArray(attrib);
            glVertexAttribPointer(attrib, 4, GL_FLOAT, GL_FALSE, 8*sizeof(float), 0);
            attrib = program->attrib("color");
            glEnableVertexAttribArray(attrib);
            glVertexAttribPointer(attrib, 4, GL_FLOAT, GL_FALSE, 8*sizeof(float), (const GLvoid*)(5 * sizeof(float)));
            break;
        case VertexFormat_XYZ:
            attrib = program->attrib("vert");
            glEnableVertexAttribArray(attrib);
            glVertexAttribPointer(attrib, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0); // czy zero
            break;
        case VertexFormat_UV:
            attrib = program->attrib("vertexUV");
            glEnableVertexAttribArray(attrib);
            glVertexAttribPointer(attrib, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0); // czy zero
            break;
        default:
            assert(false && "VertexFormat not supported");
            break;
        }
    }

    glBindVertexArray(0);
    loaded = true;
}

bool Model::IsLoaded() const
{
    return loaded;
}