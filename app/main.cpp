#include <QtGui/QApplication>
#include "mainwindow.h"

 void messageHandler(QtMsgType type, const char *msg)
 {
     switch (type) {
     case QtDebugMsg:
         fprintf(stderr, "Debug   : %s\n", msg);
         break;
     case QtWarningMsg:
         fprintf(stderr, "Warning : %s\n", msg);
         break;
     case QtCriticalMsg:
         fprintf(stderr, "Critical: %s\n", msg);
         break;
     case QtFatalMsg:
         fprintf(stderr, "FATAL   : %s\n", msg);
         abort();
     }
 }


int main(int argc, char *argv[])
{
    qInstallMsgHandler(messageHandler);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
