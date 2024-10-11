#include "scan.h"
#include "ui_scan.h"

Scan::Scan(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Scan)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::Window);
    this->setGeometry(0, 0, application_width, application_height);

    QFont font;
    font.setPointSize(12);

    this->timer_for_photo_processer = new QTimer;
    this->timer_for_photo_processer->setInterval(38);
    connect(this->timer_for_photo_processer, &QTimer::timeout, this, &Scan::photo_processer);


    this->change_mode = new QPushButton(this);
    this->change_mode->setGeometry(application_width*80/100, application_height*80/100,
                                   application_width*10/100, application_height*5/100);
    this->change_mode->setText("转换至手动输入");
    this->change_mode->setFont(font);
    connect(this->change_mode, &QPushButton::clicked, this, &Scan::_on_change_mode_clicked);

    //initialize button finish

    this->finish = new QPushButton(this);
    this->timer_for_enable_finish_button = new QTimer;
    this->timer_for_enable_finish_button->setInterval(400);
    connect(this->timer_for_enable_finish_button, &QTimer::timeout, this, [&](){
        bool has_finished = true;
        for(int i = 0; i < 6; ++i)
        {
            if(!this->is_finish[i])
            {
                has_finished = false;
                break;
            }
        }
        this->finish->setEnabled(has_finished);
    });
    this->timer_for_enable_finish_button->start();
    this->finish->setGeometry(application_width*80/100, application_height*90/100,
                              application_width*10/100, application_height*5/100);
    this->setFont(font);
    this->finish->setText("完成");

    //initialize button save
    this->save_one_face = new QPushButton(this);
    this->timer_for_save_button = new QTimer();
    this->timer_for_save_button->setInterval(400);
    connect(this->timer_for_save_button, &QTimer::timeout, this, [&](){
        bool is_ok = true;
        for(int i = 0; i < 9; ++i)
        {
            if(this->data_for_one_face[i] >6 || this->data_for_one_face[i] < 1)
            {
                is_ok = false;
                break;
            }
        }
        this->save_one_face->setEnabled(is_ok);
    });
    this->save_one_face->setText("保存当前面");
    font.setPointSize(20);
    this->save_one_face->setFont(font);
    this->save_one_face->setGeometry(application_width*25/100, application_height*35/100, application_width*10/100, application_height*4/100);
    this->timer_for_save_button->start();
    connect(this->save_one_face, &QPushButton::clicked, this, &Scan::_on_save_one_face_clicked);


    for(int i = 0; i < 3; ++i)
        for(int j = 0; j < 3; ++j)
        {
            this->show_one_face[i*3+j] = new QPushButton(this);
            this->show_one_face[i*3+j]->setGeometry(application_width*(7+j*5)/100, application_width*(7+i*5)/100, application_width*5/100, application_width*5/100);
            //            if(i*3+j == 4)
            //                continue;
            //            connect(this->show_one_face[i*3+j], &QPushButton::clicked, this, [&](){
            //                int index = i*3+j;
            //                this->current_cube = index;
            //                if(index > 4)
            //                    index--;
            //                this->current_cube = index;
            //                std::clog<<"in lambda:"<<"index"<<index<<std::endl;
            //            });
        }
    this->show_one_face[4]->setEnabled(false);
    //set connections for all buttons
    connect(this->show_one_face[0], &QPushButton::clicked, this, [&](){this->current_cube = 0;this->has_chosen_a_cube = true;this->update();});
    connect(this->show_one_face[1], &QPushButton::clicked, this, [&](){this->current_cube = 1;this->has_chosen_a_cube = true;this->update();});
    connect(this->show_one_face[2], &QPushButton::clicked, this, [&](){this->current_cube = 2;this->has_chosen_a_cube = true;this->update();});
    connect(this->show_one_face[3], &QPushButton::clicked, this, [&](){this->current_cube = 3;this->has_chosen_a_cube = true;this->update();});
    connect(this->show_one_face[5], &QPushButton::clicked, this, [&](){this->current_cube = 5;this->has_chosen_a_cube = true;this->update();});
    connect(this->show_one_face[6], &QPushButton::clicked, this, [&](){this->current_cube = 6;this->has_chosen_a_cube = true;this->update();});
    connect(this->show_one_face[7], &QPushButton::clicked, this, [&](){this->current_cube = 7;this->has_chosen_a_cube = true;this->update();});
    connect(this->show_one_face[8], &QPushButton::clicked, this, [&](){this->current_cube = 8;this->has_chosen_a_cube = true;this->update();});
    for(int i = 0; i < 9; ++i)
    {
        this->show_one_face[i]->setStyleSheet("background-color:rgb(200,200,200);");
    }


    //initialize color_button_for_set
    for(int i = 0; i < 6; ++i)
    {
        this->color_button_for_set[i] = new QPushButton(this);
        this->color_button_for_set[i]->setGeometry(application_width*(6+i*3)/100, application_width*25/100, application_width*2/100, application_width*2/100);
    }
    this->color_button_for_set[0]->setStyleSheet("background-color:red;");
    connect(this->color_button_for_set[0], &QPushButton::clicked, this, &Scan::_on_set_red_clicked);

    this->color_button_for_set[1]->setStyleSheet("background-color:blue;");
    connect(this->color_button_for_set[1], &QPushButton::clicked, this, &Scan::_on_set_blue_clicked);

    this->color_button_for_set[2]->setStyleSheet("background-color:white;");
    connect(this->color_button_for_set[2], &QPushButton::clicked, this, &Scan::_on_set_white_clicked);

    this->color_button_for_set[3]->setStyleSheet("background-color:green;");
    connect(this->color_button_for_set[3], &QPushButton::clicked, this, &Scan::_on_set_green_clicked);

    this->color_button_for_set[4]->setStyleSheet("background-color:orange;");
    connect(this->color_button_for_set[4], &QPushButton::clicked, this, &Scan::_on_set_orange_clicked);

    this->color_button_for_set[5]->setStyleSheet("background-color:yellow;");
    connect(this->color_button_for_set[5], &QPushButton::clicked, this, &Scan::_on_set_yellow_clicked);



    this->label_for_tips_1 = new QLabel(this);
    this->label_for_tips_1->setGeometry(application_width*5/109, application_height*83/100, application_width*35/100, application_height*5/100);
    font.setPointSize(18);
    this->label_for_tips_1->setFont(font);
    this->label_for_tips_1->setText("请选择中心块颜色，进行该面的颜色录入");

    this->label_for_show_picture = new QLabel(this);

    this->label_for_tips_2 = new QLabel(this);
    this->label_for_tips_2->setGeometry(application_width*59/100, application_height*57/100, application_width*19/100, application_height*5/100);

    for(int i = 0; i < 6; ++i)
    {
        this->button_for_choose_color[i] = new QPushButton(this);
        this->button_for_choose_color[i]->setGeometry(application_width*(5+6*i)/100, application_height*90/100, application_width*4/100, application_width*4/100);
        //        this->button_for_choose_color[i]->setFont(font);
    }


    this->button_for_choose_color[0]->setStyleSheet("background-color:red;");
    connect(this->button_for_choose_color[0], &QPushButton::clicked, this, &Scan::_on_choose_red_clicked);

    this->button_for_choose_color[1]->setStyleSheet("background-color:blue;");
    connect(this->button_for_choose_color[1], &QPushButton::clicked, this, &Scan::_on_choose_blue_clicked);

    this->button_for_choose_color[2]->setStyleSheet("background-color:white;");
    connect(this->button_for_choose_color[2], &QPushButton::clicked, this, &Scan::_on_choose_white_clicked);

    this->button_for_choose_color[3]->setStyleSheet("background-color:green;");
    connect(this->button_for_choose_color[3], &QPushButton::clicked, this, &Scan::_on_choose_green_clicked);

    this->button_for_choose_color[4]->setStyleSheet("background-color:orange;");
    connect(this->button_for_choose_color[4], &QPushButton::clicked, this, &Scan::_on_choose_orange_clicked);

    this->button_for_choose_color[5]->setStyleSheet("background-color:yellow;");
    connect(this->button_for_choose_color[5], &QPushButton::clicked, this, &Scan::_on_choose_yellow_clicked);



    //just for test, remember to delete these.

