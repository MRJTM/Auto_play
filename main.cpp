#include "char_match.h"
#include "position.h"
#include "find_chess.h"


int main()
{
	/*------------通过摄像头拍取一张图片-------------*/
	//Mat src;
	//VideoCapture inputVideo(0);

	//if (inputVideo.isOpened())//若摄像头成功打开，就读取一张图片
	//{
	//	inputVideo >> src;
	//}
	//src = src(Rect(80,350, 1840, 1850));

	/*------------利用现成图片进行测试---------------*/
	Mat src = imread("棋盘.jpg");
	imshow("拍到的棋盘", src);
	

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

	/*--------------根据起点终点坐标，通过串口来控制机械臂摆棋------------------*/

	waitKey(20170717);
	return 0;
}



