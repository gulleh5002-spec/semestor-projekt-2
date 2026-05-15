#include "ArUcoDetector.h"
#include <iostream>

ArUcoDetector::ArUcoDetector()
    : realWidthCm(5.0f), focalLength(600.0f), hFOV(65.0f)
{
    loadCalibration("calibration.yml");
    cap.open(1);
    for (int i = 0; i < 10; i++)
    {
        cv::Mat dummy;
        cap.read(dummy);
    }
}

bool ArUcoDetector::loadCalibration(const std::string& path)
{
    cv::FileStorage fs(path, cv::FileStorage::READ);
    if (!fs.isOpened())
        return false;
    fs["cameraMatrix"] >> cameraMatrix; // reads matrix from calibration file
    fs["distCoeffs"]   >> distCoeffs; // reads matrix from calibration file
    fs.release(); // closes file
    return true;
}

void ArUcoDetector::onMouse(int event, int x, int y, int flags, void* userdata) // Used to calibrate the color HSV
{
    if (event == cv::EVENT_LBUTTONDOWN) // if mouse button clicked
    {
        ArUcoDetector* self = static_cast<ArUcoDetector*>(userdata);
        cv::Vec3b pixel = self->imgHSV.at<cv::Vec3b>(y, x); // reads the hsv pixel at the clicked pos
        std::cout << "HSV: H=" << (int)pixel[0]
            << " S=" << (int)pixel[1]
            << " V=" << (int)pixel[2] << std::endl;
    }
}

cv::Mat ArUcoDetector::buildTransformMatrix(cv::Vec3d rvec, cv::Vec3d tvec)
{
    cv::Mat rotMat;
    cv::Rodrigues(rvec, rotMat);  // 3x3 rotation matrix

    // builds 4x4 homogeneous transform
    cv::Mat transform = cv::Mat::eye(4, 4, CV_64F); // identity matrix and the last part tells how to store it. opencv, 64 bits, and float

    // insert rotation (top left 3x3)
    rotMat.copyTo(transform(cv::Rect(0, 0, 3, 3)));

    // insert translation (top right column), tvec is in meters
    transform.at<double>(0, 3) = tvec[0] * 1000.0;  // X in mm
    transform.at<double>(1, 3) = tvec[1] * 1000.0;  // Y in mm
    transform.at<double>(2, 3) = tvec[2] * 1000.0;  // Z in mm

    return transform;
}

// Serialize matrix to string for TCP sending
std::string ArUcoDetector::matrixToString(cv::Mat transform)
{
    std::string result = "";
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            result += std::to_string(transform.at<double>(row, col)); // add number
            if (col < 3) result += ","; // comma between coloumns
        }
        result += ";";  // row separator
    }
    return result;
}

