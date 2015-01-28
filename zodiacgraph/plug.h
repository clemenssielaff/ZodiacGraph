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

#ifndef ZODIAC_PLUG_H
#define ZODIAC_PLUG_H

/// \file plug.h
///
/// \brief Contains the definition of the zodiac::Plug class and zodiac::PlugDirection enum.
///

#include <QGraphicsObject>
#include <QSet>
#include <QVector2D>

namespace zodiac {

class Node;
class Scene;
class Perimeter;
class PlugEdge;
class PlugLabel;

///
/// \brief The direction in which a Plug leads in relation to its Node.
///
enum class PlugDirection {
    IN      = 0, ///< INcoming Plug%s only accept incoming PlugEdge%s.
    OUT     = 1, ///< OUTcoming Plug%s only accept outoing PlugEdge%s.
    BOTH    = 2, ///< Plug%s cannot work in BOTH directions, but BOTH can be used as an argument.
};


///
/// \brief A single Plug of a Node.
///
/// Plug%s are managed by their respective Node and should not be tempered with directly.
///
///
class Plug : public QGraphicsObject
{

    Q_OBJECT

    ///
    /// \brief Since Plug%s are managed by Node%s, the Node class has access to otherwise private interface members.
    ///
    /// For example to rename a Plug or change its direction.
    ///
    friend class Node;

public: // methods

    ///
    /// \brief Constructor.
    ///
    /// \param [in] parent      Node to which this Plug belongs.
    /// \param [in] name        Name of this Plug, is unique in its Node.
    /// \param [in] direction   Plug%s can either be incoming or outgoing.
    ///
    explicit Plug(Node* parent, const QString &name, PlugDirection direction);

    ///
    /// \brief Adds a new PlugEdge to this Plug.
    ///
    /// Make sure that one of the two Plug%s of the PlugEdge is this Plug and the other Plug is not
    /// yet connected to this one.
    /// Also, incoming Plug%s can only have one connected PlugEdge.
    /// If any of those criteria is not met, an assertion error is thrown in debug mode while in release mode, this
    /// function just does nothing.
    ///
    /// \param [in] edge    New PlugEdge to add.
    ///
    void addEdge(PlugEdge* edge);

    ///
    /// \brief Removes an existing PlugEdge from this Plug.
    ///
    /// If the given PlugEdge is not connected to this Plug, an assertion error is thrown in debug mode, in relase mode
    /// the function just quietly returns without doing anything.
    ///
    /// \param [in] edge    PlugEdge to remove.
    ///
    void removeEdge(PlugEdge* edge);

    ///
    /// \brief Returns a list of all Plug%s that are connected to this one through an edge.
    ///
    /// \return List of all Plug%s connected to this one.
    ///
    QList<Plug*> getConnectedPlugs() const {return m_connectedPlugs.toList();}

    ///
    /// \brief Defines the shape of this Plug using angular values.
    ///
    /// \param [in] normal      New normal of this Plug in regards to its Node.
    /// \param [in] arclength   New arc length of this Plug in radians.
    ///
    void defineShape(QVector2D normal, qreal arclength);

    ///
    /// \brief Updates all connected PlugEdge%s, if the Plug has changed.
    ///
    void updateEdges() const;

    ///
    /// \brief Repositions the Plug based on the expansion / collapse animations of its Node.
    ///
    /// \param [in] expansion   New expansion factor [0 -> 1].
    ///
    void updateExpansion(qreal expansion);

    ///
    /// \brief Sets the highlight-state of the Plug.
    ///
    /// \param [in] highlight   <i>true</i> if the Plug is highlighted, <i>false</i> if it is not.
    ///
    void setHighlight(bool highlight);

    ///
    /// \brief The 2D normal vector of this Plug with regards to its Node.
    ///
    /// \return The normal vector.
    ///
    inline QVector2D getNormal() const {return m_normal;}

    ///
    /// \brief The unique name of this Plug.
    ///
    /// \return Name of this Plug.
    ///
    inline QString getName() const {return m_name;}

    ///
    /// \brief The Node containing and managing this Plug.
    ///
    /// \return Node of this Plug.
    ///
    inline Node* getNode() const {return m_node;}

