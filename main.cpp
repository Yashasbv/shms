#include <QMainWindow>
#include <QApplication>
#include <mainwindow.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QMainWindow window;
    window.setCentralWidget(new MainWindow);
    window.setWindowTitle("Smart Hospital Management System");
    window.resize(800, 500);
    window.show();
    return app.exec();
}
