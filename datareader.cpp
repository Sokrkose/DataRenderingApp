#include "datareader.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

data dataReader::read(const QString &filePath) {
    data result;
    result.timestep_ns = 100; // default

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open file:" << filePath;
        return result;
    }

    QTextStream in(&file);
    bool dataSection = false;

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();

        if (line.startsWith("TIMESTEP", Qt::CaseInsensitive)) {
            QStringList parts = line.split(';');
            if (parts.size() > 1)
                result.timestep_ns = parts[1].toInt();
        }

        if (line.trimmed().toUpper() == "DATA;") {
            dataSection = true;
            continue;
        }

        if (dataSection && !line.isEmpty()) {
            bool ok;
            int value = line.remove(';').toInt(&ok);  // remove trailing semicolon
            result.values.push_back(value);
        }
    }

    return result;
}
