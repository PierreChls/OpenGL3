#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <math.h>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>

using namespace glimac;
using namespace glm;

 struct Vertex2DUV{
    glm::vec2 position;
    glm::vec2 coord_texture;

    Vertex2DUV(){
        position=glm::vec2(0.0);
        coord_texture=glm::vec2(0.0);
    };
    
    Vertex2DUV(glm::vec2 p,glm::vec2 c){
        position=p;
        coord_texture=c;
    }

 };

glm::mat3 translate(float tx, float ty){
    glm::mat3 M = glm::mat3(glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(tx, ty, 1));
    return M;
}
glm::mat3 scale(float sx, float sy){
    glm::mat3 M = glm::mat3(glm::vec3(sx, 0, 0), glm::vec3(0, sy, 0), glm::vec3(0, 0, 1));
    return M;
}

glm::mat3 rotate(float b){
    float a = (b * M_PI) / 180.0f;
    glm::mat3 M = glm::mat3(glm::vec3(cos(a), sin(a), 0), glm::vec3(-sin(a), cos(a), 0), glm::vec3(0, 0, 1));
    return M;
}

int main(int argc, char** argv) {

    if(argc != 3){
        std::cerr << "Usage : ./programme shader.vs.glsl shader.fs.glsl \n" << std::endl;
        exit(0);
    }


    // Initialize SDL and open a window
    SDLWindowManager windowManager(800, 600, "GLImac");

    // Initialize glew for OpenGL3+ support
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

    FilePath applicationPath(argv[0]);
    Program program = loadProgram(applicationPath.dirPath() + "shaders/" + argv[1],
                                  applicationPath.dirPath() + "shaders/" + argv[2]);        
    program.use();

    //glGetUniformLocation(program.getGLId(), "uTime") permet de récupérer la location de la variable uniforme uTime
    //glUniform permet de rotater la variable uTime de 45

    GLint id_uModelMatrix = glGetUniformLocation(program.getGLId(), "uModelMatrix");
    GLint id_uModelColor = glGetUniformLocation(program.getGLId(), "uModelColor");
   
    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/



        // Création d'un seul VBO:
        GLuint vbo;
        glGenBuffers(1, &vbo);
        // A partir de ce point, la variable vbo contient l'identifiant d'un VBO
    
        // Binding d'un VBO sur la cible GL_ARRAY_BUFFER:
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        // On peut à présent modifier le VBO en passant par la cible GL_ARRAY_BUFFER

        //Maintenant que le VBO est bindé, on peut le modifier. Il faut envoyer les données de nos sommets à 
        //la carte graphique pour qu'elles soient placées dans le VBO.
        Vertex2DUV vertices[] = { Vertex2DUV(glm::vec2(-1, -1), glm::vec2(0, 0)), // premier sommet
                                  Vertex2DUV(glm::vec2(1, -1), glm::vec2(0, 0)), // deuxième sommet
                                  Vertex2DUV(glm::vec2(0, 1), glm::vec2(0, 0)),// troisième sommet
         };


        glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(Vertex2DUV), vertices, GL_STATIC_DRAW);
        
        //Debindage de la cible
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Création d'un seul VAO:
        GLuint vao;
        glGenVertexArrays(1, &vao);
        // A partir de ce point, la variable vbo contient l'identifiant d'un VAO

        //Binding du VAO
        glBindVertexArray(vao);

       // Spécification des attributs
        const GLuint VERTEX_ATTR_POSITION = 0;
        const GLuint VERTEX_ATTR_COORD = 1;
         
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
         
        glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
        glVertexAttribPointer(VERTEX_ATTR_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DUV), (const GLvoid*)offsetof(Vertex2DUV, position));
         
        glEnableVertexAttribArray(VERTEX_ATTR_COORD);
        glVertexAttribPointer(VERTEX_ATTR_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DUV),  (const GLvoid*)offsetof(Vertex2DUV, coord_texture));
         
        glBindBuffer(GL_ARRAY_BUFFER, 0);
     
     // Débind du VAO
     glBindVertexArray(0);

    // Application loop:
    bool done = false;

    float rotate_utime = 0;
    float rotate_speed = 1;

    vec3 red(1,0,0);
    vec3 blue(0,0,1);
    vec3 green(0,1,0);
    vec3 yellow(1,1,0);


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

         // Nettoyage de la fenêtre
         glClear(GL_COLOR_BUFFER_BIT);
         
         //Dessin du triangle
         glBindVertexArray(vao);

         //DIFFERENTES DE MATRICES DE TRANSFORMATION
         glm::mat3 M_scale = scale(0.25, 0.25); //Scale
         glm::mat3 M_translate = translate(0.50f, 0.50f); //Translate
         glm::mat3 M_translate2 = translate(-0.5, -0.5); //Translate
         glm::mat3 M_translate3 = translate(-0.5, 0.5); //Translate
         glm::mat3 M_translate4 = translate(0.5, -0.5); //Translate
         glm::mat3 M_rotate = rotate(rotate_utime); //Rotate
         glm::mat3 M_rotate2 = rotate(-rotate_utime); //Rotate

         //UN PREMIER TRIANGLE
            //On rotate le triangle
            glUniformMatrix3fv(id_uModelMatrix, 1, GL_FALSE, glm::value_ptr(M_translate * M_rotate * M_scale));
            glUniform3f(id_uModelColor,red.r,red.g,red.b);
            rotate_utime+=rotate_speed;
            glDrawArrays(GL_TRIANGLES, 0, 3);

        //UN DEUXIEME TRIANGLE
            //On translate le triangle
            glUniformMatrix3fv(id_uModelMatrix, 1, GL_FALSE, glm::value_ptr(M_translate2 * M_rotate2 * M_scale));
            glUniform3f(id_uModelColor,green.r,green.g,green.b);
            glDrawArrays(GL_TRIANGLES, 0, 3);

        //UN TROISIEME TRIANGLE
            //On translate le triangle
            glUniformMatrix3fv(id_uModelMatrix, 1, GL_FALSE, glm::value_ptr(M_translate3 * M_rotate * M_scale));
            glUniform3f(id_uModelColor,blue.r,blue.g,blue.b);
            glDrawArrays(GL_TRIANGLES, 0, 3);

        //UN TROISIEME TRIANGLE
            //On translate le triangle
            glUniformMatrix3fv(id_uModelMatrix, 1, GL_FALSE, glm::value_ptr(M_translate4 * M_rotate2 * M_scale));
            glUniform3f(id_uModelColor,yellow.r,yellow.g,yellow.b);
            glDrawArrays(GL_TRIANGLES, 0, 3);

         glBindVertexArray(0);

        // Update the display
        windowManager.swapBuffers();
    }

    return EXIT_SUCCESS;
}
