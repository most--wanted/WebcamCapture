#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //init dev list
    this->on_pbUpdate_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pbUpdate_clicked()
{
    this->ui->cbDevicesList->clear();
    std::vector<std::string> deviceList = DeviceManager::GetWebcamList();
    std::vector<std::string>::iterator deviceIterator = deviceList.begin();

    while (deviceIterator != deviceList.end())
    {
        this->ui->cbDevicesList->addItem((*deviceIterator).c_str());
        ++deviceIterator;
    }
}

void MainWindow::on_pbCaptureVideo_clicked()
{
    // current index of combo box - it is the cam index
    CvCapture *capture = cvCreateCameraCapture(this->ui->cbDevicesList->currentIndex());
    assert( capture );

    IplImage * frame = 0;
    cvNamedWindow("WebCam", CV_WINDOW_AUTOSIZE);

    while (true)
    {
        //get frame
        frame = cvQueryFrame( capture );
        //show frame
        cvShowImage("WebCam", frame);

        char c = cvWaitKey(33);
        if (c == 27)
        {
            // key ESC pressed
            break;
        }
    }

    //release resourses
    cvReleaseCapture( &capture );
    cvDestroyWindow("WebCam");
}
