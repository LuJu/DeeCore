/*
Copyright (c) 2013, Lucas Juliéron
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/
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

    static const QString get_date_time(){return QDateTime::currentDateTime().toString("yy.MM.dd_hh'h'mm");}
    static bool openGLSupport(const char * extension);
    static void saveConfiguration();
    static bool loadConfiguration(const QString & organization, const QString & application);

    static bool is_enabled(QString data){ return get_parameter(data).toBool();}
    static void switchState(QString data);

    static void enable(QString data){  set_parameter(data,QVariant(true));}
    static void disable(QString data){ set_parameter(data,QVariant(false));}

    static int get_int(QString data){ return get_parameter(data).toInt();}
    static void set_int(QString data, int value){set_parameter(data,QVariant(value));}

    static float get_float(QString data){ bool ok = false;
                                          float value = get_parameter(data).toFloat(&ok);
                                        return value;}
    static void set_float(QString data, float value){set_parameter(data,QVariant(value));}

    static QString get_string(QString data){ return get_parameter(data).toString();}
    static void set_string(QString data, QString value){set_parameter(data,QVariant(value));}


    static void defaultValue(QString data, QVariant default_value, bool replace = false){
        _lock.lockForWrite();
        if (!_settings->contains(data) || replace ){
            _values[data]=default_value;
        }
        _lock.unlock();
    }

    static QVariant get_parameter(QString data){
        QVariant value;
        _lock.lockForRead();
        value = _values[data];
        _lock.unlock();
        return value;
    }
    static void set_parameter(QString data, QVariant value){
        _lock.lockForWrite();
        _values[data] = value;
        _lock.unlock();
    }

private:
    ~GlobalConfig(){delete _settings;}
    virtual void abstract()=0;

    static QHash<QString,QVariant> _values;
    static QSettings * _settings;
    static QReadWriteLock _lock;
};

#endif // GLOBALCONFIG_H
