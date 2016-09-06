// face_detect.cpp : 定义控制台应用程序的入口点。
//
#include <opencv/cv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//人脸检测的类
CascadeClassifier faceCascade;

int main(int argc, char* argv[])
{
    faceCascade.load(argv[1]);   //加载分类器，注意文件路径

    Mat img = imread(argv[2]);
    Mat imgGray;
    vector<Rect> faces;

    if(img.empty())
    {
        return 1;
    }

    if(img.channels() ==3)
    {
        cvtColor(img, imgGray, CV_RGB2GRAY);
    }
    else
    {
        imgGray = img;
    }

    faceCascade.detectMultiScale(imgGray, faces, 1.1, 3, 0, Size(0, 0));   //检测人脸

    if(faces.size()>0)
    {
        for(int i =0; i<faces.size(); i++)
        {
            rectangle(img, Point(faces[i].x, faces[i].y), Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height),
                      Scalar(0, 255, 0), 1, 8);    //框出人脸位置
        }
    }

    imshow("FacesOfPrettyGirl", img);

    waitKey(0);
    return 0;
}
