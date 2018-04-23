#include "include/niBlackThreshold.h"

namespace pr {

void niBlackThreshold(cv::InputArray _src, cv::OutputArray _dst, double maxValue,
                       int type, int blockSize, double k, int binarizationMethod) {
    // Input grayscale image
    cv::Mat src = _src.getMat();
    CV_Assert(src.channels() == 1);
    CV_Assert(blockSize % 2 == 1 && blockSize > 1);
    if (binarizationMethod == BINARIZATION_SAUVOLA) {
        CV_Assert(src.depth() == CV_8U);
    }
    type &= cv::THRESH_MASK;
    // Compute local threshold (T = mean + k * stddev)
    // using mean and standard deviation in the neighborhood of each pixel
    // (intermediate calculations are done with floating-point precision)
    cv::Mat test;
    cv::Mat thresh;
    {
        // note that: Var[X] = E[X^2] - E[X]^2
        cv::Mat mean, sqmean, variance, stddev, sqrtVarianceMeanSum;
        double srcMin, stddevMax;
        cv::boxFilter(src, mean, CV_32F, cv::Size(blockSize, blockSize),
                  cv::Point(-1,-1), true, cv::BORDER_REPLICATE);
        sqrBoxFilter(src, sqmean, CV_32F, cv::Size(blockSize, blockSize),
                     cv::Point(-1,-1), true, cv::BORDER_REPLICATE);
        variance = sqmean - mean.mul(mean);
        cv::sqrt(variance, stddev);
        switch (binarizationMethod)
        {
            case BINARIZATION_NIBLACK:
                thresh = mean + stddev * static_cast<float>(k);

                break;
            case BINARIZATION_SAUVOLA:
                thresh = mean.mul(1. + static_cast<float>(k) * (stddev / 128.0 - 1.));
                break;
            case BINARIZATION_WOLF:
                cv::minMaxIdx(src, &srcMin,NULL);
                cv::minMaxIdx(stddev, NULL, &stddevMax);
                thresh = mean - static_cast<float>(k) * (mean - srcMin - stddev.mul(mean - srcMin) / stddevMax);
                break;
            case BINARIZATION_NICK:
                cv::sqrt(variance + sqmean, sqrtVarianceMeanSum);
                thresh = mean + static_cast<float>(k) * sqrtVarianceMeanSum;
                break;
            default:
                CV_Error(CV_StsBadArg, "Unknown binarization method");
                break;
        }
        thresh.convertTo(thresh, src.depth());

        thresh.convertTo(test, src.depth());
    }
    // Prepare output image
    _dst.create(src.size(), src.type());
    cv::Mat dst = _dst.getMat();
    CV_Assert(src.data != dst.data);  // no inplace processing
    // Apply thresholding: ( pixel > threshold ) ? foreground : background
    cv::Mat mask;
    switch (type)
    {
        case cv::THRESH_BINARY:      // dst = (src > thresh) ? maxval : 0
        case cv::THRESH_BINARY_INV:  // dst = (src > thresh) ? 0 : maxval
            compare(src, thresh, mask, (type == cv::THRESH_BINARY ? cv::CMP_GT : cv::CMP_LE));
            dst.setTo(0);
            dst.setTo(maxValue, mask);
            break;
        case cv::THRESH_TRUNC:       // dst = (src > thresh) ? thresh : src
            compare(src, thresh, mask, cv::CMP_GT);
            src.copyTo(dst);
            thresh.copyTo(dst, mask);
            break;
        case cv::THRESH_TOZERO:      // dst = (src > thresh) ? src : 0
        case cv::THRESH_TOZERO_INV:  // dst = (src > thresh) ? 0 : src
            compare(src, thresh, mask, (type == cv::THRESH_TOZERO ? cv::CMP_GT : cv::CMP_LE));
            dst.setTo(0);
            src.copyTo(dst, mask);
            break;
        default:
            CV_Error(CV_StsBadArg, "Unknown threshold type");
            break;
    }
}

}
