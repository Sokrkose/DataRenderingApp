#ifndef DATAREADER_H
#define DATAREADER_H

#include <QString>
#include <vector>

struct data {
    int timestep_ns;
    std::vector<int> values;
};

class dataReader
{
public:
    static data read(const QString &filePath);
};

#endif // DATAREADER_H
