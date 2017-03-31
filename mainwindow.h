#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
    void on_btnOpenFile_clicked();
    void on_x_rot_slider_valueChanged(int value);
    void on_y_rot_slider_valueChanged(int value);
    void on_z_rot_slider_valueChanged(int value);
};

#endif // MAINWINDOW_H
