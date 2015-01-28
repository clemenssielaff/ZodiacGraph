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
