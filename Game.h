#ifndef GAME_H
#define GAME_H

const int MAX = 100;
const int MAX_LEVEL = 5;				

void Menu();							//主界面 
void HideCursor(); 						//隐藏光标 


/*
	迷宫坐标点结构体 
*/
struct Point 
{
	int x; 								//x坐标
	int y; 								//y坐标
	int pre_x; 							//上一个路径点坐标的x 
	int pre_y; 							//上一个路径点坐标的y
	int flag; 							//该坐标点的数据类型 0:空 1:墙 2:老鼠 3:粮仓 4:已走路径点
	int visited; 						//访问变量
};



/*
	关卡类 
*/
class Stage
{ 
	public:
		Stage(int lv);
		~Stage() {};
		
		void PrintMap(); 				//打印迷宫 
		void EditMap();					//编辑迷宫 
		int MoveEvent(int& score); 		//移动事件 				
		int MinPath();					//寻找路径,有则返回1，无则返回0 

		void GotoXY(int x, int y);		//移动控制台光标 
		void outFile();					//将迷宫输出到文件 
		void inFile();					//从文件中读取迷宫 

		void EnQueue(Point& poi); 		//入队 
		Point DeQueue();				//出队
		int isQueEmpty();				//判空
		Point PoiQueue[MAX]; 			//地图坐标队列  需要设置循环队列，否则会导致队列不够用
		
		Point Poi[MAX][MAX]; 			//地图坐标数组 
	private:
		int front; 						//队列头指针 
		int rear; 						//队列尾指针 
		
		int Length; 					//迷宫长度 上下为长 
		int Width; 						//迷宫宽度 左右为宽 
		
		int level; 						//当前迷宫关卡等级
		int limit; 						//当前迷宫限定时间 
		int solved; 					//判断是否通关，通关为1，未通关为0
}; 

#endif
