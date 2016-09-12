// face_detect.cpp : 定义控制台应用程序的入口点。
//
#include <opencv/cv.hpp>
#include <iostream>
#include <opencv/ml.h>
#include "sample_create.h"
#include "lbp.h"
#include <fstream>

using namespace cv;
using namespace std;
using namespace ml;

#if 1
#define CELLSIZE_LBP  16  // LBP的窗口大小，4,8，16

void lbp_svm(int pos_count, int neg_count, string p_dir, string n_dir, string classifier) {
    char pos_name[256];
    int total_count = pos_count + neg_count;
    vector<int> imageClass;
    cout << "//////////////////////////////////////////////////////////////////" << endl;
    cout << "totalSampleCount: " << total_count << endl;
    cout << "positiveSampleCount: " << pos_count << endl;
    cout << "negativeSampleCount: " << neg_count << endl;


    // 计算样本LBP特征向量矩阵和类别矩阵
    int lengthOfFeatureVector = (32 / CELLSIZE_LBP) * (64 / CELLSIZE_LBP) * 58; // 特征向量的维数
    Mat featureVectorOfSample(total_count, lengthOfFeatureVector, CV_32FC1); // 样本的特征向量矩阵
    Mat classOfSample(total_count, 1, CV_32SC1);
    vector<string>::size_type numberOfSample = pos_count;
    Mat srcImage;
    LBP lbp;
    vector<float> featureVector;
    int cnt = 1;

    //正样本
    for (vector<string>::size_type i = 0; i <= numberOfSample - 1; ++i) {
        // 读入图片
        memset(pos_name, '\0', 256 * sizeof(char));
        sprintf(pos_name, "%s/%d.bmp", p_dir.c_str(), cnt);
        srcImage = imread(pos_name, -1);

        // 计算样本LBP特征向量
        lbp.ComputerLBPFeatureVector(srcImage, Size(CELLSIZE_LBP, CELLSIZE_LBP), featureVector);
        //printf("i = %d\n", i);
        for (vector<float>::size_type j = 0; j <= lengthOfFeatureVector - 1; ++j) {
            //printf("featureVector[%d] =%f\n", j, featureVector[j]);
            featureVectorOfSample.at<float>(i, j) = featureVector[j];
        }
        classOfSample.at<int>(i, 0) = 1;
        cnt++;
    }

    //负样本
    numberOfSample = total_count;
    cnt = 1;
    for (vector<string>::size_type i = pos_count; i <= numberOfSample - 1; ++i) {
        // 读入图片
        memset(pos_name, '\0', 256 * sizeof(char));
        sprintf(pos_name, "%s/%d.bmp", n_dir.c_str(), cnt);
        //printf("pos_name = %s\n", pos_name);
        srcImage = imread(pos_name, -1);

        // 计算样本LBP特征向量
        lbp.ComputerLBPFeatureVector(srcImage, Size(CELLSIZE_LBP, CELLSIZE_LBP), featureVector);
        //printf("i = %d\n", i);
        for (vector<float>::size_type j = 0; j <= lengthOfFeatureVector - 1; ++j) {
            //printf("featureVector[%d] =%f\n", j, featureVector[j]);
            featureVectorOfSample.at<float>(i, j) = featureVector[j];
        }
        classOfSample.at<int>(i, 0) = -1;
        cnt++;
    }



    // 使用SVM分类器训练
    // 参数设置
    SVM::Params params;
    params.svmType = ml::SVM::C_SVC;
    params.kernelType = ml::SVM::LINEAR;
    params.termCrit = TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6);
    Ptr<SVM> svm = SVM::create(params);
    svm->train(featureVectorOfSample, ROW_SAMPLE, classOfSample);
    svm->save(classifier + "/Classifier.xml");
}

void lbp_detect(string t_dir, string classifier) {
    // 识别
    Mat testImage;
    vector<string>::size_type numberOfTestImage = t_dir.size();
    vector<float> featureVectorOfTestImage;
    int cnt = 1;
    int lengthOfFeatureVector = (32 / CELLSIZE_LBP) * (64 / CELLSIZE_LBP) * 58; // 特征向量的维数
    Mat _featureVectorOfTestImage(1, lengthOfFeatureVector, CV_32FC1);
    char line[512];
    ofstream resultOfPrediction("PredictResult.txt", ios::out);
    char pos_name[256];
    LBP lbp;

    SVM::Params params;
    params.svmType = ml::SVM::C_SVC;
    params.kernelType = ml::SVM::LINEAR;
    params.termCrit = TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6);
    Ptr<SVM> svm = SVM::create(params);
    svm->load<SVM>(classifier + "/Classifier.xml");
    printf("hello\n");

    // 注意将循环体内的耗时变量和操作提取到循环体内
    for (vector<string>::size_type i = 0; i <= numberOfTestImage-1; ++i) {
        memset(pos_name, '\0', 256 * sizeof(char));
        sprintf(pos_name, "%s/%d.bmp", t_dir.c_str(), cnt);

        testImage = imread(pos_name, -1);

        // 计算LBP特征向量
        lbp.ComputerLBPFeatureVector(testImage, Size(CELLSIZE_LBP, CELLSIZE_LBP), featureVectorOfTestImage);
        for (vector<float>::size_type j = 0; j <= featureVectorOfTestImage.size() - 1; ++j)
        {
            _featureVectorOfTestImage.at<float>(0, j) = featureVectorOfTestImage[j];
        }

        int predict = svm->predict(_featureVectorOfTestImage);
        sprintf(line, "%s %d\n", pos_name, predict);
        resultOfPrediction << line;
    }
    resultOfPrediction.close();
}
#endif

