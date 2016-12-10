#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent)
{
  setFocusPolicy(Qt::ClickFocus); 
  scl = 1.0;
  posX = posY = rot = 0;
}

MyGLWidget::~MyGLWidget ()
{
  if (program != NULL)
    delete program;
}

void MyGLWidget::initializeGL ()
{
  initializeOpenGLFunctions();
  
  glClearColor (0.5, 0.7, 1.0, 1.0);
  carregaShaders();
  createBuffers();
}

void MyGLWidget::paintGL ()
{
  glClear (GL_COLOR_BUFFER_BIT);  
  
  glBindVertexArray(VAO);
  if (hasToTransform) modelTransform1(); //make mov
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);
  glBindVertexArray(0);

  glBindVertexArray(VAO1);
  
  if (hasToTransform) modelTransform2(); //make mov
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
  glBindVertexArray(0);
}

void MyGLWidget::resizeGL (int w, int h)
{
  glViewport (0, 0, w, h);
}

void MyGLWidget::createBuffers ()
{
  glm::vec3 color[5];
  color[0] = glm::vec3(1,0,0);
  color[1] = glm::vec3(1,1,0);
  color[2] = glm::vec3(1,0,1);
  color[3] = glm::vec3(1,1,1);
  color[4] = glm::vec3(0,1,0);

  glm::vec3 Vertices[5]; 
  Vertices[0] = glm::vec3( 0.0, -1.0, 0.0); 
  Vertices[1] = glm::vec3(-1.0, -1.0, 0.0);
  Vertices[2] = glm::vec3( 0.0,   0.0, 0.0);
  Vertices[3] = glm::vec3(-1.0,  0.0, 0.0);
  Vertices[4] = glm::vec3(-0.5,  0.5, 0.0);
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glGenBuffers(1, &VBO_Col);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Col);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), color, GL_STATIC_DRAW);  
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);
  
  glBindVertexArray(0);

  glm::vec3 Vertices1[3];
  Vertices1[0] = glm::vec3(0.2,   0.0, 0.0); 
  Vertices1[1] = glm::vec3(0.2,   0.1, 0.0);
  Vertices1[2] = glm::vec3(0.0,   0.3, 0.0);
  
  glGenVertexArrays(1, &VAO1);
  glBindVertexArray(VAO1);

  glGenBuffers(1, &VBO1);
  glBindBuffer(GL_ARRAY_BUFFER, VBO1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices1), Vertices1, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);
  
  glGenBuffers(1, &VBO1_Col);
  glBindBuffer(GL_ARRAY_BUFFER, VBO1_Col);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices1), color, GL_STATIC_DRAW);  
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);
  glBindVertexArray(0);
}

void MyGLWidget::carregaShaders()
{
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  fs.compileSourceFile("shaders/fragshad.frag");
  vs.compileSourceFile("shaders/vertshad.vert");
  program = new QOpenGLShaderProgram(this);
  program->addShader(&fs);
  program->addShader(&vs);
  program->link();
  program->bind();
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  colorLoc  = glGetAttribLocation (program->programId(), "color" );
  
  transLoc  = glGetUniformLocation(program->programId(), "TG"    );//make mov
  hasToTransform = true;//make mov
  varLoc    = glGetUniformLocation(program->programId(), "val"   );//make mov
  glUniform1f(varLoc, scl);//make mov
}
/***********************************************/
/*                 MAKE MOV                    */
/***********************************************/
void MyGLWidget::modelTransform1 () {
    glm::mat4 TG(1.0);
    TG = glm::translate(TG, glm::vec3(posX, posY,0));
    TG = glm::rotate(TG, (3.141516f/4.0f)*rot, glm::vec3(0,0,1));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}


void MyGLWidget::modelTransform2 () {
    glm::mat4 TG(1.0);
    TG = glm::translate(TG, glm::vec3(posX, posY,0));
    float div = 4.0f;
    if (negRot) div = -div;
    TG = glm::rotate(TG, (3.141516f/div)*rot, glm::vec3(0,0,1));
    hasToTransform = negRot = false;
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::keyPressEvent (QKeyEvent *e) {
    makeCurrent ();
    switch (e->key()) {
        case Qt::Key_S :
            scl += 0.1;
            glUniform1f (varLoc, scl);
        break;
        case Qt::Key_D :
            scl -= 0.1;
            glUniform1f (varLoc, scl);
        break;
        case Qt::Key_Up:
            posY += 0.05;
            ++rot;
            hasToTransform = true;
        break;
        case Qt::Key_Left:
            posX -= 0.05;
            ++rot;
            hasToTransform = true;
        break;
        case Qt::Key_Right:
            posX += 0.05;
            ++rot;
            hasToTransform = true;
        break;
        case Qt::Key_Down:
            posY -= 0.05;
            ++rot;
            hasToTransform = true;
        break;
        case Qt::Key_P:
            negRot = true;
            ++rot;
            hasToTransform = true;
        break;
        default: 
            e->ignore ();
    }
    update ();
}
