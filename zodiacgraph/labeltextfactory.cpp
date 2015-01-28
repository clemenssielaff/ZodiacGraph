#include "labeltextfactory.h"

#include "node.h"
#include "plug.h"
#include "plugedge.h"

namespace zodiac {

const QString LabelTextFactory::s_arrowChar = QString::fromUtf8("  \xE2\x96\xB6  ");
const QString LabelTextFactory::s_dotChar = ".";
const QString LabelTextFactory::s_whitespaceChar = " ";
const QString LabelTextFactory::s_newlineChar = "\n";
const QString LabelTextFactory::s_horizontalLineChar = "—";

LabelTextFactory::LabelTextFactory(const QSet<PlugEdge*>& edges)
    : m_namePairs(QList<QPair<QString, QString>>())
    , m_labelCount(edges.count())
    , m_maxNameLength(0)
{
    // create and store the label names
    m_namePairs.reserve(m_labelCount);
    for(const PlugEdge* edge : edges){
        Plug* startPlug = edge->getStartPlug();
        Plug* endPlug = edge->getEndPlug();
        QString fromName = startPlug->getNode()->getDisplayName() + s_dotChar + startPlug->getName();
        QString toName = endPlug->getNode()->getDisplayName() + s_dotChar + endPlug->getName();
        m_maxNameLength = qMax(m_maxNameLength, qMax(fromName.length(), toName.length()));
        m_namePairs.append(QPair<QString, QString>(fromName, toName));
    }
}

LabelTextFactory::LabelTextFactory(PlugEdge *edge)
    : LabelTextFactory(QSet<PlugEdge*>({edge}))
{
}

QString LabelTextFactory::produceLabel(int maxNameLength) const
{
    // center the arrow by pre- or appending whitespace to the shorter name
    maxNameLength = qMax(maxNameLength, m_maxNameLength);
    QStringList labelStrings;
    labelStrings.reserve(m_labelCount);
    for(const QPair<QString, QString>& labelPair : m_namePairs){
        labelStrings.append(
                    s_whitespaceChar.repeated(maxNameLength-labelPair.first.length())
                    + labelPair.first + s_arrowChar + labelPair.second +
                    s_whitespaceChar.repeated(maxNameLength-labelPair.second.length()));
    }
    return labelStrings.join(s_newlineChar);
}

QString LabelTextFactory::getHorizontalLine(int maxNameLength)
{
    ///
    /// \brief The horizontal line is always shorter than the longest label.
    ///
    /// Also, to ensure that it is centered, the number of characters in the horizontal line should be odd.
    /// Therefore, the next smaller, odd number with regards to the longest line is the base for the calculation.
    /// It may however be visually pleasing to remove more characters from both sides of the line.
    /// This is the number of characters removed from both sides after the next smaller, odd number was found.
    ///
    static const int UNDERLINE_SHORTAGE = 1;

    ///
    /// \brief Maximal length of the horizontal line in characters.
    ///
    static const int MAX_LENGTH = 7;

    int lineLength = (maxNameLength*2)+s_arrowChar.length();
    int repeats = qMin(MAX_LENGTH, lineLength-((lineLength%2)+1+(UNDERLINE_SHORTAGE*2)));
    int whitespace = lineLength-repeats;
    int spacesBefore = whitespace/2;
    return s_whitespaceChar.repeated(spacesBefore) +
            s_horizontalLineChar.repeated(repeats) +
            s_whitespaceChar.repeated(spacesBefore+(whitespace%2));
}

} // namespace zodiac

