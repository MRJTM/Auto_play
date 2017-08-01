#include "char_match.h"
#include "position.h"
#include "find_chess.h"


int main()
{
	/*------------ͨ������ͷ��ȡһ��ͼƬ-------------*/
	//Mat src;
	//VideoCapture inputVideo(0);

	//if (inputVideo.isOpened())//������ͷ�ɹ��򿪣��Ͷ�ȡһ��ͼƬ
	//{
	//	inputVideo >> src;
	//}
	//src = src(Rect(80,350, 1840, 1850));

	/*------------�����ֳ�ͼƬ���в���---------------*/
	Mat src = imread("����.jpg");
	imshow("�ĵ�������", src);
	

	/*-------------ɨ�����̣���ȡ����Ѱ�������ϵ�Բ---------------*/
	
	Mat mid;
	cvtColor(src, mid, COLOR_BGR2GRAY);
	vector<Vec3f> circles;
	circles = find_chess(mid);
	

	/*--------------ʶ�����ӣ���ȷ����ǰ������յ�����---------------*/
	//�����ֿ�
	Mat char_lib[14];
	load_char_lib(char_lib);

	//��������ÿ��Բ����һ�����ӣ�����Ҫʶ���ÿ��������ʲô
	double score = 0;
	Point src_position[32];	//��¼ÿ�����ӵ��������		
	Point dst_position[32]; //��¼ÿ�����ӵ��յ�����

	get_position(mid, circles, char_lib, src_position, dst_position);

	/*--------------��������յ����꣬ͨ�����������ƻ�е�۰���------------------*/

	waitKey(20170717);
	return 0;
}



