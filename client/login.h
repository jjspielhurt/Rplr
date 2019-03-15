#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include <QCloseEvent>
namespace Ui {
class login;
}

class login : public QMainWindow
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();

private slots:
    void on_loginButton_clicked();
    
private:
    Ui::login *ui;
    void closeEvent(QCloseEvent *event);
};

#endif // LOGIN_H
