#include "position.h"
#include "char_match.h"

//每个棋子的名字
String chess_name[14] = { "r_general","r_advisor","r_elephant","r_chariot","r_horse","r_cannon","r_solider",
"b_general","b_advisor","b_elephant","b_chariot","b_horse","b_cannon","b_solider" };

//每个切片的名字
String piece_name[16] = { "a1.png","a2.png","a3.png","a4.png","a5.png","a6.png","a7.png",
"a8.png","a9.png","a10.png","a11.png","a12.png","a13.png","a14.png","a15.png","a16.png" };

//终点的X和Y坐标
int dst_X[32] = {0,  0,0,  0,0,  0,0,  0,0,  0,0,  0,	0, 0, 0, 0,
				 4,  30,-27,  58,-53,  109,-105,   83,-79,  82,-77,  105,  58,  2,  -52,  -100};
int dst_Y[32] = { 0,  0,0,  0,0,  0,0,  0,0,  0,0, 0,  0,  0,  0,  0, 
			  135,  135,135,  135,135,  135,135,  135,135,  176,180,  197, 197, 200, 200, 200};

//为每个棋子分配一个空位，总共32个空位，因为第二次识别到相同的棋子需要放到不同的位置才行
int key[32] = { 0,0,0,0,0,  0,0,0,0,0,  0,0,0,0,0, 0,0,0,0,0, 
				0,0,0,0,0,  0,0,0,0,0,  0,0 };


int calculate_position(int label, int key[])
{
	int LABEL = 0;
	if (label == 0)
		LABEL = label;
	else if (label < 7)
		LABEL = 2 * label - 1;
	else if (label == 7)
		LABEL = 16;
	else
		LABEL = 2 * label + 1;

	for (int i = 0; i < 4; i++)
	{
		if (key[LABEL] == 0)
		{
			key[LABEL] = 1;
			break;
		}
		else
			LABEL++;
	}
	return LABEL;
}

Point get_src_position(Point figure_position)
{
	Point src_position;
	float x = figure_position.x;
	float y = figure_position.y;
	src_position.y = (int)(-1.648e-5*x*x+0.2995*x+114.7);
	src_position.x = (int)(-1.933e-7*y*y*y+2.714e-4*y*y+0.2033*y-114.3);
	cout << "像素地址" << "x:" << figure_position.x;
	cout << " " << "y:" << figure_position.y << "   ";
	return src_position;
}

Point get_dst_position(int index)
{
	Point dst_position;
	dst_position.x = dst_X[index];
	dst_position.y = dst_Y[index];
	return dst_position;
}

void get_position(Mat src,vector<Vec3f> circles,Mat char_lib[],int circles_index,
					Point &src_position, Point &dst_position)
{
	/*获取棋子切片*/
	Mat piece[8] = {};
	int r = 30;
	piece[0] = src(Rect(circles[circles_index][0] - r, circles[circles_index][1] - r, 2 * r, 2 * r));

	/*调整切片的大小，方便进行匹配*/
	int r_piece = 60;//切片的边长

	Mat dst_piece = Mat::zeros(r_piece, r_piece, CV_8UC1);
	resize(piece[0], piece[0], dst_piece.size());

	imwrite(piece_name[circles_index], piece[0]);
	

	/*进行旋转获得8个方向的图片*/
	Point2f rotation_center(r_piece/2, r_piece/2);//旋转中心
	double angle = 45;		//旋转角度为45度
	for (int i = 1; i < 8; i++)
	{
		//获取旋转变换用的矩阵
		Mat rotation_array = getRotationMatrix2D(rotation_center, angle, 1.0);
		//通过仿射变换进行旋转
		warpAffine(piece[i - 1], piece[i], rotation_array, piece[i - 1].size());
	}
		
	/*用8个方向的切片去进行匹配，确定棋子是什么棋子*/
	int LABEL[14] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	//把切片的8个不同的方向都拿去识别一下，统计识别结果,放在LABEL[]中
	for (int i = 0; i < 8; i++)
	{
		double minsocre = 100;
		int tag = 0;
		for (int l = 7; l < 14; l++)//l从7开始：跳过与红棋的匹配过程，加快速度
		{		
			//计算相似度，score越小越相似
			double score = 10 * calculate_similarity(piece[i], char_lib[l]);
			//确定piece[i]识别为哪个棋子;
			if (score < minsocre)
			{
				minsocre = score;
				tag = l;
			}
		}
		LABEL[tag]++;
	}
	//统计8个切片最多被识别为哪个棋子，就认为切片是哪个棋子
	int label = 0;
	int largest = 0;
	for (int i = 0; i < 14; i++)
	{
		if (LABEL[i] > largest)
		{
			largest = LABEL[i];
			label = i;
		}
	}
	//输出识别结果
	cout << piece_name[circles_index] << ":识别为:" << chess_name[label] << "   ";


	/*识别出棋子后，确定该棋子的终点坐标*/
	int index = calculate_position(label, key);
	dst_position = get_dst_position(index);
	cout << "棋子编号:" << index << "  ";
	cout << "终点坐标：" << dst_position <<"   ";

	/*计算棋子的起点坐标*/
	Point center(cvRound(circles[circles_index][0]), cvRound(circles[circles_index][1]));
	src_position = get_src_position(center);
	cout << "起点坐标：" << src_position<< endl;
}



