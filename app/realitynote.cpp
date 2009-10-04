#include "realitynote.h"
#include "mainwindow.h"
#include "logger.h"
#include "document.h"

#include <QtGui/QWidget>
#include <QFileDialog>

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

    //connect(win, SIGNAL(destroyed(QObject*)), SLOT(mainWindowClosed(QObject*)));
    mSettings.restoreWindowPosition(QString("main"), *win);
    win->show();

    mMainWinList << win;

    // Add a new blank document into this new main window
    Document* doc = new Document();
    mDocList << doc;

    win->addDocument(*doc);
}


void RealityNote::newDocument(MainWindow& win)
{
    // Add a new blank document into this new main window
    Document* doc = new Document();
    mDocList << doc;

    win.addDocument(*doc);
}

void RealityNote::openExisting(MainWindow& win)
{
    QString filename = QFileDialog::getOpenFileName(NULL, tr("Open File"), "",
                        "Text Files *.txt, *.log (*.txt, *.log);;Any File (*)");

    if (!filename.isNull())
    {
        // Add a new blank document into this new main window
        Document* doc = new Document();
        mDocList << doc;

        doc->loadFromFile(filename);
        win.addDocument(*doc);
    }
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
