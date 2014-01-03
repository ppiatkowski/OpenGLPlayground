#ifndef _MODEL_H_
#define _MODEL_H_

#include "shader.h"
#include "texture.h"

#include <string>
#include <memory>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

enum VertexFormat {
    VertexFormat_XYZ = 0,
    VertexFormat_UV,
    VertexFormat_NXYZ, // normal XYZ
    VertexFormat_XYZW,
    VertexFormat_XYZ_UV,
    VertexFormat_XYZW_UV,
    VertexFormat_XYZ_RGBA,
    VertexFormat_XYZW_RGBA,
};

struct VBOInfo {
    GLuint id;
    VertexFormat format;
    std::vector<float> vertexData;
};

class Model
{
public:
    Model(const char *nam, GLint drawCount, GLint drawStart, GLenum drawPrimitive, std::shared_ptr<ShaderProgram> program, std::shared_ptr<Texture> texture);
    virtual ~Model();

    void Render(const glm::mat4 &VP, const glm::mat4 &transform) const;

    //bool Load(const std::string &modelPath);
    //bool Load(unsigned int vertexCnt, float vertices[], float colors[]);

    void AddVBO(const VBOInfo &vbo); // can be used to manually add vertices to the model
    void Load();
    bool IsLoaded() const;

private:
    void ConnectToShader(const char *shaderAttrName, unsigned size, unsigned stride, unsigned offset);

    std::shared_ptr<ShaderProgram> program;
    std::shared_ptr<Texture> texture;

    GLenum drawPrimitive;
    GLint drawStart;
    GLint drawCount;

    GLuint VAO;
    std::vector<VBOInfo> VBOs;
    bool loaded;
    std::string name;

    GLuint shaderMatrixID;
    GLuint shaderTransformID;
    GLuint shaderTextureID;

    //GLfloat shininess;
    //glm::vec3 specularColor;
    
};

#endif // _MODEL_H_