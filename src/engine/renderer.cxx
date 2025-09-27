#include "renderer.hxx"
#include "glUtils.hxx"
#include <iostream>
#include <cmath>
#include <glad/glad.h>



static const char* textured_vert = R"(
#version 330 core
layout(location=0) in vec2 aPos;
layout(location=1) in vec2 aUV;
uniform mat4 uMVP;
out vec2 vUV;
void main(){
    vUV = aUV;
    gl_Position = uMVP * vec4(aPos, 0.0, 1.0);
}
)";

static const char* textured_frag = R"(
#version 330 core
in vec2 vUV;
out vec4 FragColor;
uniform sampler2D uTex;
uniform float uAlpha;
void main(){
    vec4 c = texture(uTex, vUV);
    FragColor = vec4(c.rgb, c.a * uAlpha);
}
)";

static const char* color_vert = R"(
#version 330 core
layout(location=0) in vec2 aPos;
uniform mat4 uMVP;
void main(){
    gl_Position = uMVP * vec4(aPos, 0.0, 1.0);
}
)";

static const char* color_frag = R"(
#version 330 core
out vec4 FragColor;
uniform vec4 uColor;
void main(){
    FragColor = uColor;
}
)";

Renderer::Renderer():
    sdlWindow(nullptr), glContext(nullptr), screenW(800), screenH(600),
    shaderProgram(0), vao(0), vbo(0), colorShaderProgram(0), colorVao(0), colorVbo(0)
    {}

Renderer::~Renderer(){destroy();}

void Renderer::buildOrtho(float left, float right, float bottom, float top, float out[16]){
    identityMat4(out);
    out[0] = 2/(right - left);
    out[5] = 2 / (top - bottom);
    out[10] = -1;
    out[12] = -(right + left) / (right - left);
    out[13] = -(top + bottom) / (top - bottom);
}

void Renderer::multMat4(const float a[16], const float b[16], float out[16]){
    float tmp[16];
    for(int r=0;r<4;r++){
        for(int c=0;c<4;c++){
            float v=0.0f;
            for(int k=0;k<4;k++){
                v += a[k*4 + r] * b[c*4 + k];
            }
            tmp[c*4 + r] = v;
        }
    }
    for(int i=0;i<16;i++) out[i]=tmp[i];
}

void Renderer::identityMat4(float out[16]){
    std::fill(out, out + 16, 0.0f); 
    out[0] = 1;
    out[5] = 1;
    out[10] = 1;
    out[15] = 1;
}

static bool compileAndLinkProgram(const char* vsrc, const char* fsrc, GLuint &prog){
    GLuint vs = glutils::compileShader(GL_VERTEX_SHADER, vsrc);
    if(!vs) return false;
    GLuint fs = glutils::compileShader(GL_FRAGMENT_SHADER, fsrc);
    if(!fs){ glDeleteShader(vs); return false; }
    prog = glutils::linkProgram(vs, fs);
    glDeleteShader(vs); glDeleteShader(fs);
    return prog != 0;
}

bool Renderer::initOpenGL(SDL_Window* sdlWindow, int w, int h){
    if(!sdlWindow) return false;
    this->sdlWindow = sdlWindow;
    screenW = w;
    screenH = h;
    buildOrtho(0.0f, (float)screenW, (float)screenH, 0.0f, proj); // The elements in 'proj' are now fixed
    glContext = SDL_GL_CreateContext(sdlWindow);
    if(!glContext){
        std::cerr<<"Error creating glContext: "<<SDL_GetError()<<std::endl;
        return false;
    }
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    if(!compileAndLinkProgram(textured_vert, textured_frag, shaderProgram)){
        std::cerr << "Failed to compile/link textured shader\n";
        return false;
    }
    uniMVP = glGetUniformLocation(shaderProgram, "uMVP");
    uniTex = glGetUniformLocation(shaderProgram, "uTex");
    uniAlpha = glGetUniformLocation(shaderProgram, "uAlpha");

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*4*(2+2), nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0); glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,sizeof(float)*4,(void*)(0));
    glEnableVertexAttribArray(1); glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,sizeof(float)*4,(void*)(sizeof(float)*2));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    if(!compileAndLinkProgram(color_vert, color_frag, colorShaderProgram)){
        std::cerr << "Failed to compile/link color shader\n";
        return false;
    }
    colorUniMVP = glGetUniformLocation(colorShaderProgram, "uMVP");
    colorUniColor = glGetUniformLocation(colorShaderProgram, "uColor");
    glGenVertexArrays(1, &colorVao);
    glGenBuffers(1, &colorVbo);
    glBindVertexArray(colorVao);
    glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*4*2, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0); glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE, sizeof(float)*2, (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport(0,0,screenW, screenH);
    return true;
}

