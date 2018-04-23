/*
 * 用于去掉车牌上下左右的区域
 */
#ifndef SWIFTPR_FINEMAPPING_H
#define SWIFTPR_FINEMAPPING_H

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <string>

#include "util.h"

namespace pr {

    class FineMapping {
    private:
        cv::dnn::Net net;

    public:
        FineMapping();

        FineMapping(std::string prototxt, std::string caffemodel);

        /**
         * @brief FitLineRansac 拟合直线随机抽样一致算法
         * @param pts
         * @param zeroadd
         * @return
         */
        std::pair<int, int> FitLineRansac(std::vector<cv::Point> pts, int zeroadd = 0);

        /**
         * @brief FineMappingVertical 获得车牌上下边界
         * @param InputProposal 车牌图片
         * @param sliceNum 切片次数
         * @param upper 上限
         * @param lower 下限
         * @param windows_size 窗口大小
         * @return 剪切后的车牌图像
         */
        cv::Mat FineMappingVertical(cv::Mat InputProposal,
                                    int sliceNum = 15,
                                    int upper = 0, int lower = -50,
                                    int windows_size = 17);

        /**
         * @brief FineMappingHorizon 去掉左右边界
         * @param FinedVertical
         * @param leftPadding
         * @param rightPadding
         * @return
         */
        cv::Mat FineMappingHorizon(cv::Mat FinedVertical,
                                   int leftPadding, int rightPadding);

    };

}
#endif //SWIFTPR_FINEMAPPING_H
