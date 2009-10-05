#include "constants.h"

#include <QMessageBox>
#include <QMutexLocker>
#include <QDateTime>


#if !defined(Q_OS_WIN32)
#include <syslog.h>
#endif

#include <assert.h>
#include <stdio.h>

#ifdef Q_WS_MAC
#include <sys/types.h>
#include <pwd.h>
#endif // #ifdef Q_WS_MAC

#include "logger.h"

///////////////
// Singleton
Logger* Logger::gLoggerRef = NULL;
Logger::LOG_LEVEL_T Logger::gLogLevel = Logger::L_DEBUG;


////////////////
// Statics


void Logger::lprintf(const LOG_LEVEL_T level, const char* format, ...)
{
    if (level<Logger::gLogLevel) return;

    va_list args;
    va_start(args, format);

    /* Guess we need no more than 256 bytes. */
    int n, size = 256;
    char *p;

    p = (char*) malloc(size) ;

    while (p != NULL)
    {
        /* Try to print in the allocated space. */
#ifdef _WIN32
        n = _vsnprintf (p, size, format, args);
#else
        n = vsnprintf (p, size, format, args);
#endif

        /* If that worked, return the string. */
        if (n > -1 && n < size)
        {
            break;
        }
        /* Else try again with more space. */
        if (n > -1)    /* glibc 2.1 */
            size = n+1; /* precisely what is needed */
        else           /* glibc 2.0 */
            size *= 2;  /* twice the old size */

        if ((p = (char*) realloc (p, size)) == NULL)
        {
            break;
        }
    }

    if (p != NULL)
    {
        Logger::log(level,p) ;
        free(p) ;
    }

    va_end(args) ;
}



void Logger::initializeLogger(const LOG_LEVEL_T initialLevel, 
                             bool enableLocalFile,
                             bool enableStdOut, 
                             bool enableSysLog)
{
    if (!gLoggerRef)
    {
        gLoggerRef = new Logger();
        assert(gLoggerRef);
    }

    gLoggerRef->setLevel(initialLevel);
    gLoggerRef->enableLocalFile(enableLocalFile);
    gLoggerRef->enableStdOut(enableStdOut);
    gLoggerRef->enableSyslog(enableSysLog);

}



Logger* Logger::instance()
{
    assert(gLoggerRef);
    return gLoggerRef;
}



////////////////
// Instance
void Logger::registerListener(LogListener* listener)
{
    QMutexLocker lock(&mListenersMutex);

    if (!mListeners.contains(listener))
    {
        mListeners.append(listener);
    }
}



void Logger::unregisterListener(LogListener* listener)
{
    QMutexLocker lock(&mListenersMutex);

    int i = mListeners.indexOf(listener);
    if (i != -1)
    {
        mListeners.removeAt(i);
    }
}



void Logger::enableSyslog(bool doSysLog)
{
#if !defined(Q_OS_WIN32)
    if (doSysLog)
    {
        if (mSyslogEnabled) return; // no change

        openlog(RN_SYSLOG_NAME, LOG_NDELAY bitor LOG_PID, LOG_USER);
        mSyslogEnabled = true;
    }
    else
    {
        if (!mSyslogEnabled) return;

        closelog();
        mSyslogEnabled = false;
    }
#endif // #if !defined(Q_OS_WIN32)
}



void Logger::enableLocalFile(bool doFile)
{
    QMutexLocker lock(&mFileLoggerMutex);

    if (doFile)
    {
        if (fileLogger) return; // No worries

        fileLogger = new FileLogListener();
        fileLogger->openFile(dir.absoluteFilePath(QString(RN_LOG_FILE_NAME)));
        registerListener(fileLogger);
    }
    else
    {
        if (!fileLogger) return; // No logger, already disabled

        unregisterListener(fileLogger);
        delete fileLogger;
    }
}



void Logger::shutdown()
{
    QMutexLocker lock(&mListenersMutex);

    // Tell all the listeners we are closing
    QList<LogListener*>::const_iterator i;
    for(i = mListeners.constBegin(); i != mListeners.constEnd(); ++i)
    {
        (*i)->shutdown();
    }

    mListeners.clear();
    enableStdOut(false);
    enableSyslog(false);
}



