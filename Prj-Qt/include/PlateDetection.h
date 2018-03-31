//
// Created by 庾金科 on 20/09/2017.
//

#ifndef SWIFTPR_PLATEDETECTION_H
#define SWIFTPR_PLATEDETECTION_H

#include <opencv2/opencv.hpp>
#include "include/PlateInfo.h"
#include <vector>
namespace pr{

class PlateDetection {
private:
    cv::CascadeClassifier cascade;

public:
    PlateDetection();
    /**
     * @brief PlateDetection::PlateDetection 加载 haar 分类器
     * @param filename_cascade haar 分类器路径
     */
    PlateDetection(std::string filename_cascade);
    void LoadModel(std::string filename_cascade);

    /**
     * @brief PlateDetection::plateDetectionRough 检测车牌位置
     * @param InputImage 输入的图片
     * @param plateInfos 检测到的车牌位置和图像
     * @param min_w 车牌最小的宽度
     * @param max_w 车牌最大的宽度
     */
    void plateDetectionRough(cv::Mat InputImage,std::vector<pr::PlateInfo> &plateInfos,int min_w=36,int max_w=800);
};

}// namespace pr

#endif //SWIFTPR_PLATEDETECTION_H