//    this->button_for_save=new QPushButton(this);
//    this->button_for_save->setGeometry(application_width/2,application_height*6/10+180,150,100);
//    this->button_for_save->setText("save a picture");
//    connect(button_for_save,&QPushButton::clicked, this,[&](){
//        cv::Mat image;
//        camera>>image;
//        QString colors_ = QInputDialog::getText(this, "输入颜色", "RBWGOYN");
//        std::string colors=colors_.toStdString();
//        image = image(cv::Range(90,390), cv::Range(170,470));
//        for(int i=0;i<9;++i)
//        {
//            if(i==4)continue;
//            int* cur_color;

//            switch (colors[i]) {
//            case 'R':
//                cur_color=&(this->r_num);
//                break;
//            case 'B':
//                cur_color=&(this->b_num);
//                break;
//            case 'W':
//                cur_color=&(this->w_num);
//                break;
//            case 'G':
//                cur_color=&(this->g_num);
//                break;
//            case 'O':
//                cur_color=&(this->o_num);
//                break;
//            case 'Y':
//                cur_color=&(this->y_num);
//                break;
//            case 'N':
//                cur_color=&(this->n_num);
//                break;
//            }
//            std::string file_name = QString("D:\\train_data\\"+colors_[i]+"_"+QString::number(*cur_color)+".jpg").toStdString();
//            cv::imwrite(file_name, image(cv::Range(i/3*100, (i/3+1)*100), cv::Range((i%3)*100, (i%3+1)*100)));
//            (*cur_color)++;
//        }
//        std::clog<<"save a image"<<std::endl;
//    });
    {
        //    this->test_l = new QLabel(this);
        //    this->test_l->setGeometry(application_width/2,application_height*6/10,500,100);
        //    this->test_l->setText("HSV_min:("+QString::number(H_min)+", "+QString::number(S_min)+", "+QString::number(V_min)+")");

        //    this->test_u = new QLabel(this);
        //    this->test_u->setGeometry(application_width/2,application_height*6/10+120,500,100);
        //    this->test_u->setText("HSV_max:("+QString::number(H_max)+", "+QString::number(S_max)+", "+QString::number(V_max)+")");

        //    test=new QLabel(this);
        //    test->setGeometry(application_width*50/100,application_height*75/100,400,300);

        //    tip=new QLabel(this);
        //    tip->setGeometry(application_width*45/100,application_height*70/100,100,100);
        //    tip->setText("hello");

        //    int hsv_w=application_width*80/100;
        //    int hsv_h=application_height*45/100;
        //    for(int i=0;i<3;++i)
        //    {
        //        hsv[i]=new QPushButton(this);
        //        hsv[i]->setGeometry(hsv_w,hsv_h+i*120,70,50);
        //        hsv[i]->setText("hello world");
        //        if(i==0)
        //        {
        //            hsv[i]->setText("H");
        //            connect(hsv[i],&QPushButton::clicked,this,[&](){
        //                H_min= QInputDialog::getInt(this,"h","h",H_min,0,180,1);
        //                H_max= QInputDialog::getInt(this,"h","h",H_max,0,180,1);
        //                //               this->test_l = new QLabel(this);
        //                //               this->test_l->setGeometry(application_width/2,application_height*6/10,300,300);
        //                this->test_l->setText("HSV_min:("+QString::number(H_min)+", "+QString::number(S_min)+", "+QString::number(V_min)+")");

        //                //               this->test_u = new QLabel(this);
        //                //               this->test_u->setGeometry(application_width/2,application_height*6/10+400,300,300);
        //                this->test_u->setText("HSV_max:("+QString::number(H_max)+", "+QString::number(S_max)+", "+QString::number(V_max)+")");

        //            });
        //        }
        //        if(i==1)
        //        {
        //            hsv[i]->setText("S");
        //            connect(hsv[i],&QPushButton::clicked,this,[&](){
        //                S_min= QInputDialog::getInt(this,"s","s",S_min,0,255,1);
        //                S_max= QInputDialog::getInt(this,"s","s",S_max,0,255,1);
        //                //               this->test_l = new QLabel(this);
        //                //               this->test_l->setGeometry(application_width/2,application_height*6/10,300,300);
        //                this->test_l->setText("HSV_min:("+QString::number(H_min)+", "+QString::number(S_min)+", "+QString::number(V_min)+")");

        //                //               this->test_u = new QLabel(this);
        //                //               this->test_u->setGeometry(application_width/2,application_height*6/10+400,300,300);
        //                this->test_u->setText("HSV_max:("+QString::number(H_max)+", "+QString::number(S_max)+", "+QString::number(V_max)+")");

        //            });
        //        }
        //        if(i==2)
        //        {
        //            hsv[i]->setText("V");
        //            connect(hsv[i],&QPushButton::clicked,this,[&](){
        //                V_min= QInputDialog::getInt(this,"v","v",V_min,0,255,1);
        //                V_max= QInputDialog::getInt(this,"v","v",V_max,0,255,1);
        //                //               this->test_l = new QLabel(this);
        //                //               this->test_l->setGeometry(application_width/2,application_height*6/10,300,300);
        //                this->test_l->setText("HSV_min:("+QString::number(H_min)+", "+QString::number(S_min)+", "+QString::number(V_min)+")");

        //                //               this->test_u = new QLabel(this);
        //                //               this->test_u->setGeometry(application_width/2,application_height*6/10+400,300,300);
        //                this->test_u->setText("HSV_max:("+QString::number(H_max)+", "+QString::number(S_max)+", "+QString::number(V_max)+")");

        //            });
        //        }
        //    }

        //    timer_for_test=new QTimer();
        //    timer_for_test->start(1500);
        //    //    connect(timer_for_test,&QTimer::timeout,this, [&](){
        //    //        switch (jishuqi%8)
        //    //        {
        //    //        case 1:
        //    //            H_min+=2;
        //    //            break;
        //    //        case 2:
        //    //            H_min-=2;
        //    //            S_min+=3;
        //    //            break;
        //    //        case 3:
        //    //            S_min-=3;
        //    //            V_min+=3;
        //    //            break;
        //    //        case 4:
        //    //            V_min-=3;
        //    //            H_min+=2;
        //    //            S_min+=3;
        //    //            break;
        //    //        case 5:
        //    //            H_min-=2;
        //    //            V_min+=3;
        //    //            break;
        //    //        case 6:
        //    //            S_min-=3;
        //    //            H_min+=2;
        //    //            break;
        //    //        case 7:
        //    //            S_min+=3;
        //    //            break;
        //    //        default:
        //    //            break;
        //    //        }
        //    //        H_min=std::min(H_min,H_max);
        //    //        S_min=std::min(S_min,S_max);
        //    //        V_min=std::min(V_min,V_max);
        //    //        if(H_min>=H_max&&S_min>=S_max&&V_min>=V_max)
        //    //        {
        //    //            timer_for_test->stop();
        //    //            jishuqi=0;
        //    //            S_min=0;
        //    //            H_min=0;
        //    //            V_min=0;
        //    //            switch (order)
        //    //            {
        //    //            case 0:
        //    //                tip->setText("红");
        //    //                break;
        //    //            case 1:
        //    //                tip->setText("蓝");
        //    //                break;
        //    //            case 2:
        //    //                tip->setText("白");
        //    //                break;
        //    //            case 3:
        //    //                tip->setText("绿");
        //    //                break;
        //    //            case 4:
        //    //                tip->setText("橙");
        //    //                break;
        //    //            case 5:
        //    //                tip->setText("黄");
        //    //                break;
        //    //            }
        //    //            order++;
        //    //            if(order==6)
        //    //                timer_for_test->stop();
        //    //            this->test_l->setText("HSV_min:("+QString::number(H_min)+", "+QString::number(S_min)+", "+QString::number(V_min)+")");
        //    //            return;
        //    //        }
        //    //        this->jishuqi++;
        //    //        this->test_l->setText("HSV_min:("+QString::number(H_min)+", "+QString::number(S_min)+", "+QString::number(V_min)+")");
        //    //        this->test_u->setText("HSV_max:("+QString::number(H_max)+", "+QString::number(S_max)+", "+QString::number(V_max)+")");
        //    //    });
        //    connect(timer_for_test,&QTimer::timeout, this,[&](){

        //    });
        //    //hsv test end
    }
}

