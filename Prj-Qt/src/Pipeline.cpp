#include "include/Pipeline.h"

namespace pr {

    PipelinePR::PipelinePR(std::string detector_filename, std::string finemapping_prototxt,
                           std::string finemapping_caffemodel, std::string segmentation_prototxt,
                           std::string segmentation_caffemodel, std::string charRecognization_proto,
                           std::string charRecognization_caffemodel) {
        plateDetection = new PlateDetection(detector_filename);
        fineMapping = new FineMapping(finemapping_prototxt, finemapping_caffemodel);
        plateSegmentation = new PlateSegmentation(segmentation_prototxt, segmentation_caffemodel);
        generalRecognizer = new CNNRecognizer(charRecognization_proto,
                                              charRecognization_caffemodel);
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

            // 去掉车牌上下多余的像素
            image_finemapping = fineMapping->FineMappingVertical(image_finemapping);

            // 扭正车牌
            image_finemapping = pr::fastdeskew(image_finemapping, 5);

            // 去掉车牌左右多余的像素
            image_finemapping = fineMapping->FineMappingHorizon(image_finemapping, 2, 5);

            // 调整大小
            cv::resize(image_finemapping, image_finemapping, cv::Size(136, 36));

            // 分割
            plateinfo.setPlateImage(image_finemapping);
            std::vector<cv::Rect> rects;
            plateSegmentation->segmentPlatePipline(plateinfo, 1, rects);
            plateSegmentation->ExtractRegions(plateinfo, rects);
            cv::copyMakeBorder(image_finemapping, image_finemapping, 0, 0, 0, 20,
                               cv::BORDER_REPLICATE);

            // 识别
            plateinfo.setPlateImage(image_finemapping);
            generalRecognizer->SegmentBasedSequenceRecognition(plateinfo);
            plateinfo.decodePlateNormal(CHAR_CODE);
            results.push_back(plateinfo);
        }

        return results;
    }

} // namespace pr
