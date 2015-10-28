#include <glimac/SDLWindowManager.hpp>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <GL/glew.h>
#include <iostream>

using namespace glimac;

int main(int argc, char** argv) {

    if(argc != 3){
        std::cerr << "Usage : ./programme shader.vs.glsl shader.fs.glsl \n" << std::endl;
        exit(0);
    }

    // Initialize SDL and open a window
    SDLWindowManager windowManager(800, 600, "GLImac Triangle COULEUR");

    // Initialize glew for OpenGL3+ support
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;


    //On charge les shaders (triangle.vs.glsl et triangle.fs.glsl), les compile (à l'execution de l'application) et indique à OpenGL de les utiliser.
    FilePath applicationPath(argv[0]);
    Program program = loadProgram(applicationPath.dirPath() + "shaders/" + argv[1],
                                  applicationPath.dirPath() + "shaders/" + argv[2]);
    program.use();

    //HERE SHOULD COME THE INITIALIZATION CODE
     

        // Création d'un seul VBO:
        GLuint vbo;
        glGenBuffers(1, &vbo);
        // A partir de ce point, la variable vbo contient l'identifiant d'un VBO
    
        // Binding d'un VBO sur la cible GL_ARRAY_BUFFER:
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        // On peut à présent modifier le VBO en passant par la cible GL_ARRAY_BUFFER

        //Maintenant que le VBO est bindé, on peut le modifier. Il faut envoyer les données de nos sommets à 
        //la carte graphique pour qu'elles soient placées dans le VBO.
        GLfloat vertices[] = { -1.0f, 1.0f,
                                1.0f, 1.0f, 
                               -1.0f, -1.0f, 
                               -1.0f, -1.0f,
                                1.0f, -1.0f, 
                               1.0f, 1.0f 
         };
        glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
        
        //Debindage de la cible
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Création d'un seul VAO:
        GLuint vao;
        glGenVertexArrays(1, &vao);
        // A partir de ce point, la variable vbo contient l'identifiant d'un VAO

        //Binding du VAO
        glBindVertexArray(vao);

        //Activation des attributs
        const GLuint VERTEX_ATTR_POSITION = 0; //car le shader position a l'index numéro 0
        glEnableVertexAttribArray(VERTEX_ATTR_POSITION);

        //Bindez à nouveau le VBO sur la cible GL_ARRAY_BUFFER.
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        //Utilisez la fonction glVertexAttribPointer pour spécifier le format de l'attribut de sommet position.
        const GLuint VERTEX_ATTR_SPEC_POSITION = 0;
        //On garde 0, mais on n'est plus à 2 mais à 5 informations par sommet, d'où le chiffre 5 (j'explique pour Jeremie Quesnel qui comprenadra trèèèèèèèèèèèèèèèèèès bien comme ça).
        glVertexAttribPointer(VERTEX_ATTR_SPEC_POSITION,2,GL_FLOAT,GL_FALSE,2 * sizeof(GLfloat),(const GLvoid*)0);
        //Débindez le VBO de la cible GL_ARRAY_BUFFER.
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        //Debinder le VAO
        glBindVertexArray(0);


    //END OF TH INITIALIZATION

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
         //Begin
         //nettoyer lq fenetre
         glClear(GL_COLOR_BUFFER_BIT);
         // Binding d'un VBO sur la cible GL_ARRAY_BUFFER:
         glBindVertexArray(vao);
         glDrawArrays(GL_TRIANGLES,0, 12);
         glBindVertexArray(0);
         //END
        // Update the display
        windowManager.swapBuffers();
    }
    //La libération des ressources
    glDeleteBuffers(1,&vbo);
    glDeleteVertexArrays(1,&vao);
    return EXIT_SUCCESS;
}
