#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include "../Model/model.h"   //EXERCICI 4 Z-BUffer & hommer
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core 
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();

  protected:
    virtual void initializeGL ( );
    virtual void paintGL ( );
    virtual void resizeGL (int width, int height);
    virtual void keyPressEvent (QKeyEvent *event);

  private:
    void createBuffers  ();
    void carregaShaders ();
    void modelTransform ();
    void modelTransform2();
    void projecTransform();
    void viewTransform  ();
    
    GLuint vertexLoc, colorLoc, transLoc, projLoc, viewLoc;
    GLuint VAO_Hommer, VBO_HommerPos, VBO_HommerCol, VBO_TerraCol, VBO_Terra, VAO_Terra;
    QOpenGLShaderProgram *program;
    float scale;
    int rotate; //EXERCICI 5
    glm::vec3 pos;
    
    Model m; //EXERCICI 4 Z-BUffer & hommer
};

