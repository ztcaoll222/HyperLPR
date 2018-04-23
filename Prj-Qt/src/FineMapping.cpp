/*
 * 用于去掉车牌上下左右的区域
 */
#include "include/FineMapping.h"

namespace pr {

    FineMapping::FineMapping(std::string prototxt, std::string caffemodel) {
        net = cv::dnn::readNetFromCaffe(prototxt, caffemodel);
    }

    std::pair<int, int> FineMapping::FitLineRansac(std::vector<cv::Point> pts, int zeroadd) {
        std::pair<int, int> res;
        if (pts.size() > 2) {
            cv::Vec4f line;
            cv::fitLine(pts, line, cv::DIST_HUBER, 0, 0.01, 0.01);
            float vx = line[0];
            float vy = line[1];
            float x = line[2];
            float y = line[3];
            int lefty = static_cast<int>((-x * vy / vx) + y);
            int righty = static_cast<int>(((136 - x) * vy / vx) + y);
            res.first = lefty + PADDING_UP_DOWN + zeroadd;
            res.second = righty + PADDING_UP_DOWN + zeroadd;
            return res;
        }
        res.first = zeroadd;
        res.second = zeroadd;
        return res;
    }

    cv::Mat FineMapping::FineMappingVertical(cv::Mat InputProposal,
                                             int sliceNum,
                                             int upper, int lower,
                                             int windows_size) {

        cv::Mat PreInputProposal;
        cv::Mat proposal;

        // 因为识别出来的车牌图像大小是不定的, 所以需要 resize 变为固定的大小
        cv::resize(InputProposal, PreInputProposal, cv::Size(FINEMAPPING_W, FINEMAPPING_H));

        // 图像可能是彩色的, 所以变为灰度图
        if (InputProposal.channels() == 3) {
            cv::cvtColor(PreInputProposal, proposal, cv::COLOR_BGR2GRAY);
        } else {
            PreInputProposal.copyTo(proposal);
        }

        // 每次切片次数时的差值
        float diff = static_cast<float>(upper - lower);
        diff /= static_cast<float>(sliceNum - 1);
        // 二值化后的图
        cv::Mat binary_adaptive;
        std::vector<cv::Point> line_upper;
        std::vector<cv::Point> line_lower;
        int contours_nums = 0;

        for (int i = 0; i < sliceNum; i++) {
            // 轮廓
            std::vector<std::vector<cv::Point>> contours;
            // 差值
            float k = lower + i * diff;

            // 自适应二值化
            // ADAPTIVE_THRESH_MEAN_C 平均值
            // THRESH_BINARY 大于为最大值
            cv::adaptiveThreshold(proposal, binary_adaptive,
                                  255,
                                  cv::ADAPTIVE_THRESH_MEAN_C,
                                  cv::THRESH_BINARY,
                                  windows_size,
                                  k);
//        util::showMat(binary_adaptive);

            // 检测轮廓
            // RETR_EXTERNAL 找到的轮廓里面没有小轮廓（洞）
            // CHAIN_APPROX_SIMPLE 压缩水平线，垂直线和对角线线段并仅留下其端点。例如，一个直立的矩形轮廓用4个点编码
            cv::findContours(binary_adaptive, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

            for (auto contour : contours) {
                // 用一个最小的矩形，把找到的形状包起来
                cv::Rect bdbox = cv::boundingRect(contour);
                float lwRatio = static_cast<float>(bdbox.height) / static_cast<float>(bdbox.width);
                int bdboxAera = bdbox.width * bdbox.height;
                if ((lwRatio > 0.7 && bdboxAera > 100 && bdboxAera < 300)
                    || (lwRatio > 3.0 && bdboxAera < 100 && bdboxAera > 10)) {
                    cv::Point p1(bdbox.x, bdbox.y);
                    cv::Point p2(bdbox.x + bdbox.width, bdbox.y + bdbox.height);
                    line_upper.push_back(p1);
                    line_lower.push_back(p2);
                    contours_nums += 1;
                }
            }
        }

        if (contours_nums < 41) {
            contours_nums = 0;
            cv::Mat bak;
            // 反色
            cv::bitwise_not(InputProposal, InputProposal);
            cv::resize(InputProposal, bak, cv::Size(FINEMAPPING_W, FINEMAPPING_H));
            // 获取椭圆形的結構元素
            cv::Mat kernal = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(1, 5));
            // 侵蝕
            cv::erode(bak, bak, kernal);
            if (InputProposal.channels() == 3) {
                cv::cvtColor(bak, proposal, cv::COLOR_BGR2GRAY);
            } else {
                proposal = bak;
            }

            for (int i = 0; i < sliceNum; i++) {
                std::vector<std::vector<cv::Point> > contours;
                float k = lower + i * diff;

                cv::adaptiveThreshold(proposal, binary_adaptive,
                                      255,
                                      cv::ADAPTIVE_THRESH_MEAN_C,
                                      cv::THRESH_BINARY,
                                      windows_size,
                                      k);
                cv::findContours(binary_adaptive, contours, cv::RETR_EXTERNAL,
                                 cv::CHAIN_APPROX_SIMPLE);

                for (auto contour: contours) {
                    cv::Rect bdbox = cv::boundingRect(contour);
                    float lwRatio =
                            static_cast<float>(bdbox.height) / static_cast<float>(bdbox.width);
                    int bdboxAera = bdbox.width * bdbox.height;
                    if ((lwRatio > 0.7 && bdboxAera > 100 && bdboxAera < 300)
                        || (lwRatio > 3.0 && bdboxAera < 100 && bdboxAera > 10)) {
                        cv::Point p1(bdbox.x, bdbox.y);
                        cv::Point p2(bdbox.x + bdbox.width, bdbox.y + bdbox.height);
                        line_upper.push_back(p1);
                        line_lower.push_back(p2);
                        contours_nums += 1;
                    }
                }
            }
        }

        cv::Mat rgb;
        // 添加额外的边界
        // BORDER_REPLICATE 复制原图中最临近的行或者列
        cv::copyMakeBorder(PreInputProposal, rgb, 30, 30, 0, 0, cv::BORDER_REPLICATE);

        std::pair<int, int> A;
        std::pair<int, int> B;
        A = FitLineRansac(line_upper, -2);
        B = FitLineRansac(line_lower, 2);
        int leftyB = A.first;
        int rightyB = A.second;
        int leftyA = B.first;
        int rightyA = B.second;
        int cols = rgb.cols;
        int rows = rgb.rows;
        std::vector<cv::Point2f> corners(4);
        corners[0] = cv::Point2f(cols - 1, rightyA);
        corners[1] = cv::Point2f(0, leftyA);
        corners[2] = cv::Point2f(cols - 1, rightyB);
        corners[3] = cv::Point2f(0, leftyB);
        std::vector<cv::Point2f> corners_trans(4);
        corners_trans[0] = cv::Point2f(136, 36);
        corners_trans[1] = cv::Point2f(0, 36);
        corners_trans[2] = cv::Point2f(136, 0);
        corners_trans[3] = cv::Point2f(0, 0);
        cv::Mat transform = cv::getPerspectiveTransform(corners, corners_trans);
        cv::Mat quad = cv::Mat::zeros(36, 136, CV_8UC3);
        cv::warpPerspective(rgb, quad, transform, quad.size());
        return quad;
    }

    cv::Mat
    FineMapping::FineMappingHorizon(cv::Mat FinedVertical, int leftPadding, int rightPadding) {
        cv::Mat inputBlob = cv::dnn::blobFromImage(FinedVertical, 1 / 255.0, cv::Size(66, 16),
                                                   cv::Scalar(0, 0, 0), false);

        net.setInput(inputBlob, "data");
        cv::Mat prob = net.forward();
        int front = static_cast<int>(prob.at<float>(0, 0) * FinedVertical.cols);
        int back = static_cast<int>(prob.at<float>(0, 1) * FinedVertical.cols);
        front -= leftPadding;
        if (front < 0) front = 0;
        back += rightPadding;
        if (back > FinedVertical.cols - 1) back = FinedVertical.cols - 1;
        cv::Mat cropped = FinedVertical.colRange(front, back).clone();
        return cropped;
    }

}
