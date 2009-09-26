#ifndef MWDOCVIEW_H
#define MWDOCVIEW_H

#include <QObject>

#include "mainwindow.h"
#include "document.h"

class MWDocView : public QObject
{
public:
    MainWindow &mWin;
    Document &mDoc;
    QTextEdit mTextEdit;

    MWDocView(MainWindow& win, Document& doc);
    virtual ~MWDocView();

    virtual void docClosed();
    virtual void docTitleChanged(QString& title);
};

#endif // MWDOCVIEW_H
