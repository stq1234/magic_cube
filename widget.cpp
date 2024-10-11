#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setGeometry(0, 0, application_width, application_height);
    this->setWindowFlags(Qt::Window);

    this->start = new QPushButton(this);
    QFont font;
    font.setPointSize(39);//maybe need to be changed.
    this->start->setFont(font);
    this->start->setGeometry(application_width*40/100, application_height/2, application_width*20/100, application_height*7/100);
    this->start->setText("进入主界面");

    //    font.setPixelSize(application_width*70/3072);
    this->exit = new QPushButton(this);
    this->exit->setFont(font);
    this->exit->setGeometry(application_width*40/100, application_height*65/100, application_width*20/100, application_height*7/100);
    this->exit->setText("退出程序");

    connect(this->start, &QPushButton::clicked, this, &Widget::_on_start_clicked);
    connect(this->exit, &QPushButton::clicked, this, &Widget::_on_exit_clicked);

//    this->exit->hide();
}

Widget::~Widget()
{
    delete this->start;
    delete this->exit;
    delete ui;
}

void Widget::paintEvent(QPaintEvent* e)
{
    Q_UNUSED(e);
    //    QPainter painter(this);
    //    painter.setPen(Qt::blue);
    //    std::clog<<application_width<<' '<<application_height<<std::endl;
    //    painter.drawRect(5, 5, application_width-10, application_height-10);


    //show some pictures here.
}

void Widget::_on_start_clicked()
{
    if(this->solve == nullptr)
    {
        //        std::clog<<"solve is newed"<<std::endl;
        this->solve = new Solve();
        connect(this->solve->back, &QPushButton::clicked, this, &Widget::swap_widgets);
    }
//    this->exit->show();
    this->solve->showFullScreen();
//    this->solve->show();
    this->hide();

}


void Widget::_on_exit_clicked()
{
//    file.close();
    std::exit(0);
}

void Widget::swap_widgets()
{
    this->solve->hide();
    this->show();
}
