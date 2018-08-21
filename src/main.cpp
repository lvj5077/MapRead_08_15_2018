#include <vector>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <typeinfo>
#include <stdio.h>

#include "loadPM.h"
#include "imageOperators.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <opencv2/imgproc/imgproc.hpp> // if use cvtColor

using namespace std;
using namespace cv;

Mat I_map,cloneimg;
bool mousedown;
vector<vector<Point> > contours;
vector<Point> pts;
string outputPath;
string windowName;

struct MouseParams
{
    Mat img;
    Point pt_a = Point(1,2);
    Point pt_b = Point(3,4);
    string window_name;
};


void onMouse_ROI( int event, int x, int y, int flags, void* userdata )
{
    Mat img = *((Mat *)userdata); 

    if( event == EVENT_LBUTTONDOWN )
    {
        mousedown = true;
        contours.clear();
        pts.clear();
    }

    if( event == EVENT_LBUTTONUP )
    {
        mousedown = false;
        if(pts.size() > 2 )
        {
            Mat mask(img.size(),CV_8UC1);
            //mask = 0;
            contours.push_back(pts);
            drawContours(mask,contours,0,Scalar(255),-1);
            // imshow( "mask", mask );

            cout << "pts" <<pts <<endl;
            Mat masked(img.size(),CV_8UC3,Scalar(255,255,255));
            I_map.copyTo(masked,mask);
            // I_map.copyTo(cloneimg);
            imshow( "masked", masked );
            string saveName = outputPath+"roi.png"; 
            imwrite( saveName, masked );
            // exit(0);
        }
    }

    if(mousedown)
    {
        if(pts.size() > 2 )
            line(img,Point(x,y),pts[pts.size()-1],Scalar(0,255,0));

        pts.push_back(Point(x,y));

        imshow( "Create Mask", img );        
    }

    // exit(0);
}

void onMouse_Line( int event, int x, int y, int flags, void* param)
{
    MouseParams* mp = (MouseParams*)param;
    Mat img = mp->img;
    windowName = mp->window_name;

    if( event == EVENT_LBUTTONDOWN )
    {
        mousedown = true;
        contours.clear();
        pts.clear();
        mp->pt_a = Point(x,y);
    }

    if( event == EVENT_LBUTTONUP )
    {
        mousedown = false;
    }

    if(mousedown)
    {
        if(pts.size() > 2 )
            line(img,Point(x,y),pts[pts.size()-1],Scalar(0,255,0));

        pts.push_back(Point(x,y));
        mp->pt_b = Point(x,y);

        imshow( windowName, img );        
    }
    // exit(0);
}

Mat readFrame( int index, string imagePath, string groupName, string Ext );
Mat readMatrix( string MaskString );

