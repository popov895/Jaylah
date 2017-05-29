#ifndef PLOT_H
#define PLOT_H

#include <QtCharts/QChartView>
#include <QPointer>

namespace QtCharts
{
    class QChart;
    class QSplineSeries;
    class QValueAxis;
}

// PlotGraphData
struct PlotGraphData
{
    const QString name;
    QPointer<QtCharts::QSplineSeries> series;
    QPointer<QtCharts::QValueAxis> axisX;
    QPointer<QtCharts::QValueAxis> axisY;
};

// PlotGraph
class PlotGraph : public QObject
{
    Q_OBJECT

    friend class Plot;

private:
    PlotGraph(const PlotGraphData &data, QObject *parent);

public:
    virtual ~PlotGraph();

    void addXY(const qreal x, const qreal y);
    void addY(const qreal y);
    void setColor(const QColor &color);

private:
    PlotGraphData _data;
};

// Plot
class Plot : public QtCharts::QChartView
{
    Q_OBJECT

public:
    explicit Plot(QWidget *parent = Q_NULLPTR);

    PlotGraph* addGraph(const QString &name, const QColor &color);
    void setDisplayInterval(const int displayInterval);

private:
    QPointer<QtCharts::QChart> _chart;
    QPointer<QtCharts::QValueAxis> _axisX;
    QPointer<QtCharts::QValueAxis> _axisY;
};

#endif // PLOT_H
