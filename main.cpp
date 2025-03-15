#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDebug>
#include "trendslibrary/Channel.h"

QVector<QStringList> lines; // Вектор для хранения данных по столбцам

void parseCSV(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Не удалось открыть файл:" << filePath;
        return;
    }

    QTextStream in(&file);

    bool first = true;

    while (!in.atEnd()) {
        if(first) {
            in.readLine();
            first = false;
        }
        QString line = in.readLine();
        QStringList fields = line.split(";"); // Парсим строку на элементы
        fields.pop_front();

        for(int i = 0; i < fields.size(); i++)
        {
            fields[i] = fields[i].trimmed();
        }

        lines.push_back(fields);
    }

    file.close();
}



void fillChannel(Channel& channel) {

}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString filePath = "test_1.csv";
    parseCSV(filePath);

    qDebug() << lines;

    std::vector<std::string> params = { "SpO2", "PR", "HR" };

    ChannelDescriptor descriptor("POX", params);

    Channel pox(descriptor);

    return a.exec();
}
