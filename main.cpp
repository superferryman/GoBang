#include "mainwindow.h"
#include "menu.h"
#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Menu w;

//    MainWindow w;
    w.setWindowTitle("五子棋小游戏");
    w.move((QApplication::desktop()->width()-w.width())/2, (QApplication::desktop()->height()-w.height())/2);
    w.show();

    return a.exec();
}
