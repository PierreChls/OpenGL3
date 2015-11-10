#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/glm.hpp>
#include <glimac/Sphere.hpp>
#include <glimac/Image.hpp>
#include <cstddef>

using namespace glimac;

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

    GLint uMVPMatrix = glGetUniformLocation(program.getGLId(), "uMVPMatrix");
    GLint uMVMatrix = glGetUniformLocation(program.getGLId(), "uMVMatrix");
    GLint uNormalMatrix = glGetUniformLocation(program.getGLId(), "uNormalMatrix");

    glEnable(GL_DEPTH_TEST); //permet d'activer le test de profondeur du GPU. Sans cet appel de fonction, certains triangles non visible viendraient recouvrir des triangles situés devant.

    glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.0f), 800.0f/600.0f, 0.1f, 100.0f); //param perspective(float fovy, float aspect, float znear, float far)
    glm::mat4 MVMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 0, -5));
    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

        Sphere MySphere(1, 32, 16);

        // Création d'un seul VBO:
        GLuint vbo;
        glGenBuffers(1, &vbo);
        // A partir de ce point, la variable vbo contient l'identifiant d'un VBO
    
        // Binding d'un VBO sur la cible GL_ARRAY_BUFFER:
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        // On peut à présent modifier le VBO en passant par la cible GL_ARRAY_BUFFER

        //Maintenant que le VBO est bindé, on peut le modifier. Il faut envoyer les données de nos sommets à 
        //la carte graphique pour qu'elles soient placées dans le VBO.
        glBufferData(GL_ARRAY_BUFFER, MySphere.getVertexCount() * sizeof(ShapeVertex), MySphere.getDataPointer(), GL_STATIC_DRAW);
        
        //Debindage de la cible
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Création d'un seul VAO:
        GLuint vao;
        glGenVertexArrays(1, &vao);
        // A partir de ce point, la variable vbo contient l'identifiant d'un VAO

        //Binding du VAO
        glBindVertexArray(vao);

        const GLuint VERTEX_ATTR_POSITION = 0;
        const GLuint VERTEX_ATTR_NORMAL = 1;
        const GLuint VERTEX_ATTR_TEXCOORD = 2;
     
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

         
        glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
        glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)offsetof(ShapeVertex, position));

        glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
        glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)offsetof(ShapeVertex, normal));
         
         glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORD);
         glVertexAttribPointer(VERTEX_ATTR_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex),  (const GLvoid*)offsetof(ShapeVertex, texCoords));
         
         glBindBuffer(GL_ARRAY_BUFFER, 0);
         
         // Débind du VAO
         glBindVertexArray(0);


    uint nbLune = 32;


    std::vector<glm::vec3> AxesRotation;
    std::vector<glm::vec3> AxesTranslation;
    //std::vector<glm::vec3> AxesScale;

    for(uint i=0; i <= nbLune; i++){
        AxesRotation.push_back(glm::sphericalRand(1.0f)); //Rempli Axes rotation comme une pile ou comme un tableau, on pourra l'utiliser AxesRotation[i]
        AxesTranslation.push_back(glm::sphericalRand(2.0f));
        //AxesScale.push_back(glm::sphericalRand(2.0f));
    }
    
    // Application loop:
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
         // Nettoyage de la fenêtre
         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
         glBindVertexArray(vao);

         glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
         glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
         glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

         glDrawArrays(GL_TRIANGLES, 0, MySphere.getVertexCount());


         for(uint i=0; i <= AxesRotation.size(); i++){

            glm::mat4 newMVMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 0, -5)); // Translation
            newMVMatrix = glm::rotate(newMVMatrix, windowManager.getTime(), AxesRotation[i]); // Translation * Rotation
            newMVMatrix = glm::translate(newMVMatrix, AxesTranslation[i]); // Translation * Rotation * Translation
            newMVMatrix = glm::scale(newMVMatrix, glm::vec3(0.2, 0.2, 0.2)); // Translation * Rotation * Translation * Scale
            //newMVMatrix = glm::scale(newMVMatrix, AxesTranslation[i]);

            glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * newMVMatrix));
            glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
            glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

            glDrawArrays(GL_TRIANGLES, 0, MySphere.getVertexCount());
         }


         glBindVertexArray(0);
         //glBindTexture(GL_TEXTURE_2D, 0);


        // Update the display
        windowManager.swapBuffers();
    }

    return EXIT_SUCCESS;
}
