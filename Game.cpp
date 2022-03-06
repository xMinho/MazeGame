#include "Game.h"
#include <iostream>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <fstream>

using namespace std;

/*
	游戏主界面
	包含：闯关、攻略、编辑地图、退出 
*/
void Menu() 
{
	cout << "———————————" << endl;
	cout << "   许敏浩迷宫小游戏" << endl;
	cout << "———————————" << endl;
	cout << "     1.开始闯关" << endl;
	cout << "     2.通关攻略" << endl;
	cout << "     3.编辑迷宫" << endl;
	cout << "     4.退出游戏" << endl;
	cout << "———————————" << endl;
	cout << "输入选项 >> ";
	
	int flag;
	cin >> flag;
	if (flag>0 && flag<5) 
	{
		system("cls");
		if (flag == 1) 							//闯关 
		{
			system("cls");
			cout << endl << "  * 使用方向键↑→↓←移动老鼠" << endl;
			cout << "  * 帮助老鼠找到粮仓即为通关" << endl;
			cout << "  * 通过关卡所花时间越短积分越多" << endl << endl;
			system("pause");
			system("cls");
			
			HideCursor();						//隐藏光标 
			int score; 							//初始化积分为0 
			score = 0;
			for (int i=0; i<MAX_LEVEL+1 && flag==1; i++) 
			{
				if (i == MAX_LEVEL) 			//通关判断 
				{
					system("cls");
					cout << endl << "* 恭喜你通过所有关卡" << endl; 
					cout << "* 积分： " << score << endl << endl;
					system("pause");
				}
				else 
				{
					Stage stage(i+1);
					stage.inFile();
					stage.PrintMap();
					int solved = stage.MoveEvent(score); 	//判断是否通关
					
					flag = solved; 
					if (solved) 
					{
						cout << "  是否继续闯关(1:是 2:否) >> ";                   
						cin >> flag;
					}
					system("cls");
				}
			}
		}
		else if (flag == 2) 
		{
			HideCursor(); 							//隐藏光标 
			cout << "请输入想查看关卡的通关攻略(1-5) >> ";
			cin >> flag;
			if (flag>0 && flag <6) 
			{
				system("cls");
				Stage stage(flag);
				stage.inFile();
				stage.MinPath();
				stage.PrintMap();
				cout << "  * 关卡：" << flag << endl << endl;
			}
			system("pause");
		}
		else if (flag == 3)					//编辑关卡 
		{
			cout << "请输入想编辑的关卡的关卡(1-5) >> ";
			cin >> flag;
			if (flag>0 && flag <6) 
			{
				system("cls");
				Stage stage(flag);
				stage.inFile();
				stage.PrintMap();
				stage.EditMap();
			}
			system("pause");
		} 
		else 								//退出 
		{
			exit(0);
		}
	} 
	else
	{
		cout << endl << "* 请输入正确的选项 *" << endl << endl;
		system("pause");
	}
	
	system("cls");
	Menu();
}



/*
	构造函数 
	初始化基本变量，如关卡，队列指针 
*/

Stage::Stage(int lv) {
	level = lv; 									//初始化关卡等级 
	solved = 0;										//初始化为未通关
	
	front = -1; 									//初始化队列 
	rear = -1;
}



/* 
	打印地图 
	打印范围 0-Length+1/Width+1
	其中0、Length+1/Width+1 为空 
 */
void Stage::PrintMap() 
{
	for (int i=0; i<Length+2; i++)
	{ 
		for (int j=0; j<Width+2; j++) 
		{
			int flag = Poi[i][j].flag;
			
			if (flag == 0) 		//空 
			{
				cout << "  ";
			} 
			else if (flag == 1) //墙 
			{
				cout << "■";
			} 
			else if (flag == 2) //老鼠 
			{
				cout << "☆";
			} 
			else if (flag == 3) //粮仓 
			{
				cout << "※"; 
			}
			else if (flag == 4) //走过的路径点 
			{
				cout << "·";
			}
		}
		cout << endl;
	}
}