void Logger::setLevel(LOG_LEVEL_T level)
{
    Logger::gLogLevel = level;
}



#if !defined(Q_OS_WIN32)
/// Translate from our log levels to syslog log levels
int syslogLevels[] = { 
    LOG_DEBUG,   // trace
    LOG_INFO,    // debug
    LOG_NOTICE,  // info
    LOG_WARNING, // warn
    LOG_ERR     // error
};
#endif //#if !defined(Q_OS_WIN32)
char* levelStrings[] = {
    "TRACE",
    "DEBUG",
    "INFO ",
    "WARN ",
    "ERROR",
    "NONE "
};

void Logger::sltLog(const Logger::LOG_LEVEL_T level, const QString& msg)
{
    QMutexLocker lock(&mListenersMutex);

    QDateTime now = QDateTime::currentDateTime();
 
    QString line(now.toString("yyyyMMdd.hhmmss.zzz") + " ");
    line.append(levelStrings[level]);
	line.append(": ");
	line.append(msg);

    if (mStdOutEnabled)
    {
        fprintf(stdout, "%s\n", line.toAscii().data());
    }

#if !defined(Q_OS_WIN32)
    if (mSyslogEnabled && (level < L_NONE))
    {
        // Since syslog handles the level and time for us, we use the raw
        // message instead of our line with the prepended bits
        syslog(syslogLevels[(const int)level], "%s", msg.toAscii().data());
    }
#endif //#if !defined(Q_OS_WIN32)

    // Tell all the listeners we are closing
    QList<LogListener*>::const_iterator i;
    for(i = mListeners.constBegin(); i != mListeners.constEnd(); ++i)
    {
        (*i)->logMessage(level,line);
    }
}



////////////////
// Private

//extern char **environ;

Logger::Logger()
: QObject(NULL)
, mSyslogEnabled(false)
, mStdOutEnabled(false)
, fileLogger(false)
{
    qRegisterMetaType<Logger::LOG_LEVEL_T>();
    connect(this, SIGNAL(sigLog(const Logger::LOG_LEVEL_T, const QString&)),
        this, SLOT(sltLog(const Logger::LOG_LEVEL_T, const QString&)));
    
    // Do some log cleanup and rotation.  This could be more robust, but for
    // the moment we just want to avoid filling up the user's hard drive. 
    QString logDirname;
    
#ifdef Q_WS_MAC
    struct passwd* pw = getpwuid(getuid());    
    logDirname = QString(pw->pw_dir) + "/Library/Logs/" RN_LOG_FILE_DIR "/";
#endif // #ifdef Q_WS_MAC

    printf("logDirname=`%s`\n",logDirname.toAscii().data());
    
    dir.mkpath(logDirname);
    dir.setPath(logDirname);
    
    // Remove any old logs    
    dir.remove(QString(RN_LOG_FILE_NAME) + ".0");
    
    // Rename any existing logs
    dir.rename(QString(RN_LOG_FILE_NAME), QString(RN_LOG_FILE_NAME) + ".0");
}



Logger::~Logger()
{
    shutdown();
}





//////////////////////////////////////////
// FileLogListener

FileLogListener::FileLogListener()
: isShutdown(false)
, logFile(NULL)
, fileStream(NULL)
{
}



void FileLogListener::openFile(QString file)
{
    // Open the log file
	logFile = new QFile(file);
	if (!logFile->open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) 
    {
        QMessageBox::critical(0, RN_APPLICATION_NAME, QString("Could not open log file."));
        logFile = 0;
    }
    else
    {
        fileStream = new QTextStream(logFile);
    }
}



FileLogListener::~FileLogListener()
{
    shutdown();
}



void FileLogListener::logMessage(const Logger::LOG_LEVEL_T level, const QString& msg)
{
    if (fileStream)
    {
        (*fileStream) << msg << endl;
        fileStream->flush();
    }
}



void FileLogListener::shutdown()
{
    if (!isShutdown)
    {
        // Close the stream
        delete fileStream;
        fileStream = NULL;

        // Close the file
        if (logFile) logFile->close();
        delete logFile;
        logFile = NULL;
    }
    isShutdown = true;
}
