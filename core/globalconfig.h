#ifndef GLOBALCONFIG_H
#define GLOBALCONFIG_H

#include <iostream>
#include <QtOpenGL>
#include <QSettings>
#include <QString>

#define DEECORE

//! Static class describing the global settings of the program
/*!
    */
class GlobalConfig
{
public:

    static QSettings * _settings;
    static const QString get_date_time(){return QDateTime::currentDateTime().toString("yy.MM.dd_hh'h'mm");}
    static bool openGLSupport(const char * extension);
    static void saveConfiguration();
    static bool loadConfiguration(const QString & organization, const QString & application);

    static bool is_enabled(QString data){ _values[data].toBool();}
    static void switchState(QString data);

    static void enable(QString data){  _values[data]=QVariant(false);}
    static void disable(QString data){ _values[data]=QVariant(true);}
//    static void enable(QString data){  _bool["bool/"+data]=true;}
//    static void disable(QString data){ _bool["bool/"+data]=false;}

    static int get_int(QString data){ return _values[data].toInt();}
    static void set_int(QString data, int value){_values[data]=QVariant(value);}

    static QString get_string(QString data){ return _values[data].toString();}
    static void set_string(QString data, QString value){ _values[data]=QVariant(value);}

//    static int get_int(QString data){ return _int["int/"+data] ;}
//    static void set_int(QString data, int value){_int["int/"+data]=value;}

//    static QString get_string(QString data){ return _string["string/"+data] ;}
//    static void set_string(QString data, QString value){ _string["string/"+data]=value;}

    static void set_parameter(QString data, QVariant value){ _values[data]=value;}
    static QVariant get_parameter(QString data){return _values[data];}

    static void defaultValue(QString data, QVariant default_value, bool replace = false){
        if (!_settings->contains(data) || replace ) _values[data]=default_value;
    }

    static void parseArguments(int argc, char *argv[]);

private:
    ~GlobalConfig(){delete _settings;}
    virtual void abstract()=0;
//    static QHash<QString,bool> _bool;
//    static QHash<QString,int> _int;
//    static QHash<QString,QString> _string;
    static QHash<QString,QVariant> _values;
};

#endif // GLOBALCONFIG_H
