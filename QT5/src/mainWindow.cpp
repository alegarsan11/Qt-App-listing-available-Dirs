#include "mainwindow.h"
#include <QVBoxLayout>
#include <QDir>
#include <QDebug>
#include <QFile>
#include <QListWidgetItem>
#include <QTextEdit>

// Global base directory and script name
QString absDir = "/home/alejandro/";
QString scriptName = "run.sh";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      button(new QPushButton("&Launch Sim", this))
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    // Label on top
    currentFolderLabel = new QLabel(QString("Current folder: %1").arg(absDir), this);
    layout->addWidget(currentFolderLabel);

    // Folder list
    listWidget = new QListWidget(this);
    layout->addWidget(listWidget);

    // Output widget (hidden by default)
    outputWidget = new QTextEdit(this);
    outputWidget->setReadOnly(true);
    outputWidget->hide(); // only show when running a script
    layout->addWidget(outputWidget);

    // Launch button
    layout->addWidget(button);

    setWindowTitle("Sim Selector");
    resize(640, 480);

    // Fill list with directories
    QDir directory(absDir);
    QStringList folders = directory.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString &folder : folders)
        listWidget->addItem(folder);

    // Update label on selection change
    connect(listWidget, &QListWidget::currentItemChanged, this,
        [=](QListWidgetItem *current, QListWidgetItem *) {
            if (current)
                currentFolderLabel->setText(QString("Current folder: %1/%2").arg(absDir, current->text()));
            else
                currentFolderLabel->setText(QString("Current folder: %1").arg(absDir));
        }
    );

    // Persistent process
    process = new QProcess(this);

    // Capture stdout/stderr into the output widget
    connect(process, &QProcess::readyReadStandardOutput, this, [=]() {
        outputWidget->append(QString::fromUtf8(process->readAllStandardOutput()));
    });
    connect(process, &QProcess::readyReadStandardError, this, [=]() {
        outputWidget->append(QString::fromUtf8(process->readAllStandardError()));
    });

    // When process finishes, optionally hide the output widget
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        this, [=](int exitCode, QProcess::ExitStatus status){
            outputWidget->append(QString("\nProcess finished with code %1").arg(exitCode));
            // outputWidget->hide(); // remove hide if you want it to stay
        });

    // Button click
    connect(button, &QPushButton::clicked, this, &MainWindow::handleButton);
}

void MainWindow::handleButton()
{
    QListWidgetItem *item = listWidget->currentItem();
    QString folderPath = item ? QString("%1/%2").arg(absDir, item->text()) : absDir;
    QString program = folderPath + "/" + scriptName;

    if (!QFile::exists(program)) {
        outputWidget->show();
        outputWidget->append("File does not exist: " + program);
        return;
    }

    // Clear previous output and show widget
    outputWidget->clear();
    outputWidget->show();

    process->setWorkingDirectory(folderPath);
    process->start(program);

    if (!process->waitForStarted(1000))
        outputWidget->append("Failed to start: " + program);
    else
        outputWidget->append("Started: " + program);
}