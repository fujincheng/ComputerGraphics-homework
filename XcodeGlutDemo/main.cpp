//
//  main.cpp
//  XcodeGlutDemo
//
//  Created by 季琳雅 游义男 on 15/4/29.
//  Copyright (c) 2015年 季琳雅 游义男. All rights reserved.
//




#include"Header.h"



bool select = false; //多边形封闭状态判断变量，当为true时即按下鼠标右键，将多边形封闭 ,形成一个多边形。
void init()//初始化函数
{
    glClearColor(0.5,0.5,0.5,0.0);//屏幕背景色
    glColor3f(1.0, 1.0, 1.0);//白色
    glMatrixMode(GL_PROJECTION);//指定哪一个矩阵是当前矩阵mode 指定哪一个矩阵堆栈是下一个矩阵操作的目标,可选值: GL_MODELVIEW(投影矩阵操作)、GL_PROJECTION(模型视景矩阵操作)、GL_TEXTURE(纹理矩阵).
    gluOrtho2D(0.0,screenwidth,0.0,screenheight);//视景图 gluOrtho2D是指屏幕区域对应的模型坐标范围，比如横向范围是0到screenwidth米，纵向是0到screenheight米
}

//回调函数，画线函数
void myDisplay()
{
    
    glClear(GL_COLOR_BUFFER_BIT);//当前可写的颜色缓冲(glClearColor(0.5,0.5,0.5,0.0))
    // GL_DEPTH_BUFFER_BIT:深度缓冲;   GL_ACCUM_BUFFER_BIT:累积缓冲;    　　GL_STENCIL_BUFFER_BIT: 模板缓冲;
    glColor3f(0.0, 0.0, 0.0);//设定颜色，既是线段颜色也是填充色float 最开始画线的颜色。
    int i, j;                 //两个循环控制变量，在下面的向量循环和数组循环中将被多次调用。
    if(!s.empty())            //看多边形类向量是否为空，即判断除了当前正在画的多边形是否还有曾经已经画好的多边形
    {
        for( i = 0; i < s.size(); i++)   //对多边形类向量循环，该向量中的每个元素代表一个多边形
        {
            int k = s[i].p.size();  //将一个多边形的点的个数，后面划线会用到
            s[i].line(); //生成多边形的边
            for(j = 0; j < s[i].p.size(); j++) //画多边形
            {
                glBegin(GL_LINES); //将当前的点与后一个点连线
                glVertex2i(s[i].p[j].x, s[i].p[j].y);//输入int类型的点二维  glVertex2d glVertex2f  glVertex3i(三维)
                glVertex2i(s[i].p[(j+1)%k].x, s[i].p[(j+1)%k].y);//，通过取模操作来避免越界问题，该思路来源于循环队列.
                glEnd();
            }
            oldColor = Bcolor;
            if(s[i].color != 0)
                Bcolor = s[i].color;
            
            paint(s[i].edge);  //为当前的多边形填充颜色
            s[i].edge.clear(); //清空当前多边形的边向量
            Bcolor = oldColor;
        }
    }
    i = 0;
    j = p.size() - 1;
    while(i < j)           //循环画图，将当前正在画的多边形
    {
        glBegin(GL_LINES); //将已经确定的点连接起来
        glVertex2i(p[i].x, p[i].y);
        glVertex2i(p[i+1].x, p[i+1].y);
        glEnd();
        i++;
    }
    if(!p.empty())
    {
        //	int i = p.size() - 1; //将确定的最后一个点与当前鼠标所在位置连线，即动态画线
        glBegin(GL_LINES);
        glVertex2i(p[j].x, p[j].y);
        glVertex2i(current_x, current_y);//鼠标移动的点
        glEnd();
    }
    if(select) //判断右键是否被点下
    {
        select = false; //将状态值置为假
        if(!p.empty())
        {
            glBegin(GL_LINES); //将多边形封闭
            glVertex2i(p[j].x, p[j].y);
            glVertex2i(p[0].x, p[0].y);
            glEnd();
            polygon sq;
            //将封闭了的多边形保存到多边形类中
            for(i = 0; i < p.size(); i++)
                sq.p.push_back(p[i]);
            sq.color = Bcolor;//记忆当前选择的填充颜色
            s.push_back(sq); //将绘成的多边形存入多边形类向量中
            sq.line();//构造边集
            paint(sq.edge); //给当前画完的多边形填色
        }
        p.clear();
    }
    glFlush();
}


