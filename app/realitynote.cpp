#include "realitynote.h"
#include "mainwindow.h"
#include "logger.h"

#include <QtGui/QWidget>

RealityNote::RealityNote(int argc, char** argv)
        : mLogView(*this)
{
    Logger::instance()->registerListener(&mLogView);
}


RealityNote::~RealityNote()
{
}



void RealityNote::initialize()
{
    // Create a single main window and show it
    newMainWindow();
}



void RealityNote::newMainWindow()
{
    MainWindow* win = new MainWindow(*this);

    connect(win, SIGNAL(destroyed(QObject*)), SLOT(mainWindowClosed(QObject*)));
    mSettings.restoreWindowPosition(QString("main"), *win);
    win->show();
}



void RealityNote::showLog()
{
    mLogView.show();
}



void RealityNote::mainWindowClosed(MainWindow& window)
{
    mSettings.saveWindowPosition(QString("main"), window);

    doExit();
}


void RealityNote::doExit()
{
    mLogView.close();
    mSettings.save();

    emit timeToExit();
}
