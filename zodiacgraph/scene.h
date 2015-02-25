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

#ifndef ZODIAC_SCENE_H
#define ZODIAC_SCENE_H

/// \file scene.h
///
/// \brief Contains the definition of the zodiac::Scene class.
///
/// The visual design of the graph was in part inspired by a video from Marcin Ignac:
/// <a href="http://marcinignac.com/experiments/ring/">http://marcinignac.com/experiments/ring/</a>
///

#include <QGraphicsScene>
#include <QUuid>
#include <QSet>

namespace zodiac {

class DrawEdge;
class PlugEdge;
class Node;
class Plug;
class EdgeGroup;
class EdgeGroupPair;

///
/// \brief Scene class for the ZodiacGraph.
///
/// This is the main interaction point for users of the graph with its content.
/// It owns all instances of Node, as well as other QGraphicItems through Qt's parent-child mechanism
/// Additionally, it owns all top-level supporting classes like EdgeGroupPair and manages the DrawEdge.
///
class Scene : public QGraphicsScene
{
    Q_OBJECT

public: // methods

    ///
    /// \brief Constructor.
    ///
    /// \param [in] parent  Qt parent object owning this Scene.
    ///
    /// The constructor also creates the shared DrawEdge instance used to let the user draw new PlugEdge instances in
    /// the graph.
    ///
    explicit Scene(QObject *parent);

    ///
    /// \brief Destructor.
    ///
    /// Also deletes all EdgeGroupPair instances, prior to releasing the QGraphicsObjects.
    ///
    virtual ~Scene();

    ///
    /// \brief Creates and adds a new Node to the graph.
    ///
    /// \param [in] name    Name of the new Node.
    /// \param [in] uuid    (optional) The unique identifier of this Node.
    ///
    /// \return             The new Node.
    ///
    Node* createNode(const QString& name, const QUuid& uuid = QUuid());

    ///
    /// \brief Removes an existing Node from this Scene.
    ///
    /// A Node can only be removed if it has no PlugEdge%s attached to any of its Plug%s.
    /// If you want to test if the Node can be removed prior to calling Scene::removeNode(), use Node::isRemovable().
    ///
    /// Make sure that this Scene actually contains the given Node.
    /// If it doesn't, calling this function returns <i>false</i> in release mode and will throw an assertion error
    /// in debug mode.
    ///
    /// After calling this function, all remaining pointers to the removed Node are to be discarded without further use.
    ///
    /// \param [in] node    Node to remove.
    ///
    /// \return             <i>true</i> if the Node could be removed -- <i>false</i> otherwise.
    ///
    bool removeNode(Node* node);

    ///
    /// \brief Returns all Node%s managed by the Scene.
    ///
    /// \return All Node%s managed by the Scene.
    ///
    QList<Node*> getNodes() const {return m_nodes.toList();}

    ///
    /// \brief  Creates and adds a new PlugEdge to the graph, connecting two Plug%s.
    ///
    /// Returns <i>nullptr</i> if the PlugEdge cannot be created, for example if a PlugEdge between <i>fromPlug</i> and
    /// <i>toPlug</i> already exists or both are part of the same Node.
    ///
    /// If this is the first connection between two Node%s, this function also creates two EdgeGroup%s (of which one is
    /// empty) and an EdgeGroupPair to manage them.
    /// If there is already a connection between the two Node instances, the corresponding existing EdgeGroup will be
    /// used instead.
    ///
    /// \param [in] fromPlug    Start Plug of the PlugEdge.
    /// \param [in] toPlug      End Plug of the PlugEdge.
    ///
    /// \return                 New PlugEdge or <i>nullptr</i>, if the PlugEdge could not be created.
    ///
    PlugEdge* createEdge(Plug* fromPlug, Plug* toPlug);

    ///
    /// \brief Removes a PlugEdge from the Scene, disconnecting its two Plug%s.
    ///
    /// Also takes care of deleting EdgeGroup and EdgeGroupPair instances that are empty after removing the PlugEdge.
    ///
    /// Make sure that this Scene actually contains the given PlugEdge.
    /// If it doesn't, calling this function will throw an assertion error in debug mode and do nothing in release mode.
    ///
    /// \param [in] edge    PlugEdge to remove.
    ///
    void removeEdge(PlugEdge* edge);

    ///
    /// \brief Returns an existing PlugEdge from the Scene.
    ///
    /// \param [in] fromPlug    Start Plug of the edge.
    /// \param [in] toPlug      End Plug of the edge.
    ///
    /// \return                 PlugEdge from fromPlug to toPlug or <i>nullptr</i>, if no edge between the given Plug%s
    ///                         exists.
    ///
    PlugEdge* getEdge(Plug* fromPlug, Plug* toPlug);

    ///
    /// \brief Force-collapses all Node%s in the scene.
    ///
    void collapseAllNodes();

    ///
    /// \brief Returns the DrawEdge used to draw new PlugEdge%s in the scene.
    ///
    /// \return         The DrawEdge used to draw new PlugEdge%s in the scene.
    ///
    inline DrawEdge* getDrawEdge() {return m_drawEdge;}

    ///
    /// \brief Initiates a cascade of style updates of the complete Scene.
    ///
    void updateStyle();

private: // members

    ///
    /// \brief Edge shown when the user is creating a new PlugEdge.
    ///
    DrawEdge* m_drawEdge;

    ///
    /// \brief All Node instances in the graph.
    ///
    QSet<Node*> m_nodes;

    ///
    /// \brief All PlugEdge instances in the graph.
    ///
    QHash<QPair<Plug*, Plug*>, PlugEdge*> m_edges;

    ///
    /// \brief All EdgeGroup instances of the scene.
    ///
    /// EdgeGroup instances are stored in a hashmap with a hash value generated from EdgeGroup::getHashOf() as key.
    /// This way we can easily find the EdgeGroup for a directed connection between to Node%s in the graph.
    ///
    QHash<uint, EdgeGroup*> m_edgeGroups;

    ///
    /// \brief All EdgeGroupPair%s owned by the scene.
    ///
    QSet<EdgeGroupPair*> m_edgeGroupPairs;

};

} // namespace zodiac

#endif // ZODIAC_SCENE_H