void Scan::photo_processer()
{

    if(!this->camera.isOpened())
    {
        QMessageBox::warning(nullptr, "警告", "未能找到摄像头，已切换至手动输入模式");
        this->timer_for_photo_processer->stop();
        this->_on_change_mode_clicked();
        this->label_for_show_picture->hide();
        return;
    }
    cv::Mat image, image_for_show, small;//image为原图，image_for_show为展示图，small为操作图
    this->camera>>image;

    //show
    cv::flip(image, image_for_show, 1);
    cv::cvtColor(image_for_show,image_for_show,cv::COLOR_BGR2RGB);
    cv::drawContours(image_for_show, this->rect_for_scan,-1,cv::Scalar(0,0,255),5);
    QImage qim(image_for_show.data, image_for_show.cols, image_for_show.rows, image_for_show.step, QImage::Format_RGB888);
    double w = double(application_width)*40./100.;
    double h = w /double(qim.width())*double(qim.height());
    this->label_for_show_picture->setGeometry(application_width*50/100, application_height*5/100, w, h);
    qim =qim.scaled(this->label_for_show_picture->width(),this->label_for_show_picture->height());
    this->label_for_show_picture->setPixmap(QPixmap::fromImage(qim));
    //show end

    //small
    if(this->current_central_color == -1 || !this->has_chosen_a_face)
    {
        this->label_for_tips_2->setText("请先选择要输入的面的中心块颜色");
        return;
    }
    else
    {
        this->label_for_tips_2->setText("");
    }
    small = image(cv::Range(this->rect_for_scan_start_y, this->rect_for_scan_start_y+this->rect_for_scan_width),
                  cv::Range(this->rect_for_scan_start_x, this->rect_for_scan_start_x+this->rect_for_scan_width));

    cv::GaussianBlur(small, small, cv::Size(5,5), 0);
    for (int row = 0; row < small.size().height; row++)
        for (int col = 0; col < small.size().width; col++)
            for(int channel = 0; channel < 3; ++channel)
                small.at<cv::Vec3b>(row, col)[channel] = cv::saturate_cast<uchar>(small.at<cv::Vec3b>(row, col)[channel] * 3/2 + 15);
//    cv::imshow("improved",small);
    cv::cvtColor(small, small, cv::COLOR_BGR2HSV);

    cv::Mat temp;
    int data_temp[9] = { 0 }, get_num = 1;
    data_temp[4] = this->current_central_color;
    for(int i = 1; i <= 6; ++i)
    {
        cv::inRange(small, this->colors[i - 1][0], this->colors[i - 1][1], temp);
        for(int j = 0; j < 9; ++j)
        {
            if(j == 4)
                continue;
            if(data_temp[j] != 0)
                continue;
            double times = 0.;
            for(int k = 0; k < 1000; k++)
            {
                if(temp.at<uchar>(std::rand()%(this->rect_for_scan_width/3)+j/3*this->rect_for_scan_width/3,
                                  std::rand()%(this->rect_for_scan_width/3)+(j%3)*this->rect_for_scan_width/3) == 255)
                    times+=1.;
            }
            if(times/1000. > 0.6)
            {
                data_temp[j] = i;
                get_num++;
            }
        }
    }
    //        for(int i = 0; i < 9; ++i)
    //            this->data_for_one_face[i] = data_temp[i];
    //        this->update();
    if(get_num == 9)
    {
        this->label_for_tips_2->setText("");
        for(int i = 0; i < 9; ++i)
            this->data_for_one_face[i] = data_temp[i];
        this->update();
        return;
    }
    else
    {
        this->label_for_tips_2->setText("未识别到魔方");
    }



    //    cv::inRange(preprocessed,test_l,test_u,processed);

    //    after=processed.clone();
    //    after = cv::Mat::zeros(processed.size(), CV_8UC3);

    //    after.forEach<cv::Vec3b>(
    //        [&](cv::Vec3b& pixel, const int* position) -> void {
    //            if (processed.at<uchar>(position[0], position[1]) == 255) {
    //                pixel = cv::Vec3b(255, 255, 255);
    //            } else {
    //                pixel = cv::Vec3b(0, 0, 0);
    //            }
    //        }
    //    );
    //    cv::medianBlur(after,after,3);
    //    cv::flip(after,after,1);
    //    cv::imshow("test", processed);
    //    QImage t(after.data, after.cols, after.rows, after.step, QImage::Format_RGB888);
    //    t = t.rgbSwapped();
    //    t.save("test.jpg");


    //    cv::flip(image, image_for_show, 1);
    //    QImage qim(image_for_show.data, image_for_show.cols, image_for_show.rows, image_for_show.step, QImage::Format_RGB888);
    //    qim = qim.rgbSwapped();
    //    double w = double(application_width)*45./100.;
    //    double h = w /double(qim.width())*double(qim.height());
    //    this->label_for_show_picture->setGeometry(application_width*50/100, application_height*5/100, w, h);
    //    qim = qim.scaled(this->label_for_show_picture->width(), this->label_for_show_picture->height());
    //    this->label_for_show_picture->setPixmap(QPixmap::fromImage(qim));

    //    qim.rgbSwapped();
    //    qim.rgbSwapped();



    //    label_for_show_picture->setPixmap(QPixmap::fromImage(t));
    //    test->setGeometry(application_width*50,application_height*70,qim.width(),qim.height());
    //    cv::namedWindow("test",0);
    //    cv::resizeWindow("test", 800,600);
    //    cv::imshow("test", image_for_show);
    //    test->show();
    //    std::clog<<processed<<std::endl;
    //process the photo.


    {
        //    //by chatgpt
        //    cv::Mat gray,blurred,canny,dilated,kernel;
        //    std::vector<std::vector<cv::Point>> contours;
        //    std::vector<cv::Vec4i> hierarchy;

        //    cv::cvtColor(small, gray, cv::COLOR_BGR2GRAY);
        //    cv::GaussianBlur(gray, blurred, cv::Size(5, 5), 0);
        //    cv::Canny(blurred,canny,80, 160);
        //    cv::dilate(canny, dilated, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5), cv::Point(-1, -1)));
        //    //    cv::findContours(dilated.clone(), contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
        //    cv::findContours(dilated,contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE, cv::Point());

        //    cv::drawContours(small, contours, -1, cv::Scalar(0, 0, 255), 3);
        //    cv::imshow("small picture", small);
        //    std::vector<cv::Point> candidates;
        //    int index = 0;

        //    std::vector<std::map<std::string, int>> center;
        //    for (size_t i = 0; i < contours.size(); ++i)
        //    {
        //        std::vector<cv::Point>& contour = contours[i];
        //        double peri = cv::arcLength(contour, true);//计算周长
        //        cv::approxPolyDP(contour, contour, 0.1 * peri, true);
        //        double area = cv::contourArea(contour);//计算面积
        //        cv::Moments M = cv::moments(contour);
        //        int cX, cY;
        //        if (static_cast<bool>(M.m00))
        //        {
        //            cX = static_cast<int>(M.m10 / M.m00);
        //            cY = static_cast<int>(M.m01 / M.m00);
        //        }
        //        else
        //        {
        //            cX = -1;
        //            cY = -1;
        //        }
        //        if (area > 800 && area < 22000 && peri <= 150 && peri >= 550 && cX != -1)
        //        {
        //            std::map<std::string, int> tmp;
        //            tmp["index"] = static_cast<int>(i);
        //            tmp["cx"] = cX;
        //            tmp["cy"] = cY;
        //            //            tmp["contour"] = contour;
        //            center.push_back(tmp);
        //            ++index;
        //        }
        //    }
        //    if(!center.empty()){
        //        std::clog<<"not empty "<<center.size()<<" No"<<jishuqi<<std::endl;
        //        jishuqi++;
        //    }
        //    /*cv::Size size = image.size();

        //    std::clog<<"w "<<size.width<<" h " <<size.height<<std::endl;*/

        //    std::sort(center.begin(), center.end(), [](const std::map<std::string, int>& a, const std::map<std::string, int>& b) {
        //        return a.at("cy") < b.at("cy");
        //    });

        //    //    if(center.size()>9)
        //    //    {
        //    //        std::vector<std::map<std::string, int>> row1(center.begin(), center.begin() + 3);
        //    //        std::sort(row1.begin(), row1.end(), [](const std::map<std::string, int>& a, const std::map<std::string, int>& b) {
        //    //            return a.at("cx") < b.at("cx");
        //    //        });
        //    //        std::vector<std::map<std::string, int>> row2(center.begin() + 3, center.begin() + 6);
        //    //        std::sort(row2.begin(), row2.end(), [](const std::map<std::string, int>& a, const std::map<std::string, int>& b) {
        //    //            return a.at("cx") < b.at("cx");
        //    //        });
        //    //        std::vector<std::map<std::string, int>> row3(center.begin() + 6, center.end());
        //    //        std::sort(row3.begin(), row3.end(), [](const std::map<std::string, int>& a, const std::map<std::string, int>& b) {
        //    //            return a.at("cx") < b.at("cx");
        //    //        });
        //    //        center.clear();
        //    //        center.insert(center.end(), row1.begin(), row1.end());
        //    //        center.insert(center.end(), row2.begin(), row2.end());
        //    //        center.insert(center.end(), row3.begin(), row3.end());
        //    //    }

        //    std::vector<std::vector<cv::Point>> finalCandidates;
        //    for (const auto& component : center)
        //    {
        //        int index = component.at("index");
        //        if (index >= 0 && index < contours.size()) {
        //            finalCandidates.push_back(contours[index]);
        //        }
        //    }

        //    if(!finalCandidates.empty())
        //        cv::drawContours(image, finalCandidates, -1, cv::Scalar(0, 0, 255), 3);

        //    //chatgpt end
    }


}



