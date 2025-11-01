#include "gl_backend.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cruz/rendering/backends/opengl/gl_shader.h>
#include <iostream>

void GlBackend::Initialize() {
    if (!platform) { 
        std::cerr << "Platform not set!\n"; 
        return; 
    }
    platform->MakeContextCurrent();
    if (!gladLoadGLLoader((GLADloadproc)platform->GetProcAddress())) {
        std::cerr << "Failed to initialize GLAD\n"; 
        return;
    }
    glEnable(GL_MULTISAMPLE);

    platform->AddResizeCallback([this](int w,int h){ 
        glViewport(0,0,w,h);
    });
}

void GlBackend::Resize(int width, int height) {
    if(width <= 0 || height <= 0) return;
    glViewport(0,0,width,height);
}

void GlBackend::Update(float dt) {
    if(!platform) return;
    if(platform->WindowShouldClose()) return;
    if(platform->GetKeyPressed(GLFW_KEY_ESCAPE)) platform->SetWindowShouldClose(true);
}

void GlBackend::Clear(const float color[4]) {
    glClearColor(color[0],color[1],color[2],color[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GlBackend::SetViewport(int x,int y,int w,int h) { 
    glViewport(x,y,w,h); 
}

void GlBackend::SetPipeline(const PipelineSettings& s) {
    if(s.depthTest) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
    if(s.blend){ glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); }
    else glDisable(GL_BLEND);
}

Shader* GlBackend::CreateShader(const std::string& vs,const std::string& fs){
    GLShader* shader = new GLShader();
    shader->Compile(vs,fs);
    return shader;
}

void GlBackend::UseShader(Shader* shader){
    if(!shader) return;
    GLShader* glShader = dynamic_cast<GLShader*>(shader);
    if(glShader) glShader->Use();
}

void GlBackend::SetUniformMat4(Shader* shader,const std::string& name,const Mat4& mat){
    if(!shader) return;
    GLShader* glShader = dynamic_cast<GLShader*>(shader);
    if(glShader) glShader->SetUniformMat4(name, mat.data);
}

void GlBackend::Draw(const std::vector<Vertex>& verts){
    if(verts.empty()) return;
    UploadVertices(verts);
    coloredVAO = false;
    DrawUploadedVertices();
}

void GlBackend::Draw(const std::vector<ColoredVertex>& verts){
    if(verts.empty()) return;

    if(vao!=0){ glDeleteBuffers(1,&vbo); glDeleteVertexArrays(1,&vao); }

    glGenVertexArrays(1,&vao);
    glGenBuffers(1,&vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER, verts.size()*sizeof(ColoredVertex), verts.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(ColoredVertex),(void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,4,GL_FLOAT,GL_FALSE,sizeof(ColoredVertex),(void*)offsetof(ColoredVertex,r));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    vertexCount = GLsizei(verts.size());
    coloredVAO = true;
    DrawUploadedVertices();
}

void GlBackend::UploadVertices(const std::vector<Vertex>& verts){
    if(verts.empty()) return;
    if(vao!=0){ glDeleteBuffers(1,&vbo); glDeleteVertexArrays(1,&vao); }

    glGenVertexArrays(1,&vao);
    glGenBuffers(1,&vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glBufferData(GL_ARRAY_BUFFER, verts.size()*sizeof(Vertex), verts.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    vertexCount = GLsizei(verts.size());
    coloredVAO=false;
}

void GlBackend::DrawUploadedVertices(){
    if(vao==0) return;
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES,0,vertexCount);
    glBindVertexArray(0);
}

GlBackend::~GlBackend(){
    if(vao!=0) glDeleteVertexArrays(1,&vao);
    if(vbo!=0) glDeleteBuffers(1,&vbo);
}