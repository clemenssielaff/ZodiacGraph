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
