#include "widget.h"
#include <QPainter>

Widget::Widget(QWidget *parent)
    : QWidget(parent) {
    setMinimumHeight(150);
    setStyleSheet("background-color: black;");
}

void Widget::setSignalData(const std::vector<int> &data, int timestep_ns) {
    m_data = data;
    m_timestep_ns = timestep_ns;
    qDebug() << "m_data.size() =" << m_data.size();
    update();  // Trigger repaint
}

void Widget::paintEvent(QPaintEvent *) {
    int maxPoints = 1000;
    int pointsToDraw = std::min(maxPoints, (int)m_data.size());

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::darkGray);

    const int margin = 10;
    const int availableHeight = this->height() - 2 * margin;
    const int width = this->width() - 2 * margin;

    float x_scale = (float)width / (pointsToDraw - 1);
    int amplitude = availableHeight / 2 - 5;
    int center = amplitude + margin;

    int y_high = margin + (center - amplitude);
    int y_low = margin + (center + amplitude);

    // Draw waveform
    for (int i = 1; i < pointsToDraw; ++i) {
        int x1 = margin + static_cast<int>((i - 1) * x_scale);
        int x2 = margin + static_cast<int>(i * x_scale);

        qDebug() << "Data Point:" << i;
        qDebug() << "availableHeight:" << availableHeight;
        qDebug() << "center:" << center << "amplitude:" << amplitude;
        qDebug() << "y_high:" << y_high << "y_low:" << y_low;

        // Map each bit to y: 0 = y_low, 1 = y_high
        int y1 = m_data[i - 1] ? y_high : y_low;
        int y2 = m_data[i] ? y_high : y_low;

        painter.drawLine(x1, y1, x2, y2);
    }

    // Draw time markers
    painter.setPen(Qt::darkGray);
    for (int i = 0; i < pointsToDraw; i += 100) {
        int x = margin + static_cast<int>(i * x_scale);
        float time_ms = (i * m_timestep_ns) / 1000000.0f;
        painter.drawText(x, y_low + 15, QString::number(time_ms, 'f', 2) + " ms");
    }
}
