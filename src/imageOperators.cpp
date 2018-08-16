#include "imageOperators.h"

vector<Point> navigationManual (Mat I_in, vector<Point> pt_list){
    vector<Point> direction;
    for (int i=0; i<(pt_list.size());i++){
        direction.push_back(pt_list[i]);
    }

    return direction;
}





vector<Point> navigationSimple (Mat I_in, Point pt_a, Point pt_b){
    vector<Point> direction;
    direction.push_back(pt_a);
    direction.push_back(pt_b);

    // 我的想法是按着右墙半米走
    // 基本上按 pixel scale算的

    return direction;
}

Mat black_and_white(Mat I_in, double r){
    Mat I_bw  = I_in.clone(); 
    for(int i =0; i < I_bw.rows; i++){
        for(int j = 0; j <I_bw.cols; j ++){
            if(I_bw.at<uchar>(i,j)>100){
                I_bw.at<uchar>(i,j) = 255; // white
            }
            else{
                I_bw.at<uchar>(i,j) = 0;   // black
            }
        }
    }
    return I_bw;
}

Mat annotate_map(Mat I_in,vector<Point> direction){
    Mat annotated_map  = I_in.clone(); 
    int lineThickness = 2;

    for (int i=0; i<(direction.size()-1);i++){
        line(annotated_map, direction[i], direction[i+1], (0,255,255), lineThickness);
    }
    
    return annotated_map;
}

Mat trimMap(Mat I_map,vector<Point> direction,int rt_margin){
    Mat mask(I_map.size(),CV_8UC1);

    vector<Point> pts;
    
    int x,y;

    Point intial_dir;
    pts.push_back(direction[0]); 
    for (int i=0; i<(direction.size());i++){
        if (i<direction.size()-1){
            intial_dir = direction[i+1]-direction[i];

        }
        
        cout << "intial_dir"<<intial_dir <<endl;
        cout << "direction[i]"<<direction[i]<<endl;

        x = direction[i].x;
        y = direction[i].y;

        bool leftTurn = true;
        if(i>0){
            Point pre_dir = direction[i]-direction[i-1];
            double mag = ( pre_dir.x*intial_dir.y - pre_dir.y*intial_dir.x );
            double angle = abs (( pre_dir.x*intial_dir.x + pre_dir.y*intial_dir.y )/(norm(intial_dir)*norm(pre_dir)) );
            cout << "angle " <<i<< " "<< angle<<endl;
            if( angle <0.01 && mag >0){
            // if (i==1){
                leftTurn = false;
            }
            else{
                leftTurn = true;
            }
 
        }
        if (leftTurn){
            x = x-rt_margin*( intial_dir.y/norm(intial_dir)+(intial_dir.x/norm(intial_dir)) );
            y = y-rt_margin*( intial_dir.y/norm(intial_dir)-(intial_dir.x/norm(intial_dir)) );

        }
        else{
            x = x-rt_margin*( intial_dir.y/norm(intial_dir)+(intial_dir.x/norm(intial_dir)) );
            y = y+rt_margin*( intial_dir.y/norm(intial_dir)-(intial_dir.x/norm(intial_dir)) );

        }
     
        cout << "Point(x,y)"<<Point(x,y)<<endl<<endl;

        if (x>I_map.rows){
            x = I_map.rows;
        }
        if (x<0){
            x=0;
        }
        if (y<0){
            y=0;
        }
        if (y>I_map.cols){
            y=I_map.cols;
        }
        pts.push_back(Point(x,y)); 
    }

    pts.push_back(direction [direction.size()-1]); 

    vector<Point> pts_d;

    for (int i=0; i<(direction.size());i++){
        if (i<direction.size()-1){
            intial_dir = direction[i+1]-direction[i];

        }
        
        // cout << "intial_dir"<<intial_dir <<endl;
        // cout << "direction[i]"<<direction[i]<<endl;

        x = direction[i].x;
        y = direction[i].y;

        bool leftTurn = true;
        if(i>0){
            Point pre_dir = direction[i]-direction[i-1];
            double mag = ( pre_dir.x*intial_dir.y - pre_dir.y*intial_dir.x );
            double angle = abs (( pre_dir.x*intial_dir.x + pre_dir.y*intial_dir.y )/(norm(intial_dir)*norm(pre_dir)) );
            // cout << "angle " <<i<< " "<< angle<<endl;
            if( angle <0.01 && mag >0){
            // if (i==1){
                leftTurn = false;
            }
            else{
                leftTurn = true;
            }
 
        }
        if (leftTurn){
            x = x+rt_margin*( intial_dir.y/norm(intial_dir)+(intial_dir.x/norm(intial_dir)) );
            y = y+rt_margin*( intial_dir.y/norm(intial_dir)-(intial_dir.x/norm(intial_dir)) );

        }
        else{
            x = x+rt_margin*( intial_dir.y/norm(intial_dir)+(intial_dir.x/norm(intial_dir)) );
            y = y-rt_margin*( intial_dir.y/norm(intial_dir)-(intial_dir.x/norm(intial_dir)) );

        }
     
        cout << "Point(x,y)"<<Point(x,y)<<endl<<endl;

        if (x>I_map.rows){
            x = I_map.rows;
        }
        if (x<0){
            x=0;
        }
        if (y<0){
            y=0;
        }
        if (y>I_map.cols){
            y=I_map.cols;
        }
        pts_d.push_back(Point(x,y)); 
    }

    for (int i=0; i<(pts_d.size());i++){
        pts.push_back( pts_d[pts_d.size()-1-i] ); ;
    }

    pts.push_back(direction[0]); 

    Mat I_ct = I_map.clone();
    for (int i=0; i<(pts.size()-1);i++){
        line(I_ct, pts[i], pts[i+1], (0,255,255), 2);
    }
    for (int i=0; i<(direction.size()-1);i++){
        line(I_ct, direction[i], direction[i+1], (255,0,0), 2);
    }
    // imshow ( "contours", I_ct );

    vector<vector<Point> > contours;
    contours.push_back(pts); 
    drawContours(mask,contours,0,Scalar(255),-1);
    Mat trimmed_map(I_map.size(),CV_8UC3,Scalar(255,255,255));
    I_map.copyTo(trimmed_map,mask);

    // imshow( "mask", mask );
    // imshow( "trimmed_map", trimmed_map );

    return trimmed_map;
}



















