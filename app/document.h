#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QObject>
#include <QString>
#include <QTextEdit>

class Document : public QObject
{
    Q_OBJECT

public:
    Document();
    inline QWidget* getWidget() {return &textEdit;}

public slots:
    void loadFromFile(QString filename);

private:
    QTextEdit textEdit;

};

#endif // DOCUMENT_H
