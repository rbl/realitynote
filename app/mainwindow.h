#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "realitynote.h"

class MWDocView;

namespace Ui
{
    class MainWindowClass;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(RealityNote& rn, QWidget *parent = 0);
    ~MainWindow();
    virtual void addDocument(Document& doc);
    virtual void docViewClosed(MWDocView* view);
    virtual void docViewTitleChanged(MWDocView& view, QString& title);

public slots:
    virtual void newDocument();
    virtual void openExisting();

protected:
    virtual void closeEvent(QCloseEvent* event);

private:
    Ui::MainWindowClass *ui;
    RealityNote& mRn;

    QList<MWDocView*> mDocViewList;
};



#endif // MAINWINDOW_H