Scan::~Scan()
{
    delete ui;
}

void Scan::operator()(int x)
{
    this->scan_mode = x;
    if(x == 1)
    {
        this->camera = cv::VideoCapture(0);
        this->timer_for_photo_processer->start();
        this->change_mode->setText("转换至手动输入");
        for(int i = 0; i < 6; ++i)
        {
            this->color_button_for_set[i]->hide();
            this->color_button_for_set[i]->setEnabled(false);
        }
        for(int i = 0; i < 9; ++i)
        {
            this->show_one_face[i]->setDisabled(true);
        }
    }
    if(x == 2)
    {
        this->timer_for_photo_processer->stop();
        this->change_mode->setText("转换至拍照输入");
        for(int i = 0; i < 6; ++i)
        {
            this->color_button_for_set[i]->show();
            this->color_button_for_set[i]->setEnabled(true);
        }
        for(int i = 0; i < 9; ++i)
        {
            if(i == 4)
                continue;
            this->show_one_face[i]->setEnabled(true);
        }
    }
    return;
}

void Scan::_on_change_mode_clicked()
{
    switch (this->scan_mode)
    {
    case 1:
    {
        this->scan_mode = 2;
        this->change_mode->setText("转换至拍照输入");
        this->camera.release();
        this->timer_for_photo_processer->stop();
        this->label_for_show_picture->hide();
        for(int i = 0; i < 6; ++i)
        {
            this->color_button_for_set[i]->show();
            this->color_button_for_set[i]->setEnabled(true);
        }
        for(int i = 0; i < 9; ++i)
        {
            if(i == 4)
                continue;
            this->show_one_face[i]->setEnabled(true);
        }
        break;
    }
    case 2:
    {
        this->camera = cv::VideoCapture(0);
        if(!this->camera.isOpened())
        {
            QMessageBox::warning(nullptr, "警告", "未能找到摄像头，请使用手动输入模式");
            break;
        }
        this->scan_mode = 1;
        this->change_mode->setText("转换至手动输入");

        this->label_for_show_picture->show();
        this->timer_for_photo_processer->start();
        for(int i = 0; i < 6; ++i)
        {
            this->color_button_for_set[i]->hide();
            this->color_button_for_set[i]->setEnabled(false);
        }
        for(int i = 0; i < 9; ++i)
        {
            this->show_one_face[i]->setDisabled(true);
        }
        break;
    }
    }
}



