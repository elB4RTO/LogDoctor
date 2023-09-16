#ifndef LOGDOCTOR__CRAPLOG__DONUTS_H
#define LOGDOCTOR__CRAPLOG__DONUTS_H


#include <QGraphicsView>
#include <QtCharts>
#include <QColor>

#include <QtCharts/QPieLegendMarker>


//! DonutBreakdon
/*!
    Builds the donut chart to be displayed
*/
class DonutBreakdown : public QChart
{
public:

    explicit DonutBreakdown( QGraphicsItem* parent=nullptr, Qt::WindowFlags wFlags={} );

    //! Adds a slice to the donut
    /*!
        \param series The series to add
        \param color The color of the slice
        \param font The font to be used
        \see Craplog::makeChart()
    */
    void addBreakdownSeries( QPieSeries* series, const QColor& color, const QFont& font );


private:

    // The main series (a.k.a. the donut)
    QPieSeries* m_mainSeries;

    //! Recalculates the size of every slice in percentage over the total
    void recalculateAngles();

    //! Updates the position of the markers to be in the middle of the relative slice
    void updateLegendMarkers();
};



//! Represents a slice
/*!
    A slice to be added at the donut
*/
class MainSlice : public QPieSlice
{
    Q_OBJECT

public:

    explicit MainSlice( QPieSeries *breakdownSeries, QObject *parent=0 );

    //! Returns the series
    QPieSeries *breakdownSeries() const;

    //! Sets the name
    /*!
        \param name The name to be used
    */
    void setName( QString name );

    //! Returns the name
    QString name() const;


public Q_SLOTS:

    //! Updates the label using the name
    void updateLabel();


private:

    // The series of the slice
    QPieSeries* m_breakdownSeries;

    // The name of the slice
    QString m_name;
};


#endif // LOGDOCTOR__CRAPLOG__DONUTS_H
