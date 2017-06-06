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

#ifndef NODEPROPERTIES_H
#define NODEPROPERTIES_H

#include <QHash>
#include <QWidget>

#include "zodiacgraph/nodehandle.h"
#include "zodiacgraph/plughandle.h"

class QGridLayout;
class QLineEdit;
class QPushButton;

class Collapsible;
class NodeCtrl;
class PlugRow;

///
/// \brief Node Property widget, is a display widget of a Collapsible.
///
class NodeProperties : public QWidget
{
    Q_OBJECT

    ///
    /// \brief The PlugRow class is a logical part of the this one, but has a 1-n relationship.
    ///
    friend class PlugRow;

public: // methods

    ///
    /// \brief Constructor.
    ///
    /// \param [in] node    Node whose properties to display.
    /// \param [in] parent  Collapsible parent object.
    ///
    explicit NodeProperties(NodeCtrl* node, Collapsible *parent);

private: // for friend

    ///
    /// \brief The controller of the node whose properties are displayed in this NodeProperties widget.
    ///
    /// \return Controller of the managed node.
    ///
    NodeCtrl* getNode() const {return m_node;}

    ///
    /// \brief The layout to be used by PlugRows to place their widgets.
    ///
    /// \return The layout of the widgets related to the plugs of the node.
    ///
    QGridLayout* getPlugLayout() const {return m_plugLayout;}

    ///
    /// \brief Removes a plug from the node and the PlugRow from the editor.
    ///
    /// \param [in] plugName    Name of the plug to remove.
    ///
    void removePlugRow(const QString& plugName);

private slots:

    ///
    /// \brief Called by the name edit, when the name of the node was changed through user input.
    ///
    void renameNode();

    ///
    /// \brief Called by pressing the add-plug button.
    ///
    void createNewPlug();

    ///
    /// \brief Creates a new entry in the plug list of this property editor alongside its PlugRow.
    ///
    void addPlugRow(zodiac::PlugHandle plug);

private: // members

    ///
    /// \brief Controller of the edited node.
    ///
    NodeCtrl* m_node;

    ///
    /// \brief Node name edit.
    ///
    QLineEdit* m_nameEdit;

    ///
    /// \brief Layout of the widgets related to the plugs of the node.
    ///
    QGridLayout* m_plugLayout;

    ///
    /// \brief Button to add a new Plug to the node.
    ///
    QPushButton* m_addPlugButton;

    ///
    /// \brief All PlugRows contained in this editor.
    ///
    QHash<QString, PlugRow*> m_plugRows;

    ///
    /// \brief Hitting the add-Plug button creates incoming and outgoing Plug%s alternately.
    ///
    /// This flag keeps track of what is next.
    ///
    bool m_nextPlugIsIncoming;

private: // static members

    ///
    /// \brief Default plug name.
    ///
    static QString s_defaultPlugName;

};


///
/// \brief An extension to the NodeProperties class, responsible to manage a single row of Plug-related widgets.
///
class PlugRow : public QObject
{
    Q_OBJECT

public: // methods

    ///
    /// \brief Constructor.
    ///
    /// \param [in] editor          NodeProperties that this PlugRow is part of.
    /// \param [in] plug            Handle of the plug whose name to edit / display.
    /// \param [in] nameEdit        Plug name edit.
    /// \param [in] directionToggle Plug-direction toggle button.
    /// \param [in] removalButton   Plug-removal button.
    ///
    PlugRow(NodeProperties *editor, zodiac::PlugHandle plug,
            QLineEdit *nameEdit, QPushButton *directionToggle, QPushButton *removalButton);

private slots:

    ///
    /// \brief Called when the name of the plug was changed through user input.
    ///
    void renamePlug();

    ///
    /// \brief Called when the toggle-Plug-direction button is pressed.
    ///
    void togglePlugDirection();

    ///
    /// \brief Called when the Plug-removal button is pressed.
    ///
    void removePlug();

private: // methods

    ///
    /// \brief Single access point for setting the direction icon.
    ///
    void updateDirectionIcon();

private: // members

    ///
    /// \brief Controller of the edited node.
    ///
    NodeProperties* m_editor;

    ///
    /// \brief Handle of the plug whose name to edit / display.
    ///
    zodiac::PlugHandle m_plug;

    ///
    /// \brief Plug name edit.
    ///
    QLineEdit* m_nameEdit;

    ///
    /// \brief Plug-direction toggle button.
    ///
    QPushButton* m_directionToggle;

    ///
    /// \brief Plug-removal button.
    ///
    QPushButton* m_removalButton;
};

#endif // NODEPROPERTIES_H