void Scan::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QPainter painter(this);
    QPen pen(Qt::black, 4);
    painter.setPen(pen);

    for(int i = 0; i < 9; ++i)
    {
        this->show_one_face[i]->setStyleSheet(this->number2stylestring(this->data_for_one_face[i]));
    }

    int l = application_width*7/100;
    l = l - l % 3;
    int w_start = application_width*10/100;
    int h_start = application_height*58/100;
    for(int i = 0; i < 9; ++i)
        painter.fillRect(w_start+3*l+(i%3)*l/3, h_start+i/3*l/3, l/3, l/3, this->number2qcolor(this->total_data[0][i]));
    for(int i = 0; i < 9; ++i)
        painter.fillRect(w_start+2*l+(i%3)*l/3, h_start-l+i/3*l/3, l/3, l/3, this->number2qcolor(this->total_data[1][i]));
    for(int i = 0; i < 9; ++i)
        painter.fillRect(w_start+2*l+(i%3)*l/3, h_start+i/3*l/3, l/3, l/3, this->number2qcolor(this->total_data[2][i]));
    for(int i = 0; i < 9; ++i)
        painter.fillRect(w_start+2*l+(i%3)*l/3, h_start+l+i/3*l/3, l/3, l/3, this->number2qcolor(this->total_data[3][i]));
    for(int i = 0; i < 9; ++i)
        painter.fillRect(w_start+1*l+(i%3)*l/3, h_start+i/3*l/3, l/3, l/3, this->number2qcolor(this->total_data[4][i]));
    for(int i = 0; i < 9; ++i)
        painter.fillRect(w_start+0*l+(i%3)*l/3, h_start+i/3*l/3, l/3, l/3, this->number2qcolor(this->total_data[5][i]));


    painter.drawLine(w_start, h_start+l/3, w_start+l*4, h_start+l/3);
    painter.drawLine(w_start, h_start+l/3*2, w_start+l*4, h_start+l/3*2);
    painter.drawLine(w_start+2*l, h_start-l/3, w_start+3*l, h_start-l/3);
    painter.drawLine(w_start+2*l, h_start-l/3*2, w_start+3*l, h_start-l/3*2);
    painter.drawLine(w_start+2*l, h_start+l/3*5, w_start+3*l, h_start+l/3*5);
    painter.drawLine(w_start+2*l, h_start+l/3*4, w_start+3*l, h_start+l/3*4);
    for(int i = 1; i < 12; ++i)
        painter.drawLine(w_start+l/3*i, h_start, w_start+l/3*i, h_start+l);
    painter.drawLine(w_start+l/3*7, h_start-l, w_start+l/3*7, h_start+l*2);
    painter.drawLine(w_start+l/3*8, h_start-l, w_start+l/3*8, h_start+l*2);

    pen.setWidth(8);
    painter.setPen(pen);
    for(int i  = 0; i < 4; ++i)
        painter.drawRect(w_start+l*i, h_start, l, l);
    painter.drawRect(w_start+l*2, h_start-l, l, l);
    painter.drawRect(w_start+l*2, h_start+l, l, l);

    pen.setWidth(11);
    painter.setPen(pen);
    if(this->current_central_color>0 && this->current_central_color<7)
    {
        painter.drawRect(this->button_for_choose_color[this->current_central_color-1]->geometry());
    }
    //    pen.setWidth(4);
    //    painter.setPen(pen);
    if(this->current_cube>=0 && this->current_cube<9)
    {
        painter.drawRect(this->show_one_face[current_cube]->geometry());
        //        std::clog<<"is called"<<std::endl;
        QRect rect = this->show_one_face[current_cube]->geometry();
        rect.setX(rect.x()+10);
        rect.setY(rect.y()+10);
        rect.setWidth(rect.width()-20);
        rect.setHeight(rect.height()-20);
        //        std::clog<<rect.x()<<' '<<rect.y()<<' '<<rect.width()<<' '<<rect.height()<<std::endl;
        painter.drawRect(rect);
    }

    //    painter.drawRect(test->geometry());
}

