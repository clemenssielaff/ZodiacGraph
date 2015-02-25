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

#ifndef ZODIAC_NODE_H
#define ZODIAC_NODE_H

/// \file node.h
///
/// \brief Contains the definition of the zodiac::Node class and zodiac::NodeExpansion enum.
///

#include <QGraphicsObject>
#include <QPropertyAnimation>
#include <QSet>
#include <QUuid>

namespace zodiac {

class NodeLabel;
class Perimeter;
class Plug;
class StraightEdge;
class Scene;
enum class PlugDirection;

///
/// \enum NodeExpansion
///
/// Describes, whether the incoming or the outgoing Plug%s of a Node are displayed -- or both or none.
///
/// Note that the state change is immediate, so a Node might be in a NodeExpansion::NONE state, while its Plug%s are
/// still displaying the collapse-animation.
///
enum class NodeExpansion{
    NONE    = 0, ///< No expansion.
    IN      = 1, ///< Incoming Plug%s are expanded.
    OUT     = 2, ///< Outgoing Plug%s are expanded.
    BOTH    = 3  ///< Both types of Plug are expanded.
};

///
/// \brief A Node in the Zodiac Graph.
///
/// A Node manages its Plug%s, and acts as the parent for a Perimeter and a NodeLabel though Qt's parent-child
/// mechanism.
///
/// The class offers a variety of styling-options to control to visual appearance of a Node.
/// Depending on the changes made, you must call Scene::updateStyle() afterwards, so each Node can adapt the new style.
/// If you want to be on the safe side, just call it after every style change but realize that it forces every
/// QGraphicsItem to update which might take a while (not long, but you wouldn't want to do it every other millisecond,
/// for example).
///
/// The Node class emits several signals for user interactions that originated from the graph directly, not from the
/// business logic of the program.
/// So for example, connecting a new PlugEdge to one of the Plug%s causes a signal to be emitted,
/// but renaming the Node does not, because the only way of triggering this interaction is through the API.
///
class Node : public QGraphicsObject
{
    Q_OBJECT

    ///
    /// \brief The outgoing spread factor [0 -> 1] of this Node.
    ///
    /// When this number is 1.0, the outgoing Plug%s of this Node are fully expanded.
    /// At 0.0, they are fully collapsed (hidden inside the core)
    ///
    Q_PROPERTY(qreal outgoingSpread MEMBER m_outgoingExpansionFactor WRITE updateOutgoingSpread)

    ///
    /// \brief The incoming spread factor [0 -> 1] of this Node.
    ///
    /// When this number is 1.0, the incoming Plug%s of this Node are fully expanded.
    /// At 0.0, they are fully collapsed (hidden inside the core)
    ///
    Q_PROPERTY(qreal incomingSpread MEMBER m_incomingExpansionFactor WRITE updateIncomingSpread)

public: // methods

    ///
    /// \brief Constructor
    ///
    /// Creates the Perimeter and NodeLabel items used by this Node.
    ///
    /// \param [in] scene       Scene managing this Node.
    /// \param [in] displayName Display name of this Node, does not have to be unique.
    /// \param [in] uuid        (optional) The unique identifier of this Node.
    ///
    explicit Node(Scene* scene, const QString& displayName, const QUuid& uuid = QUuid());

    ///
    /// \brief The unique identifier of this Node.
    ///
    /// The unique identifier is generated at construction and never changes.
    /// It is a UUID, meaning that it is not humanly readible - use the display name to differentiate Nodes visually in
    /// the graph.
    ///
    /// \return Uuid of this Node.
    ///
    const QUuid& getUniqueId() const {return m_uniqueId;}

    ///
    /// \brief Creates and adds a new Plug to this Node.
    ///
    /// The name of the node is only a proposal, since every name of every Plug of a Node has to be unique.
    /// If the proposed name is already taken, a suffix is appended to it.
    ///
    /// Even though PlugDirection offers a third option, a Plug can only be either <b>IN</b>coming or <b>OUT</b>going.
    /// Trying to create a Plug with PlugDirection::BOTH will result in an incoming plug in release mode and an
    /// assertion failure in debug mode.
    ///
    /// \param [in] name        The proposed name of the plug.
    /// \param [in] direction   Direction of the plug.
    ///
    /// \return                 The created Plug.
    ///
    Plug* createPlug(const QString& name, PlugDirection direction);

