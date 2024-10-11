#include "solve.h"
#include "ui_solve.h"

Solve::Solve(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Solve)
{
    ui->setupUi(this);
    //    setFocusPolicy(Qt::StrongFocus);
    //    installEventFilter(this);
    this->setGeometry(0, 0, application_width, application_height);
    this->setWindowFlags(Qt::Window);

    QFont font;
    font.setPointSize(17);
    int &w = application_width, &h = application_height;

    this->input = new QPushButton(this);
    this->input->setFont(font);
    this->input->setGeometry(w*3/100, h*5/100, w*10/100, h*5/100);
    this->input->setText("输入魔方颜色");
    connect(this->input, &QPushButton::clicked, this, &Solve::_on_input_clicked);

    this->choose_mode = new QPushButton(this);
    this->choose_mode->setFont(font);
    this->choose_mode->setGeometry(w*3/100, h*21/100, w*10/100, h*5/100);
    this->choose_mode->setText("开始还原");
    connect(this->choose_mode, &QPushButton::clicked, this, &Solve::_on_start_clicked);

    this->start = new QPushButton(this);
    this->start->setFont(font);
    this->start->setGeometry(w*3/100, h*13/100, w*10/100, h*5/100);
    this->start->setText("计算还原步骤");
    connect(this->start, &QPushButton::clicked, this, &Solve::_on_calculate_clicked);

    this->back = new QPushButton(this);
    this->back->setFont(font);
    this->back->setGeometry(w*3/100, h*29/100, w*10/100, h*5/100);
    this->back->setText("返回主菜单");
    //    this->back->setGeometry(500,500,300,300);

    this->exit = new QPushButton(this);
    this->exit->setFont(font);
    this->exit->setGeometry(w*3/100, h*37/100, w*10/100, h*5/100);
    this->exit->setText("退出程序");
    connect(this->exit, &QPushButton::clicked, this, &Solve::_on_exit_clicked);

    this->scanner = new Scan();
    connect(this->scanner->finish, &QPushButton::clicked, this, [&](){
        this->scanner->timer_for_photo_processer->stop();
        if(this->scanner->camera.isOpened())
            this->scanner->camera.release();
        this->scanner->QWidget::hide();
        this->QWidget::show();
        this->state_for_cal(this->scanner->total_data);
        this->show_widget->state_for_show(this->scanner->total_data);
        this->show_widget->original_state(this->scanner->total_data);
        this->show_widget->update();
    });
    this->show_widget = new Show(this);
    this->show_widget->show();

    this->timer_for_show_back_progress = new QTimer();
    this->timer_for_show_back_progress->setInterval(20);
    connect(this->timer_for_show_back_progress, &QTimer::timeout,
            this, [&]{
        if(this->show_widget->is_rotating)
            return;
        if(this->has_path)
        {
            this->show_back_progress();
        }
    });
    QFont font_for_show_path;
    font_for_show_path.setPointSize(14);
    int start_x = application_width*42/100;
    int start_y = application_height*66/100;
    int wid = application_width*5/200;
    int gap_x = application_width*1/100;
    int gap_y = application_height*2/100;
    for(int i=0;i<80;++i)
    {
        this->show_path_labels[i] = new QLabel(this);
        this->show_path_labels[i]->setFont(font_for_show_path);
        this->show_path_labels[i]->setGeometry(start_x+(i%16)*(gap_x+wid), start_y+(i/16)*(gap_y+wid), wid, wid);
        //        this->show_path_labels[i]->setStyleSheet("border: 2px solid red;");
        //        this->show_path_labels[i]->setText(" U\'");
    }
}

Solve::~Solve()
{
    delete this->input;
    delete this->choose_mode;
    delete this->start;
    delete this->back;
    delete this->exit;
    //    delete this->show_path_labels[80];
    delete ui;
}