// Detects ArUco markers and returns full 6DOF pose for each
std::vector<ArUcoDetector::ArucoPose> ArUcoDetector::detectAruco(cv::Mat& img) // Takes current frame as input
{
    std::vector<ArucoPose> results; // Makes an empty list (vector) of the results

    float markerSizeM = 0.04f;  // marker size in m


    cv::aruco::Dictionary dictionary =
        cv::aruco::getPredefinedDictionary(cv::aruco::DICT_5X5_50); // The exact marker we use
    cv::aruco::DetectorParameters parameters; // Uses default detection settings (from library) saved in parameters
    cv::aruco::ArucoDetector detector(dictionary, parameters); // Uses parameters and the dictionary from above to define what markers are valid

    std::vector<int> markerIds;
    std::vector<std::vector<cv::Point2f>> markerCorners, rejected; // Holds the marker for the detected markers and rejected marker (things that look like markers (can be used for debugging)) point2f is a 2D coordinate stored as float

    detector.detectMarkers(img, markerCorners, markerIds, rejected); // Here it actually runs the detection, and fills the containers with that is detected

    if (markerIds.empty()) return results; // If it does not detect anything, it will return an empty result list

    // draw detected markers
    cv::aruco::drawDetectedMarkers(img, markerCorners, markerIds);

    // 3D corners of a single marker in its own coordinate system
    std::vector<cv::Point3f> objPoints = {
        {-markerSizeM / 2,  markerSizeM / 2, 0},
        { markerSizeM / 2,  markerSizeM / 2, 0},
        { markerSizeM / 2, -markerSizeM / 2, 0},
        {-markerSizeM / 2, -markerSizeM / 2, 0}
    };

    // estimates a pose for each marker using solvePnP (estimatePoseSingleMarkers removed in 4.8+)
    std::vector<cv::Vec3d> rvecs(markerIds.size()), tvecs(markerIds.size()); // vector x,y,z with double type
    for (int i = 0; i < (int)markerIds.size(); i++)
        cv::solvePnP(objPoints, markerCorners[i], cameraMatrix, distCoeffs, rvecs[i], tvecs[i]); // rvecs is rotational vector (angle axis) and tvecs is translation

    for (int i = 0; i < (int)markerIds.size(); i++) // loop through each found marker
    {
        cv::drawFrameAxes(img, cameraMatrix, distCoeffs, // draw the actual axes on the marker
            rvecs[i], tvecs[i], markerSizeM * 0.5f);

        ArucoPose pose;
        pose.found = true;
        pose.id = markerIds[i];

        pose.centerX = (markerCorners[i][0].x + markerCorners[i][1].x +
            markerCorners[i][2].x + markerCorners[i][3].x) / 4.0f;
        pose.centerY = (markerCorners[i][0].y + markerCorners[i][1].y +
            markerCorners[i][2].y + markerCorners[i][3].y) / 4.0f; // Finds the pixel center of x and y by averaging the 4 corners

        // position in mm extracted from the translation vectors
        pose.x = (float)(tvecs[i][0] * 1000.0);
        pose.y = (float)(tvecs[i][1] * 1000.0);
        pose.z = (float)(tvecs[i][2] * 1000.0);

        // raw axis-angle rotation vector in radians (direct from solvePnP)
        pose.rvecX = rvecs[i][0];
        pose.rvecY = rvecs[i][1];
        pose.rvecZ = rvecs[i][2];

        // rotation to rot matrix from rot vectors using Rodrigues
        cv::Mat rotMat; // empty container
        cv::Mat rvecMat(rvecs[i]); // takes each rotational vector and turns into a 3x3 matrix
        cv::Rodrigues(rvecMat, rotMat); // Uses rodrigues formula to actually fill out the matrix

        pose.rotX = (float)(atan2(rotMat.at<double>(2, 1), // finds tilt (roll) (CV_PI is just pi in opencv)
            rotMat.at<double>(2, 2)) * 180.0 / CV_PI);
        pose.rotY = (float)(atan2(-rotMat.at<double>(2, 0),
            sqrt(pow(rotMat.at<double>(2, 1), 2) +
                pow(rotMat.at<double>(2, 2), 2))) * 180.0 / CV_PI);  // pitch
        pose.rotZ = (float)(atan2(rotMat.at<double>(1, 0), // yaw
            rotMat.at<double>(0, 0)) * 180.0 / CV_PI);

        cv::Mat transform = buildTransformMatrix(rvecs[i], tvecs[i]); // Builds the 4x4 transform matrix
        std::string data = matrixToString(transform); // Converts it to a string, so it can be sent or converted to other functions in main more easily

        results.push_back(pose); // push back the poses in the vector
    }
    return results;
}

void ArUcoDetector::drawPose(cv::Mat& img, ObjectPose pose, cv::Scalar color, std::string label)
{
    if (!pose.found) return; // if not poses are found, nothing happens

    cv::Point center(pose.centerX, pose.centerY); // creates a dot in the center of the found object
    cv::circle(img, center, 5, color, -1);

    float len = std::max(pose.width, pose.height) / 2.0f + 20; // finds length of the axes and adds 20 extra pixels for easier visibility
    float rad = pose.angle * CV_PI / 180.0f; // turns it into radians

    cv::Point xEnd(center.x + len * cos(rad), center.y + len * sin(rad));
    cv::Point xStart(center.x - len * cos(rad), center.y - len * sin(rad));
    cv::arrowedLine(img, xStart, xEnd, cv::Scalar(0, 0, 255), 2); // creates x axis

    float radY = rad + CV_PI / 2.0f;
    cv::Point yEnd(center.x + len * cos(radY), center.y + len * sin(radY));
    cv::arrowedLine(img, center, yEnd, cv::Scalar(0, 255, 0), 2); // creates y axis

    cv::Point2f corners[4];
    cv::RotatedRect(cv::Point2f(pose.centerX, pose.centerY),
        cv::Size2f(pose.width, pose.height),
        pose.angle).points(corners); // draws the corner points of the rectangle
    for (int i = 0; i < 4; i++)
        cv::line(img, corners[i], corners[(i + 1) % 4], color, 2); // Creates the lines for the corners

    // Gives the info of the Distance, offset, and rotation when running the program
    std::string info = label +
        " Dist:" + std::to_string((int)pose.distance) + "cm" +
        " Off:" + std::to_string((int)pose.offsetAngle) + "deg" +
        " Rot:" + std::to_string((int)pose.angle) + "deg";

    cv::putText(img, info,
        cv::Point(pose.centerX - 100, pose.centerY - (pose.height / 2) - 15),
        cv::FONT_HERSHEY_SIMPLEX, 0.5, color, 2); // Converts the info to text
}

