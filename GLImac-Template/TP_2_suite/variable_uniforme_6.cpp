#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/glm.hpp>
#include <glimac/Image.hpp>
#include <cstddef>

using namespace glimac;
using namespace glm;

struct Vertex2DUV{
    glm::vec2 position;
    glm::vec2 texture;
    
    Vertex2DUV(){
        position=glm::vec2(0.0);
        texture=glm::vec2(0.0);
    };

    Vertex2DUV(glm::vec2 p,glm::vec2 t){
        position=p;
        texture=t;
    };
};

glm::mat3 translate(float tx, float ty){
    glm::mat3 M = glm::mat3(glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(tx, ty, 1));
    return M;
}
glm::mat3 scale(float sx, float sy){
    glm::mat3 M = glm::mat3(glm::vec3(sx, 0, 0), glm::vec3(0, sy, 0), glm::vec3(0, 0, 1));
    return M;
}

glm::mat3 rotate(float a){
    glm::mat3 M = glm::mat3(glm::vec3(cos(a), sin(a), 0), glm::vec3(-sin(a), cos(a), 0), glm::vec3(0, 0, 1));
    return M;
}

int main(int argc, char** argv) {
    

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

    GLint uTimeLocation = glGetUniformLocation(program.getGLId(), "uModelMatrix");
     GLint uColorLocation = glGetUniformLocation(program.getGLId(), "uColor");
    GLint uTexture = glGetUniformLocation(program.getGLId(), "uTexture");
   /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/
     // Création d'un seul VBO
     GLuint vbo;
     glGenBuffers(1, &vbo);

     //Création d'une texture
      std::unique_ptr<Image> img=loadImage("/home/6im2/pcharles/Documents/OpenGL/GLImac-Template/TP_2_suite/assets/textures/triforce.png");
      std::unique_ptr<Image> texture_brique=loadImage("/home/6im2/pcharles/Documents/OpenGL/GLImac-Template/TP_2_suite/assets/textures/brique.png");

     if(img == NULL){
            std::cerr << "La texture triforce n'a pas pu etre chargée. \n" << std::endl;
            exit(0);
     }

     GLuint texture;
     glGenTextures(1,&texture);
     
     
     //  Binding d'un VBO sur la cible GL_ARRAY_BUFFER:
     glBindBuffer(GL_ARRAY_BUFFER, vbo);
     
     //Binding de la texture
     glBindTexture(GL_TEXTURE_2D,texture);
    

     // glBindTexture(GL_TEXTURE_2D,texture2);
     // glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img2->getWidth(),img2->getHeight(),0,GL_RGBA,GL_FLOAT,img2->getPixels());
     // //filtrage 
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

     //debindage de la texture
     glBindTexture(GL_TEXTURE_2D,0);
     // tableau avec coordonnées et envoi des données au buffer
    Vertex2DUV vertices[] = { Vertex2DUV(glm::vec2(-1, -1), glm::vec2(0.5, 0)), // premier sommet
                              Vertex2DUV(glm::vec2(1, -1), glm::vec2(0, 1)), // deuxième sommet
                              Vertex2DUV(glm::vec2(0, 1), glm::vec2(1, 1)),// troisième sommet
    };

    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(Vertex2DUV), vertices, GL_STATIC_DRAW);

     // Débind du VBO et de sa cible
     glBindBuffer(GL_ARRAY_BUFFER, 0);
     
     // création du VAO
     GLuint vao;
     glGenVertexArrays(1, &vao);
     
     // Binding du VAO
     glBindVertexArray(vao);
     
     // Spécification des attributs
     const GLuint VERTEX_ATTR_POSITION = 0;
     const GLuint VERTEX_ATTR_TEXTURE = 1;
     
     glBindBuffer(GL_ARRAY_BUFFER, vbo);

     
     glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
     glVertexAttribPointer(VERTEX_ATTR_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DUV), (const GLvoid*)offsetof(Vertex2DUV, position));
     
     glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);
     glVertexAttribPointer(VERTEX_ATTR_TEXTURE, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DUV),  (const GLvoid*)offsetof(Vertex2DUV, texture));
     
     glBindBuffer(GL_ARRAY_BUFFER, 0);
     
     // Débind du VAO
     glBindVertexArray(0);
    float uTime = 0,b=0,bb=2;

   
    float rotate_utime = 0;
    float rotate_speed = 0.01;

    vec3 red(1,0,0);
    vec3 blue(0,0,1);
    vec3 green(0,1,0);
    vec3 yellow(1,1,0);
  
   
    // u loop:
    bool done = false;
    while(!done) {
        
        // u loop:
        SDL_Event e;
        while(windowManager.pollEvent(e)) {
            if(e.type == SDL_QUIT) {
                done = true; // Leave the loop after this iteration
            }
        }

        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

         glm::mat3 M_scale = scale(0.25, 0.25); //Scale
         glm::mat3 M_translate = translate(0.50f, 0.50f); //Translate
         glm::mat3 M_translate2 = translate(-0.5, -0.5); //Translate
         glm::mat3 M_translate3 = translate(-0.5, 0.5); //Translate
         glm::mat3 M_translate4 = translate(0.5, -0.5); //Translate
         glm::mat3 M_rotate = rotate(rotate_utime); //Rotate
         glm::mat3 M_rotate2 = rotate(-rotate_utime); //Rotate

         // Nettoyage de la fenêtre
         glClear(GL_COLOR_BUFFER_BIT);
         glBindTexture(GL_TEXTURE_2D, texture);
         glUniform1i(uTexture, 0);
         glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,img->getWidth(),img->getHeight(),0,GL_RGBA,GL_FLOAT,img->getPixels());

         //UN PREMIER TRIANGLE
            //On rotate le triangle
            glBindVertexArray(vao);
            glUniformMatrix3fv(uTimeLocation, 1, GL_FALSE, glm::value_ptr(M_translate * M_rotate * M_scale));
            glUniform3f(uColorLocation,red.r,red.g,red.b);
            rotate_utime+=rotate_speed;
            glDrawArrays(GL_TRIANGLES, 0, 3);
            glBindVertexArray(0);

        //UN DEUXIEME TRIANGLE
            //On translate le triangle
            glBindVertexArray(vao);
            glUniformMatrix3fv(uTimeLocation, 1, GL_FALSE, glm::value_ptr(M_translate2 * M_rotate2 * M_scale));
            glUniform3f(uColorLocation,green.r,green.g,green.b);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            glBindVertexArray(0);


         glBindTexture(GL_TEXTURE_2D, 0);


        // Update the display
        windowManager.swapBuffers();
    }
    glDeleteTextures(1,&texture);
    return EXIT_SUCCESS;
}
