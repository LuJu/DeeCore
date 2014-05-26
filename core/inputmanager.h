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
#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <Qt>
#include <QDebug>
#include <QPoint>
#include "uistate.h"

class Viewer;
class InputManager
{
public:
    enum EventTypes {
        Key_Pressed,
        Key_Released,
        Moved
    };

    Viewer * _view;

    //! Indicates the press of a key on the keyboard
    /*!
        \n Status  1 : not implemented
        \param  key The key pressed
        \param eventType Key_Pressed or Key_Released
    */
    void keyboardEvent( int key,int eventType);

    //! Indicates the press of a key on the mouse
    /*!
        This function only concerns key pushes, for the wheel, see wheel() method.
        \n Status  1 : not implemented
        \param  key The key pressed
        \param eventType Key_Pressed or Key_Released
    */
    void mouseEvent(int eventType, QPoint pos, int key=0);

    //! Gets the action of a key
    /*!
        Returns the actions done by the key passed in parameter, -1 if no action is done
        \n Status  1 : not implemented
        \param  key The key to return the action
        \return The action done by the key, or -1 if no action is done
    */
    int get_key_action(int key);

    //! Indicates an event with the wheel of the mouse
    /*!
        description
        \n Status  1 : not implemented
        \param  orientation Vertical or horizontal
        \param  delta The distance of movement
    */
    void wheelEvent(int orientation, int delta, QPoint pos);

    //! Allows to change the default assignment of a key
    /*!
        \n Status  1 : not implemented
        \param old_key_id The key the is to be changed
        \param new_key The new key ID
    */
    bool changeAssignment(int old_key_id, int new_key);

    //! Puts the assignment of all keys back to default
    /*!
    */
    void resetAssignment();

//    static InputManager * instance();
    InputManager();

    void set_ui(UIState * ui){_ui = ui;}
private:
    InputManager(const InputManager&);


    static const quint8 NUMBER_OF_KEYS=40;
    static const int _default_keyboard_matching[NUMBER_OF_KEYS];
    bool _keyboard_activated;
    bool _mouse_activated;
    bool _mouse_left_dragging;
    bool _keys_activated[NUMBER_OF_KEYS];
    int _keyboard_matching[NUMBER_OF_KEYS];
    UIState * _ui;
};

#endif // INPUTMANAGER_H
