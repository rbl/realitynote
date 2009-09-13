#ifndef REALITYNOTE_H
#define REALITYNOTE_H

#include <QObject>
#include "settings.h"
#include "logview.h"

class MainWindow;

class RealityNote : public QObject
{
    Q_OBJECT

public:
    RealityNote(int argc, char** argv);
    virtual ~RealityNote();

    Settings mSettings;

    void mainWindowClosed(MainWindow& window);

public slots:
    void initialize();
    void showLog();

signals:
    void timeToExit();

protected:
    Logview mLogView;

    virtual void newMainWindow();
    virtual void doExit();

};

#endif // REALITYNOTE_H
