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

    /**
     * This closes the document itself, which in turn will cause
     * all views of that doc to get closed.
     *
     * If the document is changed the user will be prompted and the
     * actual close may not occur. If the user cancels the close
     * operation this function returns false, otherwise it returns
     * true.
     */
    virtual bool closeDoc();

    /**
     * Only closes this single view. If other views are open, the
     * doc stays open. If this is the last view, the doc will
     * close.
     *
     * If the document is changed the user will be prompted and the
     * actual close may not occur. If the user cancels the close
     * operation this function returns false, otherwise it returns
     * true.
     */
    virtual bool closeView();
};

#endif // MWDOCVIEW_H
