#include "document.h"
#include "mwdocview.h"
#include <QDebug>
#include <QFile>
#include <QDir>

Document::Document()
: mTitle("Untitled")
{
}


void Document::setTitle(QString title)
{
    mTitle = title;

    // Should broadcast a change event to all views that are listening
    QListIterator<MWDocView*> it(mViewList);
    while(it.hasNext())
    {
        it.next()->docTitleChanged(title);
    }
}


void Document::addView(MWDocView* view)
{
    mViewList << view;
}


void Document::removeView(MWDocView* view)
{
    mViewList.removeOne(view);
}


void Document::loadFromFile(QString& filename)
{
    qDebug() << "Document::loadFromFile(" << filename << ")";

    QFile file(filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        mTextDoc.setPlainText(QString(file.readAll()));

        file.close();

        setTitle(QDir(filename).dirName());
    }
}
