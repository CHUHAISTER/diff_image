#include "diff_image.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    qputenv("QT_QPA_PLATFORM", "windows:renderer=software");
    qputenv("QT_OPENGL", "software");
    QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);

    QApplication app(argc, argv);

    diff_image window;
    window.show();
    return app.exec();
}
