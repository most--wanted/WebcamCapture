#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "devicemanager.h"
#include "opencv/cv.h"
#include "opencv/highgui.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_pbUpdate_clicked();

    void on_pbCaptureVideo_clicked();

private:
    Ui::MainWindow *ui;
    // <int, int> - combobox index, webcam index
    std::map<int, int> camIndexesMap_;
};

#endif // MAINWINDOW_H
