#include "mainwindow.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QDir>
#include <QListWidget>
#include <QDebug>
#include <QProcess>

char* absDir = "/home/alejandro/";
char* scriptName = "/run.sh";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      button(new QPushButton("&Launch Sim", this))
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    listWidget = new QListWidget(this);

    setWindowTitle("Sim Selector");
    resize(640, 480);

    // Fill the list with folders
    QDir directory(absDir);
    QStringList folders = directory.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString &folder : folders) {
        listWidget->addItem(folder);
    }

    layout->addWidget(listWidget);
    layout->addWidget(button);

    // Persistent process
    process = new QProcess(this);

    // Capture stdout
    connect(process, &QProcess::readyReadStandardOutput, this, [=]() {
        QByteArray output = process->readAllStandardOutput();
        qDebug() << "STDOUT:" << output;
    });

    // Capture stderr
    connect(process, &QProcess::readyReadStandardError, this, [=]() {
        QByteArray error = process->readAllStandardError();
        qDebug() << "STDERR:" << error;
    });

    connect(button, &QPushButton::clicked, this, &MainWindow::handleButton);
}

void MainWindow::handleButton()
{
    QListWidgetItem *item = listWidget->currentItem();
    if (!item) {
        qDebug() << "No folder selected!";
        return;
    }

    QDir baseDir(absDir);
    QString folderPath = baseDir.filePath(item->text());
    QString program = folderPath + scriptName; // could be any executable

    if (!QFile::exists(program)) {
        qDebug() << "File does not exist:" << program;
        return;
    }

    process->setWorkingDirectory(folderPath);
    process->start(program); // uses OS to execute file directly


    if (!process->waitForStarted(1000)) {
        qDebug() << "Failed to start:" << program;
    } else {
        qDebug() << "Started:" << program;
    }
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}