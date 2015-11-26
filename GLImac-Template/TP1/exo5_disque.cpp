#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/glm.hpp>
#include <cstddef>
#include <vector>

using namespace glimac;

struct Vertex2DColor
{
    glm::vec2 position;
    glm::vec3 color;

    Vertex2DColor(){}
    Vertex2DColor(glm::vec2 position, glm::vec3 color):position(position), color(color){}
};

int main(int argc, char** argv) {
    int N=50;

    // Initialize SDL and open a window
    SDLWindowManager windowManager(800, 600, "GLImac");

    glewExperimental = GL_TRUE;
    // Initialize glew for OpenGL3+ support
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

    //Chargement des shaders
    FilePath applicationPath(argv[0]);
    Program program = loadProgram(
        applicationPath.dirPath() + "shaders/triangle.vs.glsl",
        applicationPath.dirPath() + "shaders/triangle.fs.glsl"
    );
    program.use();

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    //Création d'un VBO
    GLuint vbo;
    glGenBuffers(1, &vbo);

    //Bindind du vbo sur la cible
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    //Création d'un tableau de float pour stocker les points du VBO
    //On utilise la structure nouvellement créée
    std::vector<Vertex2DColor> vertices;
    
    for(int i=0;i<N;++i){
        vertices.push_back(
            Vertex2DColor(glm::vec2(0, 0), glm::vec3(0, 1, 0))
        );
        vertices.push_back(
            Vertex2DColor(glm::vec2(0.5*glm::cos(((2*glm::pi<float>())/N)*i), 0.5*glm::sin(((2*glm::pi<float>())/N)*i)) , glm::vec3(0, 0, 1)
        ));
        vertices.push_back(
            Vertex2DColor(glm::vec2(0.5*glm::cos(((2*glm::pi<float>())/N)*(i+1)), 0.5*glm::sin(((2*glm::pi<float>())/N)*(i+1))), glm::vec3(1, 0, 0)
        ));
    }
    //Puis on envois les données à la CG //3*N pour le nombre de sommets pour N triangles
    glBufferData(GL_ARRAY_BUFFER, 3*N*sizeof(Vertex2DColor), vertices.data(), GL_STATIC_DRAW);

    //Débindind du vbo de la cible pour éviter de le remodifier
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    //Création du VAO
    GLuint vao;
    glGenVertexArrays(1, &vao);

    //Binding du vao (un seul à la fois)
    glBindVertexArray(vao);

    //Dire à OpenGL qu'on utilise le VAO
    const GLuint VERTEX_ATTR_POSITION = 3;
    const GLuint VERTEX_ATTR_COLOR = 8;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_COLOR);

    //Indiquer à OpenGL où trouver les sommets
    //Bindind du vbo sur la cible
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    //Spécification du format de l'attribut de sommet position
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), (const GLvoid*)offsetof(Vertex2DColor, position));
    glVertexAttribPointer(8, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), (const GLvoid*)offsetof(Vertex2DColor, color));
    //Débindind du vbo de la cible pour éviter de le remodifier
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //Débindind du vao de la cible pour éviter de le remodifier
    glBindVertexArray(0);




    // Application loop:
    bool done = false;
    while(!done) {
        // Event loop:
        SDL_Event e;
        while(windowManager.pollEvent(e)) {
            if(e.type == SDL_QUIT) {
                done = true; // Leave the loop after this iteration
            }
        }

        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

        //On nettoit la fenêtre
        glClear(GL_COLOR_BUFFER_BIT);

        //On rebind le vao
        glBindVertexArray(vao);

        //On dessine le triangle //3*N pour le nombre de sommets pour N triangles
        glDrawArrays(GL_TRIANGLES, 0, N*3);

        //Débindind du vao de la cible pour éviter de le remodifier
        glBindVertexArray(0);







        // Update the display
        windowManager.swapBuffers();
    }

    //On libère le GPU des ressources VBO et VAO
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);


    return EXIT_SUCCESS;
}
