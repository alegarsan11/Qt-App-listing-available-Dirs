#include "mainwindow.h"
#include <QVBoxLayout>
#include <QDir>
#include <QDebug>
#include <QFile>
#include <QApplication>

QString absDir = "/home/alejandro/"; // base directory
QString scriptName = "run.sh";   // <-- global script name
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      button(new QPushButton("&Launch Sim", this))
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    listWidget = new QListWidget(this);
    currentFolderLabel = new QLabel(QString("Current folder: %1").arg(absDir), this);

    setWindowTitle("Sim Selector");
    resize(640, 480);

    QDir directory(absDir);
    QStringList folders = directory.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString &folder : folders)
        listWidget->addItem(folder);

    layout->addWidget(currentFolderLabel);
    layout->addWidget(listWidget);
    layout->addWidget(button);

    connect(listWidget, &QListWidget::currentItemChanged, this,
        [=](QListWidgetItem *current, QListWidgetItem *) {
            if (current)
                currentFolderLabel->setText(
                    QString("Current folder: %1""%2").arg(absDir, current->text()));
            else
                currentFolderLabel->setText(
                    QString("Current folder: %1").arg(absDir));
        }
    );

    process = new QProcess(this);
    connect(process, &QProcess::readyReadStandardOutput, this, [=]() {
        qDebug() << "STDOUT:" << process->readAllStandardOutput();
    });
    connect(process, &QProcess::readyReadStandardError, this, [=]() {
        qDebug() << "STDERR:" << process->readAllStandardError();
    });

    connect(button, &QPushButton::clicked, this, &MainWindow::handleButton);
}

void MainWindow::handleButton()
{
    QListWidgetItem *item = listWidget->currentItem();
    QString folderPath;

    if (item)
        folderPath = QString("%1/%2").arg(absDir, item->text());
    else
        folderPath = absDir;

    QString program = folderPath + "/" + scriptName;  // use the global variable

    if (!QFile::exists(program)) {
        qDebug() << "File does not exist:" << program;
        return;
    }

    process->setWorkingDirectory(folderPath);
    process->start(program);

    if (!process->waitForStarted(1000))
        qDebug() << "Failed to start:" << program;
    else
        qDebug() << "Started:" << program;
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}