void Renderer::clear(float r, float g, float b, float a){
    glClearColor(r,g,b,a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::present(){
    if(sdlWindow) SDL_GL_SwapWindow(sdlWindow);
}

void Renderer::drawTextureF(GLuint texID, float dstX, float dstY, float dstW, float dstH, float u0, float v0, float u1, float v1, float alpha){
    if(texID==0) return;
    // 1. Calculate Projection Matrix (MVP is based on this)

    // 2. Calculate the MVP Matrix Directly (replacing model setup and multMat4)
    float mvp[16];
    // Start by copying the PROJECTION matrix to MVP
    // For optimal speed, a fast copy/unroll function is best, but a loop is fine.
    for(int i=0; i<16; ++i) mvp[i] = proj[i];

    // Apply SCALE (dstW, dstH) to the projection's scale components
    // mvp[0] = proj[0] * dstW;
    mvp[0] *= dstW;
    // mvp[5] = proj[5] * dstH;
    mvp[5] *= dstH;

    // Apply TRANSLATION (dstX, dstY) to the projection's translation components
    // mvp[12] = proj[0] * dstX + proj[12];
    mvp[12] = proj[0] * dstX + proj[12];
    // mvp[13] = proj[5] * dstY + proj[13];
    mvp[13] = proj[5] * dstY + proj[13];

    // NOTE: If your buildOrtho is a general Z-aware ortho, you may need to apply scale/translate 
    // to mvp[10] and mvp[14] if dstZ/dstD is used, but for 2D drawing this is typically enough.

    float verts[16] = {
        0.0f, 0.0f, u0, v0,
        1.0f, 0.0f, u1, v0,
        0.0f, 1.0f, u0, v1,
        1.0f, 1.0f, u1, v1
    };

    glUseProgram(shaderProgram);
    glUniformMatrix4fv(uniMVP, 1, GL_FALSE, mvp);
    glUniform1f(uniAlpha, alpha);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);
    glUniform1i(uniTex, 0);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::drawTexture(GLuint texID, int texWidth, int texHeight,
    float dstX, float dstY, float dstW, float dstH,
    int srcX, int srcY, int srcW, int srcH,
    bool flipX, float angleDeg, float alpha)
{
    if(texID == 0) return;
    if(srcW == 0) srcW = texWidth;
    if(srcH == 0) srcH = texHeight;

    // --- UVs (flip V because OpenGL is bottom-left origin) ---
    float u0 = (float)srcX / (float)texWidth;
    float v0 = 1.0f - (float)(srcY + srcH) / (float)texHeight;
    float u1 = (float)(srcX + srcW) / (float)texWidth;
    float v1 = 1.0f - (float)srcY / (float)texHeight;
    if(flipX) std::swap(u0, u1);

    float mvp[16];
    // Copy the Projection matrix to MVP initially
    memcpy(mvp, proj, sizeof(float) * 16);

    // Apply Scale (dstW, dstH) to the first two columns of MVP
    // Column 0 (X-axis scaling)
    mvp[0] *= dstW;
    mvp[1] *= dstW;
    mvp[2] *= dstW;
    mvp[3] *= dstW;

    // Column 1 (Y-axis scaling)
    mvp[4] *= dstH;
    mvp[5] *= dstH;
    mvp[6] *= dstH;
    mvp[7] *= dstH;

    // Apply Translation (dstX, dstY) by modifying the fourth column (P12, P13, P14, P15)
    // mvp[12] = P12 + P0 * dstX + P4 * dstY;
    mvp[12] += proj[0] * dstX + proj[4] * dstY;
    mvp[13] += proj[1] * dstX + proj[5] * dstY;
    mvp[14] += proj[2] * dstX + proj[6] * dstY;
    mvp[15] += proj[3] * dstX + proj[7] * dstY;

    // --- verts with UV ---
    float verts[16] = {
        0.0f, 0.0f, u0, v0,
        1.0f, 0.0f, u1, v0,
        0.0f, 1.0f, u0, v1,
        1.0f, 1.0f, u1, v1
    };

    // --- bind + draw ---
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(uniMVP, 1, GL_FALSE, mvp);
    glUniform1f(uniAlpha, alpha > 0.0f ? alpha : 1.0f); // fallback if alpha=0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);
    glUniform1i(uniTex, 0);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // debug
    GLenum err = glGetError();
    if(err != GL_NO_ERROR){
        std::cerr << "GL error in drawTexture: 0x" << std::hex << err << std::dec << std::endl;
    }
}


void Renderer::fillRect(float x, float y, float w, float h, float r, float g, float b, float a){
    float proj[16]; buildOrtho(0.0f, (float)screenW, (float)screenH, 0.0f, proj);
    float model[16]; identityMat4(model);
    model[12] = x; model[13] = y; model[0] = w; model[5] = h;
    float mvp[16]; multMat4(proj, model, mvp);
    float verts[8] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f
    };

    glUseProgram(colorShaderProgram);
    glUniformMatrix4fv(colorUniMVP, 1, GL_FALSE, mvp);
    glUniform4f(colorUniColor, r, g, b, a);
    glBindVertexArray(colorVao);
    glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::resize(int w, int h){
    screenW = w;
    screenH = h;
    glViewport(0,0,w,h);
}

void Renderer::destroy(){
    if(shaderProgram){ glDeleteProgram(shaderProgram); shaderProgram=0; }
    if(vao){ glDeleteVertexArrays(1,&vao); vao=0; }
    if(vbo){ glDeleteBuffers(1,&vbo); vbo=0; }
    if(colorShaderProgram){ glDeleteProgram(colorShaderProgram); colorShaderProgram=0; }
    if(colorVao){ glDeleteVertexArrays(1,&colorVao); colorVao=0; }
    if(colorVbo){ glDeleteBuffers(1,&colorVbo); colorVbo=0; }
    if(glContext){
        SDL_GL_DeleteContext(glContext);
        glContext = nullptr;
    }
    sdlWindow = nullptr;
}