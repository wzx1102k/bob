// face_detect.cpp : 定义控制台应用程序的入口点。
//
#include <opencv/cv.hpp>
#include <iostream>
#include "sample_create.h"

using namespace cv;
using namespace std;



int main(int argc, char* argv[]) {
    if(argc != 4) {
        printf("Usage: ./bob srcDir desDir xx.xml\n");
        return -1;
    }
    cat_sample_create(argv[1], argv[2], argv[3]);
    return 0;
}

#if 0
    #include <opencv/cv.hpp>
    #include <iostream>  
    using namespace std;  
    using namespace cv;  
       
    int main(int argc, const char* argv[])
    {  
        //string xmlPath = "../cat_detection.xml";  
		string xmlPath = argv[1];        
		CascadeClassifier ccf;  //创建分类器对象  
        //Mat img = imread("../catX.jpg");//目标检测图片  
		Mat img = imread(argv[2]);        
		if (!ccf.load(xmlPath))   //加载训练文件  
        {  
            cout << "Can not load the xml file." << endl;  
            return 0;  
        }  
        vector<Rect> faces; //创建一个容器保存检测出来的脸  
        Mat gray;  
        cvtColor(img, gray, CV_BGR2GRAY);//灰度图转化，提取Haar特征  
        equalizeHist(gray, gray);  //直方图均衡  
        //resize(gray, gray, Size(128, 128), 0, 0, CV_INTER_LINEAR);  
        ccf.detectMultiScale(gray, faces, 1.1, 3, 0, Size(125, 125), gray.size()); //检测脸,此处的Size(x,y)参数x，y检测前需调整到合适数值  
        for (vector<Rect>::const_iterator iter = faces.begin();iter!= faces.end();iter++)  
        {  
            rectangle(img, *iter, Scalar(0, 0, 255), 2, 8); //画出脸部矩形  
        }  
        imshow("Cat Faces", img);  
        waitKey(0);  
        return 0;  
    }  
#endif

#if 0
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <list>
#include <opencv/cv.hpp>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;


int main(int argc, const char* argv[] )
{
    String cascadeName = argv[1];//训练数据
    CascadeClassifier cascade;//创建级联分类器对象
    vector<Rect> rects;
    vector<Rect>::const_iterator pRect;

    double scale = 1.;
    Mat image;
    double t;
    if( !cascade.load( cascadeName ) )//从指定的文件目录中加载级联分类器
    {
        cerr << "ERROR: Could not load classifier cascade" << endl;
        return 0;
    }

        image = imread(argv[2]);
        if( !image.empty() )//读取图片数据不能为空
        {
            Mat gray, smallImg( cvRound (image.rows/scale), cvRound(image.cols/scale), CV_8UC1 );//将图片缩小，加快检测速度
            cvtColor( image, gray, CV_BGR2GRAY );//因为用的是类haar特征，所以都是基于灰度图像的，这里要转换成灰度图像
            resize( gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR );//将尺寸缩小到1/scale,用线性插值
            equalizeHist( smallImg, smallImg );//直方图均衡

            //detectMultiScale函数中smallImg表示的是要检测的输入图像为smallImg，rects表示检测到的目标序列，1.1表示
            //每次图像尺寸减小的比例为1.1，2表示每一个目标至少要被检测到3次才算是真的目标(因为周围的像素和不同的窗口大
            //小都可以检测到目标),CV_HAAR_SCALE_IMAGE表示不是缩放分类器来检测，而是缩放图像，Size(30, 30)为目标的
            //最小最大尺寸
            rects.clear();
            printf( "begin...\n");
            t = (double)cvGetTickCount();//用来计算算法执行时间
            cascade.detectMultiScale(smallImg,rects,1.1,2,0,Size(20,20),Size(30,30));
            //|CV_HAAR_FIND_BIGGEST_OBJECT//|CV_HAAR_DO_ROUGH_SEARCH|CV_HAAR_SCALE_IMAGE,

            t = (double)cvGetTickCount() - t;
            printf( "detection time = %g ms\n\n", t/((double)cvGetTickFrequency()*1000.) );
            for(pRect = rects.begin(); pRect != rects.end(); pRect++)
            {
                rectangle(image,cvPoint(pRect->x,pRect->y),cvPoint(pRect->x+pRect->width,pRect->y+pRect->height),cvScalar(0,255,0));
            }
            imwrite(argv[3],image);
        }

    return 0;
}
#endif
