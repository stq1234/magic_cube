#include "state.h"

State::State()
{
    for(int i=0;i<6;++i)
        for(int j=0;j<9;++j)
            this->data[i][j] = static_cast<COLOR>(i+1);
}

void State::operator()(int _[6][9])
{
    for(int i=0;i<6;++i)
        for(int j=0;j<9;++j)
            this->data[i][j]=static_cast<COLOR>(_[i][j]);
    //    int test=0;//can be deleted after test
}

void State::operator()(char rotate, bool is_reverse, bool twice)
{
    int loop_num=1;
    if(!is_reverse)
    {
        loop_num = 3;
    }
    if(twice)
        loop_num = 2;
    for(int i=0;i<loop_num;++i)
    {
        COLOR _,__,___;
        switch (rotate)
        {
        case 'U':
            _=this->data[5][0];
            data[5][0]=data[5][2];
            data[5][2]=data[5][8];
            data[5][8]=data[5][6];
            data[5][6]=_;
            _=this->data[5][1];
            data[5][1]=data[5][5];
            data[5][5]=data[5][7];
            data[5][7]=data[5][3];
            data[5][3]=_;
            _=this->data[1][0];
            __=this->data[1][1];
            ___=this->data[1][2];
            data[1][0]=data[4][6];
            data[1][1]=data[4][3];
            data[1][2]=data[4][0];
            data[4][6]=data[3][8];
            data[4][3]=data[3][7];
            data[4][0]=data[3][6];
            data[3][6]=data[0][8];
            data[3][7]=data[0][5];
            data[3][8]=data[0][2];
            data[0][8]=___;
            data[0][5]=__;
            data[0][2]=_;
            break;

        case 'D':
            _=this->data[2][0];
            data[2][0]=data[2][2];
            data[2][2]=data[2][8];
            data[2][8]=data[2][6];
            data[2][6]=_;
            _=this->data[2][1];
            data[2][1]=data[2][5];
            data[2][5]=data[2][7];
            data[2][7]=data[2][3];
            data[2][3]=_;
            _=this->data[4][2];
            __=this->data[4][5];
            ___=this->data[4][8];
            data[4][2]=data[1][8];
            data[4][5]=data[1][7];
            data[4][8]=data[1][6];
            data[1][6]=data[0][0];
            data[1][7]=data[0][3];
            data[1][8]=data[0][6];
            data[0][0]=data[3][2];
            data[0][3]=data[3][1];
            data[0][6]=data[3][0];
            data[3][2]=___;
            data[3][1]=__;
            data[3][0]=_;
            break;

        case 'L':
            _=this->data[4][0];
            data[4][0]=data[4][2];
            data[4][2]=data[4][8];
            data[4][8]=data[4][6];
            data[4][6]=_;
            _=this->data[4][1];
            data[4][1]=data[4][5];
            data[4][5]=data[4][7];
            data[4][7]=data[4][3];
            data[4][3]=_;
            _=this->data[5][2];
            __=this->data[5][5];
            ___=this->data[5][8];
            data[5][2]=data[1][6];
            data[5][5]=data[1][3];
            data[5][8]=data[1][0];
            data[1][6]=data[2][6];
            data[1][3]=data[2][3];
            data[1][0]=data[2][0];
            data[2][6]=data[3][6];
            data[2][3]=data[3][3];
            data[2][0]=data[3][0];
            data[3][6]=_;
            data[3][3]=__;
            data[3][0]=___;
            break;

        case 'R':
            _=this->data[0][0];
            data[0][0]=data[0][2];
            data[0][2]=data[0][8];
            data[0][8]=data[0][6];
            data[0][6]=_;
            _=this->data[0][1];
            data[0][1]=data[0][5];
            data[0][5]=data[0][7];
            data[0][7]=data[0][3];
            data[0][3]=_;
            _=this->data[1][2];
            __=this->data[1][5];
            ___=this->data[1][8];
            data[1][2]=data[5][6];
            data[1][5]=data[5][3];
            data[1][8]=data[5][0];
            data[5][6]=data[3][2];
            data[5][3]=data[3][5];
            data[5][0]=data[3][8];
            data[3][2]=data[2][2];
            data[3][5]=data[2][5];
            data[3][8]=data[2][8];
            data[2][2]=_;
            data[2][5]=__;
            data[2][8]=___;
            break;

        case 'F':
            _=this->data[1][0];
            data[1][0]=data[1][2];
            data[1][2]=data[1][8];
            data[1][8]=data[1][6];
            data[1][6]=_;
            _=this->data[1][1];
            data[1][1]=data[1][5];
            data[1][5]=data[1][7];
            data[1][7]=data[1][3];
            data[1][3]=_;
            _=this->data[0][0];
            __=this->data[0][1];
            ___=this->data[0][2];
            data[0][0]=data[2][0];
            data[0][1]=data[2][1];
            data[0][2]=data[2][2];
            data[2][0]=data[4][0];
            data[2][1]=data[4][1];
            data[2][2]=data[4][2];
            data[4][0]=data[5][0];
            data[4][1]=data[5][1];
            data[4][2]=data[5][2];
            data[5][0]=_;
            data[5][1]=__;
            data[5][2]=___;
            break;

        case 'B':
            _=this->data[3][0];
            data[3][0]=data[3][2];
            data[3][2]=data[3][8];
            data[3][8]=data[3][6];
            data[3][6]=_;
            _=this->data[3][1];
            data[3][1]=data[3][5];
            data[3][5]=data[3][7];
            data[3][7]=data[3][3];
            data[3][3]=_;
            _=this->data[0][6];
            __=this->data[0][7];
            ___=this->data[0][8];
            data[0][6]=data[5][6];
            data[0][7]=data[5][7];
            data[0][8]=data[5][8];
            data[5][6]=data[4][6];
            data[5][7]=data[4][7];
            data[5][8]=data[4][8];
            data[4][6]=data[2][6];
            data[4][7]=data[2][7];
            data[4][8]=data[2][8];
            data[2][6]=_;
            data[2][7]=__;
            data[2][8]=___;
            break;
        }
    }
}

int State::operator()(int color, int index)
{
    return static_cast<int>(this->data[color][index]);
}

bool State::is_terminate()
{
    for(int i=0;i<6;++i)
        for(int j=0;j<9;++j)
            if(this->data[i][j]!=static_cast<COLOR>(i+1))
                return false;
    return true;
}

