#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <vector>

class Widget : public QWidget {
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    void setSignalData(const std::vector<int> &data, int timestep_ns, const QString &path);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    std::vector<int> m_data;
    int m_timestep_ns = 100;
    QString m_filePath;
    void renderToCache();
    QPixmap m_cachedPixmap;
    bool m_cacheReady = false;  // flag to know when to render
};

#endif // WIDGET_H
