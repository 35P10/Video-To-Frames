#include <iostream>
#include <string>
#include <filesystem>
#include <opencv2/opencv.hpp>

namespace fs = std::filesystem;

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " path/to/video.mp4" << std::endl;
        return 1;
    }

    std::string videoPath = argv[1];

    cv::VideoCapture videoCapture(videoPath);
    if (!videoCapture.isOpened()) {
        std::cerr << "Error: Unable to open the video" << std::endl;
        return 1;
    }

    fs::path videoDir = fs::path(videoPath).stem();
    fs::create_directory(videoDir); 

    int frameNumber = 0;
    while (true) {
        cv::Mat frame;
        videoCapture >> frame;

        if (frame.empty())
            break;

        std::string imageName = videoDir.string() + "/frame_" + std::to_string(frameNumber) + ".jpg";
        cv::imwrite(imageName, frame);

        frameNumber++;
    }

    videoCapture.release();
    std::cout << "Saved " << frameNumber << " frames as images in the folder \"" << videoDir.string() << "\"" << std::endl;

    return 0;
}
