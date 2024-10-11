#ifndef SOLVE_H
#define SOLVE_H

#include <globel.h>
#include <scan.h>
#include <show.h>

namespace Ui {
class Solve;
}


class Solve : public QWidget
{
    Q_OBJECT

public:
    explicit Solve(QWidget *parent = nullptr);
    ~Solve();
    QPushButton *input, *choose_mode, *start, *back, *exit;

    //functions and variables for search.

//    int depth_to_final = 0;
//    int depth_to_middle = 0;
//    int max_depth = 200;
//    std::vector<MOVE> path_to_final;
//    std::vector<MOVE> path_to_middle;
    std::vector<std::string> path_for_show;
    std::string transformer(int num);
//    int move_for_second_progress[10]={0,1,6,7,12,13,14,15,16,17};
//    int middle_evaluator(int index);

    std::vector<int> applyMove(int move, std::vector<int> state);
    int inverse(int move);
    std::vector<int> id(std::vector<int> state);
    std::vector<std::string> thistlethwaite(std::vector<std::string> argv);
    int applicableMoves[5] = { 0, 262143, 259263, 74943, 74898 };
    int affectedCubies[6][8] =
    {
        {  0,  1,  2,  3,  0,  1,  2,  3 },   // U
        {  4,  7,  6,  5,  4,  5,  6,  7 },   // D
        {  0,  9,  4,  8,  0,  3,  5,  4 },   // F
        {  2, 10,  6, 11,  2,  1,  7,  6 },   // B
        {  3, 11,  7,  9,  3,  2,  6,  5 },   // L
        {  1,  8,  5, 10,  1,  0,  4,  7 },   // R
    };
    int phase = 0;

private:
    Ui::Solve *ui;
    bool in_input_mode_choose = false;
    void delete_layout();
    Show* show_widget = nullptr;
    Scan* scanner = nullptr;
    State state_for_cal;
    void path_processor(std::vector<std::string> path);
    void show_back_progress();
    QTimer *timer_for_show_back_progress;
    unsigned int iterator_for_show_back_progress = 0;
    bool has_path = false;
    void solve_function();
    QLabel* show_path_labels[80];
//    int evaluate_function();
//    bool dfs_to_final(int _depth);
//    bool dfs_to_middle(int _depth, int index);

protected:
    void paintEvent(QPaintEvent* e);
    void keyPressEvent(QKeyEvent *e);

private slots:
    void _on_input_clicked();
    void _on_calculate_clicked();
    void _on_start_clicked();
    void __attribute__((noreturn)) _on_exit_clicked();
    void _on_input_mode_choose_1_clicked();
    void _on_input_mode_choose_2_clicked();

};

#endif // SOLVE_H
