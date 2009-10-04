#ifndef REALITYNOTE_H
#define REALITYNOTE_H

#include <QObject>
#include "settings.h"
#include "logview.h"

class MainWindow;
class Document;

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
    void newDocument(MainWindow& win);
    void openExisting(MainWindow& win);

signals:
    void timeToExit();

private:
    Logview mLogView;
    QList<MainWindow*> mMainWinList;
    QList<Document*> mDocList;

    virtual void newMainWindow();
    virtual void doExit();

};

#endif // REALITYNOTE_H
