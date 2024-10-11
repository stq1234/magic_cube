/*
 *
 *一些设定：
 * 颜色顺序：红 蓝 白 绿 橙 黄  ，编号：1~6
 *
 *
 * 定点顺序
        2_______3
        /      /|
       /      / |
     0/______/1 |
      |      |  /7  里面的是6
      |      | /
      |______|/
       4      5
 *
 * 面顺序：
 *     U D L R F B
 * 块顺序及编号：从上到下，同层从左到右，从前到后，黄面向上，蓝面向前0~25。顶层0~8，中层9~16，下层17~25
 *
 *
 */



#ifndef GLOBEL_H
#define GLOBEL_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
#include <QGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include <QPushButton>
#include <QFont>
#include <QVBoxLayout>
#include <QPaintEvent>
#include <QPen>
#include <QPainter>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QMessageBox>
#include <QLabel>
#include <QTimer>
#include <QWidget>
#include <QInputDialog>
#include <QMatrix4x4>
#include <queue>

#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <process.h>

class Globel
{
public:
    Globel();
};
extern int application_height;
extern int application_width;
//extern std::fstream file;


enum MOVE
{
    U  = 0,
    D  = 1,
    L  = 2,
    R  = 3,
    F  = 4,
    B  = 5,
    U_ = 6,
    D_ = 7,
    L_ = 8,
    R_ = 9,
    F_ = 10,
    B_ = 11,
    U2 = 12,
    D2 = 13,
    L2 = 14,
    R2 = 15,
    F2 = 16,
    B2 = 17
};

enum COLOR
{
    EMPTY  = 0,
    RED    = 1,
    BLUE   = 2,
    WHITE  = 3,
    GREEN  = 4,
    ORINGE = 5,
    YELLOW = 6
};

#endif // GLOBEL_H
