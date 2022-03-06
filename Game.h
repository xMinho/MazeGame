#ifndef GAME_H
#define GAME_H

const int MAX = 100;
const int MAX_LEVEL = 5;				

void Menu();							//������ 
void HideCursor(); 						//���ع�� 


/*
	�Թ������ṹ�� 
*/
struct Point 
{
	int x; 								//x����
	int y; 								//y����
	int pre_x; 							//��һ��·���������x 
	int pre_y; 							//��һ��·���������y
	int flag; 							//���������������� 0:�� 1:ǽ 2:���� 3:���� 4:����·����
	int visited; 						//���ʱ���
};



/*
	�ؿ��� 
*/
class Stage
{ 
	public:
		Stage(int lv);
		~Stage() {};
		
		void PrintMap(); 				//��ӡ�Թ� 
		void EditMap();					//�༭�Թ� 
		int MoveEvent(int& score); 		//�ƶ��¼� 				
		int MinPath();					//Ѱ��·��,���򷵻�1�����򷵻�0 

		void GotoXY(int x, int y);		//�ƶ�����̨��� 
		void outFile();					//���Թ�������ļ� 
		void inFile();					//���ļ��ж�ȡ�Թ� 

		void EnQueue(Point& poi); 		//��� 
		Point DeQueue();				//����
		int isQueEmpty();				//�п�
		Point PoiQueue[MAX]; 			//��ͼ�������  ��Ҫ����ѭ�����У�����ᵼ�¶��в�����
		
		Point Poi[MAX][MAX]; 			//��ͼ�������� 
	private:
		int front; 						//����ͷָ�� 
		int rear; 						//����βָ�� 
		
		int Length; 					//�Թ����� ����Ϊ�� 
		int Width; 						//�Թ���� ����Ϊ�� 
		
		int level; 						//��ǰ�Թ��ؿ��ȼ�
		int limit; 						//��ǰ�Թ��޶�ʱ�� 
		int solved; 					//�ж��Ƿ�ͨ�أ�ͨ��Ϊ1��δͨ��Ϊ0
}; 

#endif