void Solve::_on_input_clicked()
{

    this->in_input_mode_choose = true;
    QWidget* _child = new QWidget();
    _child->setGeometry(application_width*30/100, application_height*30/100,
                        application_width*40/100, application_height*40/100);

    QFont font;
    font.setPointSize(17);

    QPushButton *_1, *_2;
    _1 = new QPushButton(_child);
    _1->setGeometry(application_width*40/100, application_height*40/100,
                    application_width*20/100, application_height*7/100);
    _1->setFont(font);
    _1->setText("拍照识别");
    connect(_1, &QPushButton::clicked, this, &Solve::_on_input_mode_choose_1_clicked);

    _2 = new QPushButton(_child);
    _2->setGeometry(application_width*40/100, application_height*53/100,
                    application_width*20/100, application_height*7/100);
    _2->setFont(font);
    _2->setText("手动输入");
    connect(_2, &QPushButton::clicked, this, &Solve::_on_input_mode_choose_2_clicked);

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(_child);
    this->setLayout(layout);
    //    while(this->in_input_mode_choose);

}

void Solve::_on_calculate_clicked()
{
    //    file<<"the original state:"<<std::endl<<this->state_for_cal<<std::endl;
    //    this->path_to_middle.clear();
    //    this->path_to_final.clear();
    this->path_for_show.clear();
    this->iterator_for_show_back_progress = 0;
    this->phase = 0;
    for(unsigned int i=0;i<80;++i)
        this->show_path_labels[i]->setText("");
    this->state_for_cal = this->show_widget->state_for_show;//改
    this->solve_function();

}

void Solve::_on_start_clicked()
{
    this->timer_for_show_back_progress->start();
    show_back_progress();
}

void Solve::_on_exit_clicked()
{
    std::exit(0);
}

void Solve::_on_input_mode_choose_1_clicked()
{

    this->in_input_mode_choose = false;
    this->delete_layout();

    cv::VideoCapture camera(0);
    if(!camera.isOpened())
    {
        QMessageBox::warning(nullptr, "警告", "未能找到摄像头，请使用手动输入模式");
        return;
    }

    camera.release();
    (*this->scanner)(1);
    this->QWidget::hide();
    this->scanner->QWidget::showFullScreen();
    //        this->scanner->show();
    return;
}

void Solve::_on_input_mode_choose_2_clicked()
{

    this->in_input_mode_choose = false;
    this->delete_layout();
    (*this->scanner)(2);
    this->QWidget::hide();
    this->scanner->QWidget::showFullScreen();
    //        this->scanner->show();
    return;
}

void Solve::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QPainter painter(this);
    QPen pen(Qt::blue, 4);
    painter.setPen(pen);
    if(in_input_mode_choose)
    {
        painter.drawRoundRect(application_width*30/100, application_height*30/100,
                              application_width*40/100+50, application_height*40/100+50, 25, 25);
    }
}

void Solve::delete_layout()
{
    QLayout* layout = this->layout();
    QLayoutItem* item;
    while((item = layout->takeAt(0)) != nullptr)
    {
        if(item->widget() != nullptr)
            delete item->widget();
        delete item;
    }
    delete layout;
    delete this->layout();
}

