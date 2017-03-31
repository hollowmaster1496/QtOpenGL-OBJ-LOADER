#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "qglbegin.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnOpenFile_clicked()
{
    QString strFileName = QFileDialog::getOpenFileName();       // bring up open file dialog
    QStringList comments;
    QVector<QOpenGLTriangle3D> funny;


    if(strFileName == "") {                                     // if file was not chosen
        ui->lblChosenFile->setText("file not chosen");          // update label
        return;                                                 // and exit function
    }

    ui->lblChosenFile->setText(strFileName);

    ui->openGLWidget->parseObjFile(strFileName, comments, funny);

}

void MainWindow::on_x_rot_slider_valueChanged(int value)
{
    Q_UNUSED(value);
    ui->openGLWidget->setRotation(
                ui->x_rot_slider->value(),
                ui->y_rot_slider->value(),
                ui->z_rot_slider->value()
                );
}

void MainWindow::on_y_rot_slider_valueChanged(int value)
{
    Q_UNUSED(value);
    ui->openGLWidget->setRotation(
                ui->x_rot_slider->value(),
                ui->y_rot_slider->value(),
                ui->z_rot_slider->value()
                );
}

void MainWindow::on_z_rot_slider_valueChanged(int value)
{
    Q_UNUSED(value);
    ui->openGLWidget->setRotation(
                ui->x_rot_slider->value(),
                ui->y_rot_slider->value(),
                ui->z_rot_slider->value()
                );
}
