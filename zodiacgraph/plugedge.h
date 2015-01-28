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

#ifndef ZODIAC_PLUGEDGE_H
#define ZODIAC_PLUGEDGE_H

/// \file plugedge.h
///
/// \brief Contains the definition of the zodiac::PlugEdge class.
///

#include "bezieredge.h"

namespace zodiac {

class EdgeGroup;
class Scene;
class Plug;

///
/// \brief A PlugEdge in the Zodiac Graph connecting two Plug%s of different Node%s.
///
/// This is the main edge class in the graph.
/// However, most of its functionality resides in its base classes BezierEdge and BaseEdge.
///
class PlugEdge : public BezierEdge
{

    Q_OBJECT

public: // methods

    ///
    /// \brief Constructor.
    ///
    /// \param [in] scene       Scene containing this PlugEdge.
    /// \param [in] startPlug   Plug from which this PlugEdge starts.
    /// \param [in] endPlug     Plug into which this PlugEdge flows.
    /// \param [in] edgeGroup   EdgeGroup that this PlugEdge belongs to.
    ///
    explicit PlugEdge(Scene* scene, Plug* startPlug, Plug* endPlug, EdgeGroup* edgeGroup);

    ///
    /// \brief Is called by a plug to notify the edge of a change in its state.
    ///
    void plugHasChanged();

    ///
    /// \brief The start Plug of this PlugEdge is an \ref zodiac::PlugDirection::OUT "outgoing" Plug of a Node.
    ///
    /// \return Start Plug of this PlugEdge.
    ///
    inline Plug* getStartPlug() const {return m_startPlug;}

    ///
    /// \brief The end Plug of this PlugEdge is an \ref zodiac::PlugDirection::IN "incoming" Plug of a Node.
    ///
    /// \return End plug of this PlugEdge.
    ///
    inline Plug* getEndPlug() const {return m_endPlug;}

    ///
    /// \brief Every PlugEdge is part of an EdgeGroup managed by the Scene.
    ///
    /// The job of the EdgeGroup is to replace all PlugEdge%s flowing from Node <i>A</i> to Node <i>B</i>, once both
    /// Node%s are collapsed.
    /// At this stage, all PlugEdge%s are displayed as straight lines that completely overlay each other.
    /// This way, only a single edge is rendered and updated in the QGraphicsScene.
    ///
    /// \return EdgeGroup of this PlugEdge.
    ///
    inline EdgeGroup* getGroup() const {return m_group;}

    ///
    /// \brief Updates the EdgeLabel to reflect changes in the attached Plug%s and / or Node%s.
    ///
    inline void updateLabelText() {setLabelText(getLabelText());}

    ///
    /// \brief Generates the EdgeLabel's text by using information from the start and end Node of this PlugEdge.
    ///
    /// \return Text used for the EdgeLabel.
    ///
    QString getLabelText();

    ///
    /// \brief Sets the text of this edge's EdgeLabel.
    ///
    /// Extends the base functionality by also taking care of renaming the EdgeGroup%s.
    ///
    /// \param [in] text    Text to set this label to. Remove an existing EdgeLabel by passing "" (the empty string).
    ///
    virtual void setLabelText(const QString& text);

protected: // methods

    ///
    /// \brief Called, when the mouse is pressed as the cursor is on this item.
    ///
    /// \param [in] event   Qt event object.
    ///
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    ///
    /// \brief Called when this item is double-clicked.
    ///
    /// The user can expand both Node%s connected through this PlugEdge by double-clicking it.
    ///
    /// \param [in] event   Qt event object.
    ///
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

private: // members

    ///
    /// \brief The start Plug of this PlugEdge.
    ///
    Plug* m_startPlug;

    ///
    /// \brief The end Plug of this PlugEdge.
    ///
    Plug* m_endPlug;

    ///
    /// \brief The EdgeGroup of this PlugEdge.
    ///
    EdgeGroup* m_group;

    ///
    /// \brief A PlugEdge is "bent" if at least one of its Plug%s is expanded from its Node.
    ///
    bool m_isBent;

};

} // namespace zodiac

#endif // ZODIAC_PLUGEDGE_H
