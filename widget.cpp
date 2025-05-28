#include "widget.h"
#include <QPainter>
#include <QFileInfo>
#include <QWheelEvent>

Widget::Widget(QWidget *parent)
    : QWidget(parent) {
    setMinimumHeight(150);  // Ensure vertical size is usable
    setStyleSheet("background-color: black;");  // Matches rest of waveform styling
}


void Widget::setSignalData(const std::vector<int> &data, int timestep_ns, const QString &path) {
    m_data = data;
    m_timestep_ns = timestep_ns;
    m_filePath = path;
    m_cacheReady = false;

    renderToCache();      // Render waveform once
    update();             // Request repaint
    updateGeometry();     // Notify scroll area of new size
}


void Widget::renderToCache() {

    // int maxPoints = 1000;
    // int pointsToDraw = std::min(maxPoints, (int)m_data.size());

    int pointsToDraw = static_cast<int>(m_data.size());

    // int pointsToDraw = std::min(m_viewWindowSize, (int)m_data.size() - m_viewStartIndex);

    // Layout constants
    const int margin = 10;
    const int totalWidth = std::max(static_cast<int>(m_data.size()) * 2 + 2 * margin, this->width());
    const int totalHeight = this->height();
    const int width = totalWidth - 2 * margin;
    int labelSpace = 30;  // Space below waveform for time markers
    int availableHeight = this->height() - labelSpace;
    int waveformHeight = static_cast<int>(availableHeight * 1.0);
    int y_offset = 30;  // Space from top for waveform area
    float x_scale = (float)width / (pointsToDraw - 1);
    int y_high = y_offset + 20; // Logic '1' position
    int y_low = y_offset + waveformHeight - 20; // Logic '0' position

    // Create the pixmap for caching and fill with default UI background (light gray)
    m_cachedPixmap = QPixmap(totalWidth, totalHeight);
    m_cachedPixmap.fill(QColor(240, 240, 240)); // Matches QWidget background

    // Create painter to draw on cached pixmap
    QPainter painter(&m_cachedPixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    // Fill only the waveform area with black
    QRect waveformRect(margin, y_offset, width, waveformHeight);
    painter.fillRect(waveformRect, Qt::black);

    // Draw file name above waveform
    QFileInfo fileInfo(m_filePath);
    QString filename = fileInfo.fileName();
    painter.setPen(Qt::black);
    QFont pathFont = painter.font();
    pathFont.setPointSize(10);
    painter.setFont(pathFont);
    painter.drawText(margin, y_offset - 15, filename);

    // Draw waveform
    painter.setPen(Qt::green);
    int prev_x = margin;
    int prev_y = m_data[0] ? y_high : y_low;

    for (int i = 1; i < pointsToDraw; ++i) {
        int curr_x = margin + static_cast<int>(i * x_scale);
        int curr_y = m_data[i] ? y_high : y_low;

        if (curr_y == prev_y) {
            painter.drawLine(prev_x, prev_y, curr_x, curr_y);
        } else {
            painter.drawLine(prev_x, prev_y, prev_x, curr_y);
            painter.drawLine(prev_x, curr_y, curr_x, curr_y);
        }

        prev_x = curr_x;
        prev_y = curr_y;
    }

    // Time markers
    QFont timeFont = painter.font();
    timeFont.setPointSize(12);
    painter.setFont(timeFont);
    painter.setPen(Qt::white);

    for (int i = 0; i < pointsToDraw; i += 100) {
        int x = margin + static_cast<int>(i * x_scale);
        float time_ms = (i * m_timestep_ns) / 1000000.0f;
        painter.drawText(x, y_low + 15, QString::number(time_ms, 'f', 2) + " ms");
    }

    m_cacheReady = true;
}


void Widget::paintEvent(QPaintEvent *) {
    // Render waveform only once and reuse the cached image
    if (!m_cacheReady) {
        renderToCache();  // expensive operation, only done once
    }

    // Display cached waveform
    QPainter painter(this);
    painter.drawPixmap(0, 0, m_cachedPixmap);
}


QSize Widget::sizeHint() const {
    int labelSpace = 30;  // Space below waveform for time markers
    int y_offset = 30;
    const int margin = 10;
    int waveformHeight = static_cast<int>((this->height() - labelSpace) * 1.0);
    const int totalHeight = y_offset + waveformHeight + labelSpace;

    // Width should reflect how much space is needed for all data points
    int totalWidth = m_data.size() * 2; // You can adjust this scale factor (2 px per point)

    return QSize(totalWidth + 2 * margin, totalHeight);
}
