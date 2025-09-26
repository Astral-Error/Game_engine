#pragma once
#include <glad/glad.h>
#include <SDL.h>

class Renderer{
    private:
        SDL_Window* sdlWindow;
        SDL_GLContext glContext;
        int screenW, screenH;
        GLuint shaderProgram, vao, vbo, colorShaderProgram, colorVao, colorVbo;
        GLint uniMVP, uniTex, uniAlpha, colorUniMVP, colorUniColor;
        void buildOrtho(float, float, float, float, float[]);
        void multMat4(const float[], const float[], float out[]);
        void identiyMat(float out[]);
    
    public:
        Renderer();
        ~Renderer();
        bool initOpenGL(SDL_Window*, int, int);
        void clear(float, float, float, float);
        void present();
        void drawTexture(GLuint, int, int, float, float, float, float, int, int, int ,int, bool, float, float);
        void drawTextureF(GLuint, float, float, float, float, float, float, float, float, float);
        void fillRect(float, float, float, float, float, float, float, float);
        void resize(int, int);
        void destroy();
};