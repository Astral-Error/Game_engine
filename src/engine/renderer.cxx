#include "renderer.hxx"
#include "glUtils.hxx"
#include <iostream>
#include <cmath>


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

Renderer::renderer():
    sdlWindow(nullptr), glContext(nullptr), screenW(800), screenH(600),
    shaderProgram(0), vao(0), vbo(0), colorShaderProgram(0), colorVAO(0), colorVBO(0)
    {}

Renderer::~renderer(){destroy();}

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

void Renderer::identiyMat(float out[16]){
    for(int i=0;i<16;i++) out[i]=0;
    out[0]=out[5]=out[10]=out[15]=1;
}

bool Renderer::initOpenGL(SDL_Window* sdlWindow, int w, int h){
    if(!window) return false;
    this.sdlWindow = sdlWindow;
    screenW = w;
    screenH = h;
    glContext = SDL_GL_CreateContext(sldWindow);
    if(!glContext){
        std::cerr<<"Error creating glContext: "<<SDL_GetError()<<std::endl;
        return false;
    }
    glewExperimental = GL_TRUE;
    GLenum glewErr = glewInit();
    if(glewErr!=GLEW_OK){
        std::cerr << "GLEW init failed: " << glewGetErrorString(glewErr) << std::endl;
        return false;
    }
    glGetError();

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
    glGenVertexArrays(1, &colorVAO);
    glGenBuffers(1, &colorVBO);
    glBindVertexArray(colorVAO);
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
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
    float proj[16]; buildOrtho(0.0f, (float)screenW, (float)screenH, 0.0f, proj);

    float model[16]; identityMat4(model);
    model[12] = dstX;
    model[13] = dstY;
    model[0] = dstW;
    model[5] = dstH;

    float mvp[16]; multMat4(proj, model, mvp);

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
    int srcX, int srcY, int srcW, int srcH, bool flipX, float angleDeg, float alpha){
    if(texID==0) return;
    if(srcW==0) srcW = texWidth;
    if(srcH==0) srcH = texHeight;
    float u0 = (float)srcX / (float)texWidth;
    float v0 = (float)srcY / (float)texHeight;
    float u1 = (float)(srcX + srcW) / (float)texWidth;
    float v1 = (float)(srcY + srcH) / (float)texHeight;
    if(flipX) std::swap(u0, u1);

    if(std::abs(angleDeg) > 0.001f){
        float proj[16]; buildOrtho(0.0f, (float)screenW, (float)screenH, 0.0f, proj);
        float cx = dstW * 0.5f;
        float cy = dstH * 0.5f;
        float T1[16]; identityMat4(T1); T1[12] = dstX; T1[13] = dstY;
        float Tc[16]; identityMat4(Tc); Tc[12] = cx; Tc[13] = cy;
        float Tnc[16]; identityMat4(Tnc); Tnc[12] = -cx; Tnc[13] = -cy;
        float S[16]; identityMat4(S); S[0]=dstW; S[5]=dstH;
        float R[16]; identityMat4(R);
        float a = angleDeg * 3.14159265358979323846f / 180.0f;
        float c = cosf(a), s = sinf(a);
        R[0] = c; R[1] = s; R[4] = -s; R[5] = c;
        float tmp1[16], tmp2[16], tmp3[16];
        multMat4(Tc, R, tmp1);
        multMat4(tmp1, S, tmp2);
        multMat4(tmp2, Tnc, tmp3);
        multMat4(T1, tmp3, tmp1);
        float mvp[16]; multMat4(proj, tmp1, mvp);

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
    else {
        drawTextureF(texID, dstX, dstY, dstW, dstH, u0, v0, u1, v1, alpha);
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
    glBindVertexArray(colorVAO);
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::resize(int w, int h){
    screenW = w;
    screenH = h;
    glViewPort(0,0,w,h);
}

void Renderer::destroy(){
    if(shaderProgram){ glDeleteProgram(shaderProgram); shaderProgram=0; }
    if(vao){ glDeleteVertexArrays(1,&vao); vao=0; }
    if(vbo){ glDeleteBuffers(1,&vbo); vbo=0; }
    if(colorShaderProgram){ glDeleteProgram(colorShaderProgram); colorShaderProgram=0; }
    if(colorVAO){ glDeleteVertexArrays(1,&colorVAO); colorVAO=0; }
    if(colorVBO){ glDeleteBuffers(1,&colorVBO); colorVBO=0; }
    if(glContext){
        SDL_GL_DeleteContext(glContext);
        glContext = nullptr;
    }
    sdlWindow = nullptr;
}