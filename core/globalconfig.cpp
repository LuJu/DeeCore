#include "globalconfig.h"

//QHash<QString,bool> GlobalConfig::_bool;
//QHash<QString,int> GlobalConfig::_int;
//QHash<QString,QString> GlobalConfig::_string;
QHash<QString,QVariant> GlobalConfig::_values;
QSettings* GlobalConfig::_settings = NULL;

bool GlobalConfig::openGLSupport(const char * extension){
    const char *exts = (char*)glGetString(GL_EXTENSIONS);
    QString s(exts);
    qDebug()<<"Extensions supported : "<<s;
    if(strstr((const char*)exts, extension) == NULL) {
        qDebug()<<"extension "<<extension<<" non supportee";
        return false ;
    } else
        return true;
}

void GlobalConfig::switchState(QString data){
    if (is_enabled(data))
        disable(data);
    else enable(data);
}

void GlobalConfig::saveConfiguration(){
    QHashIterator<QString, QVariant> l(_values);
    while (l.hasNext()) {
        l.next();
        _settings->setValue(l.key(),l.value());
    }
}

bool GlobalConfig::loadConfiguration(const QString & organization, const QString & application){
    QString option;
    QString value;
    QStringList keys;

    QCoreApplication::setOrganizationName(organization);
    QCoreApplication::setApplicationName(application);
    _settings = new QSettings(organization,application);

    keys=_settings->allKeys();
    for (int i =0; i < keys.size() ; i++){
        option=keys[i];
        value=_settings->value(keys[i]).toString();
        _values[option]=QVariant(value);
    }
    qDebug()<<"Previous configuration loaded";
    return true;
}


