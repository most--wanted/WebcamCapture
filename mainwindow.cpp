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
    this->camIndexesMap_.clear();

    std::map<int, std::string> deviceList = DeviceManager::GetWebcamList();
    std::map<int, std::string>::iterator deviceItterator;
    int cbIndexCounter = 0;

    for (deviceItterator = deviceList.begin(); deviceItterator != deviceList.end(); ++deviceItterator)
    {
        this->ui->cbDevicesList->addItem(deviceItterator->second.c_str());

        //connect comboBox idexes with webcam indexes
        this->camIndexesMap_.insert(std::pair<int, int>(cbIndexCounter, deviceItterator->first));
        cbIndexCounter++;
    }
}

void MainWindow::on_pbCaptureVideo_clicked()
{
    if ( this->ui->cbDevicesList->currentIndex() < 0 )
    {
        //cam was not selected
        return;
    }

    //get current webcam index from MAP
    std::map<int, int>::iterator it;
    it = this->camIndexesMap_.find(this->ui->cbDevicesList->currentIndex());

    CvCapture *capture = cvCreateCameraCapture(it->second);
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
