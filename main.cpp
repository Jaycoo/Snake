#include <iostream>
#include <ctime>
#include <windows.h>
#include <cstdlib>
#include <vector>
#include <queue>

#define R 0
#define D 1
#define L 2
#define U 3

#define m 20
using namespace std;


struct position
{
    int x;
    int y;
};
position apple;                    //苹果
vector<position> snake;            // 整个蛇身体
position tail;                     //蛇尾
static int wall[20][20] = {{0}};          //地图
int score = 0,add = 10;           //总分和加分；
int status,sleeptime = 10;
int endgamestatus = 0;      //游戏结束的情况 1：撞到墙 2：咬到自己 3：主动退出游戏


/**全部函数*/
void Pos(int,int);
void creatmap();
void initSnake();
bool biteitself();
void creatapple();
void cantcrosswall();
void snakemove();
void pause();
void gamecircle();
void welcometogame();
void endgame();
void gamestart();
bool virtualsnake(vector<position> s);
vector<int> FindPath(position start, position finish,vector<position> s); //暴力算法求最短路径
bool randomchoice(vector<position> s,int randdir);                        //预测下一条方向是否可行
void Pos(int x,int y)  //设置光标位置
{
    COORD pos;
    HANDLE hOutput;
    pos.X = 2*x;
    pos.Y = y;
    hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hOutput,pos);
}

void creatmap()                   //初始化地图；
{
    for(int i = 0; i < 20; i++)
    {
        wall[i][0] = 1;
        wall[0][i] = 1;
        wall[i][19] = 1;
        wall[19][i] = 1;
    }
    for (int i = 0; i < 20; i++)
    {
        for(int j = 0; j < 20; j++)
        {
            if(wall[i][j])
            {
                Pos(i,j);
                cout << "■";
            }
            else
            {
                Pos(i,j);
                cout << " ";
            }
        }
    }
}
void initSnake()               //初始化蛇
{
    tail.x = 6;
    tail.y = 5;
    for (int i = 0; i < 4; i++)
    {
        tail.x--;
        tail.y;
        snake.push_back(tail);
    }
    tail = snake[snake.size()-1];
    for(int i = 0; i < snake.size(); i++)
    {
        wall[snake[i].x][snake[i].y] =1;
        Pos(snake[i].x,snake[i].y);
        cout << "★";
    }
}
void creatapple()               //初始化苹果
{
    apple.x = rand() % 18 + 1;
    apple.y = rand() % 18 + 1;
    for(int i = 0; i < snake.size(); i++)
    {
        if(snake[i].x == apple.x && snake[i].y == apple.y)
            creatapple();
    }
    Pos(apple.x,apple.y);
    cout << "●";
}
bool biteitself()
{
    for(int i = 1; i < snake.size();i++)
    {
        if(snake[0].x == snake[i].x && snake[0].y == snake[i].y)
            return true;
    }
    return false;
}


void cantcrosswall()                 //不能撞墙
{
    if (snake[0].x == 0 || snake[0].x == 19 || snake[0].y == 0 || snake[0].y == 19)
    {
        endgamestatus = 1;
        endgame();
    }
}

void snakemove()
{
    cantcrosswall();
    position head;
    if(status == U)
    {
        head.x = snake[0].x;
        head.y = snake[0].y - 1;

    }
    else if(status == D)
     {
        head.x = snake[0].x;
        head.y = snake[0].y + 1;
     }
    else if(status == L)
     {
        head.x = snake[0].x - 1;
        head.y = snake[0].y;
     }
     else
     {
        head.x = snake[0].x + 1;
        head.y = snake[0].y;
     }
    snake.insert(snake.begin(),head);
    wall[head.x][head.y] = 1;
    if(head.x == apple.x && head.y == apple.y)
    {
        score += add;
        creatapple();
    }
    else
    {
        wall[snake[snake.size()-1].x][snake[snake.size()-1].y] = 0;
        Pos(snake[snake.size()-1].x,snake[snake.size()-1].y);
        cout << " ";
        snake.pop_back();
    }

     for(int i = 0; i < snake.size(); i++)
     {
         Pos(snake[i].x,snake[i].y);
         cout << "★";
     }
     if (biteitself())
    {
        endgamestatus = 2;
        endgame();
    }
}

void pause()                        //空格键暂停
{
    while(1)
    {
        Sleep(300);
        if(GetAsyncKeyState(VK_SPACE))
        {
            break;
        }
    }
}

