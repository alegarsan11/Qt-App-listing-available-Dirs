#include "mainwindow.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QDir>
#include <QListWidget>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      button(new QPushButton("&Launch Sim", this))
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    // List widget to display directories
    QListWidget *listWidget = new QListWidget(this);

    setWindowTitle("Sim Selector");
    resize(640, 480);

    // Create QDir properly
    QDir directory("/home/alejandro/");

    // Get only folders
    QStringList folders = directory.entryList(
        QDir::Dirs | QDir::NoDotAndDotDot
    );

    // Add folders to list widget
    for (const QString &folder : folders) {
        listWidget->addItem(folder);
    }

    layout->addWidget(listWidget);
    layout->addWidget(button);
}

void MainWindow::handleButton()
{
    qDebug() << "Button clicked!";
}


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}