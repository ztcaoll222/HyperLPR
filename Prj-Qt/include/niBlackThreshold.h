//
// Created by 庾金科 on 26/10/2017.
//
#ifndef SWIFTPR_NIBLACKTHRESHOLD_H
#define SWIFTPR_NIBLACKTHRESHOLD_H

#include "base.h"
#include "util.h"

namespace pr {

    enum LocalBinarizationMethods {
        BINARIZATION_NIBLACK = 0, //!< Classic Niblack binarization. See @cite Niblack1985 .
        BINARIZATION_SAUVOLA = 1, //!< Sauvola's technique. See @cite Sauvola1997 .
        BINARIZATION_WOLF = 2,    //!< Wolf's technique. See @cite Wolf2004 .
        BINARIZATION_NICK = 3     //!< NICK technique. See @cite Khurshid2009 .
    };

    void niBlackThreshold(cv::InputArray _src, cv::OutputArray _dst, double maxValue,
                          int type, int blockSize, double k, int binarizationMethod);

} // namespace pr

#endif //SWIFTPR_NIBLACKTHRESHOLD_H
