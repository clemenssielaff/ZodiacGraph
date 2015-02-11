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

#ifndef ZODIAC_PLUGLABEL_H
#define ZODIAC_PLUGLABEL_H

///
/// \file pluglabel.h
///
/// \brief Contains the definition of the zodiac::PlugLabel class.
///

#include <QFont>
#include <QGraphicsItem>
#include <QStaticText>

namespace zodiac {

class Plug;

///
/// \brief The label of a Plug.
///
/// It is placed on the outside of the Plug and fades in and out with the Plug's expansion.
/// A PlugLabel does not react to mouse events.
///
class PlugLabel : public QGraphicsItem
{

public: // methods

    ///
    /// \brief Constructor.
    ///
    /// \param [in] parent  Plug item parenting this label.
    ///
    explicit PlugLabel(Plug* parent);

    ///
    /// \brief Updates the Plug's label text and transformation using information from its Plug.
    ///
    void updateShape();

    ///
    /// \brief Defines, whether to draw the PlugLabel as highlighted or not.
    ///
    /// \param [in] highlight   <i>true</i> if the label is highlighted, <i>false</i> if it is not.
    ///
    inline void setHighlight(bool highlight) {m_isHighlighted=highlight; update();}

    ///
    /// \brief Applies style changes in the class' static members to this instance.
    ///
    /// Is part of the scene-wide cascade of %updateStyle()-calls after a re-styling of the ZodiacGraph.
    ///
    void updateStyle();

public: // static methods

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

    ///
    /// \brief Color to render the label text.
    ///
    /// \return Label color.
    ///
    static inline QColor getColor() {return s_color;}

    ///
    /// \brief Defines the color to render the label text.
    ///
    /// \\param [in] color  Label color.
    ///
    static inline void setColor(const QColor& color) {s_color = color;}

    ///
    /// \brief Distance from the label to the Plug in pixels.
    ///
    /// \return Plug to PlugLabel distance in pixels
    ///
    static inline qreal getLabelDistance() {return s_labelDistance;}

    ///
    /// \brief Define a new distance from the PlugLabel to its Plug in pixels.
    ///
    /// \param [in] distance    New distance from the PlugLabel to its Plug in pixels.
    ///
    static inline void setLabelDistance(qreal distance) {s_labelDistance=distance;}

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
    /// \return Shape of the item in local coordinates.
    ///
    QPainterPath shape() const;

private: // members

    ///
    /// \brief Plug owning this PlugLabel.
    ///
    Plug* m_plug;

    ///
    /// \brief Text of this label.
    ///
    QStaticText m_text;

    ///
    /// \brief Bounding rectangle of this item.
    ///
    QRectF m_boundingRect;

    ///
    /// \brief Current transformation of the label text.
    ///
    QTransform m_transform;

    ///
    /// \brief <i>true</i> if this PlugLabel is drawn as highlighted -- <i>false</i> otherwise.
    ///
    bool m_isHighlighted;

private: // static members

    ///
    /// \brief Font used to draw the label.
    ///
    static QFont s_font;

    ///
    /// \brief Color used to draw the label.
    ///
    static QColor s_color;

    ///
    /// \brief Distance from the PlugLabel to its Plug in pixels.
    ///
    static qreal s_labelDistance;
};

} // namespace zodiac

#endif // ZODIAC_PLUGLABEL_H