#if 0
void LBP_SVM_Rotation()
{
    // 读入训练样本路径和类别
    vector<string> imagePaths;
    vector<int> imageClass;
    string buffer;
    int numberOfLine = 0;
    ifstream file(string(PATH) + "TrainData.txt", ios::in);
    while (!file.eof())
    {
        if (getline(file, buffer))
        {
            ++numberOfLine;
            if (numberOfLine % 2 == 0)//读到样本类别
            {
                imageClass.push_back(atoi(buffer.c_str()));
            }
            else
            {
                imagePaths.push_back(buffer);

            }

        }

    }

    file.close();

    // 计算样本LBP特征向量矩阵和类别矩阵
    int lengthOfFeatureVector = (32 / CELLSIZE_LBP)*(64 / CELLSIZE_LBP) * 9; // 特征向量的维数
    Mat featureVectorOfSample(imagePaths.size(), lengthOfFeatureVector, CV_32FC1); // 样本的特征向量矩阵
    Mat classOfSample(imagePaths.size(), 1, CV_32SC1);
    vector<string>::size_type numberOfSample = imagePaths.size();
    Mat srcImage;
    LBP lbp;
    vector<float> featureVector;
    for (vector<string>::size_type i = 0; i <= numberOfSample - 1; ++i)
    {
        // 读入图片
        srcImage = imread(imagePaths[i], -1);

        // 计算样本LBP特征向量
        lbp.ComputerLBPFeatureVector_Rotation(srcImage, Size(CELLSIZE_LBP, CELLSIZE_LBP), featureVector);

        for (vector<float>::size_type j = 0; j <= lengthOfFeatureVector - 1; ++j)
        {
            featureVectorOfSample.at<float>(i, j) = featureVector[j];
        }

        classOfSample.at<int>(i, 0) = imageClass[i];
    }

    // 使用SVM分类器训练
    // 参数设置
    Ptr<SVM> svm = SVM::create();
    svm->setType(SVM::C_SVC);
    svm->setKernel(SVM::LINEAR);
    svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6));

    svm->train(featureVectorOfSample, ROW_SAMPLE, classOfSample);
    svm->save(string(PATH) + "Classifier.xml");

    // 使用训练好的分类器进行识别
    vector<string> testImagePath;
    ifstream testFile(string(PATH) + "TestData.txt", ios::in);// 注意要去掉最后一行的换行，否则最后一幅图片读出来就是空的
    while (!testFile.eof())
    {
        getline(testFile, buffer);
        testImagePath.push_back(buffer);
    }

    // 识别
    Mat testImage;
    vector<string>::size_type numberOfTestImage = testImagePath.size();
    vector<float> featureVectorOfTestImage;
    Mat _featureVectorOfTestImage(1, lengthOfFeatureVector, CV_32FC1);
    char line[512];
    ofstream resultOfPrediction(string(PATH) + "PredictResult.txt", ios::out);

    // 注意将循环体内的耗时变量和操作提取到循环体内
    for (vector<string>::size_type i = 0; i <= numberOfTestImage - 1; ++i)
    {
        testImage = imread(testImagePath[i], -1);

        // 计算LBP特征向量
        lbp.ComputerLBPFeatureVector_Rotation(testImage, Size(CELLSIZE_LBP, CELLSIZE_LBP), featureVectorOfTestImage);
        for (vector<float>::size_type j = 0; j <= featureVectorOfTestImage.size() - 1; ++j)
        {
            _featureVectorOfTestImage.at<float>(0, j) = featureVectorOfTestImage[j];
        }

        int predict = svm->predict(_featureVectorOfTestImage);
        sprintf(line, "%s %d\n", testImagePath[i].c_str(), predict);
        resultOfPrediction << line;
    }
    resultOfPrediction.close();
}
#endif

int main(int argc, char* argv[]) {
    /*if(argc != 4) {
        printf("Usage: ./bob srcDir desDir xx.xml\n");
        return -1;
    }*/
    cat_sample_create(argv[1], argv[2], argv[3]);
    //int p_count = atoi(argv[4]);
    //int n_count = atoi(argv[5]);
    //lbp_svm(p_count, n_count, argv[1], argv[2], argv[3]);
    //lbp_detect(argv[6], argv[2]);
    return 0;
}