//在头文件中定义了point类存储点的坐标值(x,y) select默认false
void onTapped(int button, int state ,int x, int y) //鼠标点击事件响应函数
{
    if(state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)//当鼠标左键被点击
    {
        point v;  //申请一个点类变量，点类为自定义类，在Header.h中定义 point类
        v.x = x; //将点击处的点坐标，即x和y的值存入v中
        v.y = screenheight - y;
        cout << "x:"<<v.x<<" y:"<<v.y<<endl;
        p.push_back(v); //将点信息存入多边形点集向量p中
        glutPostRedisplay(); //重绘窗口
    }
    
    if(state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON) //当鼠标右键被点击
    {
        select = true;
        glutPostRedisplay();
    }
    
    
}

void mouseMoved(int x, int y) //鼠标移动事件响应函数
{
    current_x = x; //保存当前鼠标所在的坐标的值
    current_y = screenheight - y;
    glutPostRedisplay();//在图像绘制的所有操作之后，要加入 glutPostRedisplay() 来重绘图像
    
}

//define 3 指针 a p q;
void paint(vector<Evertex> v)//边类，存储一条边的两个端点
{
    int i,j; //两个循环控制变量，在下面的向量循环和数组循环中将被多次调用。
    Node<AELvertex>* a[screenheight]; //扫描线数组指针 AELvertex 有线段顶点的y值，和另一端点的x值，还有斜率的倒数
    Node<AELvertex>* p; //p和q为扫描线类指针，是用来对指针链表进行插入和循环时用的
    Node<AELvertex>* q;
    
    int m;
    
    bool tag = true;//填充颜色变化的控制变量
    bool red_tag = true;
    bool green_tag = true;
    bool blue_tag = true;

    
    for(i = 0; i < screenheight; i++) //将扫描线数组指针制空 水平scan
        a[i] = NULL;
    int k = 0; //下面要进行的是将多边形的边信息转化为扫描线信息
    
    for(j = 0; j < v.size(); j++)//边的个数
    {
        p = new Node<AELvertex>; //new一个扫描线类空间，next为null
        p->next = NULL;
        if(v[j].y1 < v[j].y2) //两个if判断将线段两个端点中y坐标较大，将y值存入扫描线中，x坐标较小的，将x值存入扫描线中。
        {
            k = v[j].y1;//
            p->data.x = v[j].x1;
            p->data.y = v[j].y2;
            p->data.dx = (v[j].x1 * 1.0 - v[j].x2 * 1.0) / (v[j].y1 * 1.0 - v[j].y2 * 1.0);
        }
        if(v[j].y1 > v[j].y2)
        {
            k = v[j].y2;
            p->data.x = v[j].x2;
            p->data.y = v[j].y1;
            p->data.dx = (v[j].x1 * 1.0 - v[j].x2 * 1.0) / (v[j].y1 * 1.0 - v[j].y2 * 1.0);
        } //两个y值相等说明线段平行于x轴，直接忽略
        //将申请的空间连接到扫描线链表中
        if(a[k] == NULL) //若为空，则直接将数组元素指针指向新空间
            a[k] = p;
        
        else //不为空，则通过尾插法将新空间插入链表尾部,
        {
            q = a[k];
            while(q->next)
                q = q->next;
            q->next = p;
        }
    }
    //填充部分
    
    LinkList<AELvertex> AEL; //动态扫描线链表，动态扫描线类
    
    for(i = 0; i < screenheight; i++) //动态扫描链表的结点添加
    {
        
        if(a[i] != NULL) //当元素不为空，即在此处有扫描线结点时
        {
            
            p = AEL.first; //尾插法将a[i]中的结点信息插入AEL中
            while(p->next != NULL)
                p = p->next;
            p->next = a[i];
            a[i] =  NULL; //a[i]制空，防止野指针出现
            vector<AELvertex> sort; //一个动态扫描线类向量，用来对AEL链表进行排序用的
            p = AEL.first->next;
            while(p) //将AEL链表中的所有扫描线点的信息给sort
            {
                sort.push_back(p->data);
                p = p->next;
            }
            for(j = 0; j < sort.size(); j++) //对sort中的值排序，扫描线生成时需要所有点从小到大排好
            {
                for(k = 0; k < sort.size() - 1; k++)
                {
                    if(sort[k].x > sort[k+1].x)
                        Sort(sort[k], sort[k+1]); //值交换，开头声明的Sort函数就是在这里用的
                }
            }
            p = AEL.first->next;
            j = 0;
            while(p && j < sort.size()) //将排好序的结点在放回AEL链表中
            {
                Sort(p->data, sort[j]);
                j++;
                p = p->next;
            }
            sort.clear();
        }
        
        
        //扫描线生成，核心中的核心
        p = AEL.first->next;
        if(p) //判断AEL中是否有结点
        {
            for(j = 1; j <= AEL.Length(); j++)
            {
                k = AEL.Get(j).y; //取出每个结点的y的值，判断与当前扫描线的y坐标值即i比较，判断是否已经到线段顶部
                if(k <= i) //这里我用了一个小技巧，没有按课上讲的通过端点是否为极值点来判断端点应该去还是留
                    //我用的方法是判断端点是线段起点还是终点，是起点的改点保存，是终点就delete，
                {
                    AEL.Delete(j);
                    j--;
                }
            }
            p = AEL.first;
            while(p->next) //将AEL链表中的点信息结点两两取出连线
            {
                
                p = p->next;
                q = p->next;
                k = (p->data.x + 0.5); //简单的四舍五入取整
                j = (q->data.x + 0.5);
                
                switch(Bcolor)
                {
                    case red://红
                        glColor3f(1.0,0.0,0.0);
                        glBegin(GL_LINES);
                        glVertex2i(k, i);
                        glVertex2i(j, i);
                        glEnd();
                        break;
                    case green://绿
                        glColor3f(0.0,1.0,0.0);
                        glBegin(GL_LINES);
                        glVertex2i(k, i);
                        glVertex2i(j, i);
                        glEnd();
                        break;
                    case blue://蓝
                        glColor3f(0.0,0.0,1.0);
                        glBegin(GL_LINES);
                        glVertex2i(k, i);
                        glVertex2i(j, i);
                        glEnd();
                        break;
                    case colorful://彩色填充
                        
//                        if(i%12 ==0 || i%12 ==1 || i%12 ==3)
//                        {
//                            red_tag = !red_tag;
//                        }
//                        if (red_tag) {
//                            glColor3f(1.0, 0.0, 0.0);
//                        }
//                        glBegin(GL_LINES);
//                        glVertex2i(k, i);
//                        glVertex2i(j, i);
//                        glEnd();
//                        
//                        if(i%12 ==4 || i%12 ==5 || i%12 ==6)
//                        {
//                            green_tag = !green_tag;
//                        }
//                        if (green_tag) {
//                            glColor3f(0.0, 1.0, 0.0);
//                        }
//                        glBegin(GL_LINES);
//                        glVertex2i(k, i);
//                        glVertex2i(j, i);
//                        glEnd();
                        if(i%12 == 1 )
                        {
                            glColor3f(0.2, 0.2, 0.2);
                            //blue_tag = !blue_tag;
                        }
                        
                        else if(i%12 == 0 )
                        {
                            glColor3f(0.0, 1.0, 0.0);
                            //blue_tag = !blue_tag;
                        }
                        else if(i%12 == 2 )
                        {
                            glColor3f(0.0, 0.0, 1.0);
                            //blue_tag = !blue_tag;
                        }
                        else if(i%12 == 3 )
                        {
                            glColor3f(1.0, 1.0, 0.0);
                            //blue_tag = !blue_tag;
                        }
                        else if(i%12 == 4 )
                        {
                            glColor3f(1.0, 0.0, 1.0);
                            //blue_tag = !blue_tag;
                        }
                        else if(i%12 == 5 )
                        {
                            glColor3f(0.5, 1.0, 0.0);
                            //blue_tag = !blue_tag;
                        }
                        else if(i%12 == 6 )
                        {
                            glColor3f(0.0, 1.0, 0.5);
                            //blue_tag = !blue_tag;
                        }

                        else {
                            glColor3f(1, 1, 1);//white
                        }
                        glBegin(GL_LINES);
                        glVertex2i(k, i);
                        glVertex2i(j, i);
                        glEnd();
                        break;
                    case black_white_line:	//水平间隔填充
                        if(i%8 == 0)//int i,j; 两个循环控制变量.
                        {
                            tag = !tag;// bool tag = true,填充颜色变化的控制变量 表示变化快每隔12翻转一次
                        }
                        if(tag)
                            glColor3f(0.0,0.0,0.0);   //设定颜色，既是线段颜色也是填充色
                        else
                            glColor3f(1.0,1.0,1.0);
                        glBegin(GL_LINES);
                        glVertex2i(k, i);
                        glVertex2i(j, i);
                        glEnd();
                        break;
                    case col_line://竖直间隔填充
                        glBegin(GL_POINTS);
                        for(m = MIN(k,j); m <= MAX(k,j) ; m++)
                        {
                            
                            if(m%12 == 0 || m%12 == 1 || m%12 == 2 || m%12 == 3 ||m%12 ==4)
                                glColor3f(0,0,0.0);   //设定颜色，既是线段颜色也是填充色
                            else
                                glColor3f(1.0,1.0,1.0);
                            glVertex2i(m,i);
                        }
                        glEnd();
                        break;
                    case xie_line://斜线间隔填充
                        glBegin(GL_POINTS);
                        for(m = MIN(k,j); m <= MAX(k,j) ; m++)
                        {
                            if((m+i)%10 >= 0 &&(m+i)%10 <= 4)
                                glColor3f(0,0,0.0);   //设定颜色，既是线段颜色也是填充色
                            else
                                glColor3f(1.0,1.0,1.0);
                            glVertex2i(m,i);
                        }
                        glEnd();
                        break;
                    case black_white_sqare://heibai方格填充
                        glBegin(GL_POINTS);//对电进行扫描
                        for(m = MIN(k,j); m <= MAX(k,j) ; m++)
                        {
                            
                            if(m%30 >= 0 && m%30 <= 14)
                                if(i%30 >= 0 && i%30 <= 14)
                                    glColor3f(0.0,0.0,0.0);
                                else
                                    glColor3f(1.0,1.0,1.0);
                                else
                                    if(i%30 >= 0 && i%30 <= 14)
                                        glColor3f(1.0,1.0,1.0);
                                else
                                    glColor3f(0.0,0.0,0.0);
                            
                            
                            glVertex2i(m,i);
                        }
                        glEnd();
                        break;
//                    case black_white_triangle://三角形:
//                        glBegin(GL_POINTS);
//                        for(m = MIN(k,j); m <= MAX(k,j) ; m++)//k 多边形的边信息转化为扫描线信息
//                        {
//                            
//                            if(m%30 >= 0 && m%30 <= 14)
//                                if(i%30 >= 0 && i%30 <= 14)
//                                    glColor3f(1.0,0.0,0.0);//red
//                                else
//                                    glColor3f(0.0,1.0,0.0);//green
//                                else
//                                    if(i%30 >= 0 && i%30 <= 14)
//                                        glColor3f(0.0,0.0,1.0);//blue
//                                    else
//                                        glColor3f(0.0,0.0,0.0);//black
//                            
//                            
//                            glVertex2i(m,i);
//
//                        }
//                        glEnd();
//                        break;
                    case black_white_circle://方格填充
                        glBegin(GL_POINTS);
                        for(m = MIN(k,j); m <= MAX(k,j) ; m++)
                        {
                                                        if(m%40 >= 0 && m%40 <= 2)
                                                            if(i%6 >= 0 && i%6 <= 2)
                                                                glColor3f(0.0,0.0,0.0);
                                                            else
                                                                glColor3f(1.0,1.0,1.0);
                            
                                                        else
                                                                if(i%6 >= 0 && i%6 <= 2)
                                                                    glColor3f(1.0,1.0,1.0);
                                                                else
                                                                    glColor3f(0.0,0.0,0.0);
                                                        
                                                        
                                                        glVertex2i(m,i);
                        }
                        glEnd();
                        break;
                }
                
                
                p->data.x = p->data.x + p->data.dx; //对点进行数据更新
                q->data.x = q->data.x + q->data.dx;
                p = q;
                
            }
            glColor3f(0.0,0.0,0.0);//black
        }
        
    }
}
void Sort(AELvertex &v1, AELvertex &v2) //交换AELvertex类变量的值
{
    double i;
    i = v1.x;
    v1.x = v2.x;
    v2.x = i;
    i = v1.dx;
    v1.dx = v2.dx;
    v2.dx = i;
    i = v1.y;
    v1.y = v2.y;
    v2.y = i;
}




