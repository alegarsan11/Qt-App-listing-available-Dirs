#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCamera>
#include <QVideoWidget>
#include <QMediaCaptureSession>
#include <QPushButton>
#include <QDir>
#include <QStringList>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void handleButton();

private:
    QCamera *camera;
    QPushButton *button;
};

#endif // MAINWINDOW_H