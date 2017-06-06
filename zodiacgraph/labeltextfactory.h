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

#ifndef ZODIAC_LABELTEXTFACTORY_H
#define ZODIAC_LABELTEXTFACTORY_H

///
/// \file labeltextfactory.h
///
/// \brief Contains the definition of the zodiac::LabelTextFactory class.
///

#include <QSet>
#include <QString>

namespace zodiac {

class PlugEdge;

///
/// \brief The LabelTextFactory is a helper class for creating label texts.
///
/// The challenge is to produce text that appears centered around the arrow denoting the direction of the edge flow.
///
class LabelTextFactory
{

public: // methods

    ///
    /// \brief Constructor.
    ///
    /// \param [in] edges   The PlugEdge%s used to create the label text.
    ///
    explicit LabelTextFactory(const QSet<PlugEdge*>& edges);

    ///
    /// \brief Constructor overload.
    ///
    /// \param [in] edge    Single edge for which to create the labe.
    ///
    explicit LabelTextFactory(PlugEdge* edge);

    ///
    /// \brief Produces the label string.
    ///
    /// You can use the <code>maxNameLength</code> parameter to override the length of the longest name in the label.
    /// This is useful for EdgeGroupPair%s that have two LabelTextFactory%s that have to produce a single label that is
    /// centered around the arrow from both.
    ///
    /// \param [in] maxNameLength   (optional) Overrides the default length of the longest name
    ///
    /// \return                     Label string.
    ///
    QString produceLabel(int maxNameLength = 0) const;

    ///
    /// \brief The number of individual labels in this factory.
    ///
    /// \return Number of labels.
    ///
    int getLabelCount() const {return m_labelCount;}

    ///
    /// \brief The number of characters in the longest name of all labels.
    ///
    /// \return Maximum name length.
    ///
    int getMaxNameLength() const {return m_maxNameLength;}

public: // static methods

    ///
    /// \brief The newline-character used in the LabelTextFactory.
    ///
    /// Is the default one (<code>\\n</code>), but just in case, we have a single point of access for it.
    ///
    /// \return The newline-character.
    ///
    static const QString& getNewlineChar() {return s_newlineChar;}

    ///
    /// \brief Produces a horizontal line that can be used to separate the output of several LabelTextFactory%s.
    ///
    /// The maxNameLength paramter ensures that the length of the horizontal line does not exceed the rest of the label.
    ///
    /// \param [in] maxNameLength   The length of the longest name.
    ///
    /// \return                     Unbroken, horizontal line, vertically centered.
    ///
    static QString getHorizontalLine(int maxNameLength);

private: // members

    ///
    /// \brief List of a pair of label names.
    ///
    /// A <i>name</i> is one half of the label -- two <i>names</i> are connected using an arrow.
    ///
    QList<QPair<QString, QString>> m_namePairs;

    ///
    /// \brief The number of labels taken into account.
    ///
    int m_labelCount;

    ///
    /// \brief Character count of the longest name in all labels.
    ///
    int m_maxNameLength;

private: // static members

    ///
    /// \brief Character denoting the flow direction in an EdgeLabel.
    ///
    /// Should be something like: <code>" -> "</code> (including spaces) to produce: <code>foo.a -> bar.b</code>.
    ///
    static const QString s_arrowChar;

    ///
    /// \brief Character denoting the scope of a Plug in an EdgeLabel.
    ///
    /// Should be something like: <code>"."</code> to produce: <code>foo.a -> bar.b</code>.
    ///
    static const QString s_dotChar;

    ///
    /// \brief Character for whitespace fill in an EdgeLabel.
    ///
    /// If found a simple space works without problems.
    ///
    static const QString s_whitespaceChar;

    ///
    /// \brief Character for a linebreak in an EdgeLabel.
    ///
    static const QString s_newlineChar;

    ///
    /// \brief Character for an (somewhat) uninterrupted horizontal line.
    ///
    static const QString s_horizontalLineChar;

};

} // namespace zodiac

#endif // ZODIAC_LABELTEXTFACTORY_H
