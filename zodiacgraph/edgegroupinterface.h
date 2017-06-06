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

#ifndef ZODIAC_EDGEGROUPINTERFACE_H
#define ZODIAC_EDGEGROUPINTERFACE_H

///
/// \file edgegroupinterface.h
///
/// \brief Contains the definition of the zodiac::EdgeGroupInterface mixin class.
///

class QString;

namespace zodiac {

///
/// \brief Mix-in interface for EdgeGroup and EdgeGroupPair.
///
/// This is necessary to mirror the inheritance tree of StraightEdge and StraightDoubleEdge.
/// Now, a StraightEdge can always ask its EdgeGroupInterface for a label text, even if the StraightEdge is really a
/// StraightDoubleEdge.
///
class EdgeGroupInterface
{

public: // method

    ///
    /// \brief  Generates a text used for a group of edges.
    ///
    /// \return Text used for the EdgeLabel.
    ///
    virtual QString getLabelText() = 0;

};

} // namespace zodiac

#endif // ZODIAC_EDGEGROUPINTERFACE_H
