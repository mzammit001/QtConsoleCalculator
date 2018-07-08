#include "mainwindow.h"

using namespace TextResource;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle(QString(UIString::WindowTitle));
}
