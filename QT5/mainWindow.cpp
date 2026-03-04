#include "mainwindow.h"
#include <QVBoxLayout>
#include <QDir>
#include <QDebug>
#include <QFile>

char* absDir = "/home/alejandro/"; // change to your base path
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

    // Fill the list with directories
    QDir directory(absDir);
    QStringList folders = directory.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString &folder : folders) {
        listWidget->addItem(folder);
    }

    layout->addWidget(listWidget);
    layout->addWidget(button);

    // Persistent process
    process = new QProcess(this);

    connect(process, &QProcess::readyReadStandardOutput, this, [=]() {
        QByteArray output = process->readAllStandardOutput();
        qDebug() << "STDOUT:" << output;
    });

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

#ifdef Q_OS_WIN
    process->start(program);
#else
    process->start(program); // Linux/macOS
#endif

    if (!process->waitForStarted(1000)) {
        qDebug() << "Failed to start:" << program;
    } else {
        qDebug() << "Started:" << program;
    }
}