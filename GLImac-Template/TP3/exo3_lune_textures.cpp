#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/glm.hpp>
#include <glimac/Image.hpp>
#include <glimac/Sphere.hpp>
#include <glimac/Geometry.hpp>
#include <cstddef>
#include <vector>

using namespace glimac;

int main(int argc, char** argv) {
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

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    Sphere sphere(1, 64, 32);

    //Chargement des shaders
    FilePath applicationPath(argv[0]);
    Program program = loadProgram(
        applicationPath.dirPath() + "shaders/3D.vs.glsl",
        applicationPath.dirPath() + "shaders/tex3D2.fs.glsl"
    );
    program.use();

    //Obtention de l'id de la variable uniforme
    GLint uMVPMatrix = glGetUniformLocation(program.getGLId(), "uMVPMatrix");
    GLint uMVMatrix = glGetUniformLocation(program.getGLId(), "uMVMatrix");
    GLint uNormalMatrix = glGetUniformLocation(program.getGLId(), "uNormalMatrix");
    GLint uTexture = glGetUniformLocation(program.getGLId(), "uTexture");

    glEnable(GL_DEPTH_TEST);

    //Création d'un VBO
    GLuint vbo;
    glGenBuffers(1, &vbo);

    //Bindind du vbo sur la cible
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    
    //Création d'une texture
        std::unique_ptr<Image> EarthMap=loadImage("/Users/PierreCharles/Documents/IMAC/IMAC2/Synthèse d'image/TD_MacOS/GLImac-Template/assets/textures/EarthMap.jpg");
        std::unique_ptr<Image> MoonMap=loadImage("/Users/PierreCharles/Documents/IMAC/IMAC2/Synthèse d'image/TD_MacOS/GLImac-Template/assets/textures/MoonMap.jpg");

        if(EarthMap == NULL || MoonMap == NULL){
            std::cerr << "Une des textures n'a pas pu etre chargée. \n" << std::endl;
            exit(0);
        }

        GLuint texture;
        glGenTextures(1,&texture);
    
        // Binding d'un VBO sur la cible GL_ARRAY_BUFFER:
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        // On peut à présent modifier le VBO en passant par la cible GL_ARRAY_BUFFER

        //Binding de la texture
        glBindTexture(GL_TEXTURE_2D,texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //debindage de la texture
        glBindTexture(GL_TEXTURE_2D,0);
    

    //Puis on envois les données à la CG
    glBufferData(GL_ARRAY_BUFFER, sphere.getVertexCount()*sizeof(ShapeVertex), sphere.getDataPointer(), GL_STATIC_DRAW);

    //Débindind du vbo de la cible pour éviter de le remodifier
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //Création du VAO
    GLuint vao;
    glGenVertexArrays(1, &vao);

    //Binding du vao (un seul à la fois)
    glBindVertexArray(vao);

    //Dire à OpenGL qu'on utilise le VAO
    const GLuint VERTEX_ATTR_POSITION = 0;
    const GLuint VERTEX_ATTR_NORMAL = 1;
    const GLuint VERTEX_ATTR_TEXCOORDS = 2;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORDS);

    //Indiquer à OpenGL où trouver les sommets
    //Bindind du vbo sur la cible
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    //Spécification du format de l'attribut de sommet position
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)offsetof(ShapeVertex, position));
    glVertexAttribPointer(VERTEX_ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)offsetof(ShapeVertex, normal));
    glVertexAttribPointer(VERTEX_ATTR_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)offsetof(ShapeVertex, texCoords));
    //Débindind du vbo de la cible pour éviter de le remodifier
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //Débindind du vao de la cible pour éviter de le remodifier
    glBindVertexArray(0);



    std::vector<glm::vec3> AxesRotation;
    std::vector<glm::vec3> Translations;
    for (int i = 0; i < 32; ++i)
    { 
        AxesRotation.push_back(glm::sphericalRand(1.0f));
        Translations.push_back(glm::sphericalRand(2.0f));
    }


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

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindTexture(GL_TEXTURE_2D, texture);
         glUniform1i(uTexture, 0);
         glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,EarthMap->getWidth(),EarthMap->getHeight(),0,GL_RGBA,GL_FLOAT,EarthMap->getPixels());
         glBindVertexArray(vao);


        glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), 800.f/600.f, 0.1f, 100.f);
    

        glm::mat4 MVMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 0, -5));
        glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
        glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
        glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
        glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());

        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,MoonMap->getWidth(),MoonMap->getHeight(),0,GL_RGBA,GL_FLOAT,MoonMap->getPixels());

        for (int i = 0; i < AxesRotation.size(); ++i)
        {
            glm::mat4 MVMatrix2 = glm::translate(glm::mat4(1), glm::vec3(0, 0, -5));
            glm::mat4 NormalMatrix2 = glm::transpose(glm::inverse(MVMatrix2));
            MVMatrix2 = glm::rotate(MVMatrix2, windowManager.getTime(), AxesRotation.at(i)); // Translation * Rotation
            MVMatrix2 = glm::translate(MVMatrix2, Translations.at(i)); // Translation * Rotation * Translation
            MVMatrix2 = glm::scale(MVMatrix2, glm::vec3(0.2, 0.2, 0.2)); // Translation * Rotation * Translation * Scale
            glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix2));
            glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix2));
            glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix2));
            glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
        }
        

        glBindVertexArray(0);
         glBindTexture(GL_TEXTURE_2D, 0);

        // Update the display
        windowManager.swapBuffers();
    }

    return EXIT_SUCCESS;
}
