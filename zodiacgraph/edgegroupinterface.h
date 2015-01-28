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
