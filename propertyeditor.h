//
//    ZodiacGraph - A general-purpose, circular node graph UI module.
//    Copyright (C) 2015  Clemens Sielaff
//
//    The MIT License
//
//    Permission is hereby granted, free of charge, to any person obtaining a copy of
//    this software and associated documentation files (the "Software"), to deal in
//    the Software without restriction, including without limitation the rights to
//    use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
//    of the Software, and to permit persons to whom the Software is furnished to do so,
//    subject to the following conditions:
//
//    The above copyright notice and this permission notice shall be included in all
//    copies or substantial portions of the Software.
//
//    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//    SOFTWARE.
//

#ifndef NODEPROPERTYEDITOR_H
#define NODEPROPERTYEDITOR_H

#include <QScrollArea>
#include <QHash>

#include "zodiacgraph/nodehandle.h"

class Collapsible;
class MainCtrl;
class QVBoxLayout;

class PropertyEditor : public QScrollArea
{
    Q_OBJECT

public: // methods

    ///
    /// \brief Constructor.
    ///
    /// \param [in] parent  Qt parent.
    ///
    explicit PropertyEditor(QWidget *parent);

    ///
    /// \brief Defines a new main controller managing this PropertyEditor.
    ///
    /// Should only be called once, after initialization.
    /// Is not part of the constructor to avoid the chicken-egg problem with the MainCtrl.
    ///
    /// \param [in] mainCtrl    The main controller managing this PropertyEditor.
    ///
    void setMainCtrl(MainCtrl* mainCtrl) {Q_ASSERT(!m_mainCtrl); m_mainCtrl = mainCtrl;}

    ///
    /// \brief Shows 0-n NodeProperty%s in the PropertyEditor.
    ///
    /// \param [in] selection   Handles of all nodes for which to display NodeProperty%s.
    ///
    void showNodes(const QList<zodiac::NodeHandle>& selection);

private: // members

    ///
    /// \brief The main controller managing this PropertyEditor.
    ///
    MainCtrl* m_mainCtrl;

    ///
    /// \brief Layout of the widget displayed in the scroll area, manages the individual Collapsible%s.
    ///
    QVBoxLayout* m_layout;

    ///
    /// \brief All Collapsible%s representing NodeCtrl%s, identified by their NodeHandle.
    ///
    QHash<zodiac::NodeHandle, Collapsible*> m_nodes;
};

#endif // NODEPROPERTYEDITOR_H
