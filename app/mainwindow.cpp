#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QtGui>

MainWindow::MainWindow(RealityNote& rn, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindowClass)
    , mRn(rn)
{
    ui->setupUi(this);
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionShowLog, SIGNAL(triggered()), &mRn, SLOT(showLog()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    qDebug() << "MainWindow::closeEvent()";

    // For now always accept
    event->accept();

    // But in the future, we might ignore
    // event->ignore();

    // Tell our parent we have closed
    mRn.mainWindowClosed(*this);
}

