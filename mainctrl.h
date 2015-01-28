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

#ifndef NODEMANAGER_H
#define NODEMANAGER_H

#include <QObject>
#include <QHash>

#include "zodiacgraph/nodehandle.h"
#include "zodiacgraph/scenehandle.h"

class NodeCtrl;
class PropertyEditor;
namespace zodiac {
    class Scene;
}

///
/// \brief Controller Manager managing all NodeCtrls.
///
/// This is the main class for controlling the business logic of the application.
///
class MainCtrl : public QObject
{
    Q_OBJECT

public: // methods

    ///
    /// \brief Constructor.
    ///
    /// \param [in] parent          Qt parent.
    /// \param [in] scene           Handle of a zodiac::Scene.
    /// \param [in] propertyEditor  Property editor.
    ///
    explicit MainCtrl(QObject *parent, zodiac::Scene* scene, PropertyEditor* propertyEditor);

    ///
    /// \brief Creates a new node in the graph.
    ///
    /// \param [in] name    Name of the new node or empty to create a default name.
    ///
    /// \return Control for new node.
    ///
    NodeCtrl* createNode(const QString& name = "");

    ///
    /// \brief Deletes an existing node from the graph.
    ///
    /// \param [in] node    Node to delete.
    ///
    /// \return             <i>true</i> if the node was removed -- <i>false</i> otherwise.
    ///
    bool deleteNode(NodeCtrl* node);

    ///
    /// \brief Returns the NodeCtrl that manages a given NodeHandle.
    ///
    /// If there is no corresponding NodeCtrl known to this manager, the <i>nullptr</i> is returned.
    ///
    /// \param [in] handle  NodeHandle for which to find the corresponding NodeCtrl.
    ///
    /// \return             Corresponding NodeCtrl or the <i>nullptr</i> if none was found.
    ///
    inline NodeCtrl* getCtrlForHandle(zodiac::NodeHandle handle) const { return m_nodes.value(handle, nullptr); }

    ///
    /// \brief Prints the current state of the zodiac::Scene.
    ///
    /// This is not a serialized form, even though it might in the future extend to be used in such a fashion.
    /// For now, it is used for helping me creating the initial "ZODIAC"-logo setup.
    ///
    void printZodiacScene();

    ///
    /// \brief Must be called before closing the application.
    ///
    /// \return             <i>true</i> if the manager was shut down successfully -- <i>false</i> if the user objected.
    ///
    bool shutdown();

public slots:

    ///
    /// \brief Creates a new node in the graph selects and activates it.
    ///
    void createDefaultNode();

private slots:

    ///
    /// \brief Called when the selection in the managed scene has changed.
    ///
    /// \param [in] selection   Handles to all selected nodes.
    ///
    void selectionChanged(QList<zodiac::NodeHandle> selection);

private: // members

    ///
    /// \brief Handle to the zodiac::Scene representing the graph.
    ///
    zodiac::SceneHandle m_scene;

    ///
    /// \brief The Property Editor widget.
    ///
    PropertyEditor* m_propertyEditor;

    ///
    /// \brief NodeCtrls managed by this manager (but deleted through Qt).
    ///
    QHash<zodiac::NodeHandle, NodeCtrl*> m_nodes;

    ///
    /// \brief Ever increasing index value for default names of the nodes in this manager.
    ///
    uint m_nodeIndex;

private: // static members

    ///
    /// \brief Default node name. "Node_" will result in a default name of "Node_12" for example.
    ///
    static QString s_defaultName;

};

#endif // NODEMANAGER_H
