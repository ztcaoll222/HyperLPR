//
// Created by 庾金科 on 22/10/2017.
//
#include "include/Recognizer.h"

namespace pr {

/**
 * @brief GeneralRecognizer::SegmentBasedSequenceRecognition 基于分段的序列识别
 * @param plateinfo
 */
    void GeneralRecognizer::SegmentBasedSequenceRecognition(PlateInfo &plateinfo) {
        for (auto char_instance:plateinfo.plateChars) {
            std::pair<CharType, cv::Mat> res;
            cv::Mat code_table = recognizeCharacter(char_instance.second);
//        util::showMat(code_table);
            res.first = char_instance.first;
            code_table.copyTo(res.second);
            plateinfo.appendPlateCoding(res);
        }
    }

}
