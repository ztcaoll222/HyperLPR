//
// Created by 庾金科 on 16/10/2017.
//
#ifndef SWIFTPR_PLATESEGMENTATION_H
#define SWIFTPR_PLATESEGMENTATION_H

#include "base.h"
#include "PlateInfo.h"
#include "util.h"

namespace pr {

    class PlateSegmentation {

    private:
        cv::dnn::Net net;
    public:
        const int PLATE_NORMAL = 6;
        const int PLATE_NORMAL_GREEN = 7;
        const int DEFAULT_WIDTH = 20;

        PlateSegmentation(std::string phototxt,std::string caffemodel);
        PlateSegmentation();

        void segmentPlatePipline(PlateInfo &plateInfo,int stride,std::vector<cv::Rect> &Char_rects);

        void segmentPlateBySlidingWindows(cv::Mat &plateImage,int windowsWidth,int stride,cv::Mat &respones);

        void templateMatchFinding(const cv::Mat &respones,int windowsWidth,std::pair<float,std::vector<int>> &candidatePts);

        void refineRegion(cv::Mat &plateImage,const std::vector<int> &candidatePts,const int padding,std::vector<cv::Rect> &rects);

        void ExtractRegions(PlateInfo &plateInfo,std::vector<cv::Rect> &rects);

        cv::Mat classifyResponse(const cv::Mat &cropped);
    };

} // namespace pr

#endif //SWIFTPR_PLATESEGMENTATION_H
