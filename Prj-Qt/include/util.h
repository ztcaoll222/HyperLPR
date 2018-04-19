//
// Created by 庾金科 on 04/04/2017.
//
#ifndef SWIFTPR_UTIL_H
#define SWIFTPR_UTIL_H

#include <opencv2/opencv.hpp>

namespace util {

template <class T> void swap ( T& a, T& b );

template <class T> T min(T& a,T& b );

cv::Mat cropFromImage(const cv::Mat &image, cv::Rect rect);

cv::Mat cropBox2dFromImage(const cv::Mat &image,cv::RotatedRect rect);

cv::Mat calcHist(const cv::Mat &image);

float computeSimilir(const cv::Mat &A,const cv::Mat &B);

/**
 * @brief showMat 显示图像并在对应位置画框
 * @param image 图像
 * @param plate 位置
 * @param name 窗口名字
 */
void showMat(cv::Mat image, cv::Rect plate, std::string name="Image");

/**
 * @brief showMat 显示图像
 * @param image 图像
 * @param name 窗口名字
 */
void showMat(cv::Mat image, std::string name="Image");

} //namespace util
#endif //SWIFTPR_UTIL_H
