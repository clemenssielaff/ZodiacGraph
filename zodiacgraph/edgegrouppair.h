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

#ifndef ZODIAC_EDGEGROUPPAIR_H
#define ZODIAC_EDGEGROUPPAIR_H

///
/// \file edgegrouppair.h
///
/// \brief Contains the definition of the zodiac::EdgeGroupPair class.
///

#include "edgegroupinterface.h"

namespace zodiac {

class StraightDoubleEdge;
class EdgeGroup;
class Node;
class Scene;

///
/// \brief A pair of EdgeGroup%s, both connecting the same Node%s but in different directions.
///
/// While a single EdgeGroup contains 1-n PlugEdge%s, an EdgeGroupPair contains and owns two EdgeGroup%s.
///
class EdgeGroupPair : public EdgeGroupInterface
{

public: // methods

    ///
    /// \brief Constructor.
    ///
    /// \param [in] scene   Scene owning this EdgeGroupPair.
    /// \param [in] nodeA   One Node in the group.
    /// \param [in] nodeB   The other Node in the group (direction does not matter).
    ///
    explicit EdgeGroupPair(Scene* scene, Node* nodeA, Node* nodeB);

    ///
    /// \brief Destructor.
    ///
    /// Destroys both EdgeGroup%s and their respective DoubleEdge%s / DoubleStraightEdge%s.
    ///
    virtual ~EdgeGroupPair();

    ///
    /// \brief One of the two EdgeGroup%s in this pair.
    ///
    /// \return First EdgeGroup of the pair.
    ///
    inline EdgeGroup* getFirstGroup() const {return m_firstGroup;}

    ///
    /// \brief The other EdgeGroup in this pair.
    ///
    /// \return Second EdgeGroup of the pair.
    ///
    inline EdgeGroup* getSecondGroup() const {return m_secondGroup;}

    ///
    /// \brief Tests if none of the two EdgeGroup%s in this pair contain any PlugEdge%s.
    ///
    /// \return <i>true</i> if both EdgeGroup%s are empty -- <i>false</i> otherwise.
    ///
    bool isEmpty() const;

    ///
    /// \brief If both EdgeGroup%s of the pair are visible, shows the DoubleStraightEdge instead.
    ///
    void updateDoubleEdgeVisibility();

    ///
    /// \brief Hides the DoubleStraightEdge of this EdgeGroupPair.
    ///
    void hideDoubleEdge();

    ///
    /// \brief Updates the label of the DoubleStraightEdge.
    ///
    void updateLabel();

    ///
    /// \brief Generates the label text for this EdgeGroupPair.
    ///
    /// \return Label text based on the PlugEdge%s contained in both EdgeGroup%s of this pair.
    ///
    QString getLabelText();

    ///
    /// \brief Applies style changes in the class' static members to this instance.
    ///
    /// Is part of the scene-wide cascade of %updateStyle()-calls after a re-styling of the ZodiacGraph.
    ///
    void updateStyle();

private: // members

    ///
    /// \brief Scene containing this pair.
    ///
    Scene* m_scene;

    ///
    /// \brief First EdgeGroup of this pair, containing PlugEdge%s leading from Node <i>A</i> to Node <i>B</i>.
    ///
    /// Is owned by this EdgeGroupPair.
    ///
    EdgeGroup* m_firstGroup;

    ///
    /// \brief Second EdgeGroup of this pair, containing PlugEdge%s leading from Node <i>B</i> to Node <i>A</i>.
    ///
    /// Is owned by this EdgeGroupPair.
    ///
    EdgeGroup* m_secondGroup;

    ///
    /// \brief Double edge to display instead of two overlaying StraightEdge%s pointing in opposite directions.
    ///
    /// Is owned by this EdgeGroupPair.
    ///
    StraightDoubleEdge* m_edge;
};

} // namespace zodiac

#endif // ZODIAC_EDGEGROUPPAIR_H
