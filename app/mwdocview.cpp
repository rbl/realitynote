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
