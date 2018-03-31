//
// Created by 庾金科 on 22/09/2017.
//
#ifndef SWIFTPR_FASTDESKEW_H
#define SWIFTPR_FASTDESKEW_H

#include <math.h>
#include <opencv2/opencv.hpp>

namespace pr {

    cv::Mat fastdeskew(cv::Mat skewImage, int blockSize);

}//namepace pr


#endif //SWIFTPR_FASTDESKEW_H
