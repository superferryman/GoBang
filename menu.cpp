#include "menu.h"
#include "about.h"
#include "mainwindow.h"
#include "ui_menu.h"
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QPushButton>
#define background ":/res/sound/wuziqi.jepg"

Menu::Menu(QMainWindow *parent) :
    QMainWindow(parent),
    ui(new Ui::Menu)
{
    ui->setupUi(this);
//    ui->PVP->setStyleSheet("color:white");
//    ui->PVC->setStyleSheet("color:white");
//    ui->About->setStyleSheet("color:white");
//    ui->exit->setStyleSheet("color:white");
    ui->label_2->setStyleSheet("color:brown");

    // 添加菜单
    QMenuBar *mbar = this->menuBar();
    QMenu *gameMenu = new QMenu("Game");
    QAction *actionPVP = new QAction("Person VS Person", this);
    connect(actionPVP, SIGNAL(triggered()), this, SLOT(on_PVP_clicked()));
    gameMenu->addAction(actionPVP);

    QAction *Method_1 = new QAction("Computer 1", this);
    QAction *Method_2 = new QAction("Computer 2", this);
    QMenu *secondMenu = new QMenu("Person VS Computer");
    gameMenu->addMenu(secondMenu);
    connect(Method_1, SIGNAL(triggered()), this, SLOT(choose_method_1()));
    connect(Method_2, SIGNAL(triggered()), this, SLOT(choose_method_2()));
    secondMenu->addAction(Method_1);
    secondMenu->addAction(Method_2);

    QMenu *help = new QMenu("Help");
    QAction *about = new QAction("About Author");
    connect(about, SIGNAL(triggered()), this, SLOT(on_About_clicked()));
    help->addAction(about);

    mbar->addMenu(gameMenu);
    mbar->addMenu(help);
}

Menu::~Menu()
{
    delete ui;
}

void Menu::on_PVP_clicked()
{
    this->close();
    game->show();
    game->initPVPGame();
}

void Menu::on_exit_clicked()
{
    this->close();
}

void Menu::on_About_clicked()
{
    about->show();
}

void Menu::choose_method_1()
{
    this->hide();
    game->show();
    game->initPVCGame(1);
}

void Menu::choose_method_2()
{
    this->hide();
    game->show();
    game->initPVCGame(2);
}