//读取文件
void ReadFile()
{
    p.clear();
    
    point v;  //申请一个点类变量
    
    FILE * f1;
    f1 = fopen("data.out","r");
    if(f1 != NULL)
    {
        
        while(!feof(f1))
        {
            fscanf(f1,"%d%d",&v.x,&v.y);
	
            if(v.x != -1 && v.y != -1)
                p.push_back(v);
            else	
            {
                polygon sqq;
                for(int i = 0; i < p.size(); i++)
                {
                    sqq.p.push_back(p[i]);
                }
                sqq.color = 0;
                s.push_back(sqq);
                p.clear();
                glutPostRedisplay();
            }
        }
        
    }
    fclose(f1);
}


//保存文件，将图形的顶点保存到文件中
void SaveFile()
{
    freopen("data.out","w",stdout);
    if(!s.empty())            //看多边形类向量是否为空，即判断除了当前正在画的多边形是否还有曾经已经画好的多边形
    {
        for(int  i = 0; i < s.size(); i++)   //对多边形类向量循环，该向量中的每个元素代表一个多边形
        {
            for( int j = 0; j < s[i].p.size(); j++) //画多边形
            {
                cout << s[i].p[j].x<< " "<<s[i].p[j].y <<endl;
            }
            cout <<-1<<" "<<-1 <<endl;
        }
        
    }
    fclose(stdout);
}


