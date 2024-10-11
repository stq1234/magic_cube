#ifndef SHOW_H
#define SHOW_H

#include <globel.h>
#include <state.h>

namespace Ui {
class Show;
}

class Show : public QOpenGLWidget, QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit Show(QWidget* parent = nullptr);
    ~Show();
    State state_for_show, original_state;
    void vertices_initializer(int index);
    void cube_matrix_initializer();
    void rotate(char rotation_, bool is_twice = false);
    QOpenGLTexture *faces[8];
    GLfloat edge_length=0.44f, half_edge_length = 0.22f;
    GLfloat vertices[216]=
    {
        //up

        0.0f, edge_length, edge_length, 0.0f, 0.0f, 0.0f,//0
        edge_length, edge_length, edge_length, 1.0f, 0.0f, 0.0f,//1
        0.0f, edge_length, 0.0f, 0.0f, 1.0f, 0.0f, //2

        edge_length, edge_length, edge_length, 1.0f, 0.0f, 0.0f,//1
        0.0f, edge_length, 0.0f, 0.0f, 1.0f, 0.0f,//2
        edge_length, edge_length, 0.0f, 1.0f, 1.0f, 0.0f,//3

        //down

        0.0f, 0.0f, edge_length, 0.0f, 0.0f, 0.0f,//4
        edge_length, 0.0f, edge_length, 1.0f, 0.0f, 0.0f,//5
        0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,//6
        edge_length, 0.0f, edge_length, 1.0f, 0.0f, 0.0f,//5
        0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,//6
        edge_length, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,//7

        //left

        0.0f, edge_length, edge_length, 1.0f, 1.0f, 0.0f,//0
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,//6
        0.0f, edge_length, 0.0f, 0.0f, 1.0f, 0.0f,//2

        0.0f, edge_length, edge_length, 1.0f, 1.0f, 0.0f,//0
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,//6
        0.0f, 0.0f, edge_length, 1.0f, 0.0f, 0.0f,//4

        //right

        edge_length, edge_length, edge_length, 1.0f, 1.0f, 0.0f,//1
        edge_length, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,//7
        edge_length, edge_length, 0.0f, 0.0f, 1.0f, 0.0f,//3

        edge_length, edge_length, edge_length, 1.0f, 1.0f, 0.0f,//1
        edge_length, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,//7
        edge_length, 0.0f, edge_length, 1.0f, 0.0f, 0.0f,//5

        //front
        0.0f, edge_length, edge_length, 0.0f, 1.0f, 0.0f,//0
        edge_length, 0.0f, edge_length, 1.0f, 0.0f, 0.0f,//5
        edge_length, edge_length, edge_length, 1.0f, 1.0f, 0.0f,//1

        0.0f, edge_length, edge_length, 0.0f, 1.0f, 0.0f,//0
        edge_length, 0.0f, edge_length, 1.0f, 0.0f, 0.0f,//5
        0.0f, 0.0f, edge_length, 0.0f, 0.0f, 0.0f,//4

        //back

        0.0f, edge_length, 0.0f, 0.0f, 1.0f, 0.0f,//2
        edge_length, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,//7
        edge_length, edge_length, 0.0f, 1.0f, 1.0f, 0.0f,//3

        0.0f, edge_length, 0.0f, 0.0f, 1.0f, 0.0f,//2
        edge_length, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,//7
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f//6
    };
    QMatrix4x4 cubes[26];

    GLuint VAO_id, VBO_id;
    QOpenGLShaderProgram shaderProgram;

    //variables for show the magic cube.
    bool is_reverse = false;
    int rotation_interval = 40;
    int total_angle = 0;
    int angle_max = 90;
    char rotation;
    bool is_rotating = false;
    bool is_twice = false;
    QMatrix4x4 view;    QMatrix4x4 model;    QMatrix4x4 projection;

protected:
    virtual void paintGL();
    virtual void resizeGL(int w, int h);
    virtual void initializeGL();


private:
    Ui::Show *ui;
    QVector3D calculator(int total_1, int half_1, int total_2, int half_2, int total_3, int half_3);    
    QTimer* timer_for_rotate;
    int angle = 6;

};

#endif // SHOW_H
