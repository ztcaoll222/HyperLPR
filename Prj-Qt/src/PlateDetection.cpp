//
// Created by 庾金科 on 20/09/2017.
//
#include "include/PlateDetection.h"
#include "include/util.h"

namespace pr{

PlateDetection::PlateDetection(std::string filename_cascade)
{
    cascade.load(filename_cascade);
}

void PlateDetection::LoadModel(std::string filename_cascade)
{
    cascade.load(filename_cascade);
}

void PlateDetection::plateDetectionRough(cv::Mat InputImage, std::vector<pr::PlateInfo> &plateInfos, int min_w, int max_w)
{
    // 灰度图
    cv::Mat processImage;
    cv::cvtColor(InputImage, processImage, cv::COLOR_BGR2GRAY);

    std::vector<cv::Rect> platesRegions;

    // 因为车牌的高宽是固定的, 所以只要确定了宽就可以确定高度, 比例大概为4:1
    // Size(int width, int height)
    cv::Size minSize(min_w, min_w/4);
    cv::Size maxSize(max_w, max_w/4);

//    processImage--待检测图片
//    platesRegions--被检测物体的矩形框向量组；
//    1.1--表示在前后两次相继的扫描中，搜索窗口的比例系数。默认为1.1即每次搜索窗口依次扩大10%;
//    3--表示构成检测目标的相邻矩形的最小个数(默认为3个)。
//      如果组成检测目标的小矩形的个数和小于 min_neighbors - 1 都会被排除。
//      如果 min_neighbors 为 0, 则函数不做任何操作就返回所有的被检候选矩形框，
//      这种设定值一般用在用户自定义对检测结果的组合程序上；
//    cv::CASCADE_SCALE_IMAGE：flags
//    minSize, maxSize：用来限制得到的目标区域的范围。
    cascade.detectMultiScale(processImage, platesRegions,
                             1.1, 3, cv::CASCADE_SCALE_IMAGE, minSize, maxSize);

    for(cv::Rect plate:platesRegions) {
//        util::showMat(processImage, plate);

        // 适当扩大范围, 不清楚为什么要这么定数值
        int zeroadd_x = static_cast<int>(plate.width * 0.14);
        int zeroadd_y = static_cast<int>(plate.height * 0.6);
        int zeroadd_w  = static_cast<int>(plate.width * 0.28);
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
