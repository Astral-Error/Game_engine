#pragma once
#include <glad/glad.h>
#include <SDL.h>
#include <vector>

struct Quad {
    float x, y, w, h;
    float u0, v0, u1, v1;
    float alpha;
    GLuint texID;
};

class Renderer{
    private:
        SDL_Window* sdlWindow;
        SDL_GLContext glContext;
        int screenW, screenH;
        GLuint shaderProgram, vao, vbo, colorShaderProgram, colorVao, colorVbo, ebo;
        GLint uniMVP, uniTex, uniAlpha, colorUniMVP, colorUniColor;
        std::vector<Quad> quadBatch;
        float proj[16];
        void buildOrtho(float, float, float, float, float[]);
        void multMat4(const float[], const float[], float out[]);
        void identityMat4(float out[]);
    
    public:
        Renderer();
        ~Renderer();
        bool initOpenGL(SDL_Window*, int, int);
        void clear(float, float, float, float);
        void present();

        // Batch API
        void beginBatch();
        void submitQuad(GLuint texID, float dstX, float dstY, float dstW, float dstH, float u0, float v0, float u1, float v1, float alpha = 1.0f);
        void endBatch();

        void drawTexture(GLuint texID, int texWidth, int texHeight, float dstX, float dstY, float dstW, float dstH, int srcX=0, int srcY=0, int srcW=0, int srcH=0, bool flipX=false, float angleDeg=0.0f, float alpha=1.0f);
        void drawTextureF(GLuint texID, float dstX, float dstY, float dstW, float dstH, float u0=0.0f, float v0=0.0f, float u1=1.0f, float v1=1.0f, float alpha=1.0f);
        void fillRect(float, float, float, float, float, float, float, float);
        void flushBatch();
        void resize(int, int);
        void destroy();
};