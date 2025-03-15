#include "Cortege.h"
#include <QTimeZone>

Cortege::Cortege(const QStringList& row)
{
    initChannels();

    fillCortegeByRow(row);
}

void Cortege::initChannels()
{
    std::vector<std::string> params = { "SpO2", "PR"};

    ChannelDescriptor descriptor("POX", params);

    Channel pox(descriptor);

    channels.push_back(pox);
}

void Cortege::fillCortegeByRow(const QStringList& row)
{
    timeMark = fillDateFromString(row[0]);
    QVector<ChannelParams*> channelEnums;

    ParamsPOX* par = new ParamsPOX();

    channelEnums.push_back(par);
    QStringList::const_iterator first   = row.begin() + poxOffset;
    QStringList::const_iterator last    = row.begin() + ParamsPOX::sizeOfPOX * 4 + 1;

    for(int i = 0; i < trendableChannels::size; i++)
    {
        fillChannel(channels[i], channelEnums[i], QStringList(first, last));
    }

    //qDebug() << *this;

}

void Cortege::fillChannel(Channel& pox, ChannelParams* channelEnum, const QStringList& data)
{
    //qDebug() << data;
    double min, max, n_records, mean;

    for(int i = 0; i < channelEnum->getSize(); i++)
    {
        min         = data[0+i*4].toDouble();
        mean        = data[1+i*4].toDouble();
        max         = data[2+i*4].toDouble();
        n_records   = data[3+i*4].toDouble();

        pox.getParamStat(QtEnumToString((ParamsPOX::params)(i))) = ParamStat<double>(min, max, n_records, mean);
    }
}

QDebug operator<< (QDebug out, Cortege& cortege)
{
    out << cortege.timeMark << "\n";
    for(int i = 0; i < cortege.channels.size(); i++)
    {
        int size = cortege.channels[i].getParamStats().size();

        for(int j = 0; j < size; j++)
        {
            out << QString(cortege.channels[i].getDescriptor().params[j].c_str()) << "\n";
            out << cortege.channels[i].getParamStats()[j].GetMin() \
            << cortege.channels[i].getParamStats()[j].GetMean() \
            << cortege.channels[i].getParamStats()[j].GetMax() \
                << cortege.channels[i].getParamStats()[j].GetNumber() << "\n";
        }

    }
    return out;
}


QDateTime fillDateFromString(QString timeString)
{
    QChar sign = timeString[8];
    QStringList parts = timeString.split(sign);
    QString timePart = parts[0];
    QString timezoneOffset = sign + parts[1];
    qDebug() << sign;

    QTime time = QTime::fromString(timePart, "HH:mm:ss");

    if (!time.isValid()) {
        qWarning() << "Неверный формат времени!";
        abort();
    }

    QTimeZone timeZone = formTimeZoneFromOffset(timezoneOffset);

    //TODO - сделать дату из файла

    QDate currentDate = QDate::currentDate();

    QDateTime dateTime(currentDate, time, timeZone);

    return dateTime;
}

QTimeZone formTimeZoneFromOffset(QString offsetString)
{
    if (!offsetString.startsWith('+') && !offsetString.startsWith('-')) {
        offsetString.prepend('+');     }

    QRegExp regex(R"(([+-]?)(\d{2}):(\d{2}))");
    int offsetMinutes = 0;

    if (regex.exactMatch(offsetString)) {
        QString sign = regex.cap(1);
        int hours = regex.cap(2).toInt();
        int minutes = regex.cap(3).toInt();

        offsetMinutes = hours * 60 + minutes;
        if (sign == "-") {
            offsetMinutes = -offsetMinutes;
        }
    } else {
        qWarning() << "Неверный формат времени смещения!";
        abort();
    }

    QTimeZone timeZone(offsetMinutes * 60);
    return timeZone;
}

