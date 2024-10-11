#include "show.h"
#include "ui_show.h"
//bool test=false;



//int angle =1;
Show::Show(QWidget* parent):QOpenGLWidget (parent), state_for_show(),
    ui(new Ui::Show)
{
    ui->setupUi(this);
    this->setGeometry(application_width*44/100, application_height*5/100, application_height*55/100, application_height*55/100);
    this->cube_matrix_initializer();
    timer_for_rotate = new QTimer();
    timer_for_rotate->setInterval(this->rotation_interval);
    QMatrix4x4 test1, test2, test3;
    test1.rotate(25, 1.0f, 0.0f, 0.0f);
    test2.rotate(-25,0,1,0);
    test3.rotate(-10,0,0,1.0f);
    model = test3*test2*test1*model;
    view.translate(0.0, 0.0, -3.0);
    projection.perspective(45, (float)width()/height(), 0.1, 100);
}

Show::~Show()
{
    delete ui;
}

QVector3D Show::calculator(int total_1, int half_1, int total_2, int half_2, int total_3, int half_3)
{
    return QVector3D(float(total_1)*this->edge_length + float(half_1)*this->half_edge_length,
                     float(total_2)*this->edge_length + float(half_2)*this->half_edge_length,
                     float(total_3)*this->edge_length + float(half_3)*this->half_edge_length);
}

void Show::cube_matrix_initializer()//将每块移动到对应位置
{
    for(int i = 0; i < 26; ++i)
        this->cubes[i].setToIdentity();
    this->cubes[0].translate(this->calculator(-1,-1,0,1,0,1));
    this->cubes[1].translate(this->calculator(0,-1,0,1,0,1));
    this->cubes[2].translate(this->calculator(0,1,0,1,0,1));
    this->cubes[3].translate(this->calculator(-1,-1,0,1,0,-1));
    this->cubes[4].translate(this->calculator(0,-1,0,1,0,-1));
    this->cubes[5].translate(this->calculator(0,1,0,1,0,-1));
    this->cubes[6].translate(this->calculator(-1,-1,0,1,-1,-1));
    this->cubes[7].translate(this->calculator(0,-1,0,1,-1,-1));
    this->cubes[8].translate(this->calculator(0,1,0,1,-1,-1));

    this->cubes[9].translate(this->calculator(-1,-1,0,-1,0,1));
    this->cubes[10].translate(this->calculator(0,-1,0,-1,0,1));
    this->cubes[11].translate(this->calculator(0,1,0,-1,0,1));
    this->cubes[12].translate(this->calculator(-1,-1,0,-1,0,-1));
    this->cubes[13].translate(this->calculator(0,1,0,-1,0,-1));
    this->cubes[14].translate(this->calculator(-1,-1,0,-1,-1,-1));
    this->cubes[15].translate(this->calculator(0,-1,0,-1,-1,-1));
    this->cubes[16].translate(this->calculator(0,1,0,-1,-1,-1));

    this->cubes[17].translate(this->calculator(-1,-1,-1,-1,0,1));
    this->cubes[18].translate(this->calculator(0,-1,-1,-1,0,1));
    this->cubes[19].translate(this->calculator(0,1,-1,-1,0,1));
    this->cubes[20].translate(this->calculator(-1,-1,-1,-1,0,-1));
    this->cubes[21].translate(this->calculator(0,-1,-1,-1,0,-1));
    this->cubes[22].translate(this->calculator(0,1,-1,-1,0,-1));
    this->cubes[23].translate(this->calculator(-1,-1,-1,-1,-1,-1));
    this->cubes[24].translate(this->calculator(0,-1,-1,-1,-1,-1));
    this->cubes[25].translate(this->calculator(0,1,-1,-1,-1,-1));
}

