#include "mainwindow.h"

#include <QApplication>
using namespace std;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow *w = new MainWindow();
    w->init_ui();
    w->init_toolbar();
    w->showMaximized();
    return a.exec();
}

