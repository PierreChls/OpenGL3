#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/glm.hpp>
#include <cstddef>

using namespace glimac;
struct Vertex2DColor{
	glm::vec2 position;
	glm::vec3 color;
	
	Vertex2DColor(){
		position=glm::vec2(0.0);
		color=glm::vec3(0.0);
	};
	
	Vertex2DColor(glm::vec2 p,glm::vec3 c){
		position=p;
		color=c;
	};
	
};

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

    /*********************************
     * HERE COMES THE INITIALIZATION CODE
     *********************************/ 
     
     // Création d'un seul VBO
     GLuint vbo;
     glGenBuffers(1, &vbo);
     
     //  Binding d'un VBO sur la cible GL_ARRAY_BUFFER:
     glBindBuffer(GL_ARRAY_BUFFER, vbo);
     
     // tableau avec coordonnées et envoi des données au buffer
	Vertex2DColor vertices[] = { 
		Vertex2DColor(glm::vec2(-1, 1), glm::vec3(1, 1, 1)),
		Vertex2DColor(glm::vec2(1, 1), glm::vec3(1, 1, 1)),
		Vertex2DColor(glm::vec2(1, -1), glm::vec3(1, 1, 1)),
		Vertex2DColor(glm::vec2(-1, -1), glm::vec3(1, 1, 1)),
		Vertex2DColor(glm::vec2(1, -1), glm::vec3(1, 1, 1)),
		Vertex2DColor(glm::vec2(-1, 1), glm::vec3(1, 1, 1))
	};
		 
     glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex2DColor), vertices, GL_STATIC_DRAW);
     
     // Débind du VBO et de sa cible
     glBindBuffer(GL_ARRAY_BUFFER, 0);
     
     // création du VAO
     GLuint vao;
     glGenVertexArrays(1, &vao);
     
     // Binding du VAO
     glBindVertexArray(vao);
     
	 // Spécification des attributs
     const GLuint VERTEX_ATTR_POSITION = 0;
     const GLuint VERTEX_ATTR_COLOR = 1;
     
     glBindBuffer(GL_ARRAY_BUFFER, vbo);
     
	 glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
	 glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), (const GLvoid*)offsetof(Vertex2DColor, position));
	 
	 glEnableVertexAttribArray(VERTEX_ATTR_COLOR);
	 glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor),  (const GLvoid*)offsetof(Vertex2DColor, color));
	 
	 glBindBuffer(GL_ARRAY_BUFFER, 0);
	 
	 // Débind du VAO
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
         * HERE COMES THE RENDERING CODE
         *********************************/
         // Nettoyage de la fenêtre
         glClear(GL_COLOR_BUFFER_BIT);
         
         //Dessin du triangle
         glBindVertexArray(vao);
         glDrawArrays(GL_TRIANGLES, 0, 6);
         glBindVertexArray(0);
         

        // Update the display
        windowManager.swapBuffers();
    }

	// Libération des ressources
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

    return EXIT_SUCCESS;
}