/*
	移动事件 
	即开始游戏，玩家通过操作方向键进行在限定时间内走出迷宫 
	
	* 通过移动光标更改输出防止闪烁 
*/
int Stage::MoveEvent(int& score)
{
	time_t t_start; 								//起始时间 
	time_t t_now; 									//现在时间 
	
	int timing = 0; 								//累计时长
	
	int x = Length/2+1; 							//老鼠的坐标 x,y 
	int y = Width/2+1;
	
	t_start = time(NULL); 							//记录起始时间 
	cout << "  剩余时间 " << limit << " 秒，当前关卡：" << level << endl;
	while((timing < limit) && (!solved)) 			//未超过规定时长，并且未通关
	{
		t_now = time(NULL);
		timing = t_now - t_start;
		
		if(!_kbhit())  								//没有敲击事件就更新时间 
		{
			GotoXY(Length+2, 4); 					//光标移动至时间前进行修改 
			cout << " " << limit - timing << " "; 	//限定时间减去累计时长	
		}
		
		if(_kbhit()) 								//有敲击事件 
		{
			char enter; 							//获取敲击键 
			enter = getch();
			
			if(enter == -32) 						//方向键第一个读取出来的字符ASCII码值是-32 
			{
				enter = getch(); 					//第二个才是方向键对应的ASCII码值
				 
				if(enter == 75) 		//左键，y-1
				{
					if(Poi[x][y-1].flag != 1)
					{
						GotoXY(x, y-1); 			//移动光标，移动老鼠 
						cout << "  ";
						GotoXY(x, y-2);
						cout << "☆";
						
						Poi[x][y-1].flag = 2; 		//将该坐标对应类型改为老鼠
						Poi[x][y].flag = 4; 		//设置为走过的路径点
						y = y - 1;					//更新老鼠坐标
					}
				}
				else if(enter == 77)	 //右键 y+1
				{
					if(Poi[x][y+1].flag != 1)
					{
						GotoXY(x, y-1);
						cout << "  ";
						GotoXY(x, y);
						cout << "☆";
						
						Poi[x][y+1].flag = 2;
						Poi[x][y].flag = 4;
						y =  y + 1;
					}	
				}
				else if(enter == 72) 	 //上键，x-1
				{
					if(Poi[x-1][y].flag != 1)
					{
						GotoXY(x, y-1);
						cout << "  ";
						GotoXY(x-1, y-1);
						cout << "☆";
						
						Poi[x-1][y].flag = 2;
						Poi[x][y].flag = 4;
						x = x - 1;
					}	
				} 
				else if(enter == 80) 	//下键，x+1
				{
					if (Poi[x+1][y].flag != 1)
					{
						GotoXY(x, y-1); 
						cout << "  ";
						GotoXY(x+1, y-1);
						cout << "☆";
						
						Poi[x+1][y].flag = 2;
						Poi[x][y].flag = 4;
						x = x + 1;
					}
				}
			}
			
			if(Poi[Length-1][Width-1].flag == 2) {			//老鼠到达仓库 
				solved = 1;
				score += limit - timing;
				system("cls");
				PrintMap();
				cout << "  * 闯关成功，当前关卡：" << level << " 当前积分：" << score << endl << endl;
			}
		}
	}
	
	if (!solved) {
		system("cls");
		cout << endl << "  * 闯关失败，请重新尝试！" << endl;
		cout << "  * 关卡：" << level << endl;
		cout << "  * 积分：" << score << endl << endl;
		system("pause");
	}
	
	return solved;
}



