//
// Created by �׽�� on 22/10/2017.
//
#ifndef SWIFTPR_PIPLINE_H
#define SWIFTPR_PIPLINE_H

#include "PlateDetection.h"
#include "PlateSegmentation.h"
#include "CNNRecognizer.h"
#include "PlateInfo.h"
#include "FastDeskew.h"
#include "FineMapping.h"
#include "Recognizer.h"
#include "util.h"

namespace pr{

class PipelinePR {
private:
    std::vector<std::string> CHAR_CODE{ "京","沪","津","渝","冀","晋","蒙","辽","吉","黑","苏","浙","皖","闽","赣","鲁","豫","鄂","湘","粤","桂","琼","川","贵","云","藏","陕","甘","青","宁","新","0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F","G","H","J","K","L","M","N","P","Q","R","S","T","U","V","W","X","Y","Z" };

    GeneralRecognizer *generalRecognizer;
    PlateDetection *plateDetection;
    PlateSegmentation *plateSegmentation;
    FineMapping *fineMapping;

public:
    PipelinePR(std::string detector_filename,
               std::string finemapping_prototxt, std::string finemapping_caffemodel,
               std::string segmentation_prototxt, std::string segmentation_caffemodel,
               std::string charRecognization_proto, std::string charRecognization_caffemodel);

    ~PipelinePR();

    std::vector<PlateInfo> RunPiplineAsImage(cv::Mat plateImage);
};

}
#endif //SWIFTPR_PIPLINE_H
