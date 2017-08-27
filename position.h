#include "main.h"

int calculate_position(int label, int key[]);
void get_position(Mat src, vector<Vec3f> circles, Mat char_lib[], int circles_index,
					Point &src_position, Point &dst_position);
Point get_src_position(Point figure_position);
Point get_dst_position(int index);

