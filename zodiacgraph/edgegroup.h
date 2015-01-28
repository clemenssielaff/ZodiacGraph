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

#ifndef ZODIAC_EDGEGROUP_H
#define ZODIAC_EDGEGROUP_H

///
/// \file edgegroup.h
///
/// \brief Contains the definition of the zodiac::EdgeGroup class.
///


#include <QObject>
#include <QPair>
#include <QSet>

#include "edgegroupinterface.h"

namespace zodiac {

class EdgeGroupPair;
class Node;
class Scene;
class PlugEdge;
class StraightEdge;

///
/// \brief An EdgeGroup is an additional layer of management for all PlugEdge%s between two Node%s.
///
/// Its main function is to display a single edge, if both Node%s are fully collapsed and all PlugEdge%s overlay each
/// other in a single, straight line.
/// This would make it impossible to find a specific connection from the stack of PlugEdge%s alone.
/// Also, drawing the edges on top of each other is a drain on perfomance without visual improvements.
///
class EdgeGroup : public QObject, public EdgeGroupInterface
{

    Q_OBJECT

public: // methods

    ///
    /// \brief Constructor.
    ///
    /// \param [in] scene       Scene owning this EdgeGroup.
    /// \param [in] fromNode    Node from which the PlugEdge%s originate.
    /// \param [in] toNode      Target Node of the PlugEdge%s in this group.
    /// \param [in] pair        EdgeGroupPair owning this EdgeGroup.
    ///
    explicit EdgeGroup(Scene* scene, Node* fromNode, Node* toNode, EdgeGroupPair* pair);

    ///
    /// \brief Destuctor.
    ///
    /// Also deletes the StraightEdge of this EdgeGroup.
    ///
    ~EdgeGroup();

    ///
    /// \brief Adds a new PlugEdge to this EdgeGroup.
    ///
    /// Make sure that the PlugEdge flows between the correct Node%s in the correct direction.
    /// If it doesn't, this function will raise an assertion error in debug mode and do nothing in release mode.
    ///
    /// \param [in] edge    New PlugEdge to add.
    ///
    void addEdge(PlugEdge* edge);

    ///
    /// \brief Removes an existing PlugEdge from this EdgeGroup.
    ///
    /// Make sure that the PlugEdge is part of this EdgeGroup.
    /// If it is not, this function will raise an assertion error in debug mode and do nothing in release mode.
    ///
    /// \param [in] edge    Existing PlugEdge to remove.
    ///
    void removeEdge(PlugEdge* edge);

    ///
    /// \brief Increase the number of bent curves in this group.
    ///
    /// A "bent" edge has one or both of its connecting Plug%s expanded from its Node, visually bending its curve.
    /// As soon as a single edge in the EdgeGroup is bent, the StraightEdge of the EdgeGroup becomes invisible and
    /// all PlugEdge%s are displayed instead.
    ///
    void increaseBentCount();

    ///
    /// \brief Decrease the number of bent curves in this group.
    ///
    /// See \ref zodiac::EdgeGroup::increaseBentCount() "increaseBentCount()" for details on bent edges.
    ///
    void decreaseBentCount();

    ///
    /// \brief Lets the EdgeGroup determine its own visibility, based on its bent-count.
    ///
    void updateVisibility();

    ///
    /// \brief The hash of this EdgeGroup, as calculated by \ref zodiac::EdgeGroup::getHashOf() "getHashOf()".
    ///
    /// \return Hash of this EdgeGroup..
    ///
    uint getHash() const;

    ///
    /// \brief <i>true</i> if the straight edge of this group is visible -- <i>false</i> otherwise.
    ///
    /// \return Visibility of this EdgeGroup.
    ///
    bool isVisible() const;

    ///
    /// \brief Hides the StraightEdge of this EdgeGroup in favour of the DoubleStraightEdge of the EdgeGroupPair, even
    /// if it would otherwise be visible.
    ///
    /// \param [in] visibility   <i>true</i> if the straight edge should be visible -- <i>false</i> otherwise.
    ///
    void setVisibility(bool visibility);

    ///
    /// \brief The number of edges in this group.
    ///
    /// \return Edge count.
    ///
    inline int getEdgeCount() const {return m_edges.size();}

    ///
    /// \brief All PlugEdge%s of this EdgeGroup.
    ///
    /// \return All PlugEdge%s of this EdgeGroup.
    ///
    inline const QSet<PlugEdge*>& getEdges() const {return m_edges;}

    ///
    /// \brief The EdgeGroupPair owning this EdgeGroup.
    ///
    /// \return This EdgeGroup's EdgeGroupPair.
    ///
    inline EdgeGroupPair* getEdgeGroupPair() const {return m_pair;}

    ///
    /// \brief Generates the label text for this EdgeGroup.
    ///
    /// \return Label text based on the PlugEdge%s contained in this group.
    ///
    QString getLabelText();

    ///
    /// \brief Updates the label text of the EdgeGroup's StraightEdge, as well as the StraightDouleEdge label.
    ///
    void updateLabelText();

    ///
    /// \brief Applies style changes in the class' static members to this instance.
    ///
    /// Is part of the scene-wide cascade of %updateStyle()-calls after a re-styling of the ZodiacGraph.
    ///
    void updateStyle();

public: // static methods

    ///
    /// \brief Calculates a hash value based on two nodes.
    ///
    /// The order of nodes matters.
    ///
    /// \param [in] fromNode    Node from which the PlugEdge%s originate.
    /// \param [in] toNode      Target Node of the PlugEdge%s in this group.
    /// \param [in] seed        Seed value for the <a href="http://doc.qt.io/qt-5/qhash.html#qHash-33">qhash</a>
    ///                         function.
    ///
    /// \return                 Hash value of an EdgeGroup connecting the two given Node%s from fromNode to toNode.
    ///
    static inline uint getHashOf(Node* fromNode, Node* toNode, uint seed=0){
        return qHash(QPair<Node*, Node*>(fromNode, toNode), seed);
    }

private slots:

    ///
    /// \brief Is called, when the StraightEdge is clicked with the removal button.
    ///
    /// If there is more than one PlugEdge in the group, the request is ignored.
    /// Otherwise, the PlugEdge is removed which in turn could also remove this group if its pair is also empty.
    ///
    void removalRequested();

private: // members

    ///
    /// \brief Scene containing this EdgeGroup.
    ///
    Scene* m_scene;

    ///
    /// \brief The Node from which the PlugEdge%s of this group originate.
    ///
    Node* m_fromNode;

    ///
    /// \brief The Node to which the PlugEdge%s of this group flow.
    ///
    Node* m_toNode;

    ///
    /// \brief EdgeGroupPair that owns this group.
    ///
    EdgeGroupPair* m_pair;

    ///
    /// \brief All PlugEdge%s in this EdgeGroup.
    ///
    QSet<PlugEdge*> m_edges;

    ///
    /// \brief StraightEdge to draw when all of the group's PlugEdge%s are hidden.
    ///
    /// Owned by this EdgeGroup.
    ///
    StraightEdge* m_straightEdge;

    ///
    /// \brief The current number of bent edges in this group.
    ///
    int m_bentEdgesCount;

};

} // namespace zodiac

#endif // ZODIAC_EDGEGROUP_H
