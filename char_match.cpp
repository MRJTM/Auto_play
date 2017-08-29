#include "char_match.h"

//A,B位比较相似度的两幅图片，sim为求出来的相似度
double calculate_similarity(Mat A, Mat B)
{
	//利用SURF检测器提取特征点
	Ptr<Feature2D> surf = xfeatures2d::SURF::create();
	std::vector<KeyPoint>KeyPointsA, KeyPointsB;
	surf->detect(A, KeyPointsA);
	surf->detect(B, KeyPointsB);

	//计算特征向量
	Mat descrpA, descrpB;
	surf->compute(A, KeyPointsA, descrpA);
	surf->compute(B, KeyPointsB, descrpB);

	//使用暴力匹配方法，寻找特征点匹配对
	BFMatcher matcher;
	//FlannBasedMatcher matcher;
	vector<DMatch>matches;
	matcher.match(descrpA, descrpB, matches);

	//将matches中的特征对按照距离从小到大排序
	sort(matches.begin(), matches.end());

	//计算相似程度
	int num_matches = 3;
	double score_similarity = 0;
	for (int j = 0; j < num_matches; j++)
	{
		score_similarity += matches[j].distance;
	}

	return score_similarity*10/ num_matches;
}

//导入字库
void load_char_lib(Mat char_lib[])
{
	char_lib[0] = imread("r_general.png", 0);	//红——帅
	char_lib[1] = imread("r_advisor.png", 0);	//红——仕
	char_lib[2] = imread("r_elephant.png", 0);	//红——相
	char_lib[3] = imread("r_chariot.png", 0);	//红——車
	char_lib[4] = imread("r_horse.png", 0);		//红——馬
	char_lib[5] = imread("r_cannon.png", 0);	//红——炮
	char_lib[6] = imread("r_solider.png", 0);	//红——兵

	char_lib[7] = imread("b_general.png", 0);	//黑——将
	char_lib[8] = imread("b_advisor.png", 0);	//黑——士
	char_lib[9] = imread("b_elephant.png", 0);	//黑——象
	char_lib[10] = imread("b_chariot.png", 0);	//黑——车
	char_lib[11] = imread("b_horse.png", 0);	//黑——马
	char_lib[12] = imread("b_cannon.png", 0);	//黑——砲
	char_lib[13] = imread("b_solider.png", 0);	//黑——卒

	//调整字库的大小
	int char_lib_size =60;
	for (int i = 0; i < 14; i++)
	{
		resize(char_lib[i], char_lib[i], Size(60,60));
	}
	
}