void Solve::solve_function()
{
    std::vector<std::string> state;
    state.push_back(transformer(this->state_for_cal(5,1))+transformer(this->state_for_cal(1,1)));
    state.push_back(transformer(this->state_for_cal(5,3))+transformer(this->state_for_cal(0,5)));
    state.push_back(transformer(this->state_for_cal(5,7))+transformer(this->state_for_cal(3,7)));
    state.push_back(transformer(this->state_for_cal(5,5))+transformer(this->state_for_cal(4,3)));

    state.push_back(transformer(this->state_for_cal(2,1))+transformer(this->state_for_cal(1,7)));
    state.push_back(transformer(this->state_for_cal(2,5))+transformer(this->state_for_cal(0,3)));
    state.push_back(transformer(this->state_for_cal(2,7))+transformer(this->state_for_cal(3,1)));
    state.push_back(transformer(this->state_for_cal(2,3))+transformer(this->state_for_cal(4,5)));

    state.push_back(transformer(this->state_for_cal(1,5))+transformer(this->state_for_cal(0,1)));
    state.push_back(transformer(this->state_for_cal(1,3))+transformer(this->state_for_cal(4,1)));
    state.push_back(transformer(this->state_for_cal(3,5))+transformer(this->state_for_cal(0,7)));
    state.push_back(transformer(this->state_for_cal(3,3))+transformer(this->state_for_cal(4,7)));

    state.push_back(transformer(this->state_for_cal(5,0))+transformer(this->state_for_cal(1,2))+transformer(this->state_for_cal(0,2)));
    state.push_back(transformer(this->state_for_cal(5,6))+transformer(this->state_for_cal(0,8))+transformer(this->state_for_cal(3,8)));
    state.push_back(transformer(this->state_for_cal(5,8))+transformer(this->state_for_cal(3,6))+transformer(this->state_for_cal(4,6)));
    state.push_back(transformer(this->state_for_cal(5,2))+transformer(this->state_for_cal(4,0))+transformer(this->state_for_cal(1,0)));

    state.push_back(transformer(this->state_for_cal(2,2))+transformer(this->state_for_cal(0,0))+transformer(this->state_for_cal(1,8)));
    state.push_back(transformer(this->state_for_cal(2,0))+transformer(this->state_for_cal(1,6))+transformer(this->state_for_cal(4,2)));
    state.push_back(transformer(this->state_for_cal(2,6))+transformer(this->state_for_cal(4,8))+transformer(this->state_for_cal(3,0)));
    state.push_back(transformer(this->state_for_cal(2,8))+transformer(this->state_for_cal(3,2))+transformer(this->state_for_cal(0,6)));

    this->path_for_show = this->thistlethwaite(state);
    unsigned int iter = 0;
    while(iter<this->path_for_show.size())
    {
        if(iter+1<this->path_for_show.size() && path_for_show[iter][0] == path_for_show[iter+1][0])
        {
            int _ = path_for_show[iter][1]-'0', __ = path_for_show[iter+1][1] - '0';
            int sum = (_ + __)%4;
            path_for_show.erase(path_for_show.begin()+iter+1);
            if(sum == 0)
                path_for_show.erase(path_for_show.begin()+iter);
            else
                path_for_show[iter][1] = sum + '0';
            continue;
        }
        else
            iter++;
    }

    this->has_path = true;

    // show the path to the user.
    for(unsigned int i=0;i<this->path_for_show.size();++i)
    {
        std::string move;
        move += this->path_for_show[i][0];
        if(this->path_for_show[i][1] == '2')
            move += this->path_for_show[i][1];
        if(this->path_for_show[i][1] == '3')
            move += '\'';
        this->show_path_labels[i]->setText(QString::fromStdString(move));
    }
}

std::string Solve::transformer(int num)
{
    switch (num)
    {
    case 1:
        return "R";
    case 2:
        return "F";
    case 3:
        return "D";
    case 4:
        return "B";
    case 5:
        return "L";
    case 6:
        return "U";
    }
}



void Solve::show_back_progress()
{
    if(this->iterator_for_show_back_progress == this->path_for_show.size())
    {
        timer_for_show_back_progress->stop();
        this->show_path_labels[this->path_for_show.size()-1]->setStyleSheet("");
        has_path = false;
        this->state_for_cal = this->show_widget->state_for_show;
        //        this->  //需要把original改掉
        return;
    }
    if(this->iterator_for_show_back_progress > 0)
        this->show_path_labels[this->iterator_for_show_back_progress-1]->setStyleSheet("");
    this->show_path_labels[this->iterator_for_show_back_progress]->setStyleSheet("border: 2px solid blue; color: blue;");
    std::string temp = path_for_show[this->iterator_for_show_back_progress];
    bool _reverse = false, is_twice = false;
    if(temp.size()>1 && temp[1]=='3')
        _reverse = true;
    if(temp.size()>1 && temp[1]=='2')
        is_twice = true;
    this->show_widget->is_reverse = _reverse;
    this->show_widget->is_rotating = true;
    this->show_widget->rotate(temp[0], is_twice);
    this->iterator_for_show_back_progress++;
}

