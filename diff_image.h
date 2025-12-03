#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_diff_image.h"
#include "QFileDialog"
#include <QFileDialog>
#include <QMessageBox>


class diff_image : public QMainWindow
{
    Q_OBJECT

public:
    diff_image(QWidget *parent = nullptr);
    ~diff_image();

private:
    QImage img1;
    QImage img2;
    QImage result;
    int tolerance = 0;

    Ui::diff_imageClass ui;
private slots:
    void on_btnLoadFirst_clicked();
    void on_btnLoadSecond_clicked();
    void on_btnProcessImage_clicked();
    void on_toleranceSlider_valueChanged(int);
    void on_saveImage_clicked();
    void on_btnSwap_clicked();

};

