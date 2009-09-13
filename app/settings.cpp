#include "settings.h"
#include "constants.h"
#include "logger.h"

#include <QMutexLocker>
#include <QCoreApplication>

#define CURRENT_SCHEMA_VERSION 1


Settings::Settings()
: QSettings(QSettings::UserScope, RB_SETTINGS_DOMAIN, RN_APPLICATION_NAME)
{
    loadSettings();
}



Settings::~Settings(void)
{
    save();    
}



void Settings::loadSettings()
{
    QMutexLocker lock(&mStorageMutex);
    
    beginGroup("general");
    mUsername = value("username").toString();
    mLastVersionSkipped = value("lastversionskipped").toString();
    endGroup();
    
    mDirty = false;
}



void Settings::save(void) 
{
    QMutexLocker lock(&mStorageMutex);
    
    setValue("schemaversion",CURRENT_SCHEMA_VERSION);

    beginGroup("general");
    setValue("username", mUsername);
    setValue("lastversionskipped", mLastVersionSkipped);
    endGroup();
    
    mDirty = false;
}



bool Settings::haveSettings()
{
    bool result = true;
    
    if (value("schemaversion").toInt() != CURRENT_SCHEMA_VERSION)
    {
        result = false;
    }
    
//    QString username = value("general/username").toString();
//    if (username.isNull() || username=="")
//    {
//        result = false;
//    }
    
    return result;
}



void Settings::saveWindowPosition(const QString& name, const QWidget& window)
{
    QMutexLocker lock(&mStorageMutex);
    
    beginGroup("windows");
    beginGroup(name);

    setValue("size",window.size());
    setValue("pos",window.pos());
    
    endGroup();
    endGroup();
}



void Settings::restoreWindowPosition(const QString& name, QWidget& window)
{
    QMutexLocker lock(&mStorageMutex);

    beginGroup("windows");
    beginGroup(name);

    QVariant vSize = value("size");
    QVariant vPos = value("pos");
    
    if (vSize.isValid() && vPos.isValid())
    {
        Logger::lprintf(Logger::L_DEBUG,"Restoring window %s to %d,%d size %d,%d",
                    name.toAscii().data(), 
                    vPos.toPoint().x(), vPos.toPoint().y(),
                    vSize.toSize().width(), vSize.toSize().height());
        
        window.resize(vSize.toSize());
        window.move(vPos.toPoint());
    }    
    
    endGroup();
    endGroup();
}