void Solve::keyPressEvent(QKeyEvent *e)
{
    setFocusPolicy(Qt::StrongFocus);
    installEventFilter(this);
    switch(e->key())
    {
    case Qt::Key_Up:

        break;
    case Qt::Key_Down:

        break;
    case Qt::Key_Left:

        break;
    case Qt::Key_Right:

        break;
    case Qt::Key_Apostrophe:
        if(this->show_widget->is_rotating)
            break;
        if(this->show_widget->is_reverse)
            this->show_widget->is_reverse = false;
        else
            this->show_widget->is_reverse = true;
        break;
    case Qt::Key_U:
        if(this->show_widget->is_rotating)
            break;
        this->show_widget->is_rotating = true;
        this->show_widget->rotate('U');
        //        this->state_for_cal = this->show_widget->state_for_show;
        break;
    case Qt::Key_D:
        if(this->show_widget->is_rotating)
            break;
        this->show_widget->is_rotating = true;
        this->show_widget->rotate('D');
        //        this->state_for_cal = this->show_widget->state_for_show;
        break;
    case Qt::Key_L:
        if(this->show_widget->is_rotating)
            break;
        this->show_widget->is_rotating = true;
        this->show_widget->rotate('L');
        //        this->state_for_cal = this->show_widget->state_for_show;
        break;
    case Qt::Key_R:
        if(this->show_widget->is_rotating)
            break;
        this->show_widget->is_rotating = true;
        this->show_widget->rotate('R');
        //        this->state_for_cal = this->show_widget->state_for_show;
        break;
    case Qt::Key_F:
        if(this->show_widget->is_rotating)
            break;
        this->show_widget->is_rotating = true;
        this->show_widget->rotate('F');
        //        this->state_for_cal = this->show_widget->state_for_show;
        break;
    case Qt::Key_B:
        if(this->show_widget->is_rotating)
            break;
        this->show_widget->is_rotating = true;
        this->show_widget->rotate('B');
        //        this->state_for_cal = this->show_widget->state_for_show;
        break;
    }
}

std::vector<int> Solve::applyMove(int move, std::vector<int> state)
{
    int turns = move % 3 + 1;
    int face = move / 3;
    while (turns--)
    {
        std::vector<int> oldState = state;
        for (int i = 0; i < 8; i++)
        {
            int isCorner = i > 3;
            int target = affectedCubies[face][i] + isCorner * 12;
            int killer = affectedCubies[face][(i & 3) == 3 ? i - 3 : i + 1] + isCorner * 12;;
            int orientationDelta = (i < 4) ? (face > 1 && face < 4) : (face < 2) ? 0 : 2 - (i & 1);
            state[target] = oldState[killer];
            //state[target+20] = (oldState[killer+20] + orientationDelta) % (2 + isCorner);
            state[target + 20] = oldState[killer + 20] + orientationDelta;
            if (!turns)
                state[target + 20] %= 2 + isCorner;
        }
    }
    return state;
}

int Solve::inverse(int move)
{
    return move + 2 - 2 * (move % 3);
}