void Scan::_on_choose_red_clicked()
{
    this->has_chosen_a_face = true;
    this->current_central_color = 1;
    this->load_data_from_total_to_one_face();
    this->data_for_one_face[4] = this->current_central_color;
    this->update();
}

void Scan::_on_set_red_clicked()
{
    if(!(this->has_chosen_a_face && this->has_chosen_a_cube))
        return;
    this->data_for_one_face[current_cube] = 1;
    this->update();}

void Scan::_on_choose_blue_clicked()
{
    this->has_chosen_a_face = true;
    this->current_central_color = 2;
    this->load_data_from_total_to_one_face();
    this->data_for_one_face[4] = this->current_central_color;
    this->update();
}

void Scan::_on_set_blue_clicked()
{
    if(!(this->has_chosen_a_face && this->has_chosen_a_cube))
        return;
    this->data_for_one_face[current_cube] = 2;
    this->update();
}

void Scan::_on_choose_white_clicked()
{
    this->has_chosen_a_face = true;
    this->current_central_color = 3;
    this->load_data_from_total_to_one_face();
    this->data_for_one_face[4] = this->current_central_color;
    this->update();
}

void Scan::_on_set_white_clicked()
{
    if(!(this->has_chosen_a_face && this->has_chosen_a_cube))
        return;
    this->data_for_one_face[current_cube] = 3;
    this->update();
}

