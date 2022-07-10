#ifndef DONUTS_H
#define DONUTS_H

#include <QGraphicsView>
#include <QtCharts>
#include <QColor>

#include <QtCharts/QPieLegendMarker>


class DonutBreakdown : public QChart
{
public:
    DonutBreakdown( QGraphicsItem *parent=nullptr, Qt::WindowFlags wFlags={} );

    void addBreakdownSeries ( QPieSeries *series, const QColor& color, const QFont& font );

private:
    void recalculateAngles();
    void updateLegendMarkers();

    QPieSeries *m_mainSeries;
};


class MainSlice : public QPieSlice
{
Q_OBJECT
public:
    MainSlice( QPieSeries *breakdownSeries, QObject *parent=0 );

    QPieSeries *breakdownSeries() const;

    void setName( QString name );
    QString name() const;

public Q_SLOTS:
    void updateLabel();

private:
    QPieSeries *m_breakdownSeries;
    QString m_name;
};

#endif // DONUTS_H
