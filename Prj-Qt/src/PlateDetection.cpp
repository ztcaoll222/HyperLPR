//
// Created by 庾金科 on 20/09/2017.
//
#include "include/PlateDetection.h"

namespace pr {

    PlateDetection::PlateDetection(std::string filename_cascade) {
        cascade.load(filename_cascade);
    }

    void PlateDetection::LoadModel(std::string filename_cascade) {
        cascade.load(filename_cascade);
    }

    void
    PlateDetection::plateDetectionRough(cv::Mat InputImage, std::vector<pr::PlateInfo> &plateInfos,
                                        int min_w, int max_w) {
        // 灰度图
        cv::Mat processImage;
        cv::cvtColor(InputImage, processImage, cv::COLOR_BGR2GRAY);

        std::vector<cv::Rect> platesRegions;

        // 因为车牌的高宽是固定的, 所以只要确定了宽就可以确定高度, 比例大概为4:1
        // Size(int width, int height)
        cv::Size minSize(min_w, min_w / 4);
        cv::Size maxSize(max_w, max_w / 4);

        // 检测车牌位置
        cascade.detectMultiScale(processImage, platesRegions,
                                 1.1, 3, cv::CASCADE_SCALE_IMAGE, minSize, maxSize);

        for (cv::Rect plate:platesRegions) {
//        util::showMat(processImage, plate);

            // 适当扩大范围
            int zeroadd_x = static_cast<int>(plate.width * 0.14);
            int zeroadd_y = static_cast<int>(plate.height * 0.6);
            int zeroadd_w = static_cast<int>(plate.width * 0.28);
            int zeroadd_h = static_cast<int>(plate.height * 1.2);
            plate.x -= zeroadd_x;
            plate.y -= zeroadd_y;
            plate.height += zeroadd_h;
            plate.width += zeroadd_w;

//        util::showMat(processImage, plate);

            // 切图
            cv::Mat plateImage = util::cropFromImage(InputImage, plate);

//        util::showMat(plateImage, plate);

            PlateInfo plateInfo(plateImage, plate);
            plateInfos.push_back(plateInfo);
        }
    }

}//namespace pr
