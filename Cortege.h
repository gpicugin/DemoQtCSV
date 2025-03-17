#ifndef CORTEGE_H
#define CORTEGE_H
#include <QDateTime>
#include "Channel.h"
#include <QDebug>

enum trendableChannels {
    T1,
    T2,
    POX,
    size
};

class Cortege
{
public:
    Cortege(const QStringList& row);
private:
    void initChannels();
    void fillCortegeByRow(const QStringList &row);
    void fillChannel(Channel &channel, const QStringList& data);
public:
    QDateTime timeMark;
    QVector<Channel> channels;
    QMap<int, int> offset;

};

QDebug operator<< (QDebug out, Cortege& cortege);

QDateTime fillDateFromString(QString str);
QTimeZone formTimeZoneFromOffset(QString offsetString);



#endif // CORTEGE_H
