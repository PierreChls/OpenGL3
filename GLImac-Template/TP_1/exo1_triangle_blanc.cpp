#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>

using namespace glimac;

int main(int argc, char** argv) {
    // Initialize SDL and open a window
    SDLWindowManager windowManager(800, 600, "GLImac TRIANGLE BLANC");

    // Initialize glew for OpenGL3+ support
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

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
        GLfloat vertices[] = { -0.5f, -0.5f, 0.5f, -0.5f, 0.0f, 0.5f };
        glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
        
        //Debindage de la cible
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Création d'un seul VAO:
        GLuint vao;
        glGenVertexArrays(1, &vao);
        // A partir de ce point, la variable vbo contient l'identifiant d'un VAO

        //Binding du VAO
        glBindVertexArray(vao);

        //Activation des attributs
        const GLuint VERTEX_ATTR_POSITION = 0;
        glEnableVertexAttribArray(VERTEX_ATTR_POSITION);

        //Bindez à nouveau le VBO sur la cible GL_ARRAY_BUFFER.
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        //Utilisez la fonction glVertexAttribPointer pour spécifier le format de l'attribut de sommet position.
        const GLuint VERTEX_ATTR_SPEC = 0;
        glVertexAttribPointer(VERTEX_ATTR_SPEC,2,GL_FLOAT,GL_FALSE,2 * sizeof(GLfloat),0);
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
         glDrawArrays(GL_TRIANGLES,0, 3);
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
