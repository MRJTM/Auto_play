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
	/*------------通过摄像头拍取一张图片-------------*/
	Mat img;
	VideoCapture inputVideo(0);
	inputVideo.set(CV_CAP_PROP_FRAME_WIDTH, 1080);
	inputVideo.set(CV_CAP_PROP_FRAME_HEIGHT, 960);
	if (inputVideo.isOpened())//若摄像头成功打开，就读取一张图片
	{
		cout << "打开摄像头成功" << endl;
		inputVideo >> img;

		while (waitKey(30)!=13)
		{
			inputVideo >> img;
			imshow("拍到的棋盘", img);
		}

	}
	else
	{
		cout << "打开摄像头失败" << endl;
		system("pause");
		return 1;
	}
	cout << "图像的尺寸是：" << img.cols<<" "<<img.rows<< endl;
	imshow("拍到的棋盘", img);

	/*-----------------利用现成图片进行测试----------------------*/
	//Mat img = imread("Origin_board.jpg");
	////imshow("拍到的棋盘", img);
	//cout << "图像的尺寸是：" << img.cols << " " << img.rows << endl;

	/*---------------棋盘矫正，获得标准棋盘-----------------------*/
	int flag = 0;//判断是否矫正成功用的标志位
	src = board_adjust(img,&flag);
	if (flag == false)
	{
		return 1;
	}
	imshow("矫正的棋盘", src);

	/*-------------扫描棋盘，读取棋盘寻找棋盘上的圆---------------*/
	Mat mid;
	cvtColor(src, mid, COLOR_BGR2GRAY);
	vector<Vec3f> circles;
	circles = find_chess(mid);
	

	/*--------------识别棋子，并确定当前坐标和终点坐标---------------*/
	//导入字库
	Mat char_lib[14];
	load_char_lib(char_lib);

	//检测出来的每个圆代表一个棋子，我需要识别出每个棋子是什么
	double score = 0;
	Point src_position[32];	//记录每个棋子的起点坐标		
	Point dst_position[32]; //记录每个棋子的终点坐标
	get_position(mid, circles, char_lib, src_position, dst_position);

	/*------------根据起点终点坐标，通过串口来控制机械臂摆棋-----------*/
	/*打开和配置串口:打开串口2，波特率115200,设定要发送的棋子数，起点坐标，终点坐标*/
	int num_of_chess = circles.size();
	Usart_Arm_Control("COM3", 115200, num_of_chess, src_position, dst_position);

	while (waitKey(30) != 13);
	return 0;
}



