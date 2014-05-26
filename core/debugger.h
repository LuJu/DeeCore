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
#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <iostream>
#include <Qt>
#include <QDateTime>
#include <QString>
#include <QtDebug>
#include <QFile>
#include <QTextStream>
#include <GL/gl.h>
#include "globalconfig.h"

//! Abstract class allowing the redirection of messages
/*!
        The Debugger class redirects the debug and warning messages dispatched to a file. Call the function QtInstallMsgHandler to specify the redirection
        \n Status  5 : implemented and functionnal
    */
class Debugger
{
public:
    //! Redirects the message dispatched (debug, warning...)
    /*!
        \n Status  5 : implemented and functionnal
    */
    static void customMessageHandler(QtMsgType type, const char *msg);

    //! Promps the errors that can happen with opengl functions
    /*!
        \n Status  5 : implemented and functionnal
    */
    static void promptOpenGLError();

private:
    static QString _logfile;
    //! Function to make the class not instanciable
    /*!
    */
    virtual void abstract()=0;


};

#endif // DEBUGGER_H
