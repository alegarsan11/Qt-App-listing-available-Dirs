#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QProcess>
#include <QMainWindow>
#include <QCamera>
#include <QVideoWidget>
#include <QMediaCaptureSession>
#include <QPushButton>
#include <QDir>
#include <QListWidget>
#include <QStringList>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void handleButton();

private:
    QPushButton *button;
    QListWidget *listWidget;  
    QProcess *process;   
};
#endif // MAINWINDOW_H