void ArUcoDetector::drawArucoPose(cv::Mat& img, ArucoPose pose)
{
    if (!pose.found) return;

    // text showing full 6DOF pose
    std::string line1 = "ID:" + std::to_string(pose.id) +
        " X:" + std::to_string((int)pose.x) + "mm" +
        " Y:" + std::to_string((int)pose.y) + "mm" +
        " Z:" + std::to_string((int)pose.z) + "mm";
    std::string line2 = "Rx:" + std::to_string((int)pose.rotX) + "deg" +
        " Ry:" + std::to_string((int)pose.rotY) + "deg" +
        " Rz:" + std::to_string((int)pose.rotZ) + "deg";

    cv::putText(img, line1,
        cv::Point(pose.centerX - 60, pose.centerY - 20),
        cv::FONT_HERSHEY_SIMPLEX, 0.45, cv::Scalar(0, 255, 255), 2);
    cv::putText(img, line2,
        cv::Point(pose.centerX - 60, pose.centerY - 5),
        cv::FONT_HERSHEY_SIMPLEX, 0.45, cv::Scalar(0, 255, 255), 2);
}

std::vector<ArUcoDetector::ArucoPose> ArUcoDetector::detectOnce(int cameraIndex)
{
    if (cameraMatrix.empty())
    {
        std::cout << "Cannot find calibration.yml — run calibration first!" << std::endl;
        return {};
    }

    if (!cap.isOpened())
    {
        std::cout << "Failed to capture frame from camera " << cameraIndex << std::endl;
        return {};
    }

    cv::Mat img;
    cap.read(img);

    if (img.empty())
    {
        std::cout << "Failed to capture frame from camera " << cameraIndex << std::endl;
        return {};
    }

    return detectAruco(img);
}

void ArUcoDetector::run()
{
    // load calibration for the ArUco Marker (done from a seperate file)
    if (!loadCalibration("calibration.yml"))
    {
        std::cout << "Cannot find calibration.yml — run calibration first!" << std::endl;
        return;
    }
    std::cout << "Calibration loaded." << std::endl;

    cv::VideoCapture cap(1); // connects cam (sometimes 1 works, sometimes 0)
    std::cout << "1" ;
    cv::Mat img; // empty variable that holds each frame for the camera

    cv::namedWindow("Camera");
    cv::setMouseCallback("Camera", onMouse, this); // passes this so onMouse can access imgHSV

    while (true)
    {
        cap.read(img); // read the frame
        if (img.empty()) break;

        cv::cvtColor(img, imgHSV, cv::COLOR_BGR2HSV); // switches color from RGB to HSV, which is better for detection for cams

        // detect ArUco markers — pass img directly so axes get drawn on it
        std::vector<ArucoPose> arucoPoses = detectAruco(img);

        // print ArUco pose data
        for (int i = 0; i < (int)arucoPoses.size(); i++)
        {
            std::cout << "ArUco ID:" << arucoPoses[i].id
                << " X=" << arucoPoses[i].x
                << " Y=" << arucoPoses[i].y
                << " Z=" << arucoPoses[i].z << "mm"
                << " Rx=" << arucoPoses[i].rotX
                << " Ry=" << arucoPoses[i].rotY
                << " Rz=" << arucoPoses[i].rotZ << "deg" << std::endl;
        }

        for (int i = 0; i < (int)arucoPoses.size(); i++) // displays text overlay for aruco poses
            drawArucoPose(img, arucoPoses[i]);

        cv::imshow("Camera", img); // displays current frame
        if (cv::waitKey(1) == 27) break; // waits 1 ms which is needed for it to process everything
    }
}
