#include "char_match.h"
#include "position.h"
#include "find_chess.h"
#include "board_adjust.h"
#include "USART.h"

extern HANDLE com;//串口指示变量
extern OVERLAPPED OverLapped;
Mat src;

int main()
{

	/*-----------------------打开配置摄像头，确保棋盘摆放位置正确---------------------*/
	Mat img;
	VideoCapture inputVideo(0);
	//设置摄像头的分辨率为1080*960
	inputVideo.set(CV_CAP_PROP_FRAME_WIDTH, 1080);
	inputVideo.set(CV_CAP_PROP_FRAME_HEIGHT, 960);
	if (inputVideo.isOpened())//若打开摄像头成功
	{
		cout << "打开摄像头成功" << endl;
		cout << "等待棋盘摆放，确保四个角点都能看见...按回车继续..." << endl;
		while (waitKey(30) != 13)
		{
			inputVideo >> img;
			imshow("拍到的棋盘", img);
		}

	}
	else	//若打开摄像头失败
	{
		cout << "打开摄像头失败" << endl;
		system("pause");
		return 1;
	}

	/*-------------------------------打开配置串口---------------------------------*/
	int flag_Usart = 0;
	flag_Usart = Open_Config_Usart("COM3", 115200);
	//若打开串口失败，就结束程序
	if(flag_Usart==0)
	{
		system("pause");
		return 1;
	}

	/*-------------------------------导入字库--------------------------------------*/
	Mat char_lib[14];
	load_char_lib(char_lib);
	cout << "导入字库成功" << endl;


	/*-----------------利用现成图片进行测试----------------------*/
	//Mat img = imread("Origin_board.jpg");
	//imshow("拍到的棋盘", img);
	//cout << "图像的尺寸是：" << img.cols << " " << img.rows << endl;

	/*----------------------------棋盘矫正，获得标准棋盘----------------------------*/
	int flag = 0;//判断是否矫正成功用的标志位
	src = board_adjust(img, &flag);
	if (flag == false)
	{
		return 1;
	}
	imshow("矫正的棋盘", src);



	/*-------------扫描棋盘，读取棋盘寻找棋盘上的圆---------------*/
	Mat mid;
	cvtColor(src, mid, COLOR_BGR2GRAY);
	vector<Vec3f> circles;
	bool flag_find_chess = false;
	flag_find_chess = find_chess(mid,circles);
	if (flag_find_chess==false)
	{
		cout << "寻找棋子失败" << endl;
		system("pause");
		return 1;
	}

	/*-------------切片识别棋子，发送坐标，再找圆，识别，发送的循环-----------------*/
	for(int i=0;i<circles.size();i++)
	{
		/*--------------识别棋子，并确定当前坐标和终点坐标---------------*/

		//识别棋子，计算起点终点坐标
		Point src_position;	//记录每个棋子的起点坐标		
		Point dst_position; //记录每个棋子的终点坐标
		get_position(mid, circles, char_lib,i, src_position, dst_position);

		/*------------根据起点终点坐标，通过串口来控制机械臂摆棋-----------*/
		bool flag_send=false;
		flag_send=Usart_Arm_Control(src_position, dst_position);
		//判断是否发送成功
		if (flag_send)
		{
			cout << "棋子" << i+1 << "坐标发送成功" << endl;
		}
		else
		{
			cout << "棋子" << i+1 << "坐标发送失败！！！" << endl;
		}

		/*--------------延时3s，给机械臂充足时间动作------------------------*/
		Sleep(4000);
	}


	cout << "按回车结束程序！....." << endl;
	while (waitKey(30) != 13);
	return 0;
}



