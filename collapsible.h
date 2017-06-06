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
#ifndef COLLAPSIBLE_H
#define COLLAPSIBLE_H

#include <QFrame>

class QPushButton;
class QVBoxLayout;

///
/// \brief Container class for a collapsible display widget and a title button.
///
class Collapsible : public QFrame
{
    Q_OBJECT

public: // methods

    ///
    /// \brief Constructor.
    ///
    /// \param [in] parent      Qt parent widget.
    ///
    Collapsible(QWidget *parent);

    ///
    /// \brief Defines a new widget to contain in this Collapsible.
    ///
    /// Takes possession of the widget and resets its parent to this.
    /// If the Collapsible already contains a widget,
    ///
    /// \param [in] displayWidget   The widget to display / hide in this Collapsible.
    ///
    void setWidget(QWidget* displayWidget);

public slots:

    ///
    /// \brief Updates the direction of the arrow next to the title as well as the title string.
    ///
    /// \param [in] title   New title string. If undefinded, the current title is used.
    ///
    void updateTitle(const QString& title = 0);

public: // static methods

    ///
    /// \brief Returns the maximum width of a Collapsible widget in pixels.
    ///
    /// \return The maximum width of a Collapsible widget in pixels.
    ///
    static int getMaximumWidth() {return s_maxWidth;}

private slots:

    ///
    /// \brief Toggles the display of the collapsible widget.
    ///
    void toggleCollapse();

private: // members

    ///
    /// \brief The layout stacking the collapsible title and the widget.
    ///
    QVBoxLayout* m_mainLayout;

    ///
    /// \brief The widget contained and displayed / hidden in this Collapsible.
    ///
    QWidget* m_displayWidget;

    ///
    /// \brief Title button of the Collapsible.
    ///
    QPushButton* m_titleButton;

    ///
    /// \brief Title of this Collapsible widget.
    ///
    QString m_title;

private : // static members

    ///
    /// \brief Character sequence of the downward, "expansion" arrow in front of the tile.
    ///
    static const QString s_downArrow;

    ///
    /// \brief Character sequence of the upward, "collapse" arrow in front of the tile.
    ///
    static const QString s_upArrow;

    ///
    /// \brief Maximum width of a Collapsible in pixels.
    ///
    static const int s_maxWidth;
};

#endif // COLLAPSIBLE_H