/*
	编辑迷宫 
	通过方向键移动光标，使用空格键更改墙路 
*/
void Stage::EditMap() 
{
	cout << "  * 使用方向键↑→↓←移动光标" << endl;
	cout << "  * 使用空格键更改墙/路" << endl;
	cout << "  * 使用ESC键退出编辑" << endl;
	
	Stage stage = *this;
	
	int x = 2; 										//光标位置 光标指向位置行方向减一 
	int y = 1;
	GotoXY(2, 1); 									//初始化光标位置
	
	int flag;
	flag = 1;
	while (flag) 
	{
		if(_kbhit()) 								//有敲击事件 
		{
			char enter; 							//获取敲击键 
			enter = getch();
			
			if (enter == 32) {
				if (stage.Poi[x][y+1].flag == 0) {
					stage.Poi[x][y+1].flag = 1;		//更新坐标类型 
					cout << "■"; 					//更新界面显示 
					GotoXY(x, y);					//光标位置保持不变 
				}
				else if (stage.Poi[x][y+1].flag == 1) 
				{
					stage.Poi[x][y+1].flag = 0;
					cout << "  ";
					GotoXY(x, y);
				}
			}
			else if(enter == -32) 					//方向键第一个读取出来的字符ASCII码值是-32 
			{
				enter = getch(); 					//第二个才是方向键对应的ASCII码值
				if(enter == 75) 					//左键，y-1
				{
					if(y-1 > 0) 					//在墙内 
					{
						GotoXY(x, y-1);
						y = y -1;
					}
				}
				else if(enter == 77) 				//右键 y+1
				{
					if(y+1 < Width-1)
					{
						GotoXY(x, y+1);	
						y =  y + 1;
					}	
				}
				else if(enter == 72) 				//上键，x-1
				{
					if(x-1 > 1)
					{
						GotoXY(x-1, y);	
						x = x - 1;
					}	
				} 
				else if(enter == 80) 				//下键，x+1
				{
					if (x+1 < Length)
					{
						GotoXY(x+1, y);	
						x = x + 1;
					}
				}
			}
			else if (enter == 27) 					//ESC键退出编辑 
			{
				flag = 0;
			}
		}
	}
	
	if (stage.MinPath()) {							//如果路径可以通行 
		system("cls");
		for (int i=0; i<MAX; i++)
  		{
			for (int j=0; j<MAX; j++) 
			{
				if (stage.Poi[i][j].flag == 4)		//清除路径点 
				{
					stage.Poi[i][j].flag = 0;
				}
			}
		}
		
		stage.outFile(); 	 						//编辑成功，更新迷宫地图文件 
		stage.PrintMap();
		cout << "  * 编辑成功，地图已保存" << endl << endl;
	} 
	else 											//如果路径不可以通行 
	{
		system("cls");
		stage.PrintMap();
		cout << "  * 编辑失败，编辑后的地图无法通关" << endl << endl;
	}
	
	system("pause");
	system("cls");
	Menu();
}



/*
	更新迷宫文件
	将迷宫文件更新到对应的文件中 
*/
void Stage::outFile() {
	char lv = level + 48; 					//获取关卡的文件名 
	string maze = "maze/Maze_";
	string suffix = ".txt";
	maze.push_back(lv); 					
	maze.append(suffix);
	
	ofstream outfile;
	outfile.open(maze.c_str(), ios::out | ios::trunc);
	
	int out;
	outfile << Length << " " << Width << " " << limit << endl;
	for (int i=0; i<Length+2; i++) 
	{
		for (int j=0; j<Width+2; j++) 
		{
			outfile << Poi[i][j].flag;
		}
		outfile << "\n";
	}
}



/*
	文件读取操作
	读取对应关卡的迷宫文件
	Maze_x.txt (x: 1-5)
*/

void Stage::inFile() {
	char lv = level + 48; 					//获取关卡的文件名 
	string maze = "maze/Maze_";
	string suffix = ".txt";
	maze.push_back(lv); 					
	maze.append(suffix);
	
	ifstream infile;						//打开关卡文件 
	infile.open(maze.c_str(), ios::in);
	
	int l;
	int w;
	int t;
	infile >> l >> w >> t; 					//读取长度宽度和限定时间 
	
	Length = l;
	Width = w;
	limit = t;
	
	char temp;
	for (int i=0; i<w+2; i++) 				//初始化坐标点类型 
	{
		for (int j=0; j<w+2; j++)
		{
			infile >> temp;
			Poi[i][j].flag = temp - 48; 	//转换为值相同的整型
		}
	}
	
	for (int i=0; i<MAX; i++) 				//初始化坐标、访问变量 
	{
		for (int j=0; j<MAX; j++) 
		{
			Poi[i][j].x = i;
			Poi[i][j].y = j;

			Poi[i][j].pre_x = -1;			//将前驱结点设置为-1 
			Poi[i][j].pre_y = -1;
			Poi[i][j].visited = 0;
		}
	}
}