void Show::vertices_initializer(int index)//设置每面的颜色
{
    float temp[6]={0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    switch (index)
    {
    case 0:
        temp[0] = float(this->state_for_show(5,2));
        temp[2] = float(this->state_for_show(4,0));
        temp[4] = float(this->state_for_show(1,0));
        break;
    case 1:
        temp[0] = float(this->state_for_show(5,1));
        temp[4] = float(this->state_for_show(1,1));
        break;
    case 2:
        temp[0] = float(this->state_for_show(5,0));
        temp[3] = float(this->state_for_show(0,2));
        temp[4] = float(this->state_for_show(1,2));
        break;
    case 3:
        temp[0] = float(this->state_for_show(5,5));
        temp[2] = float(this->state_for_show(4,3));
        break;
    case 4:
        temp[0] = float(this->state_for_show(5,4));
        break;
    case 5:
        temp[0] = float(this->state_for_show(5,3));
        temp[3] = float(this->state_for_show(0,5));
        break;
    case 6:
        temp[0] = float(this->state_for_show(5,8));
        temp[2] = float(this->state_for_show(4,6));
        temp[5] = float(this->state_for_show(3,6));
        break;
    case 7:
        temp[0] = float(this->state_for_show(5,7));
        temp[5] = float(this->state_for_show(3,7));
        break;
    case 8:
        temp[0] = float(this->state_for_show(5,6));
        temp[3] = float(this->state_for_show(0,8));
        temp[5] = float(this->state_for_show(3,8));
        break;
    case 9:
        temp[2] = float(this->state_for_show(4,1));
        temp[4] = float(this->state_for_show(1,3));
        break;
    case 10:
        temp[4] = float(this->state_for_show(1,4));
        break;
    case 11:
        temp[3] = float(this->state_for_show(0,1));
        temp[4] = float(this->state_for_show(1,5));
        break;
    case 12:
        temp[2] = float(this->state_for_show(4,4));
        break;
    case 13:
        temp[3] = float(this->state_for_show(0,4));
        break;
    case 14:
        temp[2] = float(this->state_for_show(4,7));
        temp[5] = float(this->state_for_show(3,3));
        break;
    case 15:
        temp[5] = float(this->state_for_show(3,4));
        break;
    case 16:
        temp[3] = float(this->state_for_show(0,7));
        temp[5] = float(this->state_for_show(3,5));
        break;
    case 17:
        temp[1] = float(this->state_for_show(2,0));
        temp[2] = float(this->state_for_show(4,2));
        temp[4] = float(this->state_for_show(1,6));
        break;
    case 18:
        temp[1] = float(this->state_for_show(2,1));
        temp[4] = float(this->state_for_show(1,7));
        break;
    case 19:
        temp[1] = float(this->state_for_show(2,2));
        temp[3] = float(this->state_for_show(0,0));
        temp[4] = float(this->state_for_show(1,8));
        break;
    case 20:
        temp[1] = float(this->state_for_show(2,3));
        temp[2] = float(this->state_for_show(4,5));
        break;
    case 21:
        temp[1] = float(this->state_for_show(2,4));
        break;
    case 22:
        temp[1] = float(this->state_for_show(2,5));
        temp[3] = float(this->state_for_show(0,3));
        break;
    case 23:
        temp[1] = float(this->state_for_show(2,6));
        temp[2] = float(this->state_for_show(4,8));
        temp[5] = float(this->state_for_show(3,0));
        break;
    case 24:
        temp[1] = float(this->state_for_show(2,7));
        temp[5] = float(this->state_for_show(3,1));
        break;
    case 25:
        temp[1] = float(this->state_for_show(2,8));
        temp[3] = float(this->state_for_show(0,6));
        temp[5] = float(this->state_for_show(3,2));
        break;
    }
    for(int i=0;i<6;++i)
        for(int j=0;j<6;++j)
            this->vertices[36*i+6*j+5] = temp[i];

}

void Show::initializeGL()
{
    initializeOpenGLFunctions();

    /*VAO & VBO*/
    glGenVertexArrays(1, &VAO_id);
    glGenBuffers(1, &VBO_id);
    //    glGenBuffers(1, &EBO_id);

    glBindVertexArray(VAO_id);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_id);
    //    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_id);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/shader.vert");
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/shader.frag");
    shaderProgram.link();
    shaderProgram.bind();
    shaderProgram.setUniformValue("texture0", 0);
    shaderProgram.setUniformValue("texture1", 1);
    shaderProgram.setUniformValue("texture2", 2);
    shaderProgram.setUniformValue("texture3", 3);
    shaderProgram.setUniformValue("texture4", 4);
    shaderProgram.setUniformValue("texture5", 5);
    shaderProgram.setUniformValue("texture6", 6);

    faces[0] = new QOpenGLTexture(QImage(":/image/images/gray.png"));
    faces[1] = new QOpenGLTexture(QImage(":/image/images/red.png"));
    faces[2] = new QOpenGLTexture(QImage(":/image/images/blue.png"));
    faces[3] = new QOpenGLTexture(QImage(":/image/images/white.png"));
    faces[4] = new QOpenGLTexture(QImage(":/image/images/green.png"));
    faces[5] = new QOpenGLTexture(QImage(":/image/images/orange.png"));
    faces[6] = new QOpenGLTexture(QImage(":/image/images/yellow.png"));
    faces[7] = new QOpenGLTexture(QImage(":/image/images/R_79_pure.jpg"));

    for(GLuint i=0;i<8;++i)
        faces[i]->bind(i);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);//VBO
    //    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);//EBO
    glBindVertexArray(0);//VAO
    //    this->timer->start(300);
