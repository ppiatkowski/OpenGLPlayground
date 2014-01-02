#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <GL/glew.h>
#include <string>

// TODO implement resource manager that handles textures (among others)

class Texture 
{
public:
    enum Format {
        Format_BMP = 0,
        Format_DDS,
        Format_TGA,
    };

    Texture(Format format, std::string path);
    virtual ~Texture();

    GLuint load();
    Format format() const;
    GLuint id() const;
    std::string path() const;

private:
    // Load a .BMP file using our custom loader
    GLuint loadBMP_custom();

    // Load a .TGA file using GLFW's own loader
    GLuint loadTGA_glfw();

    // Load a .DDS file using GLFW's own loader
    GLuint loadDDS();

    std::string texturePath;
    Format textureFormat;
    GLuint textureId;

};



#endif // _TEXTURE_H_