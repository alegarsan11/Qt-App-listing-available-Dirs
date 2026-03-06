#include <QApplication>
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QListWidgetItem>
#include <QRegularExpression>

/* Base directories */

QString absDir = "/home/alejandro/";
QString auxDir = "/home/alejandro/Imágenes/";
QString scriptName = "run.sh";


/* ------------------------------------------------ */
/*                 Helper Functions                 */
/* ------------------------------------------------ */

QString MainWindow::getSelectedSimFolder()
{
    QListWidgetItem *item = listWidget->currentItem();

    if (!item)
        return "";

    return absDir + "/" + item->text();
}


/* find the .le file inside the selected aux folder */

QString MainWindow::getSelectedAuxFile()
{
    QListWidgetItem *item = listPWidget->currentItem();

    if (!item)
        return "";

    QString folderPath = auxDir + "/" + item->text();

    QDir dir(folderPath);

    QStringList files = dir.entryList(QStringList() << "*.le", QDir::Files);

    if (files.isEmpty())
        return "";

    return folderPath + "/" + files.first();
}


/* copy file removing only the .le extension */

bool MainWindow::moveFileWithoutExtension(QString src, QString dstFolder)
{
    QFileInfo info(src);

    /* removes only last extension (.le) */
    QString newName = info.completeBaseName();

    QString dst = dstFolder + "/" + newName;

    if (!QFile::exists(src))
        return false;

    QFile::remove(dst);     // overwrite if exists
    return QFile::copy(src, dst);
}


/* fill simulation list */

void MainWindow::populateSimFolders()
{
    QDir directory(absDir);

    QStringList folders = directory.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    for (const QString &folder : folders)
        listWidget->addItem(folder);
}


/* fill aux list (only folders containing .le files) */

void MainWindow::populateAuxFolders()
{
    QDir directory(auxDir);

    QStringList folders = directory.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    for (const QString &folder : folders)
    {
        QDir sub(auxDir + "/" + folder);

        QStringList files = sub.entryList(QStringList() << "*.le", QDir::Files);

        if (!files.isEmpty())
            listPWidget->addItem(folder);
    }
}


/* ------------------------------------------------ */
/*                 Main Window Setup                */
/* ------------------------------------------------ */

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      button(new QPushButton("&Launch Sim", this))
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *layout = new QVBoxLayout(centralWidget);


    /* current folder label */

    currentFolderLabel =
        new QLabel(QString("Current folder: %1").arg(absDir), this);

    layout->addWidget(currentFolderLabel);


    /* simulation folders list */

    listWidget = new QListWidget(this);
    layout->addWidget(listWidget);


    /* aux folders list */

    listPWidget = new QListWidget(this);
    layout->addWidget(listPWidget);


    /* output terminal */

    outputWidget = new QTextEdit(this);
    outputWidget->setReadOnly(true);
    outputWidget->hide();
    layout->addWidget(outputWidget);


    /* launch button */

    layout->addWidget(button);


    setWindowTitle("Sim Selector");
    resize(640, 480);


    /* fill lists */

    populateSimFolders();
    populateAuxFolders();


    /* update label when sim folder changes */

    connect(listWidget,
            &QListWidget::currentItemChanged,
            this,
            [=](QListWidgetItem *current, QListWidgetItem *)
            {
                if (current)
                    currentFolderLabel->setText(
                        QString("Current folder: %1/%2")
                        .arg(absDir, current->text()));
                else
                    currentFolderLabel->setText(
                        QString("Current folder: %1").arg(absDir));
            });


    /* process to run script */

    process = new QProcess(this);


    connect(process,
            &QProcess::readyReadStandardOutput,
            this,
            [=]()
            {
                outputWidget->append(
                    QString::fromUtf8(process->readAllStandardOutput()));
            });


    connect(process,
            &QProcess::readyReadStandardError,
            this,
            [=]()
            {
                outputWidget->append(
                    QString::fromUtf8(process->readAllStandardError()));
            });


    connect(process,
            QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this,
            [=](int exitCode, QProcess::ExitStatus)
            {
                outputWidget->append(
                    QString("\nProcess finished with code %1")
                    .arg(exitCode));
            });


    connect(button,
            &QPushButton::clicked,
            this,
            &MainWindow::handleButton);
}


/* ------------------------------------------------ */
/*                    Button Logic                  */
/* ------------------------------------------------ */

void MainWindow::handleButton()
{
    QString simFolder = getSelectedSimFolder();
    QString auxFile = getSelectedAuxFile();

    QString script = simFolder + "/" + scriptName;

    outputWidget->clear();
    outputWidget->show();


    if (simFolder.isEmpty())
    {
        outputWidget->append("No simulation folder selected.");
        return;
    }

    if (auxFile.isEmpty())
    {
        outputWidget->append("No .le file found in selected aux folder.");
        return;
    }


    if (!moveFileWithoutExtension(auxFile, simFolder))
    {
        outputWidget->append("Failed copying file.");
        return;
    }

    outputWidget->append("File copied successfully.");


    if (!QFile::exists(script))
    {
        outputWidget->append("run.sh not found.");
        return;
    }


    process->setWorkingDirectory(simFolder);

    process->start(script, QStringList(), QIODevice::ReadWrite);


    if (!process->waitForStarted(1000))
        outputWidget->append("Failed to start run.sh");
    else
        outputWidget->append("Simulation started.");
}


/* ------------------------------------------------ */
/*                       main()                     */
/* ------------------------------------------------ */

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}