#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <vector>
#include <QScrollBar>

class Widget : public QWidget {
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    void setSignalData(const std::vector<int> &data, int timestep_ns, const QString &path);

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override;
    void renderToCache();

private:
    std::vector<int> m_data;
    int m_timestep_ns = 100;
    QString m_filePath;
    QPixmap m_cachedPixmap;
    bool m_cacheReady = false;        // flag to know when to render

    int m_viewStartIndex = 0;         // Start index of current visible window
    int m_viewWindowSize = 1000;      // How many samples to show
    QScrollBar *m_scrollBar = nullptr;
};

#endif // WIDGET_H