    ///
    /// \brief Removes an existing Plug from this Node.
    ///
    /// Only plugs without any PlugEdge%s can be removed.
    /// Trying to removing a Plug that has connected PlugEdge%s causes this function to return <i>false</i> and not to
    /// anything.
    ///
    /// If this method returns true, the given Plug has been scheduled for deletion by Qt and may dissappear at any
    /// moment.
    /// Most likely, it is already gone by the time this function returns.<br>
    /// <b>Do not make any further use of the pointer after this method returns true!</b>
    ///
    /// Make sure that the Node actually contains the given Plug.
    /// If it doesn't, calling this function returns <i>false</i> in release mode and will throw an assertion error
    /// in debug mode.
    ///
    /// \param [in] plug    The Plug to remove.
    ///
    /// \return             <i>true</i> if the Plug was removed, <i>false</i> otherwise.
    ///
    bool removePlug(Plug* plug);

    ///
    /// \brief Adds a new StraightEdge to this Node.
    ///
    /// The Node does not care, if it is a "real" StraightEdge or a StraightDoubleEdge, as both behave the same with
    /// regards to the Node.
    ///
    /// If <i>edge</i> is already part of the Node, the function call is effectively ignored.
    ///
    /// \param [in] edge    New StraightEdge to add.
    ///
    inline void addStraightEdge(StraightEdge* edge) {m_straightEdges.insert(edge);}

    ///
    /// \brief Removes an existing StraightEdge from this Node.
    ///
    /// If the passed StraightEdge is not part of this Node, this method will do nothing in release mode and throw
    /// an assertion error in debug mode.
    ///
    /// \param [in] edge    StraightEdge to remove.
    ///
    inline void removeStraightEdge(StraightEdge* edge) {Q_ASSERT(m_straightEdges.contains(edge));
                                                        m_straightEdges.remove(edge);}

    ///
    /// \brief Returns the Plug closest to a position within the Node or its Perimeter.
    ///
    /// The method takes a second parameter to limit the search to incoming / outgoing nodes or include both.
    /// If incoming Plug%s are requested, only those without an existing connection are considered.
    ///
    /// \param [in] pos         Position from which to calculate the nearest Plug, in coordinates local to this Node.
    /// \param [in] direction   Direction of the requested Plug%s, can also be PlugDirection::BOTH.
    ///
    /// \return                 Closest Plug or <i>nullptr</i>, if the Node has no Plug%s.
    ///
    Plug* getClosestPlugTo(const QPointF& pos, PlugDirection direction);

    ///
    /// \brief The radius of the Perimeter of this Node.
    ///
    /// The Perimeter can shrink and grow with the number of Plug%s in a Node.
    ///
    /// \return Radius of the Perimeter of this Node.
    ///
    qreal getPerimeterRadius() const;

    ///
    /// \brief The Scene that this Node belongs to.
    ///
    /// \return Scene of this Node.
    ///
    inline Scene* getScene() {return m_scene;}

    ///
    /// \brief The display name of this Node.
    ///
    /// \return Display name of this Node.
    ///
    inline QString getDisplayName() const {return m_displayName;}

    ///
    /// \brief Sets s new display name for this Node.
    ///
    /// Since the Node is not internally identified by this name but its unique ID, the display name coes not have to be
    /// unique in the scene.
    ///
    /// \param [in] displayName New display name of this Node.
    ///
    void setDisplayName(const QString& displayName);

    ///
    /// \brief Renames an existing Plug of this Node.
    ///
    /// Make sure that this Node actually contains the given Plug.
    /// If it doesn't, calling this function returns an empty QString in release mode and will throw an assertion error
    /// in debug mode.
    ///
    /// \param [in] plug        The Plug to rename.
    /// \param [in] newName     Proposed new name of the plug.
    ///
    /// \return                 Actual, unique new name of the plug.
    ///
    QString renamePlug(Plug* plug, const QString& newName);

