#include "document.h"
#include <QDebug>

Document::Document()
        : textEdit()
{
}


void Document::loadFromFile(QString filename)
{
    qDebug() << "Document::loadFromFile(" << filename << ")";

}
