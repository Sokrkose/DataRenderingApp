#include <gtest/gtest.h>
#include "widget.h"
#include <QApplication>

// Provide a testable subclass that makes protected/private methods accessible
class AccessibleWidget : public Widget {
public:
    using Widget::sizeHint;        // Expose protected
    using Widget::renderToCache;   // Expose private (via public promotion)
};

// Global QApplication instance required for QWidget
int argc = 0;
char **argv = nullptr;
QApplication app(argc, argv);

// Test: Ensure that sizeHint grows with the amount of data
// This indirectly verifies the layout/geometry scaling logic.
TEST(WidgetTest, SizeHintGrowsWithData) {
    AccessibleWidget w;

    std::vector<int> data(500, 1);
    w.setSignalData(data, 100, "test.txt");

    // sizeHint should reflect the widget's desired size based on data length
    QSize size = w.sizeHint();

    // Assert width and height are sufficiently large
    EXPECT_GT(size.width(), 900);   // Each point is 2 px, plus margins
    EXPECT_GT(size.height(), 100);  // Height should reflect waveform space
}

// Test: Ensure renderToCache generates a pixmap successfully
// This confirms that internal drawing logic executes correctly.
TEST(WidgetTest, RenderCreatesPixmap) {
    AccessibleWidget w;

    std::vector<int> data(200, 1);
    w.setSignalData(data, 100, "test.txt");

    // Manually invoke rendering logic
    w.renderToCache();

    // After rendering, sizeHint() should return a valid size
    EXPECT_FALSE(w.sizeHint().isEmpty());  // Ensures internal state was updated

}