std::vector<int> Solve::id(std::vector<int> state)
{

    //--- Phase 1: Edge orientations.
    if (phase < 2)
        return std::vector<int>(state.begin() + 20, state.begin() + 32);

    //-- Phase 2: Corner orientations, E slice edges.
    if (phase < 3)
    {
        std::vector<int> result(state.begin() + 31, state.begin() + 40);
        for (int e = 0; e < 12; e++)
            result[0] |= (state[e] / 8) << e;
        return result;
    }

    //--- Phase 3: Edge slices M and S, corner tetrads, overall parity.
    if (phase < 4)
    {
        std::vector<int> result(3);
        for (int e = 0; e < 12; e++)
            result[0] |= ((state[e] > 7) ? 2 : (state[e] & 1)) << (2 * e);
        for (int c = 0; c < 8; c++)
            result[1] |= ((state[c + 12] - 12) & 5) << (3 * c);
        for (int i = 12; i < 20; i++)
            for (int j = i + 1; j < 20; j++)
                result[2] ^= state[i] > state[j];
        return result;
    }

    //--- Phase 4: The rest.
    return state;
}

std::vector<std::string> Solve::thistlethwaite(std::vector<std::string> argv)
{
    std::vector<std::string> ret;
    //    std::clog<<this->state_for_cal<<std::endl;
    //--- Define the goal.
    std::string goal[] = { "UF", "UR", "UB", "UL", "DF", "DR", "DB", "DL", "FR", "FL", "BR", "BL",
                           "UFR", "URB", "UBL", "ULF", "DRF", "DFL", "DLB", "DBR" };
    // UR UB UL UF DF DR DB DL FR FL BR BL URB UBL ULF UFR DRF DFL DLB DBR
    //--- Prepare current (start) and goal state.
    std::vector<int> currentState(40), goalState(40);
    for (int i = 0; i < 20; i++)
    {

        //--- Goal state.
        goalState[i] = i;

        //--- Current (start) state.
        std::string cubie = argv[i];
        while ((currentState[i] = std::find(goal, goal + 20, cubie) - goal) == 20)
        {
            cubie = cubie.substr(1) + cubie[0];
            currentState[i + 20]++;
        }
    }
    //--- Dance the funky Thistlethwaite...
    while (++phase < 5)
    {

        //--- Compute ids for current and goal state, skip phase if equal.
        std::vector<int> currentId = id(currentState), goalId = id(goalState);
        if (currentId == goalId)
            continue;

        //--- Initialize the BFS queue.
        std::queue<std::vector<int>> q;
        q.push(currentState);
        q.push(goalState);

        //--- Initialize the BFS tables.
        std::map<std::vector<int>, std::vector<int>> predecessor;
        std::map<std::vector<int>, int> direction, lastMove;
        direction[currentId] = 1;
        direction[goalId] = 2;

        //--- Dance the funky bidirectional BFS...
        while (1)
        {

            //--- Get state from queue, compute its ID and get its direction.
            std::vector<int> oldState = q.front();
            q.pop();
            std::vector<int> oldId = id(oldState);
            int& oldDir = direction[oldId];

            //--- Apply all applicable moves to it and handle the new state.
            for (int move = 0; move < 18; move++)
            {
                if (applicableMoves[phase] & (1 << move))
                {

                    //--- Apply the move.
                    std::vector<int> newState = applyMove(move, oldState);
                    std::vector<int> newId = id(newState);
                    int& newDir = direction[newId];

                    //--- Have we seen this state (id) from the other direction already?
                    //--- I.e. have we found a connection?
                    if (newDir && newDir != oldDir)
                    {

                        //--- Make oldId represent the forwards and newId the backwards search state.
                        if (oldDir > 1)
                        {
                            swap(newId, oldId);
                            move = inverse(move);
                        }

                        //--- Reconstruct the connecting algorithm.
                        std::vector<int> algorithm(1, move);
                        while (oldId != currentId)
                        {
                            algorithm.insert(algorithm.begin(), lastMove[oldId]);
                            oldId = predecessor[oldId];
                        }
                        while (newId != goalId)
                        {
                            algorithm.push_back(inverse(lastMove[newId]));
                            newId = predecessor[newId];
                        }

                        //--- Print and apply the algorithm.
                        for (int i = 0; i < (int)algorithm.size(); i++)
                        {
                            //<< "UDFBLR"[algorithm[i] / 3] << algorithm[i] % 3 + 1;
                            std::string temp;
                            temp += "UDFBLR"[algorithm[i] / 3];
                            temp += '0' + (algorithm[i] % 3 + 1);
                            ret.push_back(temp);
                            currentState = applyMove(algorithm[i], currentState);
                        }

                        //--- Jump to the next phase.
                        goto nextPhasePlease;
                    }

                    //--- If we've never seen this state (id) before, std::vector<int>sit it.
                    if (!newDir)
                    {
                        q.push(newState);
                        newDir = oldDir;
                        lastMove[newId] = move;
                        predecessor[newId] = oldId;
                    }
                }
            }
        }
nextPhasePlease:
        ;
    }
    return ret;
}


