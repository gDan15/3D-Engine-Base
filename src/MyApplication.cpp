#include "Application.h"
#include <iostream>


using namespace std;

class MyApplication: public Application
{
private:
    Program* program = NULL;
    Renderer *renderer = NULL;
    float angle = 0;
    
public:
    MyApplication(int argc, char* argv[]);
    void update(int time);
    void render();
    void setup();
    void teardown();
};

MyApplication::MyApplication(int argc, char* argv[]) : Application(argc, argv) {}

void MyApplication::update(int elapsedTime)
{
    angle = 180.0f / 1000 * elapsedTime;
    mat4 modelViewMatrix = rotate(angle, 0.0f, 1.0f, 0.0f);
    renderer->setMatrix("modelViewMatrix", modelViewMatrix);
}

void MyApplication::render()
{
    renderer->render();   
}

void MyApplication::setup()
{   
    program = new Program(); 
    program->addShader(Shader::fromFile("shaders/perspective.vert"));
    program->addShader(Shader::fromFile("shaders/white.frag"));
    program->compile();

    renderer = program->createRenderer();

    mat4 projectionMatrix = perspective(1.0f, 640.0f/480, 1.0f, 100.0f);
    mat4 lookatMatrix = lookat(vec3(50, 50, 50), vec3(0, 0, 0), vec3(0, 1, 0));
    projectionMatrix = projectionMatrix * lookatMatrix;

    mat4 modelViewMatrix = rotate(angle, 0.0f, 1.0f, 0.0f); 
    renderer->setMatrix("projectionMatrix", projectionMatrix);
    renderer->setMatrix("modelViewMatrix", modelViewMatrix);

    //IBO data
	GLuint indexData[] = { 0, 1, 2, 0, 2, 3 };

    //VBO data
    vec3 vertexData[] =
    {
        vec3(-0.5f, -0.5f, 0.0f),
        vec3(0.5f, -0.5f, 0.0f),
        vec3(0.5f,  0.5f, 0.0f),
        vec3(-0.5f,  0.5f, 0.0f)
    };

    renderer->put("VertexPos3D", vertexData, 4);
    renderer->index(indexData, 6);
    
    setClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
}

void MyApplication::teardown()
{
    delete renderer;
    delete program;
}

int main(int argc, char** argv)
{
    try 
    {
        MyApplication app = MyApplication(argc, argv); 
	    return app.run();
    }
	catch(Exception e)
    {
        cout << e.getMessage() << endl;
        return 1;
    }
}