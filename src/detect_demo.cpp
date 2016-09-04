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
