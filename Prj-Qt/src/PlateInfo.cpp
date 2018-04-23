#include "include/PlateInfo.h"

namespace pr {

PlateInfo::PlateInfo(const cv::Mat &plateData, std::string plateName, cv::Rect plateRect, PlateColor plateType)
{
    licensePlate = plateData;
    name = plateName;
    ROI = plateRect;
    Type = plateType;
}

PlateInfo::PlateInfo(const cv::Mat &plateData, cv::Rect plateRect, PlateColor plateType)
{
    licensePlate = plateData;
    ROI = plateRect;
    Type = plateType;
}

PlateInfo::PlateInfo(const cv::Mat &plateData, cv::Rect plateRect)
{
    licensePlate = plateData;
    ROI = plateRect;
}

PlateInfo::PlateInfo()
{

}

cv::Mat PlateInfo::getPlateImage()
{
    return licensePlate;
}

void PlateInfo::setPlateImage(cv::Mat plateImage)
{
    licensePlate = plateImage;
}

cv::Rect PlateInfo::getPlateRect()
{
    return ROI;
}

void PlateInfo::setPlateRect(cv::Rect plateRect)
{
    ROI = plateRect;
}

cv::String PlateInfo::getPlateName()
{
    return name;
}

void PlateInfo::setPlateName(cv::String plateName)
{
    name = plateName;
}

int PlateInfo::getPlateType()
{
    return Type;
}

void PlateInfo::appendPlateChar(const std::pair<CharType, cv::Mat> &plateChar)
{
    plateChars.push_back(plateChar);
}

void PlateInfo::appendPlateCoding(const std::pair<CharType, cv::Mat> &charProb)
{
    plateCoding.push_back(charProb);
}

std::string PlateInfo::decodePlateNormal(std::vector<std::string> mappingTable)
{
    std::string decode;
    for(auto plate: plateCoding) {
        float *prob = (float *)plate.second.data;
        if(plate.first == CHINESE) {
            decode += mappingTable[std::max_element(prob,prob+31) - prob];
            confidence+=*std::max_element(prob,prob+31);
        }

        if(plate.first == LETTER) {
            decode += mappingTable[std::max_element(prob + 41, prob + 65) - prob];
            confidence += *std::max_element(prob + 41, prob + 65);
        }

        if(plate.first == LETTER_NUMS) {
            decode += mappingTable[std::max_element(prob + 31, prob + 65) - prob];
            confidence += *std::max_element(prob + 31, prob + 65);
        }
    }

    name = decode;

    confidence /= 7;

    return decode;
}

}