    ///
    /// \brief The direction of this Plug, either incoming or outgoing.
    ///
    /// \return Direction of this Plug.
    ///
    inline PlugDirection getDirection() const {return m_direction;}

    ///
    /// \brief The target direction of this Plug.
    ///
    /// The target direction is calculated from the spacial arrangement of its connected Plug%s and is used for
    /// re-arranging the Plug%s around the Node.
    /// See \ref zodiac::Node::arrangePlugs() "Node::arrangePlugs()" for details.
    ///
    /// \brief The target normal of this Plug.
    ///
    QVector2D getTargetNormal() const;

    ///
    /// \brief The number of PlugEdge%s connected to this Plug.
    ///
    /// \return This Plug's edge count.
    ///
    inline int getEdgeCount() const {return m_edges.size();}

    ///
    /// \brief Only Plug%s with no connections can be removed.
    ///
    /// \return <i>true</i> if the Plug can be removed, <i>false</i> otherwise.
    ///
    inline bool isRemovable() const {return getEdgeCount() == 0;}

    ///
    /// \brief Tests if a Plug is connected to another Plug via a direct PlugEdge.
    ///
    /// \param [in] plug    Other Plug to test for connections.
    ///
    /// \return             <i>true</i> if this Plug is conneced to the other Plug via an PlugEdge.
    ///S
    inline bool isConnectedWith(Plug* plug) const {return m_connectedPlugs.contains(plug);}

    ///
    /// \brief Aquires the DrawEdge for this Plug.
    ///
    /// Make sure to release the DrawEdge again by calling releaseDrawEdge!
    /// These two functions are a safety mechanism to ensure that only one Plug at the time can work with the DrawEdge.
    ///
    /// This function should always succeed as long as the user has only a single mouse for input.
    /// If there is another Plug using the DrawEdge, an assertion error is thrown in debug mode while in release mode,
    /// the call is simply ignored.
    ///
    void aquireDrawEdge();

    ///
    /// \brief Advances the DrawEdge in reaction to the user moving the mouse while dragging the DrawEdge.
    ///
    /// Any calls to this function if the current Plug is not the edge-drawing Plug, are ignored in release mode and
    /// throw an assertion error in debug mode.
    ///
    void advanceDrawEdge(const QPointF& scenePos);

    ///
    /// \brief Releases the previously aquired DrawEdge.
    ///
    /// Any calls to this function if the current Plug is not the edge-drawing Plug, are ignored in release mode and
    /// throw an assertion error in debug mode.
    ///
    void releaseDrawEdge();

    ///
    /// \brief Updates all connected edges' labels to reflect changes in the Plug or its Node.
    ///
    void updateEdgeLabels();

    ///
    /// \brief Applies style changes in the class' static members to this instance.
    ///
    /// Is part of the scene-wide cascade of %updateStyle()-calls after a re-styling of the ZodiacGraph.
    ///
    void updateStyle();

    ///
    /// \brief Calculates and returns the priority factor for the arrangement of this Plug.
    ///
    /// The higher the priority factor, the more likely it is for a Plug to be placed at the best angular position in
    /// the \ref zodiac::Node::arrangePlugs "Plug arrangement".
    ///
    /// \return Arrangement priority factor.
    ///
    qreal getArrangementPriority();

signals:

    ///
    /// \brief Is emitted, when the Plug was renamed.
    ///
    /// \param [out] name   New name of the Plug.
    ///
    void plugRenamed(const QString& name);

public: // static methods

    ///
    /// \brief Width of a Plug in pixels.
    ///
    /// \return Plug width.
    ///
    static inline qreal getWidth() {return s_width;}

    ///
    /// \brief Sets a new width for all Plug%s.
    ///
    /// \param [in] width   New width of the Plug%s.
    ///
    static inline void setWidth(qreal width) {s_width=width;}

    ///
    /// \brief Color used to draw an incoming Plug when it is idle.
    ///
    /// \return Incoming idle color.
    ///
    static inline QColor getInColor() {return s_inColor;}

    ///
    /// \brief Sets a new color used to draw an incoming Plug when it is idle.
    ///
    /// \param [in] color   New color to use to draw an incoming Plug when it is idle.
    ///
    static inline void setInColor(QColor color) {s_inColor=color;}

