#ifndef CORTEGE_H
#define CORTEGE_H
#include <QDateTime>
#include "Channel.h"
#include <QDebug>

enum trendableChannels {
    POX,
    size
};

enum offsets {
    poxOffset = 1
};



class Cortege
{
public:
    Cortege(const QStringList& row);
    QDateTime timeMark;
    QVector<Channel> channels;
    void initChannels();
    void fillCortegeByRow(const QStringList &row);
    void fillChannel(Channel &channel, const QStringList& data);
};

QDebug operator<< (QDebug out, Cortege& cortege);

QDateTime fillDateFromString(QString str);
QTimeZone formTimeZoneFromOffset(QString offsetString);



#endif // CORTEGE_H
