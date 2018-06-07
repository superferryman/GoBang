#include "about.h"
#include "ui_about.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QLabel>
#include <QPixmap>
#define AUTHOR_PIC ":/res/sound/avatar.jpg"

ABout::ABout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ABout)
{
    ui->setupUi(this);
//    this->setFixedSize(200, 200);
    this->move((QApplication::desktop()->width()-this->width())/2, (QApplication::desktop()->height()-this->height())/2);
    this->setWindowTitle("关于作者");
    this->setWindowModality(Qt::ApplicationModal);
//    QLabel *author = new QLabel;
//    QPixmap pixMap("AUTHOR_PIC");
//    author->setPixmap(pixMap);//初始化一个默认图片,可按需要调用这个接口改变label的图片
//    author->setGeometry(10, 10, 10, 10);
    ui->about->setStyleSheet("color:brown");
    ui->label_2->setStyleSheet("color:red");
//    ui->label_3->setStyleSheet("color:red");
//    ui->label_4->setStyleSheet("color:red");
//    ui->label_5->setStyleSheet("color:blue");
}

ABout::~ABout()
{
    delete ui;
}
