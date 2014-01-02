// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <iostream>
#include <memory>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GL/glfw.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "shader.h"
#include "texture.h"
#include "controls.h"

#include "vertexData.h"
#include "entity.h"
#include "model.h"

const int LINE_CNT = 100;
const float GRID_SIZE = 100.0f;

class Renderer
{
public:
    Renderer() {}

    void AddEntity(const std::shared_ptr<Entity> &entity) 
    {
        entities.push_back(entity);
    }

    void Render()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Compute the MVP matrix from keyboard and mouse input
        computeMatricesFromInputs();
        glm::mat4 ProjectionMatrix = getProjectionMatrix();
        glm::mat4 ViewMatrix = getViewMatrix();
        glm::mat4 ModelMatrix = glm::mat4(1.0);//glm::rotate(glm::mat4(1.0), 45.f, glm::vec3(0,1,0));
        glm::mat4 VP = ProjectionMatrix * ViewMatrix;

        // render loop
        std::list< std::shared_ptr<Entity> >::iterator it = entities.begin();
        for (; it != entities.end(); ++it) {
            (*it)->Render(VP);
        }

        // Swap buffers
        glfwSwapBuffers();
    }

private:
    std::list< std::shared_ptr<Entity> > entities;
};

int init( void )
{
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return -1;
    }

    glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    if( !glfwOpenWindow( 1024, 768, 0,0,0,0, 32,0, GLFW_WINDOW ) )
    {
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        glfwTerminate();
        return -1;
    }

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    glfwSetWindowTitle( "Playground" );

    // Ensure we can capture the escape key being pressed below
    glfwEnable( GLFW_STICKY_KEYS );
    glfwSetMousePos(1024/2, 768/2);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS); 

    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);

    return 0;
}

int main( void )
{
    if (init()) {
        return 1;
    }

    Renderer *renderer = new Renderer();

    /* GRID */
    float newGridVert[8*LINE_CNT*2*2];
    int index = 0;
    const float INTERVAL = GRID_SIZE / LINE_CNT;
    for (int i = -LINE_CNT / 2; i < LINE_CNT / 2; i++) {
        // pos1
        newGridVert[index] = i * INTERVAL;
        newGridVert[index + 1] = 1.0f;
        newGridVert[index + 2] = GRID_SIZE / 2.f;
        newGridVert[index + 3] = 1.0f;
        // color1
        newGridVert[index + 4] = 1.0f;
        newGridVert[index + 5] = 1.0f;
        newGridVert[index + 6] = 1.0f;
        newGridVert[index + 7] = 1.0f;

        // pos2
        newGridVert[index + 8] = i * INTERVAL;
        newGridVert[index + 9] = 1.0f;
        newGridVert[index + 10] = -GRID_SIZE / 2.f;
        newGridVert[index + 11] = 1.0f;
        // color2
        newGridVert[index + 12] = 1.0f;
        newGridVert[index + 13] = 1.0f;
        newGridVert[index + 14] = 1.0f;
        newGridVert[index + 15] = 1.0f;

        index += 16;
    }
    for (int i = -LINE_CNT / 2; i < LINE_CNT / 2; i++) {
        // pos1
        newGridVert[index] = GRID_SIZE / 2.f;
        newGridVert[index + 1] = 1.0f;
        newGridVert[index + 2] = i * INTERVAL;
        newGridVert[index + 3] = 1.0f;
        // color1
        newGridVert[index + 4] = 1.0f;
        newGridVert[index + 5] = 1.0f;
        newGridVert[index + 6] = 1.0f;
        newGridVert[index + 7] = 1.0f;

        // pos2
        newGridVert[index + 8] = -GRID_SIZE / 2.f;
        newGridVert[index + 9] = 1.0f;
        newGridVert[index + 10] = i * INTERVAL;
        newGridVert[index + 11] = 1.0f;
        // color2
        newGridVert[index + 12] = 1.0f;
        newGridVert[index + 13] = 1.0f;
        newGridVert[index + 14] = 1.0f;
        newGridVert[index + 15] = 1.0f;

        index += 16;
    }

    std::shared_ptr<Model> gridModel = std::make_shared<Model>("grid");
    gridModel->drawCount = LINE_CNT*2*2;
    gridModel->drawStart = 0;
    gridModel->drawPrimitive = GL_LINES;
    gridModel->program = std::make_shared<ShaderProgram>("resources/shaders/GridAlpha.vertex", "resources/shaders/GridAlpha.frag");

    VBOInfo vbo;
    vbo.format = VertexFormat_XYZW_RGBA;
    vbo.vertexData = std::vector<float>(newGridVert, newGridVert + index);
    gridModel->AddVBO(vbo);

    gridModel->Load();

    std::shared_ptr<Entity> grid = std::make_shared<Entity>(gridModel);
    renderer->AddEntity(grid);

    std::shared_ptr<Entity> grid2 = std::make_shared<Entity>(gridModel);
    grid2->SetPosition(glm::vec3(0.0, 10.0, 0.0));
    renderer->AddEntity(grid2);

    /* CUBE */
    std::shared_ptr<Model> cubeModel = std::make_shared<Model>("cube");
    cubeModel->drawCount = 12 * 3;
    cubeModel->drawStart = 0;
    cubeModel->drawPrimitive = GL_TRIANGLES;
    cubeModel->program = std::make_shared<ShaderProgram>("resources/shaders/CubeShader.vertex", "resources/shaders/CubeShader.frag");
    cubeModel->texture = std::make_shared<Texture>(Texture::Format_DDS, std::string("resources/textures/uvtemplate.DDS"));

    VBOInfo cubeVert;
    cubeVert.format = VertexFormat_XYZ;
    cubeVert.vertexData = std::vector<float>(g_vertex_buffer_data, g_vertex_buffer_data + 12*3*3);
    cubeModel->AddVBO(cubeVert);

    VBOInfo cubeUV;
    cubeUV.format = VertexFormat_UV;
    cubeUV.vertexData = std::vector<float>(g_uv_buffer_data, g_uv_buffer_data + 12*2*3);
    cubeModel->AddVBO(cubeUV);
    cubeModel->Load();

    std::shared_ptr<Entity> cube = std::make_shared<Entity>(cubeModel);
    renderer->AddEntity(cube);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    do {

        renderer->Render();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS &&
        glfwGetWindowParam( GLFW_OPENED ) );

    delete renderer;

    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    return 0;
}
