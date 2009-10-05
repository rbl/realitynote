#include "mwdocview.h"

MWDocView::MWDocView(MainWindow& win, Document& doc)
: mWin(win)
, mDoc(doc)
{
    mDoc.addView(this);
    mTextEdit.setDocument(&mDoc.document());
}

MWDocView::~MWDocView()
{
}

void MWDocView::docClosed()
{
    mWin.docViewClosed(this);
}

void MWDocView::docTitleChanged(QString& title)
{
    mWin.docViewTitleChanged(*this, title);
}

bool MWDocView::closeDoc()
{
    // Close the doc for everyone

    // Check for changes and a potential cancel of the close

    // Okay to go ahead and do the close
    mDoc.close();

    return true;
}

bool MWDocView::closeView()
{
    // Just remove ourself from the doc. This might
    // result in the document itself being closed if we were the
    // last view.

    // Before that though, we need to make sure that there are
    // no changes if we are the last view.

    // Okay to remove ourself
    mDoc.removeView(this);
    mWin.docViewClosed(this);

    return true;
}