//mine begin

//int Solve::evaluate_function()//to the end
//{
//    if(this->state_for_cal.is_terminate())
//        return 0;
//    int ret = 0;
//    if(state_for_cal(1,0) != COLOR::BLUE || state_for_cal(4,0)!=COLOR::ORINGE || state_for_cal(5,2)!=COLOR::YELLOW)
//        ret++;
//    if(state_for_cal(1,1) != COLOR::BLUE || state_for_cal(5,1)!=COLOR::YELLOW)
//        ret++;
//    if(state_for_cal(1,2) != COLOR::BLUE || state_for_cal(0,2)!=COLOR::RED || state_for_cal(5,0)!=COLOR::YELLOW)
//        ret++;
//    if(state_for_cal(4,3) != COLOR::ORINGE || state_for_cal(5,5)!=COLOR::YELLOW)
//        ret++;
//    if(state_for_cal(0,5) != COLOR::RED || state_for_cal(5,3)!=COLOR::YELLOW)
//        ret++;
//    if(state_for_cal(3,6) != COLOR::GREEN || state_for_cal(4,6)!=COLOR::ORINGE || state_for_cal(5,8)!=COLOR::YELLOW)
//        ret++;
//    if(state_for_cal(3,7) != COLOR::GREEN || state_for_cal(5,7)!=COLOR::YELLOW)
//        ret++;
//    if(state_for_cal(3,8) != COLOR::GREEN || state_for_cal(0,8)!=COLOR::RED || state_for_cal(5,6)!=COLOR::YELLOW)
//        ret++;
//    if(state_for_cal(4,1) != COLOR::ORINGE || state_for_cal(1,3)!=COLOR::BLUE)
//        ret++;
//    if(state_for_cal(1,5) != COLOR::BLUE || state_for_cal(0,1)!=COLOR::RED)
//        ret++;
//    if(state_for_cal(3,3) != COLOR::GREEN || state_for_cal(4,7)!=COLOR::ORINGE)
//        ret++;
//    if(state_for_cal(0,7) != COLOR::RED || state_for_cal(3,5)!=COLOR::GREEN)
//        ret++;
//    if(state_for_cal(4,2) != COLOR::ORINGE || state_for_cal(2,0)!=COLOR::WHITE || state_for_cal(1,6)!=COLOR::BLUE)
//        ret++;
//    if(state_for_cal(2,1) != COLOR::WHITE || state_for_cal(1,7)!=COLOR::BLUE)
//        ret++;
//    if(state_for_cal(0,0) != COLOR::RED || state_for_cal(1,8)!=COLOR::BLUE || state_for_cal(2,2)!=COLOR::WHITE)
//        ret++;
//    if(state_for_cal(2,3) != COLOR::WHITE || state_for_cal(4,5)!=COLOR::ORINGE)
//        ret++;
//    if(state_for_cal(2,5) != COLOR::WHITE || state_for_cal(0,3)!=COLOR::RED)
//        ret++;
//    if(state_for_cal(2,6) != COLOR::WHITE || state_for_cal(4,8)!=COLOR::ORINGE || state_for_cal(3,0)!=COLOR::GREEN)
//        ret++;
//    if(state_for_cal(2,7) != COLOR::WHITE || state_for_cal(3,1)!=COLOR::GREEN)
//        ret++;
//    if(state_for_cal(0,6) != COLOR::RED || state_for_cal(2,8)!=COLOR::WHITE || state_for_cal(3,2)!=COLOR::GREEN)
//        ret++;
//    return ret;
//}

