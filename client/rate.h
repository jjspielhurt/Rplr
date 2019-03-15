#ifndef RATE_H
#define RATE_H

#include <QDialog>

namespace Ui {
class rate;
}

class rate : public QDialog
{
    Q_OBJECT

public:
    explicit rate(QWidget *parent = nullptr);
    ~rate();

private slots:
    void on_pushButton_5_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_1_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::rate *ui;
};

#endif // RATE_H