    ///
    /// \brief Color used to draw an outgoing Plug when it is idle.
    ///
    /// \return Outgoing idle color.
    ///
    static inline QColor getOutColor() {return s_outColor;}

    ///
    /// \brief Sets a new color used to draw an outgoing Plug when it is idle.
    ///
    /// \param [in] color    New color to use to draw an outgoing Plug when it is idle.
    ///
    static inline void setOutColor(QColor color) {s_outColor=color;}

    ///
    /// \brief Color used to draw the Plug (incoming or outgoing) when it is highlighted.
    ///
    /// \return Highlight color.
    ///
    static inline QColor getHighlightColor() {return s_highlightColor;}

    ///
    /// \brief Sets a new color used to draw the Plug when it is highlighted.
    ///
    /// \param [in] color   New color to use to draw the Plug when it is highlighted.
    ///
    static inline void setHighlightColor(QColor color) {s_highlightColor=color;}

    ///
    /// \brief Behaviour flag that determines, if a Node automatically changes its expansion state to
    /// \ref zodiac::NodeExpansion::OUT "OUT" after connecting an incoming PlugEdge or not.
    ///
    /// \return <i>true</i> if so -- <i>false</i> otherwise.
    ///
    static inline bool getToggleExpansionOnEdgeCreation() {return s_toggleNodeExpansionOnEdgeCreation;}

    ///
    /// \brief Defines, whether the target Node's expansion state is changed to \ref zodiac::NodeExpansion::OUT "OUT"
    /// after connecting an incoming PlugEdge or not.
    ///
    /// \param [in] activate    <i>true</i> if so -- <i>false</i> otherwise.
    ///
    static inline void setToggleExpansionOnEdgeCreation(bool activate) {s_toggleNodeExpansionOnEdgeCreation=activate;}

    ///
    /// \brief Mouse button used to draw the DrawEdge from a Plug.
    ///
    /// \return The edge draw button.
    ///
    static inline Qt::MouseButton getDrawEdgeButton() {return s_drawEdgeButton;}

    ///
    /// \brief Defines the mouse button used to draw the DrawEdge from a Plug.
    ///
    /// \param [in] button  Mouse button used to draw the DrawEdge from a Plug.
    ///
    static inline void setDrawEdgeButton(Qt::MouseButton button) {s_drawEdgeButton=button;}

    ///
    /// \brief The Node currently targeted by the DrawEdge, can be nulltpr.
    ///
    /// \return Drag target Node or <i>nullptr</i>.
    ///
    static inline Node* getDragTargetNode() {return s_dragTargetNode;}

    ///
    /// \brief Clears the Node currently targeted by an edge-drag.
    ///
    static inline void clearDragTargetNode() {s_dragTargetNode=nullptr;}

    ///
    /// \brief The Plug currently targeted by the DrawEdge, can be nulltpr.
    ///
    /// \return Drag target Plug or <i>nullptr</i>.
    ///
    static inline Plug* getDragTargetPlug() {return s_dragTargetPlug;}

    ///
    /// \brief Clears the Plug currently targeted by an edge-drag, useful for Plug removal.
    ///
    static inline void clearDragTargetPlug() {s_dragTargetPlug=nullptr;}

protected: // methods

    ///
    /// \brief Rectangular outer bounds of the item, used for redraw testing.
    ///
    /// \return Boundary rectangle of the item.
    ///
    QRectF boundingRect() const;

    ///
    /// \brief Paints this item.
    ///
    /// \param [in] painter Painter used to paint the item.
    /// \param [in] option  Provides style options for the item.
    /// \param [in] widget  Optional widget that this item is painted on.
    ///
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

    ///
    /// \brief Exact boundary of the item used for collision detection among other things.
    ///
    /// \return Shape in local coordinates.
    ///
    QPainterPath shape() const;

    ///
    /// \brief Called when the mouse enteres the shape of the item.
    ///
    /// \param [in] event   Qt event object
    ///
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event);

