#include "Game.h"
#include <iostream>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <fstream>

using namespace std;

/*
	��Ϸ������
	���������ء����ԡ��༭��ͼ���˳� 
*/
void Menu() 
{
	cout << "����������������������" << endl;
	cout << "   �������Թ�С��Ϸ" << endl;
	cout << "����������������������" << endl;
	cout << "     1.��ʼ����" << endl;
	cout << "     2.ͨ�ع���" << endl;
	cout << "     3.�༭�Թ�" << endl;
	cout << "     4.�˳���Ϸ" << endl;
	cout << "����������������������" << endl;
	cout << "����ѡ�� >> ";
	
	int flag;
	cin >> flag;
	if (flag>0 && flag<5) 
	{
		system("cls");
		if (flag == 1) 							//���� 
		{
			system("cls");
			cout << endl << "  * ʹ�÷�������������ƶ�����" << endl;
			cout << "  * ���������ҵ����ּ�Ϊͨ��" << endl;
			cout << "  * ͨ���ؿ�����ʱ��Խ�̻���Խ��" << endl << endl;
			system("pause");
			system("cls");
			
			HideCursor();						//���ع�� 
			int score; 							//��ʼ������Ϊ0 
			score = 0;
			for (int i=0; i<MAX_LEVEL+1 && flag==1; i++) 
			{
				if (i == MAX_LEVEL) 			//ͨ���ж� 
				{
					system("cls");
					cout << endl << "* ��ϲ��ͨ�����йؿ�" << endl; 
					cout << "* ���֣� " << score << endl << endl;
					system("pause");
				}
				else 
				{
					Stage stage(i+1);
					stage.inFile();
					stage.PrintMap();
					int solved = stage.MoveEvent(score); 	//�ж��Ƿ�ͨ��
					
					flag = solved; 
					if (solved) 
					{
						cout << "  �Ƿ��������(1:�� 2:��) >> ";                   
						cin >> flag;
					}
					system("cls");
				}
			}
		}
		else if (flag == 2) 
		{
			HideCursor(); 							//���ع�� 
			cout << "��������鿴�ؿ���ͨ�ع���(1-5) >> ";
			cin >> flag;
			if (flag>0 && flag <6) 
			{
				system("cls");
				Stage stage(flag);
				stage.inFile();
				stage.MinPath();
				stage.PrintMap();
				cout << "  * �ؿ���" << flag << endl << endl;
			}
			system("pause");
		}
		else if (flag == 3)					//�༭�ؿ� 
		{
			cout << "��������༭�Ĺؿ��Ĺؿ�(1-5) >> ";
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
		else 								//�˳� 
		{
			exit(0);
		}
	} 
	else
	{
		cout << endl << "* ��������ȷ��ѡ�� *" << endl << endl;
		system("pause");
	}
	
	system("cls");
	Menu();
}



/*
	���캯�� 
	��ʼ��������������ؿ�������ָ�� 
*/

Stage::Stage(int lv) {
	level = lv; 									//��ʼ���ؿ��ȼ� 
	solved = 0;										//��ʼ��Ϊδͨ��
	
	front = -1; 									//��ʼ������ 
	rear = -1;
}



/* 
	��ӡ��ͼ 
	��ӡ��Χ 0-Length+1/Width+1
	����0��Length+1/Width+1 Ϊ�� 
 */
void Stage::PrintMap() 
{
	for (int i=0; i<Length+2; i++)
	{ 
		for (int j=0; j<Width+2; j++) 
		{
			int flag = Poi[i][j].flag;
			
			if (flag == 0) 		//�� 
			{
				cout << "  ";
			} 
			else if (flag == 1) //ǽ 
			{
				cout << "��";
			} 
			else if (flag == 2) //���� 
			{
				cout << "��";
			} 
			else if (flag == 3) //���� 
			{
				cout << "��"; 
			}
			else if (flag == 4) //�߹���·���� 
			{
				cout << "��";
			}
		}
		cout << endl;
	}
}



/*
	�ƶ��¼� 
	����ʼ��Ϸ�����ͨ������������������޶�ʱ�����߳��Թ� 
	
	* ͨ���ƶ������������ֹ��˸ 
*/
int Stage::MoveEvent(int& score)
{
	time_t t_start; 								//��ʼʱ�� 
	time_t t_now; 									//����ʱ�� 
	
	int timing = 0; 								//�ۼ�ʱ��
	
	int x = Length/2+1; 							//��������� x,y 
	int y = Width/2+1;
	
	t_start = time(NULL); 							//��¼��ʼʱ�� 
	cout << "  ʣ��ʱ�� " << limit << " �룬��ǰ�ؿ���" << level << endl;
	while((timing < limit) && (!solved)) 			//δ�����涨ʱ��������δͨ��
	{
		t_now = time(NULL);
		timing = t_now - t_start;
		
		if(!_kbhit())  								//û���û��¼��͸���ʱ�� 
		{
			GotoXY(Length+2, 4); 					//����ƶ���ʱ��ǰ�����޸� 
			cout << " " << limit - timing << " "; 	//�޶�ʱ���ȥ�ۼ�ʱ��	
		}
		
		if(_kbhit()) 								//���û��¼� 
		{
			char enter; 							//��ȡ�û��� 
			enter = getch();
			
			if(enter == -32) 						//�������һ����ȡ�������ַ�ASCII��ֵ��-32 
			{
				enter = getch(); 					//�ڶ������Ƿ������Ӧ��ASCII��ֵ
				 
				if(enter == 75) 		//�����y-1
				{
					if(Poi[x][y-1].flag != 1)
					{
						GotoXY(x, y-1); 			//�ƶ���꣬�ƶ����� 
						cout << "  ";
						GotoXY(x, y-2);
						cout << "��";
						
						Poi[x][y-1].flag = 2; 		//���������Ӧ���͸�Ϊ����
						Poi[x][y].flag = 4; 		//����Ϊ�߹���·����
						y = y - 1;					//������������
					}
				}
				else if(enter == 77)	 //�Ҽ� y+1
				{
					if(Poi[x][y+1].flag != 1)
					{
						GotoXY(x, y-1);
						cout << "  ";
						GotoXY(x, y);
						cout << "��";
						
						Poi[x][y+1].flag = 2;
						Poi[x][y].flag = 4;
						y =  y + 1;
					}	
				}
				else if(enter == 72) 	 //�ϼ���x-1
				{
					if(Poi[x-1][y].flag != 1)
					{
						GotoXY(x, y-1);
						cout << "  ";
						GotoXY(x-1, y-1);
						cout << "��";
						
						Poi[x-1][y].flag = 2;
						Poi[x][y].flag = 4;
						x = x - 1;
					}	
				} 
				else if(enter == 80) 	//�¼���x+1
				{
					if (Poi[x+1][y].flag != 1)
					{
						GotoXY(x, y-1); 
						cout << "  ";
						GotoXY(x+1, y-1);
						cout << "��";
						
						Poi[x+1][y].flag = 2;
						Poi[x][y].flag = 4;
						x = x + 1;
					}
				}
			}
			
			if(Poi[Length-1][Width-1].flag == 2) {			//���󵽴�ֿ� 
				solved = 1;
				score += limit - timing;
				system("cls");
				PrintMap();
				cout << "  * ���سɹ�����ǰ�ؿ���" << level << " ��ǰ���֣�" << score << endl << endl;
			}
		}
	}
	
	if (!solved) {
		system("cls");
		cout << endl << "  * ����ʧ�ܣ������³��ԣ�" << endl;
		cout << "  * �ؿ���" << level << endl;
		cout << "  * ���֣�" << score << endl << endl;
		system("pause");
	}
	
	return solved;
}



/*
	�༭�Թ� 
	ͨ��������ƶ���꣬ʹ�ÿո������ǽ· 
*/
void Stage::EditMap() 
{
	cout << "  * ʹ�÷�������������ƶ����" << endl;
	cout << "  * ʹ�ÿո������ǽ/·" << endl;
	cout << "  * ʹ��ESC���˳��༭" << endl;
	
	Stage stage = *this;
	
	int x = 2; 										//���λ�� ���ָ��λ���з����һ 
	int y = 1;
	GotoXY(2, 1); 									//��ʼ�����λ��
	
	int flag;
	flag = 1;
	while (flag) 
	{
		if(_kbhit()) 								//���û��¼� 
		{
			char enter; 							//��ȡ�û��� 
			enter = getch();
			
			if (enter == 32) {
				if (stage.Poi[x][y+1].flag == 0) {
					stage.Poi[x][y+1].flag = 1;		//������������ 
					cout << "��"; 					//���½�����ʾ 
					GotoXY(x, y);					//���λ�ñ��ֲ��� 
				}
				else if (stage.Poi[x][y+1].flag == 1) 
				{
					stage.Poi[x][y+1].flag = 0;
					cout << "  ";
					GotoXY(x, y);
				}
			}
			else if(enter == -32) 					//�������һ����ȡ�������ַ�ASCII��ֵ��-32 
			{
				enter = getch(); 					//�ڶ������Ƿ������Ӧ��ASCII��ֵ
				if(enter == 75) 					//�����y-1
				{
					if(y-1 > 0) 					//��ǽ�� 
					{
						GotoXY(x, y-1);
						y = y -1;
					}
				}
				else if(enter == 77) 				//�Ҽ� y+1
				{
					if(y+1 < Width-1)
					{
						GotoXY(x, y+1);	
						y =  y + 1;
					}	
				}
				else if(enter == 72) 				//�ϼ���x-1
				{
					if(x-1 > 1)
					{
						GotoXY(x-1, y);	
						x = x - 1;
					}	
				} 
				else if(enter == 80) 				//�¼���x+1
				{
					if (x+1 < Length)
					{
						GotoXY(x+1, y);	
						x = x + 1;
					}
				}
			}
			else if (enter == 27) 					//ESC���˳��༭ 
			{
				flag = 0;
			}
		}
	}
	
	if (stage.MinPath()) {							//���·������ͨ�� 
		system("cls");
		for (int i=0; i<MAX; i++)
  		{
			for (int j=0; j<MAX; j++) 
			{
				if (stage.Poi[i][j].flag == 4)		//���·���� 
				{
					stage.Poi[i][j].flag = 0;
				}
			}
		}
		
		stage.outFile(); 	 						//�༭�ɹ��������Թ���ͼ�ļ� 
		stage.PrintMap();
		cout << "  * �༭�ɹ�����ͼ�ѱ���" << endl << endl;
	} 
	else 											//���·��������ͨ�� 
	{
		system("cls");
		stage.PrintMap();
		cout << "  * �༭ʧ�ܣ��༭��ĵ�ͼ�޷�ͨ��" << endl << endl;
	}
	
	system("pause");
	system("cls");
	Menu();
}



/*
	�����Թ��ļ�
	���Թ��ļ����µ���Ӧ���ļ��� 
*/
void Stage::outFile() {
	char lv = level + 48; 					//��ȡ�ؿ����ļ��� 
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
	�ļ���ȡ����
	��ȡ��Ӧ�ؿ����Թ��ļ�
	Maze_x.txt (x: 1-5)
*/

void Stage::inFile() {
	char lv = level + 48; 					//��ȡ�ؿ����ļ��� 
	string maze = "maze/Maze_";
	string suffix = ".txt";
	maze.push_back(lv); 					
	maze.append(suffix);
	
	ifstream infile;						//�򿪹ؿ��ļ� 
	infile.open(maze.c_str(), ios::in);
	
	int l;
	int w;
	int t;
	infile >> l >> w >> t; 					//��ȡ���ȿ�Ⱥ��޶�ʱ�� 
	
	Length = l;
	Width = w;
	limit = t;
	
	char temp;
	for (int i=0; i<w+2; i++) 				//��ʼ����������� 
	{
		for (int j=0; j<w+2; j++)
		{
			infile >> temp;
			Poi[i][j].flag = temp - 48; 	//ת��Ϊֵ��ͬ������
		}
	}
	
	for (int i=0; i<MAX; i++) 				//��ʼ�����ꡢ���ʱ��� 
	{
		for (int j=0; j<MAX; j++) 
		{
			Poi[i][j].x = i;
			Poi[i][j].y = j;

			Poi[i][j].pre_x = -1;			//��ǰ���������Ϊ-1 
			Poi[i][j].pre_y = -1;
			Poi[i][j].visited = 0;
		}
	}
}



/*
	��ȡ���·��
	������ȣ�ʹ�ö���ʵ�� 
	·������������ͽ���Ϊ4������ʾΪ�� 
*/
int Stage::MinPath() {
	int flag;
	flag = 0;
	
	int s_x; //���x 
	int s_y; //���y 
	int o_x; //�յ�x 
	int o_y; //�յ�y 

	s_x = Length/2+1;
	s_y = Width/2+1;
	o_x = Length-1;
	o_y = Width-1;

	EnQueue(Poi[s_x][s_y]); //��������
	Poi[s_x][s_y].visited = 1; //���ʹ��õ�
    
	int now_x; //��¼��ǰ������ 
	int now_y; 
	int next_x; //��¼�µ�λ������ 
	int next_y;
	int count = 0;
	while (!isQueEmpty() && !flag) {
		Point p;
		p = DeQueue();

		now_x = p.x;
		now_y = p.y;

		if (now_x == o_x && now_y == o_y) //�ҵ��յ� 
		{
			flag = 1;
		} 
		else 
		{
			for (int i=0; i<4; i++)  //�ϡ��ҡ��¡��� ˳���ȡ����
			{
				switch (i) {
					case 0: //�� x-1
						next_x = now_x - 1;
						next_y = now_y;
						break;
					case 1: //�� y+1
						next_x = now_x;
						next_y = now_y + 1;
						break;				
					case 2: //�� x+1
						next_x = now_x + 1;
						next_y = now_y;
						break;
					case 3: //�� y-1
						next_x = now_x;
						next_y = now_y - 1;
						break;
				}
				
				// ����жϻ�ȡ�ķ���ĵ��ǿյĲ���δ�����ʣ����߻�ȡ�ķ���ĵ����յ㣬����� 
				if ((Poi[next_x][next_y].flag==0 && Poi[next_x][next_y].visited==0) || (next_x==o_x && next_y==o_y)) 
				{
					EnQueue(Poi[next_x][next_y]);			//������������
					
					Poi[next_x][next_y].pre_x = now_x; 		//��¼���������ʱ������� 
					Poi[next_x][next_y].pre_y = now_y;
					
					Poi[next_x][next_y].visited = 1;		//���ø������Ϊ���ʹ� 
				}
			}
		}
	}
	
	if (flag) 											//ֻ���ҵ��յ�Ž��л��� 
	{
		now_x = Poi[o_x][o_y].pre_x; 					//�����궨λΪ���ֵ�ǰ����㿪ʼ���� 
		now_y = Poi[o_x][o_y].pre_y;
		
		while (Poi[now_x][now_y].x!=s_x || Poi[now_x][now_y].y!=s_y) //ѭ������·��
		{
			Poi[now_x][now_y].flag = 4; //����Ϊ���ӻ�·�� 
			
			int x = now_x;
			int y = now_y;
			now_x = Poi[x][y].pre_x;
			now_y = Poi[x][y].pre_y;
	//		now_x = Poi[now_x][now_y].pre_x; 
	//		now_y = Poi[now_x][now_y].pre_y; //�ڶ���ʹ�õ��ǵ�һ�иı���now_x �ᵼ�¹ս���·������ 
		}	
	}
	
	return flag;
}



/*
	������ز��� 
	��ӡ����ӡ��п� 
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
	������ƶ���x,y
	x,y����Ϊ�Թ���Ӧx,y 
*/

void Stage::GotoXY(int x, int y) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = y*2 + 2; //ת��Ϊ��ͼ��x,y 
    pos.Y = x;
    SetConsoleCursorPosition(handle, pos);
}



/* 
	���ع�� 
*/
void HideCursor()
{	
	CONSOLE_CURSOR_INFO cursor_info = {1, 0};  // �ڶ���ֵΪ0��ʾ���ع��
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}
