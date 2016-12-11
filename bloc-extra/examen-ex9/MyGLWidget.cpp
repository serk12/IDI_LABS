#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent)
{
  setFocusPolicy(Qt::ClickFocus);
  xClick = yClick = 0;
  angleY = 0.0;
  perspectiva = true;
  DoingInteractive = NONE;
  radiEsc = sqrt(5);
  type = 0;
}

MyGLWidget::~MyGLWidget ()
{
  if (program != NULL)
    delete program;
}

void MyGLWidget::initializeGL ()
{
  initializeOpenGLFunctions();  

  glClearColor(0.5, 0.7, 1.0, 1.0);
  glEnable(GL_DEPTH_TEST);
  carregaShaders();
  createBuffers();
  projectTransform ();
  viewTransform ();
}

void MyGLWidget::paintGL () 
{
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glBindVertexArray (VAO_Terra);
  modelTransformTerra ();
  glDrawArrays(GL_TRIANGLES, 0, 12);

  glBindVertexArray (VAO_Cow);
  modelTransformCow ();
  glDrawArrays(GL_TRIANGLES, 0, cow.faces().size()*3);
  glBindVertexArray(0);
  
  glBindVertexArray (VAO_Patr);
  modelTransformPatricio ();
  glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);
  glBindVertexArray(0);
}

void MyGLWidget::resizeGL (int w, int h) 
{
  glViewport(0, 0, w, h);
}

