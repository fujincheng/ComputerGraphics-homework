//
//  main.h
//  XcodeGlutDemo
//
//  Created by 季琳雅 游义男 on 15/4/29.
//  Copyright (c) 2015年 季琳雅 游义男. All rights reserved.
//

#ifndef __XcodeGlutDemo__main__
#define __XcodeGlutDemo__main__

#include <stdio.h>
#include <GLUT/GLUT.h>
#include <vector>
#include <iostream>
#include <fstream>


using namespace std;
#define MAX(a,b) (a>b?a:b)
#define MIN(a,b) (a<b?a:b)

template <typename T>//根据你所需要的类型就行匹配。其实就是模板
struct Node{
    T data; //数据域
    Node* next; //指针域
};
template <typename T>
class LinkList{
    public:
    Node<T> *first;//node模版类，链表的节点将T类型的数据封装；定义一个变量名为first的node指针
    Node<T> *now;
    Node<T> *dele;
    int lg;
    LinkList(){
        first=(Node<T> *)malloc(sizeof(Node<T>));first->next = NULL;
    }
    T Get(int j)
    {
        now = first->next;
        for(int i = 1; i < j; i++)
        {
            now = now->next;
        }
        return now->data;
    }
    void Delete(int j)
    {
        dele = now = first;
        for(int i = 1; i <= j; i++)
        {
            dele = now;
            now = now->next;
        }
        dele->next=now->next;
    }
    int Length()
    {
        lg = 0;
        now = first->next;
        while(now)
        {
            now = now->next;
            lg++;
        }
        return lg;
    }
};

class point //点类，存储了一个点的两坐标值
{
public:
    int x;
    int y;
};

class AELvertex //动态扫描线类，和书上讲的一样，有线段顶点的y值，和另一端点的x值，还有斜率的倒数
{
public:
    double x;//min x
    double y;//max y
    double dx;//slope
};
class Evertex //边类，存储一条边的两个端点
{
public:
    int x1;
    int x2;
    int y1;
    int y2;
    void Insert(int a,int b,int c,int d)
    {
        x1 = a;
        y1 = b;
        x2 = c;
        y2 = d;
    }
};
class polygon //多边形类，存了一个多边形
{
public:
    vector<point> p; //多边形的顶点
    vector<Evertex> edge; //多边形的所有边
    void line() //由多边形的顶点生成边的函数
    {
        if(!p.empty())
        {
            int i, k = p.size();
            Evertex E;//lei
            for(i = 0; i <k;  i++)
            {
                E.Insert(p[i].x,p[i].y,p[(i+1)%k].x,p[(i+1)%k].y);
                edge.push_back(E);//把
            }
        }
    }
    int color ;
};

static const int screenwidth = 800;  //自定义窗口宽度常亮
static const int screenheight = 800; //自定义窗口高度
vector<point> p; //多边形点集向量
vector<polygon> s; //多边形类向量，用来保存已经画完的多边形
int current_x,current_y; //鼠标当前坐标值，在鼠标移动动态画线时使用

//主函数索要用到的所有函数
void init();
void myDisplay();
void onTapped(int button, int state ,int x, int y); //鼠标点击事件响应函数
void mouseMoved(int x, int y); //鼠标移动事件响应函数
void paint(vector<Evertex> v); //扫描线填充函数
void Sort(AELvertex &v1, AELvertex &v2); //交换函数，将两个AELvertex类中的所有值交换

/*填充方案的枚举*/
enum string{
    red,//reda
    green,//green
    blue,//blue
    colorful,
    
    black_white_line,//横线间隔填充
    col_line,//竖线间隔填充
    xie_line, //斜线填充
    
    black_white_sqare,//方块状填充
    black_white_triangle,//三角形
    black_white_circle//圆形
};

int menu;
int Bcolor;//background color
int oldColor;

#endif