void Scan::_on_choose_green_clicked()
{
    this->has_chosen_a_face = true;
    this->current_central_color = 4;
    this->load_data_from_total_to_one_face();
    this->data_for_one_face[4] = this->current_central_color;
    this->update();
}

void Scan::_on_set_green_clicked()
{
    if(!(this->has_chosen_a_face && this->has_chosen_a_cube))
        return;
    this->data_for_one_face[current_cube] = 4;
    this->update();
}

void Scan::_on_choose_orange_clicked()
{
    this->has_chosen_a_face = true;
    this->current_central_color = 5;
    this->load_data_from_total_to_one_face();
    this->data_for_one_face[4] = this->current_central_color;
    this->update();

}

void Scan::_on_set_orange_clicked()
{
    if(!(this->has_chosen_a_face && this->has_chosen_a_cube))
        return;
    this->data_for_one_face[current_cube] = 5;
    this->update();
}

void Scan::_on_choose_yellow_clicked()
{
    //    std::clog<<"yellow is chosen"<<std::endl;
    this->has_chosen_a_face = true;
    this->current_central_color = 6;
    this->load_data_from_total_to_one_face();
    this->data_for_one_face[4] = this->current_central_color;
    this->update();
}

void Scan::_on_set_yellow_clicked()
{
    if(!(this->has_chosen_a_face && this->has_chosen_a_cube))
        return;
    this->data_for_one_face[current_cube] = 6;
    this->update();
}