void gamecircle()
{
    position offset[4];
    offset[0].x = 1; offset[0].y = 0;    //→
    offset[1].x = 0; offset[1].y = 1;    //↓
    offset[2].x = -1; offset[2].y = 0;   //←
    offset[3].x = 0; offset[3].y = -1;   //↑
    while(1)
    {
        if (GetAsyncKeyState(VK_SPACE))
        {
            pause();
        }
        Pos(64,10);
        cout << "得分：" << score;
        Pos(64,11);
        cout << "每个食物得分：" << add;
        Pos(64,12);

        vector<int> pathdir1 = FindPath(snake[0],apple,snake);
        vector<int> pathdir2 = FindPath(snake[0],snake[snake.size()-1],snake);
        if(pathdir1.size())
        {
            if(virtualsnake(snake))
            {
                status = pathdir1[0];
                Sleep(sleeptime);
                snakemove();
                pathdir1.clear();
            }
            else
            {
                status = pathdir2[0];
                Sleep(sleeptime);
                snakemove();
                pathdir2.clear();
            }

        }
        else
        {
                status = pathdir2[0];
                Sleep(sleeptime);
                snakemove();
                Sleep(sleeptime);
                pathdir2.clear();
        }
        Pos(25,25);
        cout << "         " << endl;
        Pos(25,25);
        cout << snake[0].x << "," << snake[0].y << endl;
        Pos(25,26);
        cout << "         " << endl;
        Pos(25,26);
        cout << snake[snake.size()-1].x << "," << snake[snake.size()-1].y << endl;

    }
}

void welcometogame()  // 开始界面
{
    Pos(25,25);
    cout << "Welcome to snake world!";
    system("pause");
    system("cls");
    Pos(25,12);
    cout << "用↑.↓.←.→分别控制蛇的移动， F1 为加速，2 为减速" << endl;
    Pos(25,13);
    cout << "加速能得到更高的分数";
    system("pause");
    system("cls");

}

void endgame()
{
    system("cls");
    Pos(24,12);
    if (endgamestatus==1)
    {
        cout << "撞墙，GAME OVER!" ;
        system( "PAUSE ");
    }
    else if (endgamestatus==2)
    {
        cout << "咬到自己，GAME OVER!" ;
        system( "PAUSE ");
    }
    else if (endgamestatus==3)
    {
        cout << "您终结了游戏" ;
        system( "PAUSE ");
    }
    Pos(24,13);
    cout << "您的得分是:" << score <<endl;
    system( "PAUSE ");
    exit(0);
}

void gamestart()                 //游戏开始
{
    system("mode con cols=100 lines=30");
    welcometogame();
    creatmap();
    initSnake();
    creatapple();
}

/**自动最短路径选择*/
vector<int> FindPath(position start, position finish,vector<position> s)
{
    int grid[m][m] = {{0}};
    for(int i = 0; i < 20; i++)
    {
        grid[i][0] = 1;
        grid[0][i] = 1;
        grid[i][19] = 1;
        grid[19][i] = 1;
    }
    for(int i = 0; i < s.size();i++)
        grid[s[i].x][s[i].y] = 1;
    if(finish.x == s[s.size()-1].x && finish.y == s[s.size()-1].y)
        grid[s[s.size()-1].x][s[s.size()-1].y] = 0;
    int NumOfNbrs = 4;
    int i,j;
    position offset[4];
    offset[0].x = 1; offset[0].y = 0;    //→
    offset[1].x = 0; offset[1].y = 1;    //↓
    offset[2].x = -1; offset[2].y = 0;   //←
    offset[3].x = 0; offset[3].y = -1;   //↑
    position here,nbr;
    here.x = start.x; here.y = start.y;
    grid[start.x][start.y] = 2;          //封锁起始点
    //标记可到达的位置
    queue<position> Q;
    do
    {
        for(i = 0; i < NumOfNbrs; i++)
        {
            nbr.x = here.x + offset[i].x;
            nbr.y = here.y + offset[i].y;
            if(grid[nbr.x][nbr.y] == 0)
            {
                grid[nbr.x][nbr.y] = grid[here.x][here.y] + 1;
                if(nbr.x == finish.x && nbr.y == finish.y)
                    break;
                Q.push(nbr);
            }
        }
        if(nbr.x == finish.x && nbr.y == finish.y)
            break;
        if(Q.empty())
            return {};
        here = Q.front();
        Q.pop();
    }while(true);
   int  Pathlen = grid[finish.x][finish.y] - 2;
    here = finish;
    vector<int> pathdir;
    for(j = Pathlen - 1; j >= 0; j--)
    {
        for(i = 0; i < NumOfNbrs; i++)
        {
            nbr.x = here.x + offset[i].x;
            nbr.y = here.y + offset[i].y;
            if(grid[nbr.x][nbr.y] == j + 2)
            {
                if(i == 0)
                    pathdir.insert(pathdir.begin(),2);
                else if(i == 1)
                    pathdir.insert(pathdir.begin(),3);
                else if(i == 2)
                    pathdir.insert(pathdir.begin(),0);
                else if(i == 3)
                    pathdir.insert(pathdir.begin(),1);
                break;
            }
        }
        here = nbr;
    }
    return pathdir;
}


