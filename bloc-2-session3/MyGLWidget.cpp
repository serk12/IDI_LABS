#include "MyGLWidget.h"
#include <iostream>

//omited ex 2
MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent)
{
  setFocusPolicy(Qt::ClickFocus); 
  scale = 1.0f;
  rotate = 0;
  zoom = 1;
  perspectiva = true; //ex 3
  xClick = yClick = 0;
  angleY = angleZ = angleX = 0.0;
  m.load("../models/HomerProves.obj");
}

MyGLWidget::~MyGLWidget ()
{
  if (program != NULL)
    delete program;
}

void MyGLWidget::initializeGL ()
{
  initializeOpenGLFunctions();  
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.5, 0.7, 1.0, 1.0); 
  carregaShaders();
  createBuffers();
}

void MyGLWidget::resizeGL (int w, int h)
{
  ra = float (w / float (h));
  projecTransform();
  glViewport(0, 0, w, h);
}


void MyGLWidget::paintGL () 
{
  resizeGL(width(), height()); 
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glBindVertexArray (VAO_Hommer);
  modelTransform ();
  glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3);  
  glBindVertexArray (0);
  
  glBindVertexArray (VAO_Terra);
  modelTransform2();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glBindVertexArray (0);
}

void MyGLWidget::modelTransform () 
{
  glm::mat4 transform (1.0f);
  transform = glm::scale(transform, glm::vec3(scale));  
  transform = glm::rotate(transform, (3.141516f/4.0f)*rotate, glm::vec3(0.0f, 1.0f, 0.0f));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransform2()
{
  glm::mat4 transform (1.0f);
  transform = glm::scale(transform, glm::vec3(scale));  
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  switch (event->key()) {
    makeCurrent();
    case Qt::Key_O: { // canvia òptica entre perspectiva i axonomètrica  //ex 3
      perspectiva = !perspectiva;
      projecTransform();
      break;
    }
    case Qt::Key_S: {
      scale += 0.05;
      break;
    }
    case Qt::Key_D: { 
      scale -= 0.05;
      break;
    }
    case Qt::Key_R: {
      ++rotate;
      break;
    } //EX 1 {
    case Qt::Key_Z: {
      zoom /= 1.1f;
      break;
    }
    case Qt::Key_X: {
      zoom *= 1.1f;
      break;
    } // EX 1 }
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::mousePressEvent (QMouseEvent *e)
{
  xClick = e->x();
  yClick = e->y();

  if (e->button() & Qt::LeftButton &&
      ! (e->modifiers() & (Qt::ShiftModifier|Qt::AltModifier|Qt::ControlModifier)))
  {
    DoingInteractive = ROTATEY;
  }
  else if (e->button() & Qt::LeftButton &&
      ! (e->modifiers() & (Qt::AltModifier|Qt::ControlModifier)) &&
	(e->modifiers() & (Qt::ShiftModifier)))
  {
    DoingInteractive = ROTATEZ;
  }
  else if (e->button() & Qt::LeftButton &&
      ! (e->modifiers() & (Qt::ShiftModifier|Qt::AltModifier)) &&
	(e->modifiers() & Qt::ControlModifier))
  {
    DoingInteractive = ROTATEX;
  }
}

void MyGLWidget::mouseReleaseEvent( QMouseEvent *)
{
  DoingInteractive = NONE;
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  if (DoingInteractive == ROTATEY)
  {
    angleY += (e->x() - xClick) * M_PI / 180.0;
    viewTransform ();
  }
  else if (DoingInteractive == ROTATEZ) {
    angleZ += (e->x() - xClick) * M_PI / 180.0;
    viewTransform ();
  }
  else if (DoingInteractive == ROTATEX) {
    angleX += (e->x() - xClick) * M_PI / 180.0;
    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}

void MyGLWidget::createBuffers () 
{
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

  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  glBindVertexArray (0);
  
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
  projLoc   = glGetUniformLocation(program->programId(), "proj"); 
  viewLoc   = glGetUniformLocation(program->programId(), "view");
  
  calcEsfera(glm::vec3 (0,0,0), glm::vec3(0,0,0));
  up =  glm::vec3(0,1,0);
  projecTransform();                                              
  viewTransform();                                                
}

void MyGLWidget::calcEsfera(glm::vec3 mins, glm::vec3 maxs) {
    if (mins.x == maxs.x and mins.y == maxs.y and mins.z == maxs.z) {
        float minx, miny, minz, maxx, maxy, maxz;
        minx = maxx = m.vertices()[0];
        miny = maxy = m.vertices()[1];
        minz = maxz = m.vertices()[2];
        for (unsigned int i = 3; i < m.vertices().size(); i+=3) {
            if (m.vertices()[i+0] < minx)
                minx = m.vertices()[i+0];
            if (m.vertices()[i+0] > maxx)
                maxx = m.vertices()[i+0];
            if (m.vertices()[i+1] < miny)
                miny = m.vertices()[i+1];
            if (m.vertices()[i+1] > maxy)
                maxy = m.vertices()[i+1];
            if (m.vertices()[i+2] < minz)
                minz = m.vertices()[i+2];
            if (m.vertices()[i+2] > maxz)
                maxz = m.vertices()[i+2];
        }
        float zs = (maxz - minz)/2.0f;
        float ys = (maxy - miny)/2.0f;
        float xs = (maxx - minx)/2.0f;
        centerEsfer = glm::vec3(xs + minx,ys + miny,zs + minz);
        radi = xs;
        if (ys > radi) radi = ys;
        if (zs > radi) radi = zs;
    }
    else {
        float zs = (maxs.z - mins.z)/2.0f;
        float ys = (maxs.y - mins.y)/2.0f;
        float xs = (maxs.x - mins.x)/2.0f;
        centerEsfer = glm::vec3(xs + mins.x,ys + mins.y,zs + mins.z);
        radi = xs;
        if (ys > radi) radi = ys;
        if (zs > radi) radi = zs;
    }
}

void MyGLWidget::viewTransform() {
  glm::mat4 View = glm::lookAt(centerEsfer + (radi*glm::vec3(0,0,1)), centerEsfer,up);
  View = glm::rotate(View, -angleY, glm::vec3(0, 1, 0));
  View = glm::rotate(View, -angleZ, glm::vec3(0, 0, 1));
  View = glm::rotate(View, -angleX, glm::vec3(1, 0, 0));
  glUniformMatrix4fv(viewLoc,1,GL_FALSE, &View[0][0]);
}

void MyGLWidget::projecTransform() {                        
  glm::mat4 Proj;
  if (perspectiva) Proj = glm::perspective (((float)3.141516f/2.0f)*zoom,ra,0.4f,3.0f); //ex 1
  else Proj = glm::ortho(-radi*ra, radi*ra, -radi, radi, 0.4f, 3.0f);  //ex 3 4 
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, &Proj[0][0]);   
}