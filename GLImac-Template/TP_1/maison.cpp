#include <glimac/SDLWindowManager.hpp>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/glm.hpp>
#include <GL/glew.h>
#include <iostream>
#include <cstddef>

using namespace glimac;

struct Vertex2DColor{
     glm::vec2 position;
     glm::vec3 color;
     Vertex2DColor(){
        position = glm::vec2(0.0f);
        color = glm::vec3(0.0f);
     };
     Vertex2DColor(glm::vec2 p, glm::vec3 c){
        position = p;
        color = c;
     };
};

int main(int argc, char** argv) {
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
    Program program = loadProgram(applicationPath.dirPath() + "shaders/triangle.vs.glsl",
                                  applicationPath.dirPath() + "shaders/triangle.fs.glsl");
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

        int N=200,i,k=0, taille_maison=9, taille_jardin=6, taille_ciel=6;
        float r=0.15;
        float center_sun = 0.75;
        Vertex2DColor vertices[3*N+taille_maison+taille_jardin+taille_ciel];

        glm::vec3 rouge=glm::vec3(1.0,0,0);
        glm::vec3 bleu=glm::vec3(0,0,1);
        glm::vec3 vert=glm::vec3(0,1,0);
        glm::vec3 jaune=glm::vec3(0.9,0.7,0);

        vertices[0] = Vertex2DColor(glm::vec2(-1, -1), bleu); //CIEL
        vertices[1] = Vertex2DColor(glm::vec2(-1, 1), bleu);
        vertices[2] = Vertex2DColor(glm::vec2(1, -1), bleu);
        vertices[3] = Vertex2DColor(glm::vec2(-1, 1), bleu);
        vertices[4] = Vertex2DColor(glm::vec2(1, -1), bleu);
        vertices[5] = Vertex2DColor(glm::vec2(1, 1), bleu);

        vertices[6] = Vertex2DColor(glm::vec2(-1, -1), vert); //JARDIN
        vertices[7] = Vertex2DColor(glm::vec2(-1, 0), vert);
        vertices[8] = Vertex2DColor(glm::vec2(1, -1), vert);
        vertices[9] = Vertex2DColor(glm::vec2(1, -1), vert);
        vertices[10] = Vertex2DColor(glm::vec2(-1, 0), vert);
        vertices[11] = Vertex2DColor(glm::vec2(1,0),vert);

        vertices[12] = Vertex2DColor(glm::vec2(-0.25, -0.25), rouge); //MAISON
        vertices[13] = Vertex2DColor(glm::vec2(-0.25, 0.25), rouge);
        vertices[14] = Vertex2DColor(glm::vec2(0.25, -0.25), rouge);
        vertices[15] = Vertex2DColor(glm::vec2(-0.25, 0.25), rouge);
        vertices[16] = Vertex2DColor(glm::vec2(0.25, 0.25), rouge);
        vertices[17] = Vertex2DColor(glm::vec2(0.25, -0.25), rouge);
        vertices[18] = Vertex2DColor(glm::vec2(-0.25, 0.25), rouge);
        vertices[19] = Vertex2DColor(glm::vec2(0.25, 0.25), rouge);
        vertices[20] = Vertex2DColor(glm::vec2(0, 0.5), rouge);

        for(i=1; i<=N;i++){
                float x1=(r*glm::cos((2*i*glm::pi<float>())/N)), y1=(r*glm::sin((2*i*glm::pi<float>())/N));
                float x2=(r*glm::cos((2*(i-1)*glm::pi<float>())/N)), y2=(r*glm::sin((2*(i-1)*glm::pi<float>())/N));
                vertices[taille_maison+taille_jardin+taille_ciel+k]=Vertex2DColor(glm::vec2(center_sun, center_sun),jaune);
                vertices[taille_maison+taille_jardin+taille_ciel+k+1]=Vertex2DColor(glm::vec2(x1+center_sun,y1+center_sun),jaune);
                vertices[taille_maison+taille_jardin+taille_ciel+k+2]=Vertex2DColor(glm::vec2(x2+center_sun,y2+center_sun),jaune);
                k=k+3;
        }

        glBufferData(GL_ARRAY_BUFFER, (3*N+taille_maison+taille_jardin+taille_ciel) * sizeof(Vertex2DColor), vertices, GL_STATIC_DRAW);
        
        //Debindage de la cible
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Création d'un seul VAO:
        GLuint vao;
        glGenVertexArrays(1, &vao);
        // A partir de ce point, la variable vbo contient l'identifiant d'un VAO

        //Binding du VAO
        glBindVertexArray(vao);

        //Activation des attributs
        const GLuint VERTEX_ATTR_POSITION = 3; //car le shader position a l'index numéro 3
        const GLuint VERTEX_ATTR_COLOR = 8; //car le shader couleur a l'index numéro 8
        glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
        glEnableVertexAttribArray(VERTEX_ATTR_COLOR);

        //Bindez à nouveau le VBO sur la cible GL_ARRAY_BUFFER.
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        //Utilisez la fonction glVertexAttribPointer pour spécifier le format de l'attribut de sommet position.
        const GLuint VERTEX_ATTR_SPEC_POSITION = 3;
        const GLuint VERTEX_ATTR_SPEC_COLOR = 8;
        //On garde 0, mais on n'est plus à 2 mais à 5 informations par sommet, d'où le chiffre 5 (j'explique pour Jeremie Quesnel qui comprenadra trèèèèèèèèèèèèèèèèèès bien comme ça).
        glVertexAttribPointer(VERTEX_ATTR_SPEC_POSITION,2,GL_FLOAT,GL_FALSE,sizeof(Vertex2DColor), (const GLvoid*)(offsetof(Vertex2DColor, position)));
        glVertexAttribPointer(VERTEX_ATTR_SPEC_COLOR,3,GL_FLOAT,GL_FALSE,sizeof(Vertex2DColor), (const GLvoid*)(offsetof(Vertex2DColor, color)));
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
         glDrawArrays(GL_TRIANGLES,0, 3*N+taille_maison+taille_jardin+taille_ciel);
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
