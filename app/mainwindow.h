#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "realitynote.h"

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

protected:
    virtual void closeEvent(QCloseEvent* event);

private:
    Ui::MainWindowClass *ui;
    RealityNote& mRn;

};

#endif // MAINWINDOW_H
