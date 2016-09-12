//
// Created by root on 16-9-6.
//

#include "sample_create.h"
#include <opencv/cv.hpp>
#include <iostream>
#include <dirent.h>

using namespace cv;
using namespace std;

//猫脸检测的类
CascadeClassifier faceCascade;

int cat_sample_create(const char* srcDir, const char* desDir, const char* cascade) {
    DIR *dirptr = NULL;
    struct dirent *entry;
    int count = 0;
    int div = 40;
    int cat_xstart, cat_ystart, cat_xend, cat_yend;
    char cutPath[128] = {0};

    faceCascade.load(cascade);   //加载分类器，注意文件路径
    Mat srcImg, grayImg, roiImg;
    vector<Rect> faces;
    vector<int> rejLevel;
    vector<double> levelW;

    printf("srcDir: %s\n", srcDir);
    printf("desDir: %s\n", desDir);
    if((dirptr = opendir(srcDir)) == NULL) {
        printf("open dir error!\n");
        return -1;
    }

    while(entry = readdir(dirptr)) {
        printf("entry->d_name: %s\n", entry->d_name);
        memset(cutPath, 0, sizeof(cutPath));
        snprintf(cutPath, sizeof(cutPath), "./%s/%s", srcDir, entry->d_name);
        srcImg = imread(cutPath);
        if(srcImg.empty()) {
            continue;
        }

        if(srcImg.channels() ==3) {
            cvtColor(srcImg, grayImg, CV_RGB2GRAY);
        } else {
            grayImg = srcImg;
        }

        equalizeHist(grayImg, grayImg);  //直方图均衡
        faceCascade.detectMultiScale(grayImg, faces, rejLevel, levelW, 1.1, 3, 0, Size(), Size(), true);   //检测人脸
        //faceCascade.detectMultiScale(grayImg, faces, rejLevel, levelW, 1.1, 3, 0, Size(20, 20), grayImg.size(), true);   //检测人脸
        if(faces.size()>0) {
            for(int i =0; i<faces.size(); i++) {
                /*if((rejLevel[i] < 0) || (levelW[i] < 5)) {
                    continue;
                }*/
                cout << "rejLevel: " << rejLevel[i] << endl;
                cout << "levelW: " << levelW[i] << endl;

                memset(cutPath, 0, sizeof(cutPath));
                snprintf(cutPath, sizeof(cutPath), "./%s/%d.bmp", desDir, count);
                printf("cutPath = %s\n", cutPath);
                cat_xstart = faces[i].x -div;
                if(cat_xstart < 0)
                    cat_xstart = 0;
                cat_ystart = faces[i].y -div;
                if(cat_ystart < 0)
                    cat_ystart = 0;
                cat_xend = faces[i].x + faces[i].width + div;
                if(cat_xend > grayImg.cols)
                    cat_xend = grayImg.cols;
                cat_yend = faces[i].y + faces[i].height + div;
                if(cat_yend > grayImg.rows)
                    cat_yend = grayImg.rows;
                roiImg = grayImg(Range(cat_ystart, cat_yend), Range(cat_xstart, cat_xend));
                imwrite(cutPath, roiImg);
                count++;
                //rectangle(img, Point(faces[i].x, faces[i].y), \
                                 Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height), \
                                 Scalar(0, 255, 0), 1, 8);    //框出人脸位置
            }
        } else {
            continue;
        }
    }
    return 0;
}