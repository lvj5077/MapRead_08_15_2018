#include <fstream>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <typeinfo>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <opencv2/imgproc/imgproc.hpp> // if use cvtColor

using namespace std;
using namespace cv;

vector<Point> navigationManual (Mat I_in, vector<Point> pt_list);
vector<Point> navigationSimple (Mat I_in, Point pt_a, Point pt_b);

Mat annotate_map(Mat I_in,vector<Point> direction);
Mat trimMap(Mat I_map,vector<Point> direction,int rt_margin);