int main(int argc, char** argv)
{
    cout<<endl<<"Program Started!"<<endl;
    cout <<"~~~~~~~~~~~~~~~~~~"<<endl<<endl;


    string ParameterPath = "/home/jin/Lingqiu_Jin/MapRead_08_15_2018/parameters.txt";
    if(argc >=2){
    	ParameterPath = argv[1];
    }

    ParameterReader pd(ParameterPath);
    string inputMap = pd.getData( "input_Map" );
    outputPath = pd.getData( "output_Path" );

    string resultName;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//     clock_t timer;

//     timer = clock();
//     Mat I_org = cv::imread( inputMap );
//     Mat channel[3];  
//     split(I_org,channel); 
//     Mat I_r = channel[2];
//     Mat I_g = channel[1];
//     Mat I_b = channel[0];
    
//     Mat I_gray = 0.2989 * I_r + 0.5870 * I_g + 0.1140 * I_b ;

//     timer = (double)(clock() - timer)/CLOCKS_PER_SEC;
//     resultName = outputPath+"gray.png";
//     imwrite( resultName, I_gray );

//     printf("Run in %.4fs\n", timer);

//     // namedWindow( "Display window", WINDOW_AUTOSIZE );
//     // imshow( "Display window", I_gray ); 
//     // waitKey(0); 
// //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//     timer = clock();
//     Mat I_bw = black_and_white(I_gray, atof ( pd.getData("black_and_white_threshold").c_str() ));

//     timer = (double)(clock() - timer)/CLOCKS_PER_SEC;
//     resultName = outputPath+"bw.png";
//     imwrite( resultName, I_bw );

//     printf("Run in %.4fs\n", timer);

//     // namedWindow( "Display window", WINDOW_AUTOSIZE );
//     // imshow( "Display window", I_bw ); 
//     // waitKey(0); 
// //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // resultName = outputPath+"bw.png";
    // I_map = imread(resultName);
    // if(I_map.empty())
    // {
    //     return -1;
    // }

    // namedWindow("Create Mask", WINDOW_AUTOSIZE);
    // cloneimg = I_map.clone();
    // setMouseCallback( "Create Mask", onMouse_ROI, &cloneimg );
    // imshow( "Create Mask", I_map );

    // waitKey(0);
// //~~~~~~~~~~~~~~~~~~~这样 ROI 算是圈出来了~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//     resultName = outputPath+"roi.png";
//     I_map = imread(resultName);
//     if(I_map.empty())
//     {
//         return -1;
//     }

//     windowName = "Draw Line";
//     namedWindow(windowName, WINDOW_AUTOSIZE);

//     MouseParams mp;
//     mp.img = I_map.clone();

//     setMouseCallback( "Draw Line", onMouse_Line, &mp);
//     imshow( windowName, I_map );

//     waitKey(0);

//     cout << "mp.pt_a "<<mp.pt_a<<endl;
//     cout << "mp.pt_b "<<mp.pt_b<<endl;

//     double realDist;
//     cout << "Please enter the real distance of your last line: ";
//     cin >> realDist; // unit???

//     double map_scalar = realDist/ norm(mp.pt_a - mp.pt_b) ;

//     cout << "map_scalar " << map_scalar <<endl;

// //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // resultName = outputPath+"roi.png";
    // I_map = imread(resultName);
    // if(I_map.empty())
    // {
    //     return -1;
    // }

    // windowName = "Draw path";
    // namedWindow(windowName, WINDOW_AUTOSIZE);

    // MouseParams rt;
    // rt.window_name = windowName;
    // rt.img = I_map.clone();

    // setMouseCallback( windowName, onMouse_Line, &rt);
    // imshow( windowName, I_map );

    // waitKey(0);

    // cout << "mp.pt_a "<<rt.pt_a<<endl;
    // cout << "mp.pt_b "<<rt.pt_b<<endl;

    // cout << "start navigation "<<endl;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // for test purpose
    MouseParams rt;
    rt.pt_a = Point(467,333);
    rt.pt_b = Point(272,203);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    resultName = outputPath+"roi.png";
    I_map = imread(resultName);
    if(I_map.empty())
    {
        return -1;
    }

    vector<Point> pt_list = {rt.pt_a, Point(373,332), Point(374,166), Point(327,166), Point(308,157),rt.pt_b};
    // pt_list 可以根据图 手动标注的
    // 关键是A* 出图之后 都是极小的线段 不能很好的表示
    vector<Point> direction = navigationManual(I_map, pt_list);

    // vector<Point> Testdirection = navigationSimple(I_map, rt.pt_a,rt.pt_b,0.3);

    
                                                        // A* 对没有downsample的图 太智障了 有点慢
    Mat annotated_map = annotate_map(I_map,direction);    // 这一步其实可以 手工标记的 而且还比较 智能
    
    // namedWindow("Routine", WINDOW_AUTOSIZE);
    // imshow( "Routine", annotated_map );
    // resultName = outputPath+"annotated_map.png";
    // imwrite( resultName, annotated_map );
    // waitKey(0);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // int rt_margin = 10; // unit pixel | 这边是应该按实际距离Xscale 取整算出来的 先写个10 试试

    // Mat trimmed_map = trimMap(I_map,direction,rt_margin);
    // imshow( "trimmed_map", trimmed_map );
    // resultName = outputPath+"trimmed_map.png";
    // imwrite( resultName, trimmed_map );
    // waitKey(0);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // resultName = outputPath+"trimmed_map.png";
    
    // int erosion_size = 3;  
    // Mat eroded_map = imread(resultName);
    // Mat element = getStructuringElement(cv::MORPH_RECT,
    //       cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),
    //       cv::Point(erosion_size, erosion_size) );

    // erode(eroded_map,eroded_map,element);  
    // dilate(eroded_map,eroded_map,element);

    // namedWindow( "eroded_map", CV_WINDOW_AUTOSIZE );   
    // imshow( "eroded_map", eroded_map );
    // // resultName = outputPath+"eroded_map.png";
    // imwrite( resultName, eroded_map );
    // waitKey(0); 


    resultName = outputPath+"roi.png";
    
    int erosion_size = 3;  
    Mat eroded_map = imread(resultName);
    Mat element = getStructuringElement(cv::MORPH_RECT,
          cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),
          cv::Point(erosion_size, erosion_size) );

    erode(eroded_map,eroded_map,element);  
    dilate(eroded_map,eroded_map,element);

    namedWindow( "eroded_map", CV_WINDOW_AUTOSIZE );   
    imshow( "eroded_map", eroded_map );
    resultName = outputPath+"eroded_map_ROI.png";
    imwrite( resultName, eroded_map );
    waitKey(0); 


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//     Mat src = eroded_map.clone();
//     Mat dst, cdst;
//     Canny(src, dst, 20, 100, 3);
//     cvtColor(dst, cdst, CV_GRAY2BGR);

