#pragma once
#include <opencv2/imgcodecs.hpp> // for reading and writing image files
#include <opencv2/highgui.hpp> // for cmds like imshow, waitKey and for windows (not a cmd)
#include <opencv2/imgproc.hpp> // color conversions and most importantly contours which we use for the bounding boxes
#include <opencv2/objdetect.hpp> // ArUco marker detection (moved to objdetect in OpenCV 4.8+)
#include <opencv2/calib3d.hpp> // Cam calibration
#include <string>
#include <vector>

class ArUcoDetector
{
public:
    struct ObjectPose
    {
        bool  found;
        float centerX; // pixel X of the object's corner
        float centerY; // pixel Y of the object's corner
        float width; // bounding box in pixels
        float height; // bounding box in pixels
        float angle; // rot angle in degrees
        float distance; // distance from cam in cm
        float offsetAngle; // in degrees left or right from cam
    };

    struct ArucoPose
    {
        bool  found;
        int   id;
        float centerX; // pixel X of the object's corner
        float centerY; // pixel Y of the object's corner
        float x;       // position in mm from camera
        float y; // position in mm from camera
        float z; // position in mm from camera
        float rotX;    // tilt in degrees
        float rotY;     // tilt in degrees
        float rotZ; // tilt in degrees
        double rvecX = 0; // axis-angle rotation vector in radians (from solvePnP)
        double rvecY = 0;
        double rvecZ = 0;
    };

    ArUcoDetector();
    bool loadCalibration(const std::string& path);
    void run();
    std::vector<ArucoPose> detectOnce(int cameraIndex = 1);
    std::vector<ArucoPose> detectAruco(cv::Mat& img);
    void drawPose(cv::Mat& img, ObjectPose pose, cv::Scalar color, std::string label);
    void drawArucoPose(cv::Mat& img, ArucoPose pose);

private:
    cv::Mat imgHSV; // stores hsv (hue saturation value)
    cv::Mat cameraMatrix, distCoeffs; // loaded from the calibration file (lens math)

    float realWidthCm; // width of cubes
    float focalLength; // default focal length
    float hFOV; // default fov

    cv::Mat buildTransformMatrix(cv::Vec3d rvec, cv::Vec3d tvec);
    std::string matrixToString(cv::Mat transform);
    static void onMouse(int event, int x, int y, int flags, void* userdata); // Used to calibrate the color HSV
};
