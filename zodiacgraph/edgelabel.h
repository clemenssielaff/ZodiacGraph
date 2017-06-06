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

#ifndef ZODIAC_EDGELABEL_H
#define ZODIAC_EDGELABEL_H

///
/// \file edgelabel.h
///
/// \brief Contains the definition of the zodiac::EdgeLabel class.
///

#include <QFont>
#include <QGraphicsSimpleTextItem>

namespace zodiac {

///
/// \brief The label of a BaseEdge.
///
/// EdgeLabel%s are only visible, when the mouse is hovering the BaseEdge or EdgeArrow.
///
class EdgeLabel : public QGraphicsSimpleTextItem
{

public: // methods

    ///
    /// \brief Constructor.
    ///
    /// The EdgeLabel is a free-standing QGraphicsItem without a parent, even though it is closely managed by a
    /// BaseEdge instance.
    ///
    explicit EdgeLabel();

    ///
    /// \brief Extends the QGraphcisSimpleTextItem by adding an offset in relation to the size of the label.
    ///
    /// The given position is the center of where the EdgeLabel is supposed to be.
    /// The unaltered QGraphicsSimpleTextItem implementation of setPos() would place the top left corner of the label at
    /// that position -- so we add an offset first.
    ///
    /// \param [in] x   x-coordinate before offset.
    /// \param [in] y   y-coordinate before offset.
    ///
    void setPos(qreal x, qreal y);

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
    /// \brief Defines the color used to render the label text.
    ///
    /// \param [in] color   New color to use for rendering the label text.
    ///
    static inline void setColor(const QColor& color) {s_color=color;}

    ///
    /// \brief Transparency (alpha) of the of the label text.
    ///
    /// \return Label transparency (alpha) [0 -> 1].
    ///
    static inline qreal getTransparency() {return s_color.alphaF();}

    ///
    /// \brief Defines a new alpha value [0 -> 1] of the label text.
    ///
    /// \param [in] alpha   New alpha value [0 -> 1] of the label text.
    ///
    static inline void setTransparency(qreal alpha) {s_color.setAlphaF(alpha);}

    ///
    /// \brief Vertical offset of the label text to the position given with setPos.
    ///
    /// See zodiac::EdgeLabel::setVerticalOffset() for details.
    ///
    /// \return Vertical label offset, in heights of the label
    ///
    static inline qreal getVerticalOffset() {return s_verticalOffset;}

    ///
    /// \brief Vertical offset of the label to the position given with setPos in heights of the label.
    ///
    /// A value of 0.0 centers the label on the position given with setPos().<br>
    /// A value of 0.5 sets the bottom of the label at the position given with setPos().<br>
    /// A value of -0.5 sets the top of the label at the position given with setPos().<br>
    ///
    /// \param [in] offset  New vertical offset, in heights of the label
    ///
    static inline void setVerticalOffset(qreal offset) {s_verticalOffset=offset;}

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
    /// \brief Vertical offset of the label, in heights of the label.
    ///
    static qreal s_verticalOffset;

};

} // namespace zodiac

#endif // ZODIAC_EDGELABEL_H
