#include <QtGui/QApplication>
#include <QTimer>

#include "realitynote.h"
#include "logger.h"

static bool gGotLogger = false;

 void messageHandler(QtMsgType type, const char *msg)
 {
    switch (type) {
    case QtDebugMsg:
        if (gGotLogger)
        {
            Logger::log(Logger::L_DEBUG, msg);
        }
        else
        {
            fprintf(stderr, "Debug   : %s\n", msg);
        }
        break;
    case QtWarningMsg:
        if (gGotLogger)
        {
            Logger::log(Logger::L_INFO, msg);
        }
        else
        {
            fprintf(stderr, "Warning : %s\n", msg);
        }
        break;
    case QtCriticalMsg:
        if (gGotLogger)
        {
            Logger::log(Logger::L_WARNING, msg);
        }
        else
        {
            fprintf(stderr, "Critical: %s\n", msg);
        }
        break;
    case QtFatalMsg:
        if (gGotLogger)
        {
            Logger::log(Logger::L_ERROR, msg);
        }
        else
        {
            fprintf(stderr, "FATAL   : %s\n", msg);
        }
        abort();
        break;
    }
 }


int main(int argc, char *argv[])
{
    qInstallMsgHandler(messageHandler);

    QApplication a(argc, argv);

    // We can't initialize the logger until after the QApplication has
    // been initialized because it might throw up a message box if it can't
    // open the log file
    Logger::initializeLogger();
    gGotLogger = true;

    Logger::lprintf(Logger::L_TRACE,"Have %d parameters",argc);
    for(int i=0; i<argc; i++)
    {
        Logger::lprintf(Logger::L_TRACE,"argv[%d]='%s'",i,argv[i]);
    }

    RealityNote rn(argc, argv);
    QObject::connect(&rn, SIGNAL(timeToExit()), &a, SLOT(quit()));

    // Prepare an event which will fire when the event loop is begun
    QTimer::singleShot(0, &rn, SLOT(initialize()));

    Logger::log(Logger::L_TRACE,"-- Starting application exec loop --");
    return a.exec();
}
