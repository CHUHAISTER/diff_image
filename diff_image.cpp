#include "diff_image.h"

diff_image::diff_image(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);


}

diff_image::~diff_image()
{}

void diff_image::on_btnLoadFirst_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Select image", "", "Images (*.png *.jpg *.bmp)");
    if (!filename.isEmpty()) {
        img1 = QImage(filename);
        ui.lblImage1->setPixmap(QPixmap::fromImage(img1).scaled(ui.lblImage1->size(), Qt::KeepAspectRatio));
    }
}

void diff_image::on_btnLoadSecond_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Select image", "", "Images (*.png *.jpg *.bmp)");
    if (!filename.isEmpty()) {
        img2 = QImage(filename);
        ui.lblImage2->setPixmap(QPixmap::fromImage(img2).scaled(ui.lblImage2->size(), Qt::KeepAspectRatio));
    }
}

void diff_image::on_toleranceSlider_valueChanged(int value) 
{
    ui.tolerance->setNum(value);
    tolerance = value;
}

void diff_image::on_btnProcessImage_clicked()
{
    if (!(img1.isNull() && img2.isNull())) {

        img1 = img1.convertToFormat(QImage::Format_RGB32);
        img2 = img2.convertToFormat(QImage::Format_RGB32);
        int width = qMin(img1.width(), img2.width());
        int height = qMin(img1.height(), img2.height());

        

        result = QImage(QSize(width,height), QImage::Format_RGB32);
        for (int y = 0; y < height; ++y) {
            const QRgb* lineBase = reinterpret_cast<const QRgb*>(img1.scanLine(y));
            const QRgb* lineOther = reinterpret_cast<const QRgb*>(img2.scanLine(y));
            QRgb* lineRes = reinterpret_cast<QRgb*>(result.scanLine(y));

            for (int x = 0; x < width; ++x) {
                QColor c1 = QColor::fromRgb(lineBase[x]);
                QColor c2 = QColor::fromRgb(lineOther[x]);

                int diff = (c2.red() + c2.green() + c2.blue()) -
                    (c1.red() + c1.green() + c1.blue());



                if (qAbs(diff) <= tolerance || tolerance == 0) {
                    if (ui.diffSame->isChecked() && 
                        (!(c1.red() > 250 && c1.green() > 250 && c1.blue() > 250)))
                    {
                        lineRes[x] = qRgb(255, 255, 255);
                    }
                    else {
                        lineRes[x] = lineBase[x];
                    }
                }
                else {
                    int scaled = static_cast<int>(qBound(0.0, (qAbs(diff) / 765.0) * 255.0, 255.0));

                    if (diff > 0) {
                        lineRes[x] = qRgb(scaled, 0, 0);
                    }
                    else {
                        lineRes[x] = qRgb(0, 0, scaled);
                    }
                }
            }
        }
        ui.lblResult->setPixmap(QPixmap::fromImage(result).scaled(ui.lblResult->size(), Qt::KeepAspectRatio));
    }
}

void diff_image::on_saveImage_clicked()
{
    if (result.isNull()) {
        return;
    }
    ui.tolerance->setNum(0);
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Save image",
        "",
        "Image (*.png *.jpg *.bmp)"
    );

    if (fileName.isEmpty())
        return;

    if (!result.save(fileName)) {
        QMessageBox::critical(this, "Error", "Failed to save image.");
    }
    else {
        QMessageBox::information(this, "Ok", "Image saved successfully!");
    }
}

void diff_image::on_btnSwap_clicked()
{

    QImage temp = img1;
    img1 = img2;
    img2 = temp;

    ui.lblImage1->setPixmap(QPixmap::fromImage(img1).scaled(ui.lblImage1->size(), Qt::KeepAspectRatio));
    ui.lblImage2->setPixmap(QPixmap::fromImage(img2).scaled(ui.lblImage2->size(), Qt::KeepAspectRatio));

    result = QImage();
    ui.lblResult->clear();
}