    ///
    /// \brief Called when the mouse leaves the shape of the item.
    ///
    /// \param [in] event   Qt event object
    ///
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);

    ///
    /// \brief Is called, when the mouse is pressed while the cursor is on this item.
    ///
    /// \param [in] event   Qt event object.
    ///
    void mousePressEvent(QGraphicsSceneMouseEvent* event);

    ///
    /// \brief Called, when the mouse is moved after clicking on the item.
    ///
    /// Used to drag the DrawEdge.
    ///
    /// \param [in] event   Qt event object.
    ///
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);

    ///
    /// \brief Called if the mouse is released after a click on this item.
    ///
    /// Is always called after \ref zodiac::Plug::mousePressEvent "mousePressEvent" and possibly
    /// \ref zodiac::Plug::mousePressEvent "mousePressEvent".
    ///
    /// \param [in] event   Qt event object.
    ///
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

private: // methods for friends

    ///
    /// \brief  Sets a new direction for this Plug.
    ///
    /// Must only be called by zodiac::Node::togglePlugDirection().
    ///
    /// \param [in] direction   New direction of the Plug.
    ///
    inline void setDirection(PlugDirection direction) {Q_ASSERT(getEdgeCount()==0); m_direction = direction;}

    ///
    /// \brief Sets a new name for this Plug.
    ///
    /// Must only be called by zodiac::Node::renamePlug.
    ///
    /// \param [in] name    New name of this Plug.
    ///
    void setName(const QString& name);

private: // methods

    ///
    /// \brief Defines all members required for displaying the shape of the Plug.
    ///
    void updateShape();

    ///
    /// \brief Determines, if the Plug can currently react to mouse events or not.
    ///
    /// \return <i>false</i> if the Plug is incoming and has a connection or the Node is not in the right state.
    ///
    bool mayReceiveInput();

private: // members

    ///
    /// \brief Name of this Plug.
    ///
    QString m_name;

    ///
    /// \brief Direction of this Plug.
    ///
    PlugDirection m_direction;

    ///
    /// \brief Node that this Plug belongs to.
    ///
    Node* m_node;

    ///
    /// \brief Angular size of the Plug in radians.
    ///
    qreal m_arclength;

    ///
    /// \brief Normal vector from the Node to this Plug.
    ///
    QVector2D m_normal;

    ///
    /// \brief Shape used to draw this Plug.
    ///
    QPainterPath m_shape;

    ///
    /// \brief <i>true</i> if the Plug is currently highlighted -- <i>false</i> otherwise.
    ///
    bool m_isHighlighted;

    ///
    /// \brief All PlugEdge%s connected to this Plug that require updates when it is moved.
    ///
    QSet<PlugEdge*> m_edges;

    ///
    /// \brief PlugLabel of this Plug.
    ///
    PlugLabel* m_label;

    ///
    /// \brief All other Plug%s connected to this one.
    ///
    QSet<Plug*> m_connectedPlugs;

private: // static members

    ///
    /// \brief Width of the Plug around the Perimeter in pixels.
    ///
    static qreal s_width;

    ///
    /// \brief Color used to draw an incoming Plug when it is idle (not highlighted).
    ///
    static QColor s_inColor;

    ///
    /// \brief Color used to draw an outgoing Plug when it is idle (not highlighted).
    ///
    static QColor s_outColor;

    ///
    /// \brief Color used to draw the Plug when it is highlighted.
    ///
    static QColor s_highlightColor;

    ///
    /// \brief Option flag determining if, upon creation of a new PlugEdge, the target Node should expand its outgoing
    /// Plug%s.
    ///
    static bool s_toggleNodeExpansionOnEdgeCreation;

    ///
    /// \brief Mouse button used to draw the DrawEdge.
    ///
    static Qt::MouseButton s_drawEdgeButton;

    ///
    /// \brief The Node that the DrawEdge is currently hovering over.
    ///
    static Node* s_dragTargetNode;

    ///
    /// \brief The Plug that the DrawEdge is currently hovering over.
    ///
    static Plug* s_dragTargetPlug;

    ///
    /// \brief The Plug that has the DrawEdge currently aquired.
    ///
    static Plug* s_edgeDrawingPlug;

};

} // namespace zodiac

#endif // ZODIAC_PLUG_H
