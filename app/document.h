#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QObject>
#include <QString>
#include <QTextEdit>
#include <QTextDocument>

class MWDocView;

class Document : public QObject
{
public:
    Document();

    inline QString& title() { return mTitle; }
    void setTitle(QString title);

    void addView(MWDocView* view);
    void removeView(MWDocView* view);

    /**
     * Close the document completely. This will cause all views to be removed.
     * The actual close will happen asynchrnously on a small timer so there's
     * no issue unwinding listener stacks.
     */
    virtual void close();

    inline QTextDocument& document() { return mTextDoc; }

public slots:
    void loadFromFile(QString& filename);

private slots:
    /**
     * The actual close method called on the timer that does the real work
     * of closing everything.
     */
    void doClose();

private:
    QString mTitle;
    QList<MWDocView*> mViewList;
    QTextDocument mTextDoc;

};

#endif // DOCUMENT_H
