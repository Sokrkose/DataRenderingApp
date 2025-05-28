#include <gtest/gtest.h>
#include "datareader.h"  // include your logic to test

// Test: Ensure the datareader reads the expected number of values
// and correct timestep from a known test file.
// This verifies file parsing and struct population.
TEST(DataReaderTest, ReadsCorrectSize) {
    QString testPath = "C:/Users/skoseogl/cernbox/Documents/Qt/DataRenderingApp/data/waveform-data.txt";
    struct data d = dataReader::read(testPath);
    EXPECT_EQ(d.values.size(), 300000);
    EXPECT_EQ(d.timestep_ns, 100);
}

// Test: Check that the first few values match known data.
// This ensures the file is not only the right size, but values
// are parsed correctly in order and content.
TEST(DataReaderTest, ContainsCorrectValues) {
    QString testPath = "C:/Users/skoseogl/cernbox/Documents/Qt/DataRenderingApp/data/waveform-data.txt";
    struct data d = dataReader::read(testPath);
    ASSERT_EQ(d.values.size(), 300000);
    EXPECT_EQ(d.values[0], 1);
    EXPECT_EQ(d.values[1], 1);
    EXPECT_EQ(d.values[2], 0);
}
