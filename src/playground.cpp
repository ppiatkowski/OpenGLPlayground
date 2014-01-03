// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <map>
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

class App
{
public:
    App() {}

    void AddEntity(const std::shared_ptr<Entity> &entity)
    {
        entities[entity->Name()] = entity;
    }

    std::shared_ptr<Entity> GetEntity(std::string name)
    {
        return entities[name];
    }

    void Update(double dt)
    {
        // update loop
        std::map< std::string, std::shared_ptr<Entity> >::iterator it = entities.begin();
        for (; it != entities.end(); ++it) {
            // TODO
        }
    }

    void Render()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Compute the MVP matrix from keyboard and mouse input
        computeMatricesFromInputs();
        glm::mat4 ProjectionMatrix = getProjectionMatrix();
        glm::mat4 ViewMatrix = getViewMatrix();
        glm::mat4 VP = ProjectionMatrix * ViewMatrix;

        // render loop
        std::map< std::string, std::shared_ptr<Entity> >::iterator it = entities.begin();
        for (; it != entities.end(); ++it) {
            (*it).second->Render(VP);
        }

        // Swap buffers
        glfwSwapBuffers();
    }

private:
    std::map< std::string, std::shared_ptr<Entity> > entities;
};

App *app = NULL;

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
    if( !glfwOpenWindow( SCREEN_WIDTH, SCREEN_HEIGHT, 0,0,0,0, 32,0, GLFW_WINDOW ) )
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
    glfwSetMousePos(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);

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

    app = new App();

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

    std::shared_ptr<Model> gridModel = std::make_shared<Model>("grid", LINE_CNT*2*2, 0, GL_LINES, 
                                                               std::make_shared<ShaderProgram>("resources/shaders/GridAlpha.vertex", "resources/shaders/GridAlpha.frag"),
                                                               std::shared_ptr<Texture>(NULL));

    VBOInfo vbo;
    vbo.format = VertexFormat_XYZW_RGBA;
    vbo.vertexData = std::vector<float>(newGridVert, newGridVert + index);
    gridModel->AddVBO(vbo);

    gridModel->Load();

    std::shared_ptr<Entity> grid = std::make_shared<Entity>("gridBottom", gridModel);
    app->AddEntity(grid);

    std::shared_ptr<Entity> grid2 = std::make_shared<Entity>("gridTop", gridModel);
    grid2->SetPosition(glm::vec3(0.0, 10.0, 0.0));
    app->AddEntity(grid2);

    /* CUBE */
    std::shared_ptr<Model> cubeModel = std::make_shared<Model>("cube", 12*3, 0, GL_TRIANGLES, 
                                                               std::make_shared<ShaderProgram>("resources/shaders/CubeShader.vertex", "resources/shaders/CubeShader.frag"),
                                                               std::make_shared<Texture>(Texture::Format_DDS, std::string("resources/textures/uvtemplate.DDS")) );

    VBOInfo cubeVert;
    cubeVert.format = VertexFormat_XYZ;
    cubeVert.vertexData = std::vector<float>(g_vertex_buffer_data, g_vertex_buffer_data + 12*3*3);
    cubeModel->AddVBO(cubeVert);

    VBOInfo cubeUV;
    cubeUV.format = VertexFormat_UV;
    cubeUV.vertexData = std::vector<float>(g_uv_buffer_data, g_uv_buffer_data + 12*2*3);
    cubeModel->AddVBO(cubeUV);

    VBOInfo cubeNormals;
    cubeNormals.format = VertexFormat_NXYZ;
    cubeNormals.vertexData = std::vector<float>(g_normal_buffer_data, g_normal_buffer_data + 12*3*3);
    cubeModel->AddVBO(cubeNormals);

    cubeModel->Load();

    std::shared_ptr<Entity> cube = std::make_shared<Entity>("cube1", cubeModel);
    app->AddEntity(cube);

    std::shared_ptr<Entity> cube2 = std::make_shared<Entity>("cube2", cubeModel);
    cube2->SetPosition(glm::vec3(3.0, 5.0, -4.0));
    cube2->SetRotation(glm::vec3(45.0, 45.0, 45.0));
    cube2->SetScale(glm::vec3(1.0, 0.5, 0.5));
    app->AddEntity(cube2);


    // LIGHT SOURCE
    std::shared_ptr<Model> lightModel = std::make_shared<Model>("light", 12*3, 0, GL_TRIANGLES, 
                                                               std::make_shared<ShaderProgram>("resources/shaders/CubeShader.vertex", "resources/shaders/CubeShader.frag"),
                                                               std::shared_ptr<Texture>(NULL));

    VBOInfo lightVert;
    lightVert.format = VertexFormat_XYZ;
    lightVert.vertexData = std::vector<float>(g_vertex_buffer_data, g_vertex_buffer_data + 12*3*3);
    lightModel->AddVBO(cubeVert);
    lightModel->Load();

    std::shared_ptr<Entity> lightSource = std::make_shared<Entity>("lightSource", lightModel);
    lightSource->SetScale(glm::vec3(0.2, 0.2, 0.2));
    lightSource->SetPosition(glm::vec3(-1.0, 3.0, -2.0));
    app->AddEntity(lightSource);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    double prevFrame = glfwGetTime();
    do {
        double now = glfwGetTime();
        double dt = now - prevFrame;

        // update here
        app->Update(dt);

        prevFrame = now;

        app->Render();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS &&
        glfwGetWindowParam( GLFW_OPENED ) );

    delete app;

    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    return 0;
}
