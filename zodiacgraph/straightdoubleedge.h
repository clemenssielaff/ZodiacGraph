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

#ifndef ZODIAC_STRAIGHTDOUBLEEDGE_H
#define ZODIAC_STRAIGHTDOUBLEEDGE_H

///
/// \file straightdoubleedge.h
///
/// \brief Contains the definition of the zodiac::StraightDoubleEdge class.
///

#include "straightedge.h"

namespace zodiac {

class Node;
class Scene;
class EdgeGroupInterface;

///
/// \brief Edge displayed between to fully collapsed Node%s if they are connected with edges going back and forth.
///
/// While a single StraightEdge replaces 1-n PlugEdge%s when the Node%s on both ends are collapsed, a
/// StraightDoubleEdge replaces 2 StraightEdge%s that connect the same Node%s but flow in different directions.
///
class StraightDoubleEdge : public StraightEdge
{
    Q_OBJECT

public: // methods

    ///
    /// \brief Constructor.
    ///
    ///  \param [in] scene      Scene containing this edge.
    ///  \param [in] group      EdgeGroupPair containing this edge.
    ///  \param [in] fromNode   Start Node of this edge.
    ///  \param [in] toNode     End Node of this edge.
    ///
    explicit StraightDoubleEdge(Scene* scene, EdgeGroupInterface* group, Node* fromNode, Node* toNode);

    ///
    /// \brief Generates and updates the text for the EdgeLabel of this edge.
    ///
    void updateLabel();

protected: // methods

    ///
    /// \brief Updates the shape of the edge.
    ///
    virtual void updateShape() override;
};

} // namespace zodiac

#endif // ZODIAC_STRAIGHTDOUBLEEDGE_H
