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

#ifndef ZODIAC_VIEW_H
#define ZODIAC_VIEW_H

/// \file view.h
///
/// \brief Contains the definition of the zodiac::View class.
///

#include <QGraphicsView>

class QEvent;
class QGestureEvent;
class QMouseEvent;
class QWheelEvent;

///
/// \brief The zodiac namespace.
///
/// All classes, functions and enums are part of the zodiac-namespace to keep them separated from whatever else is used
/// in the project.
///
namespace zodiac {

class Scene;

///
/// \brief Widget that displays the contents of a Scene.
///
/// The view defines the background color drawn behind the graph as well as the buttons used to interact with it.
/// It can be zoomed and moved within the extend of the scene.
/// Changes applied to the interaction settings of a View apply to all instances.
///
class View : public QGraphicsView
{
    Q_OBJECT

public: // methods

    ///
    /// \brief Constructor.
    ///
    /// \param [in] parent  Parent widget of this View.
    ///
    explicit View(QWidget *parent);

    ///
    /// \brief Displays the given Scene in this view.
    ///
    /// Reimplemented to force a zodiac::Scene to be viewed instead of a generic QGraphicsScene.
    ///
    /// \param [in] scene   Scene to view.
    ///
    void setScene(Scene *scene);

    ///
    /// \brief Applies static styling changes of the View class to this instance.
    ///
    /// Does not call the Scene::updateStyle() as well, as there might be different View instances viewing the same
    /// scene.
    ///
    void updateStyle();

public: // static methods

    ///
    /// \brief The color used to fill the background of all View%s.
    ///
    /// \return The background color.
    ///
    inline static QColor getBackgroundColor() {return s_backgroundColor;}

    ///
    /// \brief Sets a new color to use for the background of all View%s.
    ///
    /// Call updateStyle() on each instance to adopt the new background.
    ///
    /// \param [in] color   New background color.
    ///
    inline static void setBackgroundColor(const QColor& color) {s_backgroundColor=color;}

    ///
    /// \brief The zoom speed of all View%s.
    ///
    /// \return Zoom speed.
    ///
    inline static qreal getZoomSpeed() {return s_zoomSpeed;}

    ///
    /// \brief Sets a new zoom speed for all View%s.
    ///
    /// \param [in] speed   New zoom speed.
    ///
    inline static void setZoomSpeed(qreal speed) {s_zoomSpeed=speed;}

    ///
    /// \brief Button to use for view movements in all View%s.
    ///
    /// \return Movement button.
    ///
    inline static Qt::MouseButton getMoveButton() {return s_dragMoveButton;}

    ///
    /// \brief Sets a new button to use for view movement in all View%s.
    ///
    /// \param [in] button  New button to use for view movement.
    ///
    inline static void setMoveButton(Qt::MouseButton button) {s_dragMoveButton=button;}

    ///
    /// \brief Button to use for Node selection in all View%s.
    ///
    /// \return Selection button.
    ///
    inline static Qt::MouseButton getSelectionButton() {return s_selectionButton;}

    ///
    /// \brief Sets a new button to use for Node selection in all View%s.
    ///
    /// \param [in] button  New button to use for selection.
    ///
    inline static void setSelectionButton(Qt::MouseButton button) {s_selectionButton=button;}

    ///
    /// \brief The mouse button used to remove a Node from the Scene.
    ///
    /// \return Removal button.
    ///
    static inline Qt::MouseButton getRemovalButton() {return s_removalButton;}

    ///
    /// \brief Defines a new mouse button to remove a Node from the Scene.
    ///
    /// \param [in] button  Button to use.
    ///
    static inline void setRemovalButton(Qt::MouseButton button) {s_removalButton=button;}

    ///
    /// \brief The key used for activating the selected Node%s.
    ///
    /// \return The activation key.
    ///
    static inline int getActivationKey() {return s_activationKey;}

    ///
    /// \brief Sets a new key for activating selected Node%s.
    ///
    /// \param [in] key New activation key, must be one of Qt::Key.
    ///
    static inline void setActivationKey(int key) {s_activationKey=key;}

protected: // methods

    ///
    /// \brief The main event handler for any QWidget.
    ///
    /// Implemented to pipe touch gestures to gestureEvent().
    ///
    /// \param [in] event   Qt event object.
    ///
    /// \return             True if the event was recognized, otherwise it returns false
    ///
    bool event(QEvent* event);

    ///
    /// \brief gestureEvent
    ///
    /// \param [in] event   Qt gesture event object.
    ///
    /// \return             True if the event was recognized, otherwise it returns false
    ///
    bool gestureEvent(QGestureEvent* event);

    ///
    /// \brief Main event handler for the scrolling area of this QGraphicsView.
    ///
    /// \param [in] event   Qt event object.
    ///
    /// \return             True if the event was recognized, otherwise it returns false
    ///
    bool viewportEvent(QEvent* event);

    ///
    /// \brief Called, when the mouse is pressed while the mouse cursor hovers on this widget.
    ///
    /// \param [in] event   Qt event object.
    ///
    void mousePressEvent(QMouseEvent* event);

    ///
    /// \brief Called after mousePressEvent, when the mouse button has been released.
    ///
    /// \param [in] event   Qt event object.
    ///
    void mouseReleaseEvent(QMouseEvent* event);

    ///
    /// \brief Called when this view is doubleclicked without hitting an item.
    ///
    /// \param [in] event   Qt event object.
    ///
    void mouseDoubleClickEvent(QMouseEvent *event);

    ///
    /// \brief Called when the mouse wheel is moved while the cursor is over this widget.
    ///
    /// \param [in] event   Qt event object.
    ///
    void wheelEvent(QWheelEvent* event);

    ///
    /// \brief Request to repaint all or part of this widget.
    ///
    /// \param [in] event   Qt event object.
    ///
    void paintEvent(QPaintEvent* event);

private: // members

    ///
    /// \brief Zoom factor of this scene (could be inferred from the transformation, but this is simpler).
    ///
    qreal m_zoomFactor;

private: // static members

    ///
    /// \brief Background color of the node view.
    ///
    static QColor s_backgroundColor;

    ///
    /// \brief Speed factor for zooming in and out.
    ///
    static qreal s_zoomSpeed;

    ///
    /// \brief Mouse button to use for drag move.
    ///
    static Qt::MouseButton s_dragMoveButton;

    ///
    /// \brief Mouse button to use for rubberband selection.
    ///
    static Qt::MouseButton s_selectionButton;

    ///
    /// \brief Mouse button to remove the node.
    ///
    static Qt::MouseButton s_removalButton;

    ///
    /// \brief Button to press to "activate" the selected Node%s.
    ///
    static int s_activationKey;

    ///
    /// \brief Minimal zoom factor.
    ///
    static qreal s_minZoomFactor;

    ///
    /// \brief Maximal zoom factor.
    ///
    static qreal s_maxZoomFactor;

};

} // namespace zodiac

#endif // ZODIAC_VIEW_H