//    file<<"original:"<<std::endl<<state_for_show<<std::endl;
}


void Show::paintGL()
{


    //            this->shaderProgram.setUniformValue("projection", projection);
    glClearColor(0.2f,0.3f,0.3f,1.0f);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderProgram.bind();
    glBindVertexArray(VAO_id);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_id);
    shaderProgram.setUniformValue("projection", projection);
    shaderProgram.setUniformValue("view", view);
    shaderProgram.setUniformValue("model", model);
    for(int i=0;i<26;++i)
    {
        this->vertices_initializer(i);
        shaderProgram.setUniformValue("self_matrix", cubes[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    //    pic2->bind(1);

    //    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_id);

    //        shaderProgram.setUniformValue("Matrix", Matrix);
    //    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0/*&indices*/);

}


void Show::resizeGL(int w, int h)
{
    Q_UNUSED(w);
    Q_UNUSED(h);
}

void Show::rotate(char rotation_, bool is_twice_)
{

    this->rotation = rotation_;
    this->is_twice = is_twice_;
    this->total_angle = 0;
    if(is_twice_)
        this->angle_max = 180;
    else
        this->angle_max = 90;
    connect(this->timer_for_rotate, &QTimer::timeout, this, [&]()
    {
        if(total_angle >= this->angle_max || total_angle <= - this->angle_max)
        {
//            file<<"before roration:"<<std::endl<<state_for_show<<std::endl;
            timer_for_rotate->stop();
            this->cube_matrix_initializer();
            this->state_for_show(this->rotation, this->is_reverse, this->is_twice);
//            switch (rotation)
//            {
//            case 'U':
//                this->state_for_show('U', this->is_reverse, is_twice);
//                break;
//            case 'D':
//                this->state_for_show('D', this->is_reverse, is_twice);
//                break;
//            case 'L':
//                this->state_for_show('L', this->is_reverse, is_twice);
//                break;
//            case 'R':
//                this->state_for_show('R', this->is_reverse, is_twice);
//                break;
//            case 'F':
//                this->state_for_show('F', this->is_reverse, is_twice);
//                break;
//            case 'B':
//                this->state_for_show('B', this->is_reverse, is_twice);
//                break;
//            }
//            file<<"after roration:"<<std::endl<<state_for_show<<std::endl<<"end"<<std::endl<<std::endl;
            total_angle = 0;
            this->update();
            timer_for_rotate->disconnect();
            this->is_rotating = false;
            return;
        }
        int angle_temp = this->angle;
        if(this->is_reverse)
            angle_temp = -this->angle;
        QMatrix4x4 temp;
        switch (rotation)
        {
        case 'U':
            temp.rotate(-angle_temp, 0,1.0,0);
            this->cubes[0] = temp * cubes[0];
            this->cubes[1] = temp * cubes[1];
            this->cubes[2] = temp * cubes[2];
            this->cubes[3] = temp * cubes[3];
            this->cubes[4] = temp * cubes[4];
            this->cubes[5] = temp * cubes[5];
            this->cubes[6] = temp * cubes[6];
            this->cubes[7] = temp * cubes[7];
            this->cubes[8] = temp * cubes[8];
            break;
        case 'D':
            temp.rotate(angle_temp, 0,1.0,0);
            this->cubes[17] = temp * cubes[17];
            this->cubes[18] = temp * cubes[18];
            this->cubes[19] = temp * cubes[19];
            this->cubes[20] = temp * cubes[20];
            this->cubes[21] = temp * cubes[21];
            this->cubes[22] = temp * cubes[22];
            this->cubes[23] = temp * cubes[23];
            this->cubes[24] = temp * cubes[24];
            this->cubes[25] = temp * cubes[25];
            break;
        case 'L':
            temp.rotate(angle_temp, 1.0,0,0);
            this->cubes[0] = temp * cubes[0];
            this->cubes[3] = temp * cubes[3];
            this->cubes[6] = temp * cubes[6];
            this->cubes[9] = temp * cubes[9];
            this->cubes[12] = temp * cubes[12];
            this->cubes[14] = temp * cubes[14];
            this->cubes[17] = temp * cubes[17];
            this->cubes[20] = temp * cubes[20];
            this->cubes[23] = temp * cubes[23];
            break;
        case 'R':
            temp.rotate(-angle_temp, 1.0,0,0);
            this->cubes[2] = temp * cubes[2];
            this->cubes[5] = temp * cubes[5];
            this->cubes[8] = temp * cubes[8];
            this->cubes[11] = temp * cubes[11];
            this->cubes[13] = temp * cubes[13];
            this->cubes[16] = temp * cubes[16];
            this->cubes[19] = temp * cubes[19];
            this->cubes[22] = temp * cubes[22];
            this->cubes[25] = temp * cubes[25];
            break;
        case 'F':
            temp.rotate(-angle_temp, 0,0,1.0);
            this->cubes[0] = temp * cubes[0];
            this->cubes[1] = temp * cubes[1];
            this->cubes[2] = temp * cubes[2];
            this->cubes[9] = temp * cubes[9];
            this->cubes[10] = temp * cubes[10];
            this->cubes[11] = temp * cubes[11];
            this->cubes[17] = temp * cubes[17];
            this->cubes[18] = temp * cubes[18];
            this->cubes[19] = temp * cubes[19];
            break;
        case 'B':
            temp.rotate(angle_temp, 0,0,1.0);
            this->cubes[6] = temp * cubes[6];
            this->cubes[7] = temp * cubes[7];
            this->cubes[8] = temp * cubes[8];
            this->cubes[14] = temp * cubes[14];
            this->cubes[15] = temp * cubes[15];
            this->cubes[16] = temp * cubes[16];
            this->cubes[23] = temp * cubes[23];
            this->cubes[24] = temp * cubes[24];
            this->cubes[25] = temp * cubes[25];
            break;
        }
        total_angle += angle_temp;
        this->update();
    });
    timer_for_rotate->start();
}


//switch (rotation)
//{
//case 'U':
//    this->state_for_show('U', this->is_reverse);
//    anti_temp.rotate(-total_angle, 0,1.0,0);
//    this->cubes[0] = anti_temp * cubes[0];
//    this->cubes[1] = anti_temp * cubes[1];
//    this->cubes[2] = anti_temp * cubes[2];
//    this->cubes[3] = anti_temp * cubes[3];
//    this->cubes[4] = anti_temp * cubes[4];
//    this->cubes[5] = anti_temp * cubes[5];
//    this->cubes[6] = anti_temp * cubes[6];
//    this->cubes[7] = anti_temp * cubes[7];
//    this->cubes[8] = anti_temp * cubes[8];
//    break;
//case 'D':
//    this->state_for_show('D', this->is_reverse);
//    anti_temp.rotate(-total_angle, 0,1.0,0);
//    this->cubes[17] = anti_temp * cubes[17];
//    this->cubes[18] = anti_temp * cubes[18];
//    this->cubes[19] = anti_temp * cubes[19];
//    this->cubes[20] = anti_temp * cubes[20];
//    this->cubes[21] = anti_temp * cubes[21];
//    this->cubes[22] = anti_temp * cubes[22];
//    this->cubes[23] = anti_temp * cubes[23];
//    this->cubes[24] = anti_temp * cubes[24];
//    this->cubes[25] = anti_temp * cubes[25];
//    break;
//case 'L':
//    this->state_for_show('L', this->is_reverse);
//    anti_temp.rotate(-total_angle, 1.0,0,0);
//    this->cubes[0] = anti_temp * cubes[0];
//    this->cubes[3] = anti_temp * cubes[3];
//    this->cubes[6] = anti_temp * cubes[6];
//    this->cubes[9] = anti_temp * cubes[9];
//    this->cubes[12] = anti_temp * cubes[12];
//    this->cubes[14] = anti_temp * cubes[14];
//    this->cubes[17] = anti_temp * cubes[17];
//    this->cubes[20] = anti_temp * cubes[20];
//    this->cubes[23] = anti_temp * cubes[23];
//    break;
//case 'R':
//    this->state_for_show('R', this->is_reverse);
//    anti_temp.rotate(-total_angle, 1.0,0,0);
//    this->cubes[2] = anti_temp * cubes[2];
//    this->cubes[5] = anti_temp * cubes[5];
//    this->cubes[8] = anti_temp * cubes[8];
//    this->cubes[11] = anti_temp * cubes[11];
//    this->cubes[13] = anti_temp * cubes[13];
//    this->cubes[16] = anti_temp * cubes[16];
//    this->cubes[19] = anti_temp * cubes[19];
//    this->cubes[22] = anti_temp * cubes[22];
//    this->cubes[25] = anti_temp * cubes[25];
//    break;
//case 'F':
//    this->state_for_show('F', this->is_reverse);
//    anti_temp.rotate(-total_angle, 0,0,1.0);
//    this->cubes[0] = anti_temp * cubes[0];
//    this->cubes[1] = anti_temp * cubes[1];
//    this->cubes[2] = anti_temp * cubes[2];
//    this->cubes[9] = anti_temp * cubes[9];
//    this->cubes[10] = anti_temp * cubes[10];
//    this->cubes[11] = anti_temp * cubes[11];
//    this->cubes[17] = anti_temp * cubes[17];
//    this->cubes[18] = anti_temp * cubes[18];
//    this->cubes[19] = anti_temp * cubes[19];
//    break;
//case 'B':
//    this->state_for_show('B', this->is_reverse);
//    anti_temp.rotate(-total_angle, 0,0,1.0);
//    this->cubes[6] = anti_temp * cubes[6];
//    this->cubes[7] = anti_temp * cubes[7];
//    this->cubes[8] = anti_temp * cubes[8];
//    this->cubes[14] = anti_temp * cubes[14];
//    this->cubes[15] = anti_temp * cubes[15];
//    this->cubes[16] = anti_temp * cubes[16];
//    this->cubes[23] = anti_temp * cubes[23];
//    this->cubes[24] = anti_temp * cubes[24];
//    this->cubes[25] = anti_temp * cubes[25];
//    break;
//}
