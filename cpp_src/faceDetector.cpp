#include "../includes/faceDetector.h"
#include <iostream>

using namespace cv;
using namespace std;

FaceDetector::FaceDetector(const string &cascadePath) {
    if (!faceCascade.load(cascadePath)) {
        cerr << "Could not load cascade file: " << cascadePath << endl;
        exit(-1);
    }
}

vector<Rect> FaceDetector::detectFaces(const Mat &image) {
    vector<Rect> faces;

    Mat resized;
    double scaleFactor = 2.0;
    resize(image, resized, Size(), scaleFactor, scaleFactor, INTER_CUBIC);

    Mat gray;
    cvtColor(resized, gray, COLOR_BGR2GRAY);
    equalizeHist(gray, gray);

    faceCascade.detectMultiScale(
        gray,
        faces,
        1.05,    
        3,
        0 | CASCADE_SCALE_IMAGE,
        Size(10, 10)
    );


    for (auto &f : faces) {
        f.x = static_cast<int>(f.x / scaleFactor);
        f.y = static_cast<int>(f.y / scaleFactor);
        f.width = static_cast<int>(f.width / scaleFactor);
        f.height = static_cast<int>(f.height / scaleFactor);
    }

    return faces;
}
