#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QKeyEvent>
#include <QOpenGLShaderProgram>
#include "glm/glm.hpp"
#define GLM_FORCE_RADIANS
#include "glm/gtc/matrix_transform.hpp"

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();
    
  protected:
    virtual void initializeGL ();

    virtual void paintGL ();
 
    virtual void resizeGL (int width, int height);  
    virtual void keyPressEvent (QKeyEvent *e);

  private:
    void createBuffers  ();
    void carregaShaders ();
    void modelTransform1();
    void modelTransform2();
    // attribute locations
    GLuint vertexLoc, colorLoc, varLoc, transLoc;
    float scl, posX, posY;
    int rot;
    bool hasToTransform, negRot;
    // Program
    QOpenGLShaderProgram *program;

    GLuint VAO, VBO, VAO1,VBO1, VBO1_Col, VBO_Col;
};
