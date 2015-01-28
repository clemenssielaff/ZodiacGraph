//
//    ZodiacGraph - A general-purpose, circular node graph UI module.
//    Copyright (C) 2015  Clemens Sielaff
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published
//    by the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
