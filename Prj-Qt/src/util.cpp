#include "include/util.h"

namespace pr {

void showMat(cv::Mat image, cv::Rect plate) {
    cv::rectangle(image, cv::Point(plate.x, plate.y), cv::Point(plate.x + plate.width, plate.y + plate.height), cv::Scalar(255, 255, 0), 2);
    cv::imshow("input", image);
    cv::waitKey(0);
}

}