//bool Solve::dfs_to_final(int _depth)
//{
//    //    std::clog<<this->path_to_final.size()<<std::endl;
//    if(this->state_for_cal.is_terminate())
//        return true;
//    int value = evaluate_function();
//    //    std::clog<<"value: "<<value<<std::endl;
//    if(_depth+value > this->depth_to_final)
//        return false;
//    if(_depth >= this->max_depth || this->depth_to_final >= this->max_depth)
//        return false;
//    //    for(int i=0;i<18;++i)
//    for(auto i : this->move_for_second_progress)
//    {
//        if(path_to_final.size()>1 && i%6 == static_cast<int>(path_to_final[path_to_final.size()-1])%6)
//            continue;
//        path_to_final.push_back(static_cast<MOVE>(i));
//        //        std::clog<<"size: "<<path_to_final.size()<<std::endl;
//        bool _reverse = false, is_twice = false;
//        if(i>5 && i<12)
//            _reverse = true;
//        if(i>11)
//            is_twice = true;
//        char ch;
//        switch (i%6)
//        {
//        case 0:
//            ch='U';
//            break;
//        case 1:
//            ch='D';
//            break;
//        case 2:
//            ch='L';
//            break;
//        case 3:
//            ch='R';
//            break;
//        case 4:
//            ch='F';
//            break;
//        case 5:
//            ch='B';
//            break;
//        }
//        this->state_for_cal(ch, _reverse, is_twice);
//        //        if(this->state_for_cal.is_terminate())
//        //            return true;
//        if(dfs_to_final(_depth+1))
//            return true;
//        this->state_for_cal(ch, !_reverse, is_twice);
//        path_to_final.pop_back();
//        //        std::clog<<"size: "<<path.size()<<std::endl;
//    }
//    return false;
//}

//int Solve::middle_evaluator(int index)
//{
//    int left = 0;
//    int temp_cubes[6][9];
//    for(int i=0;i<6;++i)
//        for(int j=0;j<9;++j)
//        {
//            if(this->state_for_cal(i,j) == COLOR::YELLOW || this->state_for_cal(i,j) == COLOR::WHITE)
//                temp_cubes[i][j] = 3;
//            if(this->state_for_cal(i,j) == COLOR::BLUE || this->state_for_cal(i,j) == COLOR::GREEN)
//                temp_cubes[i][j] = 2;
//            if(this->state_for_cal(i,j) == COLOR::ORINGE || this->state_for_cal(i,j) == COLOR::RED)
//                temp_cubes[i][j] = 1;
//        }
//    //角块方向
//    //        left = 0;
//    if(temp_cubes[2][0]!=3)
//        left++;
//    if(temp_cubes[2][2]!=3)
//        left++;
//    if(temp_cubes[2][6]!=3)
//        left++;
//    if(temp_cubes[2][8]!=3)
//        left++;
//    if(temp_cubes[5][0]!=3)
//        left++;
//    if(temp_cubes[5][2]!=3)
//        left++;
//    if(temp_cubes[5][6]!=3)
//        left++;
//    if(temp_cubes[5][8]!=3)
//        left++;
//    //棱块方向
//    //        left = 0;
//    if(temp_cubes[2][1] < temp_cubes[1][7])
//        left++;
//    if(temp_cubes[2][3] < temp_cubes[4][5])
//        left++;
//    if(temp_cubes[2][5] < temp_cubes[0][3])
//        left++;
//    if(temp_cubes[2][7] < temp_cubes[3][1])
//        left++;