QString Scan::number2stylestring(int x)
{
    QString ret("background-color:");
    switch (x)
    {
    case 1:
        return ret+"red;";

    case 2:
        return ret+"blue;";

    case 3:
        return ret+"white;";

    case 4:
        return ret+"green;";

    case 5:
        return ret+"rgb(255,165,0);";

    case 6:
        return ret+"yellow;";

    default:
        return ret+"rgb(200,200,200);";
    }
}

QColor Scan::number2qcolor(int x)
{
    switch (x)
    {
    case 1:
        return Qt::red;
    case 2:
        return Qt::blue;
    case 3:
        return Qt::white;
    case 4:
        return Qt::green;
    case 5:
        return QColor(255,165,0);
    case 6:
        return Qt::yellow;
    default:
        return QColor(200,200,200);
    }
}

void Scan::_on_save_one_face_clicked()
{
    for(int i = 0; i < 9; ++i)
    {

        this->total_data[this->current_central_color-1][i] = this->data_for_one_face[i];
    }
    this->is_finish[current_central_color-1] = true;
    this->current_central_color = -1;
    this->current_cube = -1;
    for(int i = 0; i < 9; ++i)
    {
        this->data_for_one_face[i] = 0;
    }
    this->has_chosen_a_face = false;
    this->has_chosen_a_cube = false;
    this->update();
}

void Scan::load_data_from_total_to_one_face()
{
    for(int i = 0; i < 9; ++i)
    {
        //        std::clog<<i<<std::endl;

        this->data_for_one_face[i] = this->total_data[this->current_central_color-1][i];
    }
}
