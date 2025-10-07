#ifndef IMAGEENHANCER_H
#define IMAGEENHANCER_H

#include <opencv2/opencv.hpp>
#include <vector>

class ImageEnhancer {
private:
    cv::Mat enhanceRegion(const cv::Mat &faceROI);

public:
    cv::Mat enhanceFaces(const cv::Mat &image, const std::vector<cv::Rect> &faces);
};

#endif