    ///
    /// \brief Changes the direction of a Plug from IN to OUT or vice-versa.
    ///
    /// Direction changes can only occur, if the Plug has no PlugEdge%s connected to it.
    ///
    /// Make sure that this Node actually contains the given Plug.
    /// If it doesn't, calling this function always returns <i>false</i> in release mode and will throw an assertion
    /// error in debug mode.
    ///
    /// \param [in] plug    The Plug whose direction to toggle.
    ///
    /// \return             <i>true</i> if the Plug was able to change its direction -- <i>false</i> otherwise.
    ///
    bool togglePlugDirection(Plug *plug);

    ///
    /// \brief A list of all Plug%s of this Node.
    ///
    /// \return All Plug%s of this Node.
    ///
    QList<Plug*> getPlugs() const {return m_allPlugs.values();}

    ///
    /// \brief Queries a single Plug from this Node.
    ///
    /// \param [in] name    Name of the requested Plug.
    ///
    /// \return             The requested Plug or nullptr, if there is no Plug by the name.
    ///
    Plug* getPlug(const QString& name) const {return m_allPlugs.value(name, nullptr);}

    ///QS
    /// \brief  The current expansion factor [0 -> 1] of the incoming Plug%s of this Node.
    ///
    /// \return Incoming expansion factor.
    ///
    inline qreal getIncomingExpansionFactor() const {return m_incomingExpansionFactor;}

    ///
    /// \brief  The current expansion factor [0 -> 1] of the outgoing Plug%s of this Node.
    ///
    /// \return Outgoing expansion factor.
    ///
    inline qreal getOutgoingExpansionFactor() const {return m_outgoingExpansionFactor;}

    ///
    /// \brief Tests, whether this Node could currently be removed from the Scene.
    ///
    /// \return <i>true</i> if the node has no PlugEdge%s connected -- <i>false</i> otherwise.
    ///
    inline bool isRemovable() const {return m_straightEdges.size()==0;}

    ///
    /// \brief The current Expansions state of this Node.
    ///
    /// \return Current ExpansionState of this Node.
    ///
    inline NodeExpansion getExpansionState() const {return m_expansionState;}

    ///
    /// \brief Sets the expansion of the node only if it is not currently forced open.
    ///
    /// \param [in] newState New state to expand.
    ///
    inline void softSetExpansion(NodeExpansion newState){
        if(m_expansionState!=NodeExpansion::BOTH){setExpansion(newState);}
    }

    ///
    /// \brief Soft reset means: reset if the node is not currently forced open.
    ///
    inline void softResetExpansion() {
        if(m_expansionState!=NodeExpansion::BOTH){setExpansion(m_lastExpansionState);}
    }

    ///
    /// \brief Collapses the Node if is currently forced open.
    ///
    inline void forceCollapse(){
        if(m_expansionState==NodeExpansion::BOTH){
            setExpansion(NodeExpansion::NONE); m_lastExpansionState = NodeExpansion::NONE;}
    }

    ///
    /// \brief Toggles the expansion state between IN and OUT.
    ///
    inline void toggleExpansion() {
        if(m_expansionState==NodeExpansion::IN){setExpansion(NodeExpansion::OUT);}
        else if(m_expansionState==NodeExpansion::OUT){setExpansion(NodeExpansion::IN);}
    }

    ///
    /// \brief Toggles the forced expansion of this node on or off.
    ///
    inline void toggleForcedExpansion(){
        if(m_expansionState==NodeExpansion::BOTH){resetExpansion();}else{setExpansion(NodeExpansion::BOTH);}
    }

    ///
    /// \brief Applies style changes in the class' static members to this instance.
    ///
    /// Is part of the scene-wide cascade of %updateStyle()-calls after a re-styling of the ZodiacGraph.
    ///
    void updateStyle();

signals:

    ///
    /// \brief Emitted, when the \ref zodiac::View::s_activationKey "activation key" is pressed while this Node is
    /// selected.
    ///
    void nodeActivated();

    ///
    /// \brief Emitted, when the display name of the Node was changed.
    ///
    /// \param [out] name   New display name of the Node.
    ///
    void nodeRenamed(const QString& name);

    ///
    /// \brief Emitted, when the \ref zodiac::View::s_removalButton "removal button" is pressed on this Node.
    ///
    void removalRequested();

    ///
    /// \brief Is emitted, when an incoming Plug of this Node has been connected to an outgoing Plug of another Node.
    ///
    /// \param [out] myInput        The input Plug of this Node.
    /// \param [out] otherOutput    The output Plug of the source Node.
    ///
    void inputConnected(Plug* myInput, Plug* otherOutput);

    ///
    /// \brief Is emitted, when an outgoing Plug of this Node has been connected to an incoming Plug of another Node.
    ///
    /// \param [out] myOutput   The output Plug of this Node.
    /// \param [out] otherInput The input Plug of the target Node.
    ///
    void outputConnected(Plug* myOutput, Plug* otherInput);

    ///
    /// \brief Is emitted, when a connection from an incoming Plug of this Node was removed.
    ///
    /// \param [out] myInput        The input Plug of this Node.
    /// \param [out] otherOutput    The output Plug of the source Node.
    ///
    void inputDisconnected(Plug* myInput, Plug* otherOutput);

    ///
    /// \brief Is emitted, when a connection from an outgoing Plug of this Node was removed.
    ///
    /// \param [out] myOutput   The output Plug of this Node.
    /// \param [out] otherInput The input Plug of the target Node.
    ///
    void outputDisconnected(Plug* myOutput, Plug* otherInput);

public: // static methods

    ///
    /// \brief The radius of Node cores in pixels.
    ///
    /// \return Core radius in pixels.
    ///
    static inline qreal getCoreRadius() {return s_coreRadius;}

    ///
    /// \brief Sets a new core radius for all nodes.
    ///
    /// \param [in] radius  New core radius in pixels.
    ///
    static inline void setCoreRadius(qreal radius) {s_coreRadius = qMax(0.,radius);}

    ///
    /// \brief Fill color of an idle Node core.
    ///
    /// \return Idle fill color.
    ///
    static inline const QColor& getIdleColor() {return s_idleColor;}

    ///
    /// \brief Sets a new fill color of idle Node cores.
    ///
    /// \param [in] color   New idle color of the node core.
    ///
    static inline void setIdleColor(const QColor& color) {s_idleColor = color;}

    ///
    /// \brief Fill color of a selected Node core.
    ///
    /// \return Selected fill color.
    ///
    static inline const QColor& getSelectedColor() {return s_selectedColor;}

    ///
    /// \brief Sets a new fill color of selected Node cores.
    ///
    /// \param [in] color   New selected color of the node core.
    ///
    static inline void setSelectedColor(const QColor& color) {s_selectedColor = color;}

    ///
    /// \brief The color used to draw the outline around the node core.
    ///
    /// \return Core outline color.
    ///
    static inline const QColor& getOutlineColor() {return s_outlineColor;}

    ///
    /// \brief Sets a new color used to draw the outline around the node core.
    ///
    /// \param [in] color   New core outline color.
    ///
    static inline void setOutlineColor(const QColor& color) {s_outlineColor = color; updateOutlinePen();}

    ///
    /// \brief Width of the outline around the node core in pixels.
    ///
    /// \return Core outline width;
    ///
    static inline qreal getOutlineWidth() {return s_outlineWidth;}

    ///
    /// \brief Sets a new width of the outline around the node core in pixels.
    ///
    /// \param [in] width   New core outline width;
    ///
    static inline void setOutlineWidth(qreal width) {s_outlineWidth = qMax(0.,width); updateOutlinePen();}

    ///
    /// \brief First part of the suffix used to make duplicate plug names unique.
    ///
    /// The second part is an integer counter.
    ///
    /// \return Suffix used between the proposed Plug name and the counter.
    ///
    static inline const QString& getPlugSuffix() {return s_plugSuffix;}