/*
	获取最短路径
	广度优先，使用队列实现 
	路径的坐标点类型将变为4，即显示为· 
*/
int Stage::MinPath() {
	int flag;
	flag = 0;
	
	int s_x; //起点x 
	int s_y; //起点y 
	int o_x; //终点x 
	int o_y; //终点y 

	s_x = Length/2+1;
	s_y = Width/2+1;
	o_x = Length-1;
	o_y = Width-1;

	EnQueue(Poi[s_x][s_y]); //起点入队列
	Poi[s_x][s_y].visited = 1; //访问过该点
    
	int now_x; //记录当前的坐标 
	int now_y; 
	int next_x; //记录新得位置坐标 
	int next_y;
	int count = 0;
	while (!isQueEmpty() && !flag) {
		Point p;
		p = DeQueue();

		now_x = p.x;
		now_y = p.y;

		if (now_x == o_x && now_y == o_y) //找到终点 
		{
			flag = 1;
		} 
		else 
		{
			for (int i=0; i<4; i++)  //上、右、下、左 顺序获取方向
			{
				switch (i) {
					case 0: //上 x-1
						next_x = now_x - 1;
						next_y = now_y;
						break;
					case 1: //右 y+1
						next_x = now_x;
						next_y = now_y + 1;
						break;				
					case 2: //下 x+1
						next_x = now_x + 1;
						next_y = now_y;
						break;
					case 3: //左 y-1
						next_x = now_x;
						next_y = now_y - 1;
						break;
				}
				
				// 如果判断获取的方向的点是空的并且未被访问，或者获取的方向的点是终点，则入队 
				if ((Poi[next_x][next_y].flag==0 && Poi[next_x][next_y].visited==0) || (next_x==o_x && next_y==o_y)) 
				{
					EnQueue(Poi[next_x][next_y]);			//将该坐标点入队
					
					Poi[next_x][next_y].pre_x = now_x; 		//记录该坐标点来时的坐标点 
					Poi[next_x][next_y].pre_y = now_y;
					
					Poi[next_x][next_y].visited = 1;		//设置该坐标点为访问过 
				}
			}
		}
	}
	
	if (flag) 											//只有找到终点才进行回溯 
	{
		now_x = Poi[o_x][o_y].pre_x; 					//将坐标定位为粮仓的前驱结点开始回溯 
		now_y = Poi[o_x][o_y].pre_y;
		
		while (Poi[now_x][now_y].x!=s_x || Poi[now_x][now_y].y!=s_y) //循环设置路径
		{
			Poi[now_x][now_y].flag = 4; //设置为可视化路径 
			
			int x = now_x;
			int y = now_y;
			now_x = Poi[x][y].pre_x;
			now_y = Poi[x][y].pre_y;
	//		now_x = Poi[now_x][now_y].pre_x; 
	//		now_y = Poi[now_x][now_y].pre_y; //第二行使用的是第一行改变后的now_x 会导致拐角无路径问题 
		}	
	}
	
	return flag;
}



/*
	队列相关操作 
	入队、出队、判空 
*/
void Stage::EnQueue(Point& poi) {  
	if ((rear+1)%MAX != front) {
		rear = (rear+1)%MAX;
		PoiQueue[rear] = poi;
	}
}

Point Stage::DeQueue() {
	if (front != rear) {
		front = (front+1)%MAX;
		return PoiQueue[front];
	}
}

int Stage::isQueEmpty() {
	int flag;
	if (front == rear) {
		flag = 1; 
	} else {
		flag = 0;
	}

	return flag;
}



/*
	将光标移动至x,y
	x,y换算为迷宫对应x,y 
*/

void Stage::GotoXY(int x, int y) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = y*2 + 2; //转换为地图的x,y 
    pos.Y = x;
    SetConsoleCursorPosition(handle, pos);
}



/* 
	隐藏光标 
*/
void HideCursor()
{	
	CONSOLE_CURSOR_INFO cursor_info = {1, 0};  // 第二个值为0表示隐藏光标
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}
