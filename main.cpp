#include <iostream>
#include <string>
#include <filesystem>
#include <opencv2/opencv.hpp>

namespace fs = std::filesystem;

int main(int argc, char** argv) {
    if (argc < 2 || argc > 3) {
        std::cerr << "Usage: " << argv[0] << " path/to/video.mp4 [fps]" << std::endl;
        return 1;
    }

    std::string videoPath = argv[1];
    double desiredFPS = -1.0; // Default value: use all frames

    if (argc == 3) {
        desiredFPS = std::stod(argv[2]);
    }

    cv::VideoCapture videoCapture(videoPath);
    if (!videoCapture.isOpened()) {
        std::cerr << "Error: Unable to open the video" << std::endl;
        return 1;
    }

    // get information of the video
    int totalFrames = static_cast<int>(videoCapture.get(cv::CAP_PROP_FRAME_COUNT));
    double frameRate = videoCapture.get(cv::CAP_PROP_FPS);
    double duration = totalFrames / frameRate;
    cv::Size resolution(static_cast<int>(videoCapture.get(cv::CAP_PROP_FRAME_WIDTH)), static_cast<int>(videoCapture.get(cv::CAP_PROP_FRAME_HEIGHT)));
    std::cout << "============================" << std::endl;
    std::cout << "VIDEO INFORMATION" << std::endl;
    std::cout << "============================" << std::endl;
    std::cout << "Total Frames: " << totalFrames << std::endl;
    std::cout << "Frame Rate: " << frameRate << " FPS" << std::endl;
    std::cout << "Duration: " << duration << " seconds" << std::endl;
    std::cout << "Resolution: " << resolution.width << "x" << resolution.height << std::endl;
    std::cout << "============================" << std::endl;

    fs::path videoDir = fs::path(videoPath).stem();
    fs::create_directory(videoDir);

    int frameNumber = 0;
    int framesSaved = 0;
    int frameSkip = 1;
    if (desiredFPS > 0.0) {
        frameSkip = static_cast<int>(videoCapture.get(cv::CAP_PROP_FPS) / desiredFPS);
        if (frameSkip <= 0) frameSkip = 1;
    }

    while (true) {
        cv::Mat frame;
        videoCapture >> frame;

        if (frame.empty())
            break;

        if (frameNumber % frameSkip == 0) {
            std::string imageName = videoDir.string() + "/frame_" + std::to_string(frameNumber) + ".jpg";
            cv::imwrite(imageName, frame);
            framesSaved++;
        }

        frameNumber++;
    }

    videoCapture.release();
    std::cout << "Saved " << framesSaved << " frames as images in the folder \"" << videoDir.string() << "\"" << std::endl;

    return 0;
}