void MyGLWidget::createBuffers ()
{
  patr.load("./models/Patricio.obj");

  calculaCapsaModel (false, patr);
  
  glGenVertexArrays(1, &VAO_Patr);
  glBindVertexArray(VAO_Patr);

  glGenBuffers(1, &VBO_PatrPos);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_PatrPos);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*patr.faces().size()*3*3, patr.VBO_vertices(), GL_STATIC_DRAW);

  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glGenBuffers(1, &VBO_PatrNorm);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_PatrNorm);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*patr.faces().size()*3*3, patr.VBO_normals(), GL_STATIC_DRAW);

  glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(normalLoc);

  glGenBuffers(1, &VBO_PatrMatamb);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_PatrMatamb);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*patr.faces().size()*3*3, patr.VBO_matamb(), GL_STATIC_DRAW);

  glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matambLoc);

  glGenBuffers(1, &VBO_PatrMatdiff);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_PatrMatdiff);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*patr.faces().size()*3*3, patr.VBO_matdiff(), GL_STATIC_DRAW);

  glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matdiffLoc);

  glGenBuffers(1, &VBO_PatrMatspec);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_PatrMatspec);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*patr.faces().size()*3*3, patr.VBO_matspec(), GL_STATIC_DRAW);

  glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matspecLoc);

  glGenBuffers(1, &VBO_PatrMatshin);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_PatrMatshin);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*patr.faces().size()*3, patr.VBO_matshin(), GL_STATIC_DRAW);

  glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matshinLoc);
  
  cow.load("./models/cow.obj");

  calculaCapsaModel (true, cow);
  
  glGenVertexArrays(1, &VAO_Cow);
  glBindVertexArray(VAO_Cow);

  glGenBuffers(1, &VBO_CowPos);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_CowPos);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*cow.faces().size()*3*3, cow.VBO_vertices(), GL_STATIC_DRAW);

  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glGenBuffers(1, &VBO_CowNorm);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_CowNorm);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*cow.faces().size()*3*3, cow.VBO_normals(), GL_STATIC_DRAW);

  glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(normalLoc);

  glGenBuffers(1, &VBO_CowMatamb);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_CowMatamb);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*cow.faces().size()*3*3, cow.VBO_matamb(), GL_STATIC_DRAW);

  glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matambLoc);

  glGenBuffers(1, &VBO_CowMatdiff);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_CowMatdiff);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*cow.faces().size()*3*3, cow.VBO_matdiff(), GL_STATIC_DRAW);

  glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matdiffLoc);

  glGenBuffers(1, &VBO_CowMatspec);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_CowMatspec);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*cow.faces().size()*3*3, cow.VBO_matspec(), GL_STATIC_DRAW);

  glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matspecLoc);

  glGenBuffers(1, &VBO_CowMatshin);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_CowMatshin);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*cow.faces().size()*3, cow.VBO_matshin(), GL_STATIC_DRAW);

  glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matshinLoc);
  

  glm::vec3 posterra[12] = {
	glm::vec3(-2.0, -1.0, 2.0),
	glm::vec3(2.0, -1.0, 2.0),
	glm::vec3(-2.0, -1.0, -2.0),
	glm::vec3(-2.0, -1.0, -2.0),
	glm::vec3(2.0, -1.0, 2.0),
	glm::vec3(2.0, -1.0, -2.0),
	glm::vec3(-2.0, -1.0, -2.0),
	glm::vec3(2.0, -1.0, -2.0),
	glm::vec3(-2.0, 1.0, -2.0),
	glm::vec3(-2.0, 1.0, -2.0),
	glm::vec3(2.0, -1.0, -2.0),
	glm::vec3(2.0, 1.0, -2.0)
  }; 

  glm::vec3 norm1 (0,1,0);
  glm::vec3 norm2 (0,0,1);
  glm::vec3 normterra[12] = {
	norm1, norm1, norm1, norm1, norm1, norm1,
	norm2, norm2, norm2, norm2, norm2, norm2
  };

  glm::vec3 amb(0.2,0,0.2);
  glm::vec3 diff(0.8,0,0.8);
  glm::vec3 spec(0,0,0);
  float shin = 100;

  glm::vec3 matambterra[12] = {
	amb, amb, amb, amb, amb, amb, amb, amb, amb, amb, amb, amb
  };
  glm::vec3 matdiffterra[12] = {
	diff, diff, diff, diff, diff, diff, diff, diff, diff, diff, diff, diff
  };
  glm::vec3 matspecterra[12] = {
	spec, spec, spec, spec, spec, spec, spec, spec, spec, spec, spec, spec
  };
  float matshinterra[12] = {
	shin, shin, shin, shin, shin, shin, shin, shin, shin, shin, shin, shin
  };

  glGenVertexArrays(1, &VAO_Terra);
  glBindVertexArray(VAO_Terra);

  glGenBuffers(1, &VBO_TerraPos);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraPos);
  glBufferData(GL_ARRAY_BUFFER, sizeof(posterra), posterra, GL_STATIC_DRAW);

  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glGenBuffers(1, &VBO_TerraNorm);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraNorm);
  glBufferData(GL_ARRAY_BUFFER, sizeof(normterra), normterra, GL_STATIC_DRAW);

  glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(normalLoc);

  glGenBuffers(1, &VBO_TerraMatamb);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraMatamb);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matambterra), matambterra, GL_STATIC_DRAW);

  glVertexAttribPointer(matambLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matambLoc);

  glGenBuffers(1, &VBO_TerraMatdiff);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraMatdiff);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matdiffterra), matdiffterra, GL_STATIC_DRAW);

  glVertexAttribPointer(matdiffLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matdiffLoc);

  glGenBuffers(1, &VBO_TerraMatspec);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraMatspec);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matspecterra), matspecterra, GL_STATIC_DRAW);

  glVertexAttribPointer(matspecLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matspecLoc);

  glGenBuffers(1, &VBO_TerraMatshin);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraMatshin);
  glBufferData(GL_ARRAY_BUFFER, sizeof(matshinterra), matshinterra, GL_STATIC_DRAW);

  glVertexAttribPointer(matshinLoc, 1, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(matshinLoc);

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
  normalLoc = glGetAttribLocation (program->programId(), "normal");
  matambLoc = glGetAttribLocation (program->programId(), "matamb");
  matdiffLoc = glGetAttribLocation (program->programId(), "matdiff");
  matspecLoc = glGetAttribLocation (program->programId(), "matspec");
  matshinLoc = glGetAttribLocation (program->programId(), "matshin");

  transLoc = glGetUniformLocation (program->programId(), "TG");
  projLoc = glGetUniformLocation (program->programId(), "proj");
  viewLoc = glGetUniformLocation (program->programId(), "view");
  
  lightLoc = glGetUniformLocation(program->programId(), "changeLightV");
  glUniform1i(lightLoc, type);
}