//清屏函数
void ClearScreen()
{
    s.clear();
    p.clear();
}

//主菜单
void MenuFunc(int data)
{
    glutGetMenu();
    switch(data)
    {
        case 1://背景赋值为纯色
            //Bcolor = red;
            break;
        case 2:
            //Bcolor = black_white_line;
            break;
        case 3:
            //Bcolor = black_white_sqare;//背景赋值为方格色
            break;
        case 4://文件的保存
            SaveFile();
            break;
        case 5:
            ReadFile();
            break;
        case 6:
            ClearScreen();
            break;
        case 7:
            //rotate
            break;
    }
}

//子菜单的建立
void FileColor(int data)
{
    switch(data)
    {
        case 1:
            Bcolor = black_white_line; //水平
            break;
        case 2:
            Bcolor = col_line;//竖直
            break;
        case 3:
            Bcolor = xie_line;//斜线
            break;
    }
}

void MenuColor(int data){
    switch (data) {
        case 1:
            Bcolor = red;
            break;
        case 2:
            Bcolor = green;
            break;
        case 3:
            Bcolor = blue;
            break;
        case 4:
            Bcolor = colorful;
            break;
    }
}

void ShapeColor(int data){
    switch (data) {
        case 1:
            Bcolor = black_white_sqare;//square 方块填充
            break;
        case 2:
            Bcolor = black_white_triangle;//triangle
            break;
        case 3:
            Bcolor = black_white_circle;//circle
            break;
    }
}


