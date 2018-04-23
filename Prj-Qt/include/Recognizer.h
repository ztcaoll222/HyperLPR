//
// Created by 庾金科 on 20/10/2017.
//
#ifndef SWIFTPR_RECOGNIZER_H
#define SWIFTPR_RECOGNIZER_H

#include "opencv2/dnn.hpp"

#include "PlateInfo.h"
#include "util.h"

namespace pr{
typedef cv::Mat label;
class GeneralRecognizer{
public:
    virtual label recognizeCharacter(cv::Mat character) = 0;
    void SegmentBasedSequenceRecognition(PlateInfo &plateinfo);

};

}
#endif //SWIFTPR_RECOGNIZER_H
