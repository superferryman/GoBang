#ifndef MENU_H
#define MENU_H

#include <QMainWindow>
#include "about.h"
#include "mainwindow.h"

namespace Ui {
class Menu;
}

class Menu : public QMainWindow
{
    Q_OBJECT

public:
    explicit Menu(QMainWindow *parent = 0);
    ~Menu();

private slots:
    void on_PVP_clicked();

    void on_exit_clicked();

    void on_About_clicked();

    void choose_method_1();

    void choose_method_2();

private:
    Ui::Menu *ui;
    MainWindow *game = new MainWindow;
    MainWindow *fun = new MainWindow;
//    MainWindow *PVC = new MainWindow;
    ABout *about = new ABout;
};

#endif // MENU_H
