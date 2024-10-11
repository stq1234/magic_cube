#ifndef SCAN_H
#define SCAN_H

#include <globel.h>
#include <state.h>

namespace Ui {
class Scan;
}

class Scan : public QWidget
{
    Q_OBJECT

public:
    explicit Scan(QWidget *parent = nullptr);
    ~Scan();
    void operator()(int x);
    QPushButton *finish = nullptr;
    int total_data[6][9] = { 0 };
    int data_for_one_face[9] = { 0 };
    void photo_processer();
    cv::VideoCapture camera;

    QTimer *timer_for_photo_processer;

private slots:
    void _on_change_mode_clicked();
    void _on_save_one_face_clicked();

    void _on_choose_red_clicked();
    void _on_set_red_clicked();
    void _on_choose_blue_clicked();
    void _on_set_blue_clicked();
    void _on_choose_white_clicked();
    void _on_set_white_clicked();
    void _on_choose_green_clicked();
    void _on_set_green_clicked();
    void _on_choose_orange_clicked();
    void _on_set_orange_clicked();
    void _on_choose_yellow_clicked();
    void _on_set_yellow_clicked();

protected:
    void paintEvent(QPaintEvent* e);

private:
    Ui::Scan *ui;
    int scan_mode = 0;
    int current_central_color = -1, current_cube = -1;//used for setting color
    QTimer *timer_for_enable_finish_button, *timer_for_save_button;
    bool is_finish[6] = {0, 0, 0, 0, 0, 0}, has_chosen_a_face = false, has_chosen_a_cube = false;
    QPushButton *change_mode, *save_one_face;
    QPushButton* show_one_face[9];
    QPushButton* button_for_choose_color[6];
    QPushButton* color_button_for_set[6];
    QLabel *label_for_show_picture = nullptr;
    QLabel *label_for_tips_1, *label_for_tips_2;
    void load_data_from_total_to_one_face();
    QString number2stylestring(int x);
    QColor number2qcolor(int x);
    std::vector<std::vector<cv::Point>> rect_for_scan{{cv::Point(170,90), cv::Point(470,90), cv::Point(470,390), cv::Point(170,390)}};
    int rect_for_scan_start_x=170,rect_for_scan_start_y=90,rect_for_scan_width=300;
    std::vector<std::vector<cv::Scalar>> colors{{cv::Scalar(125.,43.6,46.), cv::Scalar(180.,255.,255.)},
                                                {cv::Scalar(80.,43.6,46.), cv::Scalar(124.,255.,255.)},
                                                {cv::Scalar(0.,0.,46.),cv::Scalar(180.,43.5,255.)},
                                                {cv::Scalar(44.,43.6,46.), cv::Scalar(77.,255.,255.)},
                                                {cv::Scalar(3.,43.6,46.), cv::Scalar(28.,255.,255.)},
                                                {cv::Scalar(28.,43.6,46.), cv::Scalar(44.,255.,255.)}
                                               };

//    int r_num=24, b_num=16, w_num=24, g_num=16,o_num=16,y_num=16, n_num=48;
//    //hsv test

//    QLabel* test_l,*test_u,*test,*tip;
//    int H_min=156,H_max=180,S_min=43,S_max=255,V_min=46,V_max=255;
//    QPushButton* hsv[3], *button_for_save;
//    int jishuqi=0,order=0;
//    QTimer* timer_for_test;


};

#endif // SCAN_H
