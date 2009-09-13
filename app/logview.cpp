#include "constants.h"
#include "logview.h"
#include "realitynote.h"

#include <QTextBlock>
#include <QScrollBar>
#include <QFont>
#include <QWidget>

Logview::Logview(RealityNote& rn)
    : QWidget(0)
    , mRn(rn)
{
    ui.setupUi(this);

    connect(ui.clearButton, SIGNAL(clicked()), SLOT(clear()));
    connect(ui.closeButton, SIGNAL(clicked()), SLOT(close()));
    ui.logFileEdit->setReadOnly(true);

    // Set a maximum number of lines that it will hold. Lines get 
    // deleted from the top
    ui.logFileEdit->document()->setMaximumBlockCount(2000);

    // Initialize the formatting for the various log levels
    mColors[Logger::L_TRACE].setRgb(128,128,128);
    mColors[Logger::L_DEBUG].setRgb(128,128,0);
    mColors[Logger::L_INFO].setRgb(0,0,0);
    mColors[Logger::L_WARNING].setRgb(0,0,255);
    mColors[Logger::L_ERROR].setRgb(255,0,0);

#ifdef Q_WS_MAC
    // Change the font properties on the Mac
    QFont font = ui.logFileEdit->font();
    font.setStyleStrategy(QFont::NoAntialias);
    font.setFamily(QString("Monaco"));
    font.setPointSize(10.0);
//    ui.logFileEdit->setFontFamily(QString("Courier"));
//    ui.logFileEdit->setFontPointSize(10.0);
    ui.logFileEdit->setFont(font);
    
    //QWidget::setAttribute(Qt::WA_MacBrushedMetal);
#endif // #ifdef Q_WS_MAC
}



Logview::~Logview()
{
}



void Logview::clear()
{
    ui.logFileEdit->clear();
}



void Logview::logMessage(const Logger::LOG_LEVEL_T level, const QString& msg)
{
    QScrollBar* pBar = ui.logFileEdit->verticalScrollBar();
    bool atBottom = true;
    if (pBar)
    {
        if (pBar->value() < pBar->maximum())
        {
            atBottom = false;
        }
    }

    // Remove an excess of blocks
    QTextDocument* doc = ui.logFileEdit->document();
    if (doc->blockCount() >= 1999)
    {
        doc->setMaximumBlockCount(1000);
        doc->setMaximumBlockCount(2000);
    }

    QTextCursor cur = ui.logFileEdit->textCursor();


    cur.movePosition(QTextCursor::End);

    QTextCharFormat fmt = cur.charFormat();
    fmt.setForeground(QBrush(mColors[level]));
    cur.insertText(msg,fmt);

    cur.insertBlock();

    // Re-get the bar in case it just got added
    if (atBottom)
    {
        pBar = ui.logFileEdit->verticalScrollBar();
        if (pBar)
        {
            pBar->setValue(pBar->maximum());
        }
    }
}



void Logview::shutdown()
{
}



void Logview::showEvent(QShowEvent* event)
{
    mRn.mSettings.restoreWindowPosition("log",*this);
}



void  Logview::closeEvent(QCloseEvent* event)
{
    mRn.mSettings.saveWindowPosition("log",*this);
}
