#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent)
{
  setFocusPolicy(Qt::ClickFocus); 
  scale = 1.0f;
  rotate = 0;
  m.load("../models/HomerProves.obj");   //EXERCICI 4 Z-BUffer & hommer
}

MyGLWidget::~MyGLWidget ()
{
  if (program != NULL)
    delete program;
}

void MyGLWidget::initializeGL ()
{
  initializeOpenGLFunctions();  
  glEnable(GL_DEPTH_TEST);  //EXERCICI Z-BUFFER
  glClearColor(0.5, 0.7, 1.0, 1.0); 
  carregaShaders();
  createBuffers();
}

void MyGLWidget::paintGL () 
{
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //EXERCICI Z-BUFFER

  glBindVertexArray (VAO_Hommer);
  modelTransform ();
  glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3);  //EXERCICI 4 Z-BUffer & hommer
  glBindVertexArray (0);
  
  
  //EXERCICI 6 {
  glBindVertexArray (VAO_Terra);
  modelTransform2();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glBindVertexArray (0);
  //EXERCICI 6 }
}

void MyGLWidget::modelTransform () 
{
  glm::mat4 transform (1.0f);
  transform = glm::scale(transform, glm::vec3(scale));  
  transform = glm::rotate(transform, (3.141516f/4.0f)*rotate, glm::vec3(0.0f, 1.0f, 0.0f)); //EXERCICI 5
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransform2() //EXERCICI 6
{
  glm::mat4 transform (1.0f);
  transform = glm::scale(transform, glm::vec3(scale));  
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}


void MyGLWidget::resizeGL (int w, int h) 
{
  glViewport(0, 0, w, h);
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  switch (event->key()) {
    makeCurrent();
    case Qt::Key_S: {
      scale += 0.05;
      break;
    }
    case Qt::Key_D: { 
      scale -= 0.05;
      break;
    }
    case Qt::Key_R: { //EXERCICI 5
      ++rotate;
      break;
    }
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::createBuffers () 
{
//EXERCICI 4 Z-BUffer & hommer {
  glGenVertexArrays(1, &VAO_Hommer);
  glBindVertexArray(VAO_Hommer);

  glGenBuffers(1, &VBO_HommerPos);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_HommerPos);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* m.faces().size() *3*3, m.VBO_vertices(), GL_STATIC_DRAW);

  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glGenBuffers(1, &VBO_HommerCol);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_HommerCol);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* m.faces().size() *3*3, m.VBO_matdiff(), GL_STATIC_DRAW);

  // Activem l'atribut colorLoc
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  glBindVertexArray (0);
//EXERCICI 4 Z-BUffer & hommer }
  
//EXERCICI 6 {
  glm::vec3 Vertices[4];
  Vertices[0] = glm::vec3(1., -1., 1.0);
  Vertices[3] = glm::vec3(-1., -1, -1.0);
  Vertices[2] = glm::vec3(-1., -1, 1.0);
  Vertices[1] = glm::vec3(1., -1, -1.0);
  
  glm::vec3 color[4];
  color[0] = glm::vec3(1,0,0);
  color[1] = glm::vec3(0,1,0);
  color[2] = glm::vec3(0,0,1);
  color[3] = glm::vec3(1,0,0);
   
  glGenVertexArrays(1, &VAO_Terra);
  glBindVertexArray(VAO_Terra);
  
  glGenBuffers(1, &VBO_Terra);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Terra);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);
  
  glGenBuffers(1, &VBO_TerraCol);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraCol);
  glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);

  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);
  
  glBindVertexArray(0);
//EXERCICI 6 }
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
  colorLoc  = glGetAttribLocation (program->programId(), "color");
  transLoc  = glGetUniformLocation(program->programId(), "TG");
  projLoc   = glGetUniformLocation(program->programId(), "proj"); //EJERCICIO 1 add matrix of projection
  projecTransform();                                              //EJERCICIO 1 add matrix of projection
  viewLoc   = glGetUniformLocation(program->programId(), "view"); //EJERCICIO 2 add matrix of pos cam
  viewTransform();                                                //EJERCICIO 2 add matrix of pos cam
}

void MyGLWidget::viewTransform() {
  glm::mat4 View = glm::lookAt(glm::vec3(0,0,1),glm::vec3(0,0,0), glm::vec3(0,1,0));
  glUniformMatrix4fv(viewLoc,1,GL_FALSE, &View[0][0]);
}

void MyGLWidget::projecTransform() {//EJERCICIO 1 add matrix of projection
  glm::mat4 Proj = glm::perspective ((float)3.141516f/2.0f,1.0f,0.4f,3.0f);
  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}