void MyGLWidget::modelTransformCow ()
{
  glm::mat4 TG(1.f);  
  TG = glm::translate(TG, -centrePatr);             //centre escena
  TG = glm::translate(TG, glm::vec3(0, -0.5, 0)); //ha de tocar el terra
  TG = glm::rotate(TG, float (M_PI/2.0f), glm::vec3(0,0,-1));
  TG = glm::rotate(TG, float (M_PI/2.0f), glm::vec3(0,-1,0));
  TG = glm::scale(TG, glm::vec3(escalaCow));
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformPatricio ()
{
  glm::mat4 TG(1.f);  
  TG = glm::translate(TG, -centrePatr);
  TG = glm::translate(TG, glm::vec3(0.35, -0.4, 0));
  TG = glm::scale(TG, glm::vec3(escalaPat));
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}


void MyGLWidget::modelTransformTerra ()
{
  glm::mat4 TG(1.f);
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::projectTransform ()
{
  glm::mat4 Proj;
  if (perspectiva)
    Proj = glm::perspective(float(M_PI/3.0), 1.0f, radiEsc, 3.0f*radiEsc);
  else
    Proj = glm::ortho(-radiEsc, radiEsc, -radiEsc, radiEsc, radiEsc, 3.0f*radiEsc);

  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform ()
{
  glm::mat4 View;
  View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -2*radiEsc));
  View = glm::rotate(View, -angleY, glm::vec3(0, 1, 0));

  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::calculaCapsaModel (bool cow, Model &mod)
{
  float minx, miny, minz, maxx, maxy, maxz;
  minx = maxx = mod.vertices()[0];
  miny = maxy = mod.vertices()[1];
  minz = maxz = mod.vertices()[2];
  for (unsigned int i = 3; i < mod.vertices().size(); i+=3)
  {
    if (mod.vertices()[i+0] < minx)
      minx = mod.vertices()[i+0];
    if (mod.vertices()[i+0] > maxx)
      maxx = mod.vertices()[i+0];
    if (mod.vertices()[i+1] < miny)
      miny = mod.vertices()[i+1];
    if (mod.vertices()[i+1] > maxy)
      maxy = mod.vertices()[i+1];
    if (mod.vertices()[i+2] < minz)
      minz = mod.vertices()[i+2];
    if (mod.vertices()[i+2] > maxz)
      maxz = mod.vertices()[i+2];
  }
  if (cow) escalaCow = 0.5/(maxy-miny);   //tendria que ser 0.5
  else escalaPat = 0.25/(maxy-miny);       //tendria que ser 0.25
  centrePatr[0] = (minx+maxx)/2.0; centrePatr[1] = (miny+maxy)/2.0; centrePatr[2] = (minz+maxz)/2.0;
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_O: {
      perspectiva = !perspectiva;
      projectTransform ();
      break;
    }
    case Qt::Key_X: {
      if (type == 1) type = 0;
      else type = 1;
      glUniform1i(lightLoc, type);
      break;
    }
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
    DoingInteractive = ROTATE;
  }
}

void MyGLWidget::mouseReleaseEvent( QMouseEvent *)
{
  DoingInteractive = NONE;
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  if (DoingInteractive == ROTATE)
  {
    angleY += (e->x() - xClick) * M_PI / 180.0;
    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}


