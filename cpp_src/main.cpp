#include <opencv2/opencv.hpp>
#include <iostream>
#include "../includes/faceDetector.h"
#include "../includes/imageEnhancer.h"
#include "../includes/utils.h"

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
    cout << "[DEBUG] Program started." << endl;
    if (argc < 3) {
        cerr << "Usage: ./face_enhancer <input_image> <output_image>\n";
        return -1;
    }

    string inputPath = argv[1];
    string outputPath = argv[2];
    string faceCascadePath = "haarcascade_frontalface_default.xml";

    cout << "Reading input image from: " << inputPath << endl;
    Mat image = imread(inputPath);
    if (image.empty()) {
        cerr << "Could not read the image: " << inputPath << endl;
        return -1;
    }

    cout << "Loading face detector from: " << faceCascadePath << endl;
    FaceDetector detector(faceCascadePath);

    cout << "Detecting faces..." << endl;
    vector<Rect> faces = detector.detectFaces(image);
    cout << "Detected " << faces.size() << " faces" << endl;

    if (faces.empty()) {
        cerr << "No faces detected. Exiting.\n";
        return 0;
    }

    cout << "Enhancing faces..." << endl;
    ImageEnhancer enhancer;
    Mat enhanced = enhancer.enhanceFaces(image, faces);

    cout << "Saving enhanced image to: " << outputPath << endl;
    if (!imwrite(outputPath, enhanced)) {
        cerr << "Could not save output image.\n";
        return -1;
    }
    cout << "Enhanced image saved successfully!\n";
    return 0;
}