// // dst: Output of the edge detector. It should be a grayscale image (although in fact it is a binary one)
// // lines: A vector that will store the parameters (x_{start}, y_{start}, x_{end}, y_{end}) of the detected lines
// // rho : The resolution of the parameter r in pixels. We use 1 pixel.
// // theta: The resolution of the parameter \theta in radians. We use 1 degree (CV_PI/180)
// // threshold: The minimum number of intersections to “detect” a line
// // minLinLength: The minimum number of points that can form a line. Lines with less than this number of points are disregarded.
// // maxLineGap: The maximum gap between two points to be considered in the same line.


//     vector<Vec4i> lines;
//     HoughLinesP(dst, lines, 1, CV_PI/180, 50, 30, 18 );
//     for( size_t i = 0; i < lines.size(); i++ )
//     {
//         Vec4i l = lines[i];
//         line( cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 1, CV_AA);
//     }
    
//     imshow("source", src);
//     imshow("detected lines", cdst);
//     resultName = outputPath+"line_map.png";
//     imwrite( resultName, cdst );
//     waitKey(0);


    return 0;
}



// 暂时没啥用 以后批量读写的时候用的
Mat readFrame( int index, string imagePath, string groupName, string Ext ){

    stringstream ss; 
    if (index <10){
        ss<<imagePath<<groupName<<0<<index<<Ext;
    }
    else{
        ss<<imagePath<<groupName<<index<<Ext;
    }
    string filename;
    ss>>filename;
    ss.clear();
    Mat I_rgb;
    I_rgb = cv::imread( filename );
    // cout << "Loaded "<< filename <<endl;
    return I_rgb; 
}

Mat readMatrix( string MaskString ){

    // cout << "MaskString: " << MaskString<<endl;
    string extraStr = "{";
    int row = -1;
    while(MaskString.find(extraStr)!=string::npos){
        MaskString.replace(MaskString.find(extraStr),extraStr.length()," ");   
        row++;
    }
    extraStr = "}";
    while(MaskString.find(extraStr)!=string::npos){
        MaskString.replace(MaskString.find(extraStr),extraStr.length()," ");   
    }
    extraStr = ",";
    while(MaskString.find(extraStr)!=string::npos){
        MaskString.replace(MaskString.find(extraStr),extraStr.length()," ");   
    }

    // cout << "new MaskString: " << MaskString<<endl;
    stringstream ss(MaskString);
    vector<float> numbers;

    for(float i = 0; ss >> i; ) {
        numbers.push_back(i);
        // cout << i <<endl;
    }

    int col = numbers.size() / row;

    Mat Mask = Mat::zeros(row,col,CV_32FC1);

    int n =-1;
    for(int i =0; i < row; i++){
        for(int j = 0; j <col; j ++){
            n++;
            Mask.at<float>(i,j) = numbers[n];
        }
    }

    return Mask; 
}