    ///
    /// \brief Sets a new first part of the suffix used to make duplicate plug names unique.
    ///
    /// This method does not retroactively change existing Plug names, only future ones.
    ///
    /// \param [in] suffix  New suffix used between the proposed Plug name and the counter.
    ///
    static inline void setPlugSuffix(const QString& suffix) {s_plugSuffix=suffix;}

    ///
    /// \brief The angular size of Plug%s in pixels.
    ///
    /// \brief Angular size of Plug%s in pixels.
    ///
    static inline qreal getPlugSweep() {return s_plugSweep;}

    ///
    /// \brief Sets the angular size of Plug%s in pixels.
    ///
    /// \param [in] plugSweep   New angular size of Plug%s in pixels.
    ///
    static inline void setPlugSweep(qreal plugSweep) {s_plugSweep = qMax(0.,plugSweep);}

    ///
    /// \brief The angular size of the mandatory gap between two Plug%s in pixels.
    ///
    /// \return Angular size of the gap between two Plug%s in pixels.
    ///
    static inline qreal getGapSweep() {return s_plugGap;}

    ///
    /// \brief Sets a new angular size of the mandatory gap between two Plug%s in pixels.
    ///
    /// \param [in] gapSize New angular size of the gap between two Plug%s in pixels.
    ///
    static inline void setGapSweep(qreal gapSize) {s_plugGap = qMax(0.,gapSize);}

    ///
    /// \brief The duration for a full expansion animation of the Node in milliseconds.
    ///
    /// \return Node expansion duration in milliseconds
    ///
    static inline int getExpandDuration() {return s_expandDuration;}

    ///
    /// \brief Sets a new duration for a full expansion animation of the Node in milliseconds.
    ///
    /// \param [in] duration    New duration in milliseconds.
    ///
    static inline void setExpandDuration(int duration) {s_expandDuration=duration;}

    ///
    /// \brief The duration for a full collapse animation of the Node in milliseconds.
    ///
    /// \return Node collapse duration in milliseconds
    ///
    static inline int getCollapseDuration() {return s_collapseDuration;}

    ///
    /// \brief Sets a new duration for a full collapse animation of the Node in milliseconds.
    ///
    /// \param [in] duration    New duration in milliseconds.
    ///
    static inline void setCollapseDuration(int duration) {s_collapseDuration=duration;}

    ///
    /// \brief The easing animation curve for the expand animation.
    ///
    /// \return Expansion easing curve.
    ///
    static inline QEasingCurve getExpandCurve() {return s_expandCurve;}

    ///
    /// \brief Sets a new easing animation curve for the expand animation.
    ///
    /// \param [in] curve   New animation curve for Node expansion.
    ///
    static inline void setExpandCurve(QEasingCurve curve) {s_expandCurve=curve;}

    ///
    /// \brief The easing animation curve for the collapse animation.
    ///
    /// \return Collapse easing curve.
    ///
    static inline QEasingCurve getCollapseCurve() {return s_collapseCurve;}

    ///
    /// \brief Sets a new easing animation curve for the collapse animation.
    ///
    /// \param [in] curve   New animation curve for Node collapse.
    ///
    static inline void setCollapseCurve(QEasingCurve curve) {s_collapseCurve=curve;}

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
    void hoverEnterEvent(QGraphicsSceneHoverEvent * event);

    ///
    /// \brief Called when the mouse leaves the shape of the item.
    ///
    /// \param [in] event   Qt event object
    ///
    void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);

