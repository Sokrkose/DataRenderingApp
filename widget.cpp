#include "widget.h"
#include <QPainter>
#include <QFileInfo>

Widget::Widget(QWidget *parent)
    : QWidget(parent) {
    setMinimumHeight(150);
    setStyleSheet("background-color: black;");
}

void Widget::setSignalData(const std::vector<int> &data, int timestep_ns, const QString &path) {
    m_data = data;
    m_timestep_ns = timestep_ns;
    m_filePath = path;
    qDebug() << "m_data.size() =" << m_data.size();
    update();  // Trigger repaint
}

void Widget::paintEvent(QPaintEvent *) {
    int maxPoints = 1000;
    int pointsToDraw = std::min(maxPoints, (int)m_data.size());
    // int pointsToDraw = static_cast<int>(m_data.size());

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    const int margin = 10;
    const int width = this->width() - 2 * margin;
    int waveformHeight = static_cast<int>(this->height() * 0.45);  // Increased from 0.33
    int y_offset = 30;  // Same offset from top

    // Fill waveform area with black background
    QRect waveformRect(margin, y_offset, width, waveformHeight);
    painter.fillRect(waveformRect, Qt::black);

    float x_scale = (float)width / (pointsToDraw - 1);
    int y_high = y_offset + 20;
    int y_low = y_offset + waveformHeight - 20;


    // File above waveform
    QFileInfo fileInfo(m_filePath);
    QString filename = fileInfo.fileName(); // just the name (e.g., "waveform-data.txt")
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

    // Draw time markers inside waveform area
    QFont timeFont = painter.font();
    timeFont.setPointSize(12);
    painter.setFont(timeFont);
    painter.setPen(Qt::white);
    for (int i = 0; i < pointsToDraw; i += 100) {
        int x = margin + static_cast<int>(i * x_scale);
        float time_ms = (i * m_timestep_ns) / 1000000.0f;
        painter.drawText(x, y_low + 15, QString::number(time_ms, 'f', 2) + " ms");
    }

}