//    if(temp_cubes[5][1] < temp_cubes[1][1])
//        left++;
//    if(temp_cubes[5][3] < temp_cubes[4][0])
//        left++;
//    if(temp_cubes[5][5] < temp_cubes[4][3])
//        left++;
//    if(temp_cubes[5][7] < temp_cubes[3][7])
//        left++;

//    if(temp_cubes[1][5] < temp_cubes[0][1])
//        left++;
//    if(temp_cubes[3][3] < temp_cubes[4][7])
//        left++;
//    if(temp_cubes[3][5] < temp_cubes[0][7])
//        left++;
//    if(temp_cubes[1][3] < temp_cubes[4][1])
//        left++;
//    //        return left;

//    //中层棱块位置
//    if(!(temp_cubes[1][3]!=3 && temp_cubes[4][1]!=3))
//        left++;
//    if(!(temp_cubes[1][5]!=3 && temp_cubes[0][1]!=3))
//        left++;
//    if(!(temp_cubes[3][3]!=3 && temp_cubes[4][7]!=3))
//        left++;
//    if(!(temp_cubes[3][5]!=3 && temp_cubes[0][7]!=3))
//        left++;
//    return left;
//}

//bool Solve::dfs_to_middle(int _depth, int index)
//{
//    int left = this->middle_evaluator(index);
//    if(left == 0)
//        return true;
//    if(_depth+left > this->depth_to_middle)
//        return false;
//    if(_depth>=this->max_depth || this->depth_to_middle>= this->max_depth)
//        return false;
//    for(int i=0;i<18;++i)
//    {
//        if(path_to_middle.size()>1 && i%6 == static_cast<int>(path_to_middle[path_to_middle.size()-1])%6)
//            continue;
//        path_to_middle.push_back(static_cast<MOVE>(i));
//        bool _reverse = false, is_twice = false;
//        if(i>5 && i<12)
//            _reverse = true;
//        if(i>11)
//            is_twice = true;
//        char ch;
//        switch (i%6)
//        {
//        case 0:
//            ch = 'U';
//            break;
//        case 1:
//            ch='D';
//            break;
//        case 2:
//            ch='L';
//            break;
//        case 3:
//            ch='R';
//            break;
//        case 4:
//            ch='F';
//            break;
//        case 5:
//            ch='B';
//            break;
//        }
//        this->state_for_cal(ch, _reverse, is_twice);
//        if(this->middle_evaluator(index) == 0)
//            return true;
//        if(dfs_to_middle(_depth+1, index))
//            return true;
//        this->state_for_cal(ch, !_reverse, is_twice);
//        path_to_middle.pop_back();
//        //        std::clog<<"size: "<<path.size()<<std::endl;
//    }
//    return false;
//}

//void Solve::solve_function()
//{
//    for(int index = 1;index<2;++index)
//    {
//        depth_to_middle = 0;
//        while(1)
//        {
//            if(!dfs_to_middle(0, index))
//            {
//                ++depth_to_middle;
//                //            std::clog<<"when depth = "<<depth-1<<", the ans is not found, depth: "<<depth-1<<" -> "<<depth<<std::endl;
//                //            std::clog<<"size: "<<path.size()<<std::endl;

//            }
//            else
//            {
//                //            file<<"the ans is found, with depth = "<<depth<<std::endl;
//                //            has_path = true;
//                break;
//            }
//            if(depth_to_middle > this->max_depth)
//            {
//                //            std::clog<<"the ans is not found."<<std::endl;
//                //            std::exit(0);
//                break;
//            }
//        }
//    }
//    has_path = true;
//    while(1)
//    {
//        if(!dfs_to_final(0))
//            ++depth_to_final;
//        else
//        {
//            has_path = true;
//            break;
//        }
//        if(depth_to_final > this->max_depth)
//            break;
//    }
//    path_processor();
//}

//mine end
