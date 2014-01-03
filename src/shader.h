#ifndef _SHADER_H_
#define _SHADER_H_

#include <string>
#include <stdexcept>
#include <GL/glew.h>


#ifdef _MSC_VER
#define __func__ __FUNCTION__
#endif

class ShaderProgram
{
public:
    ShaderProgram(const char *vertexShaderPath, const char *fragmentShaderPath);
    virtual ~ShaderProgram();

    void load();
    GLint attrib(const GLchar *attribName) const;
    GLuint id() const;

    void use() const;
    void stopUsing() const;

private:
    GLuint programId;
    std::string vertexShaderPath;
    std::string fragmentShaderPath;
};

#endif // _SHADER_H_
