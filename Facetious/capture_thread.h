#ifndef CAPTURE_THREAD_H
#define CAPTURE_THREAD_H

#include <QString>
#include <QThread>
#include <QMutex>
#include <QApplication>
#include <QImage>

#include "opencv2/opencv.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/face/facemark.hpp"

using namespace std;

class CaptureThread : public QThread
{
    Q_OBJECT
public:
    CaptureThread(int camera, QMutex *lock);
    CaptureThread(QString videoPath, QMutex *lock);
    ~CaptureThread();
    void setRunning(bool run) {running = run; };

    void takePhoto() { taking_photo = true; }

protected:
    void run() override;

signals:
    void frameCaptured(cv::Mat *data); 
    void photoTaken(QString name);

private:
    void takePhoto(cv::Mat &frame);
    void detectFaces(cv::Mat &frame);
    void loadOrnaments();

    void drawGlasses(cv::Mat &frame, vector<cv::Point2f> &marks);

private:
    bool running;
    int cameraID;
    QString videoPath;
    QMutex *data_lock;
    cv::Mat frame;

    int frame_width, frame_height;

    // take photos
    bool taking_photo;

    // face detection
    cv::CascadeClassifier *classifier;

    cv::Ptr<cv::face::Facemark> mark_detector;

    // mask ornaments
    cv::Mat glasses;
    cv::Mat mustache;
    cv::Mat mouse_nose;
};

#endif // CAPTURE_THREAD_H
