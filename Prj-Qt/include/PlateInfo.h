//
// Created by 庾金科 on 20/09/2017.
//
#ifndef SWIFTPR_PLATEINFO_H
#define SWIFTPR_PLATEINFO_H

#include <opencv2/opencv.hpp>

#include "util.h"

namespace pr {

typedef std::vector<cv::Mat> Character;

enum PlateColor { BLUE, YELLOW, WHITE, GREEN, BLACK,UNKNOWN};
enum CharType {CHINESE,LETTER,LETTER_NUMS};


class PlateInfo {

private:
    cv::Mat licensePlate;
    cv::Rect ROI;
    std::string name;
    PlateColor Type;

public:
    std::vector<std::pair<CharType,cv::Mat>> plateChars;
    std::vector<std::pair<CharType,cv::Mat>> plateCoding;

    float confidence = 0;

    PlateInfo(const cv::Mat &plateData, std::string plateName, cv::Rect plateRect, PlateColor plateType);
    PlateInfo(const cv::Mat &plateData, cv::Rect plateRect, PlateColor plateType);
    PlateInfo(const cv::Mat &plateData, cv::Rect plateRect);
    PlateInfo();

    cv::Mat getPlateImage();

    void setPlateImage(cv::Mat plateImage);

    cv::Rect getPlateRect();

    void setPlateRect(cv::Rect plateRect);

    cv::String getPlateName();

    void setPlateName(cv::String plateName);

    int getPlateType();

    void appendPlateChar(const std::pair<CharType,cv::Mat> &plateChar);

    void appendPlateCoding(const std::pair<CharType,cv::Mat> &charProb);

    std::string decodePlateNormal(std::vector<std::string> mappingTable);

};

}

#endif //SWIFTPR_PLATEINFO_H
