#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDebug>
#include "trendslibrary/Channel.h"
#include "Cortege.h"

void fillCorteges(const QString &fileName, QVector<Cortege>& corteges)
{
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Не удалось открыть файл:" << fileName;
        return;
    }

    QTextStream in(&file);

    bool first = true;

    while (!in.atEnd())
    {
        if(first)
        {
            in.readLine(); // пропускаем шапку
            first = false;
        }

        QString line = in.readLine();
        QStringList row = line.split(";");


        for(int i = 0; i < row.size(); i++)
        {
            row[i] = row[i].trimmed();
        }

        corteges.push_back(Cortege(row));
    }

    file.close();
}



int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    setlocale(LC_ALL, "");

    //qRegisterMetaType<ParamsPOX>("ParamsPOX");

    QVector<Cortege> corteges;

    QString fileName = "../test_1.csv";

    fillCorteges(fileName, corteges);

    return a.exec();
}
