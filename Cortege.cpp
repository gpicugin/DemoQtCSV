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
    QStringList::const_iterator first   = row.begin() + poxOffset;
    QStringList::const_iterator last    = row.begin() + poxOffset + ParamsPOX::sizeOfPOX * 4 + 1;

    for(int i = 0; i < trendableChannels::size; i++)
    {
        fillChannel(channels[i], QStringList(first, last));
    }

    qDebug() << *this;


}

void Cortege::fillChannel(Channel& channel, const QStringList& data)
{
    double min, max, n_records, mean;

    ChannelDescriptor descriptor = channel.getDescriptor();

    for(int i = 0; i < descriptor.params.size(); i++)
    {
        min         = data[0+i*4].toDouble(); // 4 - min max, n_rec, mean
        mean        = data[1+i*4].toDouble();
        max         = data[2+i*4].toDouble();
        n_records   = data[3+i*4].toDouble();
        channel.getParamStat(descriptor.params[i]) = ParamStat<double>(min, max, n_records, mean);
    }

    QStringList alarms = data.last().split(',');

    for(int i = 0; i < alarms.size(); i++)
    {
        alarms[i].remove(QChar('\"'));
        channel.Alarm.setAlarm(alarms[i].toStdString());
    }
}

QDebug operator<< (QDebug out, Cortege& cortege)
{
    out << cortege.timeMark << "\n";
    for(int i = 0; i < cortege.channels.size(); i++)
    {
        out << QString(cortege.channels[i].getDescriptor().name.c_str()) << "\n";

        int size = cortege.channels[i].getParamStats().size();

        for(int j = 0; j < size; j++)
        {
            out << QString(cortege.channels[i].getDescriptor().params[j].c_str()) << "\n";
            out << cortege.channels[i].getParamStats()[j].GetMin() \
            << cortege.channels[i].getParamStats()[j].GetMean() \
            << cortege.channels[i].getParamStats()[j].GetMax() \
            << cortege.channels[i].getParamStats()[j].GetNumber() << "\n";
        }

        std::list<Entry<std::string>> alarms = cortege.channels[i].Alarm.getAlarms();

        for(std::list<Entry<std::string>>::iterator it = alarms.begin(); it != alarms.end(); it++)
        {
            out << QString(it->alarm.c_str()) << "\n";
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

