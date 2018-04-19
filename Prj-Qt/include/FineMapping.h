//
// Created by 庾金科 on 22/09/2017.
//

#ifndef SWIFTPR_FINEMAPPING_H
#define SWIFTPR_FINEMAPPING_H

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <string>

#include "util.h"

namespace pr {

class FineMapping{
private:
    cv::dnn::Net net;

public:
    FineMapping();
    FineMapping(std::string prototxt, std::string caffemodel);

    /**
     * @brief FineMappingVertical 获得车牌上下边界
     * @param InputProposal 车牌图片
     * @param sliceNum
     * @param upper
     * @param lower
     * @param windows_size
     * @return 剪切后的车牌图像
     */
    static cv::Mat FineMappingVertical(cv::Mat InputProposal,
                                       int sliceNum=15,
                                       int upper=0, int lower=-50,
                                       int windows_size=17);

    cv::Mat FineMappingHorizon(cv::Mat FinedVertical,
                               int leftPadding, int rightPadding);

};

}
#endif //SWIFTPR_FINEMAPPING_H