    ///
    /// \brief Called, when the mouse is pressed when the cursor is on the item.
    ///
    /// \param [in] event   Qt event object.
    ///
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    ///
    /// \brief Called, when the mouse is moved after clicking on the item.
    ///
    /// Updates all connected plugs' edges.
    /// Also works if the node is dragged outside the view.
    ///
    /// \param [in] event   Qt event object.
    ///
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);

    ///
    /// \brief Called if the mouse is released after a click on this item.
    ///
    /// Is always called after \ref zodiac::Node::mousePressEvent "mousePressEvent" and possibly
    /// \ref zodiac::Node::mouseMoveEvent "mouseMoveEvent".
    ///
    /// \param [in] event   Qt event object.
    ///
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

    ///
    /// \brief Called when this item is double-clicked.
    ///
    /// \param [in] event   Qt event object.
    ///
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

    ///
    /// \brief Called, when a key is pressed while this item is focused.
    ///
    /// \param [in] event   Qt event object.
    ///
    void keyPressEvent(QKeyEvent* event);

private: // methods

    ///
    /// \brief Returns a unique name for a Plug, based on a given proposal.
    ///
    /// The returned name is a succession of the proposal, the s_plugSuffix the next highest integer (starting at 2)
    /// that makes the name unique.
    ///
    /// The optional plug parameter can be used to find a new unique name for an existing plug.
    /// This way, if you want to rename <i>bar_2</i> to <i>bar</i> with <i>bar</i> already taken, the returned name is
    /// <i>bar_2</i> not <i>bar_3</i>.
    ///
    /// \param [in] name    Proposed name.
    /// \param [in] plug    (optional) Plug for which to get the unique name, can be <i>nullptr</i>.
    /// \return             Unique name from the proposed.
    ///
    QString getUniquePlugName(const QString& name, const Plug *plug = nullptr);

private slots:

    ///
    /// \brief Called, when the Node has completly collapsed, its Perimeter and its Plug%s are hidden.
    ///
    void hasCompletelyCollapsed();

    ///
    /// \brief Called, when the Node is about to expand fresh from its core.
    ///
    void aboutToExpandAfresh();

private: // methods

    ///
    /// \brief Sets a new expansion state for this Node.
    ///
    /// Also takes care of starting and stopping the appropriate expanding- an collapsing-animations.
    ///
    /// \param [in] newState New expansion state of this node.
    ///
    void setExpansion(NodeExpansion newState);

    ///
    /// \brief Resets the expansion state to the previous one (useful for changing in and out of the BOTH state).
    ///
    inline void resetExpansion() {setExpansion(m_lastExpansionState);}

    ///
    /// \brief Updates all edges connecting to any Plug of this Node.
    ///
    void updateConnectedEdges();

    ///
    /// \brief Called by the incoming expansion and collapse-animations to update the spread factor of incoming Plug%s.
    ///
    /// \param [in] expansion   New expansion value.
    ///
    void updateIncomingSpread(qreal expansion);

    ///
    /// \brief Called by the outgoing expansion and collapse-animations to update the spread factor of outgoing Plug%s.
    ///
    /// \param [in] expansion   New expansion value.
    ///
    void updateOutgoingSpread(qreal expansion);

    ///
    /// \brief Arranges the Plug%s of this Node around the Perimeter.
    ///
    /// The order is based on the Plug%s target direction and preferred angle.
    ///
    void arrangePlugs();

    ///
    /// \brief The sweep angle of a Plug of this Node in radians.
    ///
    /// \return Sweep angle of a Plug of this Node in radians.
    ///
    qreal getPlugAngle() const;

    ///
    /// \brief The gap angle between Plug%s of this Node in radians.
    ///
    /// \return Gap angle between Plug%s in radians.
    ///
    qreal getGapAngle() const;

    ///
    /// \brief The deadzone angle is the part of the Perimeter poentially obscured by the NodeLabel.
    ///
    /// \return The deadzone angle in radian.
    ///
    qreal getDeadZoneAngle() const;

    ///
    /// \brief Adjusts the radius of the Perimeter of this Node based on the number of its Plug%s.
    ///
    void adjustRadius();

private: // static methods

    ///
    /// \brief Updates the pen used to outline the node after its parameters have changed.
    ///
    static void updateOutlinePen();

