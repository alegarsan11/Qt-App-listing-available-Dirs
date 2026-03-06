#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QTextEdit>
#include <QProcess>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void handleButton();

private:

    /* UI */

    QPushButton *button;
    QLabel *currentFolderLabel;
    QListWidget *listWidget;
    QListWidget *listPWidget;
    QTextEdit *outputWidget;
    QProcess *process;

    /* Helper functions */

    QString getSelectedSimFolder();
    QString getSelectedAuxFile();

    bool moveFileWithoutExtension(QString src, QString dstFolder);

    void populateSimFolders();
    void populateAuxFolders();
};

#endif