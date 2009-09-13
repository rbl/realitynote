#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>
#include <QMutex>
#include <QWidget>

#ifdef Q_WS_MAC
#include <CoreFoundation/CoreFoundation.h>
#endif // #ifdef Q_WS_MAC


class Settings 
: public QSettings
{
    Q_OBJECT

public:
    Settings(void);
    ~Settings(void);
    //Settings(const Settings& s);
    //const Settings& operator=(const Settings& s);

    QString username() { return mUsername; }
    void setUsername(const QString& str) { mUsername = str; mDirty = true; }
        
    QString lastVersionSkipped() { return mLastVersionSkipped; }
    void setLastVersionSkipped(const QString& val) { mLastVersionSkipped = val; mDirty = true; }
    
    void loadSettings();
    void save();
    
    // Returns true or false based on whether the settings
    // stored in the registry are good or not.
    bool haveSettings();    
    
    void saveWindowPosition(const QString& name, const QWidget& window);
    void restoreWindowPosition(const QString& name, QWidget& window);

private:    
    bool mDirty;
    QMutex mStorageMutex;
    
    QString mUsername;
    QString mLastVersionSkipped;
    
};

#endif // SETTINGS_H