private: // members

    ///
    /// \brief Node scene containing this node.
    ///
    Scene* m_scene;

    ///
    /// \brief The name of this Node.
    ///
    QString m_displayName;

    ///
    /// \brief Unique ID of this Node used for serialization.
    ///
    QUuid m_uniqueId;

    ///
    /// \brief Animation of the outgoing Plug%s expandind out of the Node.
    ///
    QPropertyAnimation m_outgoingExpandAnimation;

    ///
    /// \brief Animation of the outgoing Plug%s collapsing back into the Node.
    ///
    QPropertyAnimation m_outgoingCollapseAnimation;

    ///
    /// \brief Animation of the incoming Plug%s expandind out of the Node.
    ///
    QPropertyAnimation m_incomingExpandAnimation;

    ///
    /// \brief Animation of the incoming Plug%s collapsing back into the Node.
    ///
    QPropertyAnimation m_incomingCollapseAnimation;

    ///
    /// \brief Factor of the outgoing Plug expansion [0 -> 1].
    ///
    qreal m_outgoingExpansionFactor;

    ///
    /// \brief Factor of the incoming Plug expansion [0 -> 1].
    ///
    qreal m_incomingExpansionFactor;

    ///
    /// \brief Perimeter item drawn around the Node core.
    ///
    Perimeter* m_perimeter;

    ///
    /// \brief All Plug%s of this Node, indexed by name.
    ///
    /// A Plug can be uniquely identified by its name.
    ///
    QMap<QString, Plug*> m_allPlugs;

    ///
    /// \brief All outgoing Plug%s of this Node.
    ///
    QSet<Plug*> m_outgoingPlugs;

    ///
    /// \brief All incoming Plug%s of this Node.
    ///
    QSet<Plug*> m_incomingPlugs;

    ///
    /// \brief All StraightEdge%s connected to this Node.
    ///
    QSet<StraightEdge*> m_straightEdges;

    ///
    /// \brief The NodeLabel of this Node.
    ///
    /// Is owned by the QGraphisScene and deleted automatically with this Node.
    ///
    NodeLabel* m_label;

    ///
    /// \brief Bounding rectangle of the Node core.
    ///
    QRectF m_boundingRect;

    ///
    /// \brief The current expansion state of this Node.
    ///
    NodeExpansion m_expansionState;

    ///
    /// \brief The last expansion state (to return to from \ref zodiac::NodeExpansion::BOTH "NodeExpansion::BOTH").
    ///
    NodeExpansion m_lastExpansionState;

private: // static members

    ///
    /// \brief Radius of the core for all Node%s in pixels.
    ///
    static qreal s_coreRadius;

    ///
    /// \brief Color used to fill the Node core when it is idle (not selected).
    ///
    static QColor s_idleColor;

    ///
    /// \brief Color used to fill the Node core when it is selected.
    ///
    static QColor s_selectedColor;

    ///
    /// \brief Color to draw the Node outline with.
    ///
    static QColor s_outlineColor;

    ///
    /// \brief Width of the Node outline in pixels.
    ///
    static qreal s_outlineWidth;

    ///
    /// \brief Suffix inserted if a Plug is renamed to an existing name.
    ///
    static QString s_plugSuffix;

    ///
    /// \brief Arclength sweep of a Plug in pixels.
    ///
    static qreal s_plugSweep;

    ///
    /// \brief Arclength size of the mandatory gap between two Plug%s.
    ///
    static qreal s_plugGap;

    ///
    /// \brief Duration of the Node expansion animation from 0% -> 100% in milliseconds.
    ///
    static int s_expandDuration;

    ///
    /// \brief Duration of the Node collapse animation from 100% -> 0% in milliseconds.
    ///
    static int s_collapseDuration;

    ///
    /// \brief Easing animation curve for Node expansion.
    ///
    static QEasingCurve s_expandCurve;

    ///
    /// \brief Easing animation curve for Node collapse.
    ///
    static QEasingCurve s_collapseCurve;

    ///
    /// \brief Pen used to draw the Node outline.
    ///
    static QPen s_linePen;

    ///
    /// \brief Used to determine, whether a mouse clicked on the Node was a click or a drag.
    ///
    static bool s_mouseWasDragged;

};

} // namespace zodiac

#endif // ZODIAC_NODE_H