int sub_menu;
int sub_menu_color;
int sub_menu_shape;


int main(int argc, char** argv)
{
    Bcolor = 0;//将填充方案置为空
    glutInit(&argc, argv);//使得应用程序可以获取命令行参数并初始化系统
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);//设置窗口属性
    glutInitWindowPosition(50,100);//窗口在显示器上的初始位置
    glutInitWindowSize(screenwidth,screenheight);//以像素为单位定义窗口的初始大小
    glutCreateWindow("计算机图形学作业");
    
    //各种菜单，子菜单.
    sub_menu_color = glutCreateMenu(MenuColor);//color sumenu
    glutAddMenuEntry("红色填充",1);
    glutAddMenuEntry("绿色填充",2);
    glutAddMenuEntry("蓝色填充",3);
    glutAddMenuEntry("彩色填充",4);
    sub_menu = glutCreateMenu(FileColor);//间隔填充
    glutAddMenuEntry("水平间隔填充",1);
    glutAddMenuEntry("竖直间隔填充",2);
    glutAddMenuEntry("斜线间隔填充",3);
    sub_menu_shape = glutCreateMenu(ShapeColor);//图形填充
    glutAddMenuEntry("正方形填充",1);
//    glutAddMenuEntry("三角形填充",2);
    glutAddMenuEntry("彩色方块填充",3);
    menu = glutCreateMenu(MenuFunc);
    glutAddSubMenu("颜色填充",sub_menu_color);
    glutAddSubMenu("间隔填充",sub_menu);//添加sub_menu为子菜单
    glutAddSubMenu("图形填充",sub_menu_shape);
//    glutAddMenuEntry("图形旋转",7);
    glutAddMenuEntry("保存图形形状文件",4);
    glutAddMenuEntry("读取已有图形文件",5);
    glutAddMenuEntry("清屏",6);
    glutAttachMenu(GLUT_MIDDLE_BUTTON);//鼠标中键
    
    init();//初始化函数
    glutMouseFunc(onTapped); //鼠标点击消息监控，即监控鼠标是否被点击，若被点击就调用该函数
    glutDisplayFunc(myDisplay);//定义显示回调函数
    glutPassiveMotionFunc(mouseMoved); //鼠标移动消息监控，即监控鼠标是否移动，若移动就调用该函数
    //当鼠标在窗口中移动时调用glutPassiveMotionFunc注册的回调函数
    glutMainLoop();
    
    return 0;
    
}