
#include "donuts.h"

#include "utilities/strings.h"


/////////////////////////
//// DONUT BREAKDOWN ////
/////////////////////////
MainSlice::MainSlice(QPieSeries *breakdownSeries, QObject *parent)
    : QPieSlice(parent),
      m_breakdownSeries(breakdownSeries)
{
    connect( this, &MainSlice::percentageChanged, this, &MainSlice::updateLabel );
}


QPieSeries *MainSlice::breakdownSeries() const
{
    return this->m_breakdownSeries;
}

void MainSlice::setName(QString name)
{
    this->m_name = name;
}

QString MainSlice::name() const
{
    return this->m_name;
}


void MainSlice::updateLabel()
{
    this->setLabel( this->m_name );
    //this->setLabel(QString("%1 %2%").arg(this->m_name).arg(this->percentage() * 100, 0, 'f', 2));
}



DonutBreakdown::DonutBreakdown( QGraphicsItem *parent, Qt::WindowFlags wFlags )
    : QChart(QChart::ChartTypeCartesian, parent, wFlags)
{
    // create the series for main center pie
    this->m_mainSeries = new QPieSeries();
    this->m_mainSeries->setPieSize( 0.7 );
    QChart::addSeries( this->m_mainSeries );
}



void DonutBreakdown::addBreakdownSeries( QPieSeries *breakdownSeries, const QColor& color, const QFont& font )
{
    // add breakdown series as a slice to center pie
    MainSlice *mainSlice = new MainSlice(breakdownSeries);
    mainSlice->setName(breakdownSeries->name());
    mainSlice->setValue(breakdownSeries->sum());
    m_mainSeries->append(mainSlice);

    // customize the slice
    mainSlice->setBrush( color );
    mainSlice->setLabelVisible();
    mainSlice->setLabelColor( Qt::white );
    mainSlice->setLabelPosition( QPieSlice::LabelInsideHorizontal );
    mainSlice->setLabelFont( font );

    // position and customize the breakdown series
    breakdownSeries->setPieSize( 0.8 );
    breakdownSeries->setHoleSize( 0.7 );
    breakdownSeries->setLabelsVisible();
    const auto slices = breakdownSeries->slices();
    for (QPieSlice *slice : slices) {
        if ( StringOps::startsWith( slice->label().toStdString(), "Blacklisted" ) ) {
            slice->setBrush( Qt::GlobalColor::black );
            if ( slice->value() == 0 ) {
                slice->setLabelVisible( false );
            }
        } else if ( StringOps::startsWith( slice->label().toStdString(), "Warnings" ) ) {
            slice->setBrush( QColor( 255, 140, 0, 255 ) );
        } else if ( StringOps::startsWith( slice->label().toStdString(), "Ignored" ) ) {
            slice->setBrush( Qt::GlobalColor::transparent );
            breakdownSeries->setPieSize( 0.0 );
        } else {
            slice->setBrush( color.lighter( 150 ) );
        }
        slice->setLabelFont( font );
    }

    // add the series to the chart
    QChart::addSeries(breakdownSeries);

    // recalculate breakdown donut segments
    this->recalculateAngles();

    // update customize legend markers
    this->updateLegendMarkers();
}



void DonutBreakdown::recalculateAngles()
{
    qreal angle = 0;
    const auto slices = m_mainSeries->slices();
    for (QPieSlice *slice : slices) {
        QPieSeries *breakdownSeries = qobject_cast<MainSlice *>(slice)->breakdownSeries();
        breakdownSeries->setPieStartAngle(angle);
        angle += slice->percentage() * 360.0; // full pie is 360.0
        breakdownSeries->setPieEndAngle(angle);
    }
}


void DonutBreakdown::updateLegendMarkers()
{
    // go through all markers
    const auto allseries = series();
    for (QAbstractSeries *series : allseries) {
        const auto markers = legend()->markers(series);
        for (QLegendMarker *marker : markers) {
            QPieLegendMarker *pieMarker = qobject_cast<QPieLegendMarker *>(marker);
            if (series == m_mainSeries) {
                // hide markers from main series
                pieMarker->setVisible(false);
            } else {
                // modify markers from breakdown series
                std::string aux = pieMarker->slice()->label().toStdString();
                if ( aux.at( aux.find('@')+1 ) != '#' ) {
                    pieMarker->setLabel( QString("%1 %2%")
                                            .arg( QString::fromStdString( aux.substr( 0, aux.find('@') ) ) )
                                            .arg( pieMarker->slice()->percentage() * 100, 0, 'f', 2) );
                    pieMarker->setFont( QFont("Arial", 8) );
                    pieMarker->slice()->setLabel( QString::fromStdString( aux.substr( aux.find('@')+1 ) ) );
                } else {
                    pieMarker->setLabel( "" );
                    pieMarker->slice()->setLabel( "" );
                }
            }
        }
    }
}
