#include "Plot.h"

#include <QtCharts/QChart>
#include <QtCharts/QLegendMarker>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>

using namespace QtCharts;

// PlotGraph

PlotGraph::PlotGraph(const PlotGraphData &data, QObject *parent)
    : QObject(parent)
    , _data(data)
{}

PlotGraph::~PlotGraph()
{
    Q_ASSERT(_data.series);
    if (_data.series->chart())
        _data.series->chart()->removeSeries(_data.series);
    delete _data.series;
}

void PlotGraph::addXY(const qreal x, const qreal y)
{
    Q_ASSERT(_data.series);
    if (0 == x)
        _data.series->clear();
    _data.series->append(x, y);
    _data.series->setName(QString("%1 : %2 °C").arg(_data.name).arg(qRound(y)));

    Q_ASSERT(_data.axisX);
    _data.axisX->setRange(x - _data.axisX->max() + _data.axisX->min(), x);

    Q_ASSERT(_data.axisY);
    _data.axisY->setMax(qMax(y + 2, _data.axisY->max()));
}

void PlotGraph::addY(const qreal y)
{
    Q_ASSERT(_data.axisX);
    addXY(_data.axisX->max(), y);
}

void PlotGraph::setColor(const QColor &color)
{
    Q_ASSERT(_data.series);
    _data.series->setPen(QPen(QBrush(color), 2));
}

// Plot

Plot::Plot(QWidget *parent)
    : QChartView(parent)
    , _chart(new QChart)
    , _axisX(new QValueAxis)
    , _axisY(new QValueAxis)
{
    _axisX->setLabelFormat("%d");
    _axisX->setRange(0, 60);
    _axisX->hide();

    _chart->setAxisX(_axisX);
    _chart->setAxisY(_axisY);

    setChart(_chart);
    setRenderHint(QPainter::Antialiasing);
}

PlotGraph* Plot::addGraph(const QString &name, const QColor &color)
{
    PlotGraphData data = {
        name,
        new QSplineSeries,
        _axisX,
        _axisY
    };

    _chart->addSeries(data.series);
    _chart->setAxisX(_axisX, data.series);
    _chart->setAxisY(_axisY, data.series);

    auto graph = new PlotGraph(data, this);
    graph->setColor(color);

    auto marker = _chart->legend()->markers(data.series).first();
    Q_CHECK_PTR(marker);

    connect(marker, &QLegendMarker::hovered, [this, marker] (bool state) {
        for (auto series : _chart->series())
            series->setOpacity((state && (marker->series() != series)) ? 0.1 : 1);
    });

    return graph;
}

void Plot::setDisplayInterval(const int displayInterval)
{
    Q_ASSERT(displayInterval > 0);

    _axisX->setMin(_axisX->max() - displayInterval);
}
