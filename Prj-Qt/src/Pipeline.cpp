#include "include/Pipeline.h"

namespace pr {

PipelinePR::PipelinePR(std::string detector_filename, std::string finemapping_prototxt, std::string finemapping_caffemodel, std::string segmentation_prototxt, std::string segmentation_caffemodel, std::__cxx11::string charRecognization_proto, std::string charRecognization_caffemodel) {
    plateDetection = new PlateDetection(detector_filename);
    fineMapping = new FineMapping(finemapping_prototxt, finemapping_caffemodel);
    plateSegmentation = new PlateSegmentation(segmentation_prototxt, segmentation_caffemodel);
    generalRecognizer = new CNNRecognizer(charRecognization_proto, charRecognization_caffemodel);
}

PipelinePR::~PipelinePR() {
    delete plateDetection;
    delete fineMapping;
    delete plateSegmentation;
    delete generalRecognizer;
}

std::vector<pr::PlateInfo> PipelinePR::RunPiplineAsImage(cv::Mat plateImage) {
    std::vector<pr::PlateInfo> plates;
    std::vector<pr::PlateInfo> results;

    // 检测车牌位置和图像
    plateDetection->plateDetectionRough(plateImage, plates);

    for (pr::PlateInfo plateinfo : plates) {

        cv::Mat image_finemapping = plateinfo.getPlateImage();
        image_finemapping = fineMapping->FineMappingVertical(image_finemapping);
        image_finemapping = pr::fastdeskew(image_finemapping, 5);
        image_finemapping = fineMapping->FineMappingHorizon(image_finemapping, 2, 5);
        cv::resize(image_finemapping, image_finemapping, cv::Size(136, 36));
        plateinfo.setPlateImage(image_finemapping);
        std::vector<cv::Rect> rects;
        plateSegmentation->segmentPlatePipline(plateinfo, 1, rects);
        plateSegmentation->ExtractRegions(plateinfo, rects);
        cv::copyMakeBorder(image_finemapping, image_finemapping, 0, 0, 0, 20, cv::BORDER_REPLICATE);

        plateinfo.setPlateImage(image_finemapping);
        generalRecognizer->SegmentBasedSequenceRecognition(plateinfo);
        plateinfo.decodePlateNormal(chars_code);
        results.push_back(plateinfo);
    }

    return results;
}

}
