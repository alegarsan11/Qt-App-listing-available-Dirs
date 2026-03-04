#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QListWidget>
#include <QLabel>
#include <QTextEdit>
#include <QProcess>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void handleButton();

private:
    QPushButton *button;
    QListWidget *listWidget;
    QLabel *currentFolderLabel;
    QTextEdit *outputWidget;   // <-- add this
    QProcess *process;
};

#endif // MAINWINDOW_H