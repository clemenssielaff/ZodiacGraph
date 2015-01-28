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

#ifndef ZODIAC_NODELABEL_H
#define ZODIAC_NODELABEL_H

///
/// \file nodelabel.h
///
/// \brief Contains the definition of the zodiac::NodeLabel class.
///

#include <QFont>
#include <QGraphicsObject>
#include <QPen>
#include <QStaticText>

namespace zodiac {

class Node;

///
/// \brief Label of a Node.
///
/// The horizontal rounded rectangle in the center of the Node core, containing the display name of the Node as label.
///
class NodeLabel : public QGraphicsObject
{
    Q_OBJECT

public: // methods

    ///
    /// \brief Constructor.
    ///
    /// \param [in] parent  Node that this label belongs to.
    ///
    explicit NodeLabel(Node* parent);

    ///
    /// \brief Defines a new label text to display.
    ///
    /// \param [in] text     New text to display.
    ///
    void setText(const QString& text);

    ///
    /// \brief The height of this label including all elements.
    ///
    /// Used in calculating the Node's \ref zodiac::Node::getDeadZoneAngle() "dead zone angle".
    ///
    /// \return Label height.
    ///
    inline qreal getHeight() const {return m_boundingRect.height();}

    ///
    /// \brief Applies style changes in the class' static members to this instance.
    ///
    /// Is part of the scene-wide cascade of %updateStyle()-calls after a re-styling of the ZodiacGraph.
    ///
    void updateStyle();

public: // static methods

    ///
    /// \brief Color used to draw the label text.
    ///
    /// \return Label text color.
    ///
    static inline QColor getTextColor() {return s_textColor;}

    ///
    /// \brief Define a new color used to draw the label text.
    ///
    /// \param [in] color    New color used to draw the label text.
    ///
    static inline void setTextColor(const QColor& color) {s_textColor=color;}

    ///
    /// \brief Color used to fill the label background.
    ///
    /// \return Background color.
    ///
    static inline QColor getBackgroundColor() {return s_backgroundColor;}

    ///
    /// \brief Define a new color used to fill the label background.
    ///
    /// \param [in] color    New color used to fill the label background.
    ///
    static inline void setBackgroundColor(const QColor& color) {s_backgroundColor=color;}

    ///
    /// \brief Color used to draw the label outline.
    ///
    /// \return Label outline color.
    ///
    static inline QColor getLineColor() {return s_lineColor;}

    ///
    /// \brief Define a new color used to draw the label outline.
    ///
    /// \param [in] color    New color used to draw the label outline.
    ///
    static inline void setLineColor(const QColor& color) {s_lineColor=color; s_linePen.setColor(color);}

    ///
    /// \brief Width of the label's outline in pixels.
    ///
    /// \return Label outline width in pixels.
    ///
    static inline qreal getLineWidth() {return s_outlineWidth;}

    ///
    /// \brief Define a new width of the outline in pixels.
    ///
    /// \param [in] width   New width of the outline in pixels.
    ///
    static inline void setLineWidth(qreal width) {s_outlineWidth=qMax(0., width); s_linePen.setWidthF(width);}

    ///
    /// \brief Radius of the label's round edges in pixels.
    ///
    /// \return Round edge radius in pixels.
    ///
    static inline qreal getCornerRadius() {return s_roundEdgeRadius;}

    ///
    /// \brief Define a new radius of the label's round edges in pixels.
    ///
    /// \param [in] radius  New radius of the label's round edges in pixels.
    ///
    static inline void setCornerRadius(qreal radius) {s_roundEdgeRadius=radius;}

    ///
    /// \brief Vertical margin on each side of the label text in pixels.
    ///
    /// \return Vertical margin in pixels.
    ///
    static inline qreal getVerticalMargin() {return s_verticalMargin;}

    ///
    /// \brief Define a new vertical margin on each side of the text in pixels.
    ///
    /// \param [in] margin  New vertical margin on each side of the text in pixels.
    ///
    static inline void setVerticalMargin(qreal margin) {s_verticalMargin=margin;}

    ///
    /// \brief Horizontal margin on each side of the label text in pixels.
    ///
    /// \return Horizontal margin in pixels.
    ///
    static inline qreal getHorizontalMargin() {return s_horizontalMargin;}

    ///
    /// \brief Define a new horizontal margin on each side of the text in pixels.
    ///
    /// \param [in] margin  New horizontal margin on each side of the text in pixels.
    ///
    static inline void setHorizontalMargin(qreal margin) {s_horizontalMargin=margin;}

    ///
    /// \brief Family of the font currently used to render the label text.
    ///
    /// \return Label font family.
    ///
    static inline QString getFontFamily() {return s_font.family();}

    ///
    /// \brief Defines a new font family to render the label text.
    ///
    /// \param [in] family  New font family of the label.
    ///
    static inline void setFontFamily(const QString& family) {s_font.setFamily(family);}

    ///
    /// \brief Text size of the label in points.
    ///
    /// \return Label size in points.
    ///
    static inline qreal getPointSize() {return s_font.pointSizeF();}

    ///
    /// \brief Defines a new text size for the label in points.
    ///
    /// \param [in] pointSize   New text size in points (fractions allowed).
    ///
    static inline void setPointSize(qreal pointSize) {s_font.setPointSizeF(qMax(0.,pointSize));}

    ///
    /// \brief Current weight (=boldness) of the font used to render the label text.
    ///
    /// \return Label font weight.
    ///
    static inline int getWeight() {return s_font.weight();}

    ///
    /// \brief Defines the weight (=boldness) of the font used to render the label text.
    ///
    /// See <a href="http://qt-project.org/doc/qt-latest/qfont.html#Weight-enum">the Qt documentation</a> for available
    /// input values.
    ///
    /// \param [in] weight  New weight of the label font [0 -> 100].
    ///
    static inline void setWeight(QFont::Weight weight) {s_font.setWeight(weight);}

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
    /// \brief Called, when the mouse is clicked within the shape of this item.
    ///
    /// \param [in] event   Qt event.
    ///
    void mousePressEvent(QGraphicsSceneMouseEvent* event);

private: // members

    ///
    /// \brief Text of this label.
    ///
    QStaticText m_text;

    ///
    /// \brief Rectangular outline of the text (drawn as a rounded rectangle).
    ///
    QRectF m_outlineRect;

    ///
    /// \brief Bounding rectangle of the label.
    ///
    QRectF m_boundingRect;

    ///
    /// \brief Top-Left position of the text.
    ///
    QPointF m_textPos;

private: // static members

    ///
    /// \brief Color used to draw the label text.
    ///
    static QColor s_textColor;

    ///
    /// \brief Color used to fill the label's background.
    ///
    static QColor s_backgroundColor;

    ///
    /// \brief Color used to draw the label outline.
    ///
    static QColor s_lineColor;

    ///
    /// \brief Width of the outline.
    ///
    static qreal s_outlineWidth;

    ///
    /// \brief Radius of the label's round edges in pixels.
    ///
    static uint s_roundEdgeRadius;

    ///
    /// \brief Vertical margin on each side of the text in pixels.
    ///
    static qreal s_verticalMargin;

    ///
    /// \brief Horizontal margin on each side of the text in pixels.
    ///
    static qreal s_horizontalMargin;

    ///
    /// \brief Font used to render the label text.
    ///
    static QFont s_font;

    ///
    /// \brief Pen used to draw the outline.
    ///
    static QPen s_linePen;
};

} // namespace zodiac

#endif // ZODIAC_NODELABEL_H
