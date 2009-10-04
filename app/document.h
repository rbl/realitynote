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

    inline QTextDocument& document() { return mTextDoc; }

public slots:
    void loadFromFile(QString& filename);

private:
    QString mTitle;
    QList<MWDocView*> mViewList;
    QTextDocument mTextDoc;

};

#endif // DOCUMENT_H
