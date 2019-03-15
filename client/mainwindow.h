#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>


namespace Ui {
class mainWindow;
}

class mainWindow : public QDialog
{
    Q_OBJECT

public:
    explicit mainWindow(QWidget *parent = nullptr);
    ~mainWindow();

private slots:
    void on_searchButton_clicked();

    void on_rateButton_clicked();

    void on_downloadButton_clicked();

    void on_quitButton_clicked();

    void on_pushButton_recom_clicked();

    void on_pushButton_refresh_clicked();

private:
    Ui::mainWindow *ui;
    void reject();
};

#endif // MAINWINDOW_H
