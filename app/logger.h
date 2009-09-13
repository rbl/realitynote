#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <QObject>
#include <QString>
#include <QMutex>
#include <QFile>
#include <QTextStream>
#include <QMetaType>
#include <QDir>


class LogListener;
class FileLogListener;

class Logger : public QObject
{
    Q_OBJECT

public:
    typedef enum {
        L_TRACE = 0,
        L_DEBUG = 1,
        L_INFO  = 2,  // This is default if nothing is specified
        L_WARNING  = 3,
        L_ERROR = 4,
        L_NONE  = 5
    } LOG_LEVEL_T;

    static void lprintf(const LOG_LEVEL_T level, const char* format, ...);

    static inline void log(const LOG_LEVEL_T level, const char* msg);
    static inline void log(const LOG_LEVEL_T level, const QString& msg);

    static inline void log(const char* msg);
    static inline void log(const QString& msg);

    // This must be called before anything else 
    static void initializeLogger(
                                 const LOG_LEVEL_T initialLevel = L_TRACE, 
                                 bool enableLocalFile=true, 
                                 bool enableStdOut=true, 
                                 bool enableSysLog=true
    );
    static Logger* instance();

    ///////
    void emitLog(const LOG_LEVEL_T level, const QString& msg) { emit sigLog(level, msg); }

    void registerListener(LogListener* listener);
    void unregisterListener(LogListener* listener);

    void enableStdOut(bool doStdOut) { mStdOutEnabled = doStdOut; }
    bool isStdOutEnabled() { return mStdOutEnabled; }

    void enableSyslog(bool doSysLog);
    bool isSysLogEnabled() { return mSyslogEnabled; }

    void enableLocalFile(bool doFile);
    bool isLocalFileEnabled() { return fileLogger != NULL; }

    void shutdown();

    void setLevel(LOG_LEVEL_T level);
    
public slots:
    void sltLog(const Logger::LOG_LEVEL_T level, const QString& msg);

signals:
    void sigLog(const Logger::LOG_LEVEL_T level, const QString& msg);

private:
    Logger();
    virtual ~Logger();

    bool mSyslogEnabled;
    bool mStdOutEnabled;

    QList<LogListener*> mListeners;
    QMutex mListenersMutex;
    QMutex mFileLoggerMutex;
    FileLogListener* fileLogger;
    QDir dir;

    static Logger* gLoggerRef;
    static Logger::LOG_LEVEL_T gLogLevel;

};

Q_DECLARE_METATYPE(Logger::LOG_LEVEL_T);

////////////////////////////////////
// Inline methods definitions
inline void Logger::log(const LOG_LEVEL_T level, const char* msg)
{
    if (level<Logger::gLogLevel) return;

    if (Logger::gLoggerRef)
    {
        Logger::instance()->emitLog(level, QString(msg));
    }
}



inline void Logger::log(const LOG_LEVEL_T level, const QString& msg)
{
    if (level<Logger::gLogLevel) return;

    if (Logger::gLoggerRef)
    {
        Logger::instance()->emitLog(level, msg);
    }
}



inline void Logger::log(const char* msg)
{
    Logger::log(Logger::L_INFO, msg);
}



inline void Logger::log(const QString& msg)
{
    Logger::log(Logger::L_INFO, msg);
}



////////////////////////////////////

class LogListener
{
public:
    virtual ~LogListener() { };

    virtual void logMessage(const Logger::LOG_LEVEL_T level, const QString& msg) = 0;
    virtual void shutdown() = 0;
};



class FileLogListener
    : public LogListener
{
public:
    FileLogListener();
    virtual ~FileLogListener();

    virtual void logMessage(const Logger::LOG_LEVEL_T level, const QString& msg);
    virtual void shutdown();

    void openFile(QString file);
private:
    bool isShutdown;
    QFile* logFile;
    QTextStream* fileStream;
};





#endif // _LOGGER_H_

