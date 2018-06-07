#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>

namespace Ui {
class ABout;
}

class ABout : public QDialog
{
    Q_OBJECT

public:
    explicit ABout(QWidget *parent = 0);
    ~ABout();

private:
    Ui::ABout *ui;
};

#endif // ABOUT_H
