#ifndef LOGVIEW_H
#define LOGVIEW_H

#include <QWidget>
#include <QCloseEvent>
#include "ui_logview.h"

#include "logger.h"

class RealityNote;

class Logview : public QWidget, public LogListener
{
    Q_OBJECT

public:
    Logview(RealityNote& rn);
    ~Logview();

    virtual void logMessage(const Logger::LOG_LEVEL_T level, const QString& msg);
    virtual void shutdown();

//    virtual void setAttribute(Qt::WidgetAttribute attribute, bool on = true );
public slots:
	void clear();

protected:
    virtual void showEvent(QShowEvent* event);
    virtual void closeEvent(QCloseEvent* event);

private:
    Ui::Logview ui;
    RealityNote& mRn;

    QColor mColors[(int)Logger::L_NONE];
};

#endif // LOGVIEW_H
