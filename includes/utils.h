#ifndef UTILS_H
#define UTILS_H

#include <opencv2/opencv.hpp>
#include <string>

namespace Utils {
    cv::Mat loadImage(const std::string &path);
    bool saveImage(const std::string &path, const cv::Mat &image);
    void showImage(const std::string &windowName, const cv::Mat &image);
    cv::Mat sharpenImage(const cv::Mat &image);
    cv::Mat enhanceResolution(const cv::Mat &image, double scaleFactor = 2.0);
    cv::Mat reduceBlur(const cv::Mat &image);
    cv::Mat toGray(const cv::Mat &image);
}

#endif
