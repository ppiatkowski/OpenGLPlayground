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
    std::shared_ptr<ShaderProgram> program;
    std::shared_ptr<Texture> texture;

    GLenum drawPrimitive;
    GLint drawStart;
    GLint drawCount;
    GLuint VAO;
    
    //GLfloat shininess;
    //glm::vec3 specularColor;


    Model(const char *name);
    virtual ~Model();

    //bool Load(const std::string &modelPath);
    //bool Load(unsigned int vertexCnt, float vertices[], float colors[]);

    void AddVBO(const VBOInfo &vbo);
    void Load();
    //void Render(const glm::mat4 &MVP) const;
    bool IsLoaded() const;

private:
    std::vector<VBOInfo> VBOs;
    bool loaded;
    std::string name;
    
};

#endif // _MODEL_H_