//
// Created by 庾金科 on 22/09/2017.
//
#ifndef SWIFTPR_FASTDESKEW_H
#define SWIFTPR_FASTDESKEW_H

#include "base.h"
#include "util.h"

namespace pr {

    const int ANGLE_MIN = 30;
    const int ANGLE_MAX = 150;
    const int PLATE_H = 36;
    const int PLATE_W = 136;

    int angle(float x, float y);

    std::vector<float> avgfilter(std::vector<float> angle_list, int windowsSize);

    void drawHist(std::vector<float> seq);

    cv::Mat correctPlateImage(cv::Mat skewPlate, float angle, float maxAngle);

    cv::Mat fastdeskew(cv::Mat skewImage, int blockSize);

} // namespace pr

#endif //SWIFTPR_FASTDESKEW_H
