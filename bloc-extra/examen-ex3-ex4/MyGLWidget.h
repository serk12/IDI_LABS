#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include <QMouseEvent>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "model.h"

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core 
{
  Q_OBJECT
  signals:
    void zoomMod(int zoom);
  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();
    float zoom;
    void projectTransform ();
    virtual void paintGL ( );
  protected:
    virtual void initializeGL ( );
    virtual void resizeGL (int width, int height);
    virtual void keyPressEvent (QKeyEvent *event);
    virtual void mousePressEvent (QMouseEvent *event);
    virtual void mouseReleaseEvent (QMouseEvent *event);
    virtual void mouseMoveEvent (QMouseEvent *event);

  private:
    void createBuffers ();
    void carregaShaders ();
    void viewTransform ();
    void modelTransformTerra ();
    void modelTransformPatricio ();
    void calculaCapsaModel ();

    GLuint VAO_Patr, VBO_PatrPos, VBO_PatrNorm, VBO_PatrMatamb, VBO_PatrMatdiff, VBO_PatrMatspec, VBO_PatrMatshin;
    GLuint VAO_Terra, VBO_TerraPos, VBO_TerraNorm, VBO_TerraMatamb, VBO_TerraMatdiff, VBO_TerraMatspec, VBO_TerraMatshin;
    
    QOpenGLShaderProgram *program;
    
    GLuint transLoc, projLoc, viewLoc;
    
    GLuint vertexLoc, normalLoc, matambLoc, matdiffLoc, matspecLoc, matshinLoc;

    Model patr;
    glm::vec3 centrePatr;
    float escala;
    float radiEsc;


    typedef  enum {NONE, ROTATE, ZOOM} InteractiveAction;
    InteractiveAction DoingInteractive;
    int xClick, yClick;
    float angleY;
    bool perspectiva;
};