bool virtualsnake(vector<position> s)               // 沿着尾巴吃的算法一定是不能吃满全屏的
{
    vector<position> vsnake(s);
    int vstatus = status;
    while(1)
    {
    vector<int> vpathdir = FindPath(vsnake[0],apple,vsnake);
    if(vpathdir.size() == 0)     //如果没有路的话
    {
        Pos(64,13);
        cout << 1 << endl;
        return false;
    }

    vstatus = vpathdir[0];
    //vsnake 移动
    position vhead;
    if(vstatus == U)
    {
        vhead.x = vsnake[0].x;
        vhead.y = vsnake[0].y - 1;

    }
    else if(vstatus == D)
     {
        vhead.x = vsnake[0].x;
        vhead.y = vsnake[0].y + 1;
     }
    else if(vstatus == L)
     {
        vhead.x = vsnake[0].x - 1;
        vhead.y = vsnake[0].y;
     }
     else
     {
        vhead.x = vsnake[0].x + 1;
        vhead.y = vsnake[0].y;
     }
    vsnake.insert(vsnake.begin(),vhead);
    if(vhead.x == apple.x && vhead.y == apple.y)
    {
        vector<int> vtailtohead = FindPath(vsnake[0],vsnake[vsnake.size()-1],vsnake);
        if(vtailtohead.size() == 0)
        {
            Pos(64,13);
            cout << 2 << endl;
            return false;
        }
        else
            return true;
    }
    else
        vsnake.pop_back();
    }
}


int nextwall(vector<position> s)
{
    int grid[m][m] = {{0}};
    for(int i = 0; i < 20; i++)
    {
        grid[i][0] = 1;
        grid[0][i] = 1;
        grid[i][19] = 1;
        grid[19][i] = 1;
    }
    for(int i = 0; i < s.size();i++)
        grid[s[i].x][s[i].y] = 1;
    position offset[4];
    offset[0].x = 1; offset[0].y = 0;    //→
    offset[1].x = 0; offset[1].y = 1;    //↓
    offset[2].x = -1; offset[2].y = 0;   //←
    offset[3].x = 0; offset[3].y = -1;   //↑
    int k = 0;
    for(int i = 0; i < 4; i++)
    {
        position next;
        next.x = s[0].x + offset[i].x;
        next.y = s[0].y + offset[i].y;
        if(grid[next.x][next.y] == 1)
            k++;
    }
    return k;
}



bool randomchoice(vector<position> s,int randdir)   //随机方向预测
{
    if(nextwall(s) == 4)
        return false;
    if(virtualsnake(s))
        return true;
    int grid[m][m] = {{0}};
    for(int i = 0; i < 20; i++)
    {
        grid[i][0] = 1;
        grid[0][i] = 1;
        grid[i][19] = 1;
        grid[19][i] = 1;
    }
    for(int i = 0; i < s.size();i++)
        grid[s[i].x][s[i].y] = 1;
    position offset[4];
    offset[0].x = 1; offset[0].y = 0;    //→
    offset[1].x = 0; offset[1].y = 1;    //↓
    offset[2].x = -1; offset[2].y = 0;   //←
    offset[3].x = 0; offset[3].y = -1;   //↑
    vector<position> dirsnake(s);
    //虚拟方向蛇移动
    int dir = randdir;
    position dirhead;
    dirhead.x = dirsnake[0].x + offset[dir].x;
    dirhead.y = dirsnake[0].y + offset[dir].y;
    if(grid[dirhead.x][dirhead.y] == 1)
        return false;
    dirsnake.insert(dirsnake.begin(),dirhead);
    dirsnake.pop_back();
    grid[dirsnake[0].x][dirsnake[0].y] = 1;
    grid[dirsnake[dirsnake.size()-1].x][dirsnake[dirsnake.size()-1].y] = 0;
    //递归
    return randomchoice(dirsnake,0) || randomchoice(dirsnake,1) ||randomchoice(dirsnake,2) ||randomchoice(dirsnake,3);
}
int main()
{
    srand((unsigned)time(NULL));
    gamestart();
    gamecircle();
    return 0;
}
