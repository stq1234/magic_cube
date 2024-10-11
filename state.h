#ifndef STATE_H
#define STATE_H

#include <globel.h>





class State
{
private:
    COLOR data[6][9];
public:
    State();
    void operator()(int _[6][9]);//传值函数
    void operator()(char rotate, bool is_reverse, bool twice = false);
    bool is_terminate();
    int operator()(int color, int index);
    std::string trans(int c_)
    {
        //    int c_ = static_cast<int>(c);
        switch (c_)
        {
        case 1:return "R";
        case 2:return "B";
        case 3:return "W";
        case 4:return "G";
        case 5:return "O";
        case 6:return "Y";
        }
    }
    friend std::ostream& operator<<(std::ostream& f, State& s)
    {
        for(int j=0;j<3;++j)
        {
            for(int i=0;i<6;++i)
                f<<" ";
            for(int i=0;i<3;++i)
                f<<s.trans(s(1,i+j*3));
            f<<std::endl;
        }
        std::vector<int> indices{5, 4, 2, 0};
        for(int j=0;j<3;++j)
        {
            for(int i=0;i<4;++i)
                for(int k=0;k<3;++k)
                    f<<s.trans(s(indices[i], k+3*j));
            f<<std::endl;
        }
        for(int j=0;j<3;++j)
        {
            for(int i=0;i<6;++i)
                f<<" ";
            for(int i=0;i<3;++i)
                f<<s.trans(s(3,i+j*3));
            f<<std::endl;
        }
        return f;
    }

};

#endif // STATE_H
