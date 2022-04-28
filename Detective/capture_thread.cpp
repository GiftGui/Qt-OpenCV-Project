#include <QTime>
#include <QDebug>

#include "utilities.h"
#include "capture_thread.h"

CaptureThread::CaptureThread(int camera, QMutex *lock):
    running(false), cameraID(camera), videoPath(""), data_lock(lock)
{
    frame_width = frame_height = 0;
    taking_photo = false;
}

CaptureThread::CaptureThread(QString videoPath, QMutex *lock):
    running(false), cameraID(-1), videoPath(videoPath), data_lock(lock)
{
    frame_width = frame_height = 0;
    taking_photo = false;
}

CaptureThread::~CaptureThread() {
}

void CaptureThread::run() {
    running = true;
    cv::VideoCapture cap(cameraID);
    // cv::VideoCapture cap("/home/kdr2/Videos/WIN_20190123_20_14_56_Pro.mp4");
    cv::Mat tmp_frame;

    frame_width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    frame_height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

    classifier = new cv::CascadeClassifier(OPENCV_DATA_DIR \
        "haarcascades/haarcascade_frontalcatface_extended.xml");

    while(running) {
        cap >> tmp_frame;
        if (tmp_frame.empty()) {
            break;
        }

        if(taking_photo) {
            takePhoto(tmp_frame);
        }

        detectObjects(tmp_frame);

        cvtColor(tmp_frame, tmp_frame, cv::COLOR_BGR2RGB);
        data_lock->lock();
        frame = tmp_frame;
        data_lock->unlock();
        emit frameCaptured(&frame);
    }
    cap.release();
    delete classifier;
    classifier = nullptr;
    running = false;
}


void CaptureThread::takePhoto(cv::Mat &frame)
{
    QString photo_name = Utilities::newPhotoName();
    QString photo_path = Utilities::getPhotoPath(photo_name, "jpg");
    cv::imwrite(photo_path.toStdString(), frame);
    emit photoTaken(photo_name);
    taking_photo = false;
}

void CaptureThread::detectObjects(cv::Mat &frame)
{
    vector<cv::Rect> objects;
    classifier->detectMultiScale(frame, objects, 1.3, 5);
    cv::Scalar color = cv::Scalar(0, 0, 255); // red
    // draw the circumscribe rectangles
    for(size_t i = 0; i < objects.size(); i++) {
        cv::rectangle(frame, objects[i], color, 2);
    }
}

void CaptureThread::detectObjectsDNN(cv::Mat &frame)
{
    int inputWidth = 416;
    int inputHeight = 416;
    if (net.empty()) {
        // give the configuration and weight files for the model
        string modelConfig = "data/yolov3.cfg";
        string modelWeights = "data/yolov3.weights";
        net = cv::dnn::readNetFromDarknet(modelConfig, modelWeights);
        objectClasses.clear();
        string name;
        string namesFile = "data/coco.names";
        ifstream ifs(namesFile.c_str());
        while(getline(ifs, name)) objectClasses.push_back(name);
    }

    cv::Mat blob;
    cv::dnn::blobFromImage(
    frame, blob, 1 / 255.0,
    cv::Size(inputWidth, inputHeight),
    cv::Scalar(0, 0, 0), true, false);
    net.setInput(blob);
    // forward
    vector<cv::Mat> outs;
    net.forward(outs, getOutputsNames(net));
}

vector<string> CaptureThread::getOutputsNames(const cv::dnn::dnn4_v20211220::Net &net)
{
    static vector<string> names;
    vector<int> outLayers = net.getUnconnectedOutLayers();
    vector<string> layersNames = net.getLayerNames();
    names.resize(outLayers.size());
    for (size_t i = 0; i < outLayers.size(); ++i)
    names[i] = layersNames[outLayers[i] - 1];
    return names;
}
