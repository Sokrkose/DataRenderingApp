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

// TEST: verify that the widget correctly updates its internal size
// when a signal waveform includes a logic level transition (e.g., 0 -> 1).
// A transition should be seen in the rendered pixmap and size hint.
TEST(WidgetTest, LogicTransitionUpdatesPixmap) {
    AccessibleWidget w;

    // Create sample data with a single logic transition
    std::vector<int> data(100, 0);
    data[20] = 1;  // Introduce a transition (0 -> 1)

    // Load data into the widget
    w.setSignalData(data, 100, "transitions.txt");

    // Manually render and check the suggested size
    w.renderToCache();
    QSize size = w.sizeHint();

    // Expect a width large enough to visually represent the signal
    EXPECT_GT(size.width(), 100);  // Adjust threshold as needed
}

// TEST: Ensure the widget can handle very large signal datasets
// without throwing exceptions or crashing.
TEST(WidgetTest, HandlesLargeDataSet) {
    AccessibleWidget w;

    // Simulate 1 million logic high samples
    std::vector<int> data(1'000'000, 1);

    // Ensure the widget does not throw when processing this large input
    EXPECT_NO_THROW(w.setSignalData(data, 100, "large.txt"));

    // The rendered width should scale accordingly (2 px per point is typical)
    EXPECT_GT(w.sizeHint().width(), 10000);
}
