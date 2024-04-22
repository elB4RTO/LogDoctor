#ifndef LOGDOCTOR__CRAPVIEW_H
#define LOGDOCTOR__CRAPVIEW_H


#include "modules/crapview/modules/query.h"

#include <QtCharts>

#include <string>


struct Warnlist;

class QTableWidget;


//! Crapview
/*!
    Performs operations related to the visualization of the statistics
*/
class Crapview final : public QObject
{
    Q_OBJECT
    CRAPVIEW_DATA_TYPEDEFS

public:

    explicit Crapview() noexcept {}
    Q_DISABLE_COPY_MOVE(Crapview)

    //! Returns the Dialogs level
    DialogsLevel getDialogsLevel() const noexcept;

    //! Sets the new Dialogs level
    void setDialogsLevel( const DialogsLevel new_level ) noexcept;


    /*//! Sets the new charts theme to use
    void setChartsTheme( const int& new_theme_id );*/


    //! Sets the new path for the logs Collection database
    /*!
        \see DbQuery::setDbPath()
    */
    void setDbPath( const std::string& path ) noexcept;


    //! Returns the month number corresponding to the given printable month
    /*!
        \param month_Str The printable month name
        \return The month number
    */
    int getMonthNumber( const QString& month_str ) const noexcept;


    //! Refreshes the list of the dates which are available in the database
    void refreshDates();

    //! Erases the list of available dates
    void clearDates() noexcept;


    //! Returns the list of available years, for the given web server
    /*!
        \param web_server The printable Web Server name
        \return The list of yearss which are avaliable
    */
    QStringList getYears(  const QString& web_server ) const noexcept;

    //! Returns the list of available months in the given year, for the given web server
    /*!
        \param web_server The printable Web Server name
        \param year The year
        \return The list of printable month names which are avaliable
    */
    QStringList getMonths( const QString& web_server, const QString& year ) const noexcept;

    //! Returns the list of available days in the given month and year, for the given web server
    /*!
        \param web_server The printable Web Server name
        \param year The year
        \param month The printable month name
        \return The list of days which are avaliable
    */
    QStringList getDays( const QString& web_server, const QString& year, const QString& month ) const noexcept;

    //! Returns all the hours of the day
    /*!
        \return The list of all the hours
    */
    QStringList getHours() const noexcept;


    //! Returns a list of the translated columns
    /*!
        To be inserted in a header
        \return The list of fields
    */
    QStringList getWarnHeaderColumns() const noexcept;

    //! Returns a list of the translated columns
    /*!
        To be inserted in a header
        \return The list of fields
    */
    QStringList getSpeedHeaderColumns() const noexcept;


    //! Sets the time interval for the Speed stats
    /*!
        \param interval The time interval to use
    */
    void setSpeedTimeInterval( const qint64 interval ) noexcept;

    //! Returns the current time interval for the Speed stats
    /*!
        \return The time interval in use
    */
    qint64 getSpeedTimeInterval() const noexcept;

    //! Sets the time format for the Speed labels on the X-axis
    /*!
        \param format The format to use
    */
    void setSpeedTimeFormat( const QString& format ) noexcept;

    //! Returns the current time format for the Speed labels on the X-axis
    /*!
        \return The time format in use
    */
    const QString& getSpeedTimeFormat() const noexcept;


    //! Sets the size of the pie chart for the Count stats
    /*!
        \param size The new size
    */
    void setCountPieSize( const qreal size ) noexcept;

    //! Returns the current size of the pie chart for the Count stats
    /*!
        \return The size in use
    */
    qreal getCountPieSize() const noexcept;

    //! Sets the maximum number of slices of pie chart for the Count stats
    /*!
        \param value The maximum number of slices
    */
    void setCountMaxSlices( const int value ) noexcept;

    //! Returns the current maximum number of slices of pie chart for the Count stats
    /*!
        \return The maximum number of slices in use
    */
    int getCountMaxSlices() const noexcept;


    //! Sets the time format for the Relational labels on the X-axis
    /*!
        \param format The format to use
    */
    void setRelatTimeFormat( const QString& format ) noexcept;

    //! Returns the current time format for the Relational labels on the X-axis
    /*!
        \return The time format in use
    */
    const QString& getRelatTimeFormat() const noexcept;


    //! Draws the chart and fills the table for the Warnings stats
    /*!
        \param table The table widget
        \param chart The chart widget
        \param theme The base theme to use for the chart
        \param web_server The printable Web Server name
        \param year The year
        \param month The printable month name
        \param day The day
        \param hout The hour
        \param warnlist The warnlists to check against
    */
    void drawWarn(
        QTableWidget* table, QChartView* chart,
        const QChart::ChartTheme& theme,
        const QString web_server,
        const QString year, const QString month, const QString day, const QString hour,
        const Warnlist& warnlist
    ) const;


    //! Draws the chart and fills the table for the Speed stats
    /*!
        \param table The table widget
        \param chart The chart widget
        \param theme The base theme to use for the chart
        \param web_server The printable Web Server name
        \param year The year
        \param month The printable month name
        \param day The day
        \param protocol The given filter for the Protocol
        \param method The given filter for the Method
        \param uri The given filter for the URI
        \param query The given filter for the Query
        \param response The given filter for the Response
    */
    void drawSpeed(
        QTableWidget* table, QChartView* chart,
        const QChart::ChartTheme& theme,
        const QString web_server,
        const QString year, const QString month, const QString day,
        const QString protocol, const QString method, const QString uri, const QString query, const QString response
    ) const;


    //! Draws the chart and fills the table for the Counts stats
    /*!
        \param table The table widget
        \param chart The chart widget
        \param theme The base theme to use for the chart
        \param web_server The printable Web Server name
        \param year The year
        \param month The printable month name
        \param day The day
        \param field The printable log field
    */
    void drawCount(
        QTableWidget* table, QChartView* chart,
        const QChart::ChartTheme& theme,
        const QString web_server,
        const QString year, const QString month, const QString day,
        const QString field
    ) const;


    //! Draws the chart for the Daytime stats
    /*!
        \param chart The chart widget
        \param theme The base theme to use for the chart
        \param web_server The printable Web Server name
        \param year The initial year
        \param month The initial printable month name
        \param day The initial day
        \param year The final year
        \param month The final printable month name
        \param day The finl day
        \param field The printable log field
        \param filter The give filter string
    */
    void drawDay(
        QChartView* chart,
        const QChart::ChartTheme& theme,
        const QString web_server,
        const QString from_year, const QString from_month, const QString from_day,
        const QString to_year, const QString to_month, const QString to_day,
        const QString field_str, const LogField field, const QString filter
    ) const;


    //! Draws the chart for the Relational stats
    /*!
        \param chart The chart widget
        \param theme The base theme to use for the chart
        \param web_server The printable Web Server name
        \param year The initial year
        \param month The initial printable month name
        \param day The initial day
        \param year The final year
        \param month The final printable month name
        \param day The finl day
        \param field_1 The first printable log field
        \param filter_1 The give filter string for the first field
        \param field_2 The second printable log field
        \param filter_2 The give filter string for the second field
    */
    void drawRelat(
        QChartView* chart,
        const QChart::ChartTheme& theme,
        const QString web_server,
        const QString from_year, const QString from_month, const QString from_day,
        const QString to_year,   const QString to_month,   const QString to_day,
        const QString field_1_str, const LogField field_1, const QString filter_1,
        const QString field_2_str, const LogField field_2, const QString filter_2
    ) const;


    //! Retrieves the data to fill the Globals stats
    /*!
        \param recur_list The list which will hold the printable recurrencies
        \param traffic_list The list which will hold the printable traffics
        \param perf_list The list which will hold the printable performances
        \param work_list The list which will hold the printable overall work infos
        \param web_server The printable Web Server name
        \return Whether the operation has been successful or not
    */
    bool calcGlobals(
        std::vector<std::tuple<QString,QString>>& recur_list,
        std::vector<std::tuple<QString,QString>>& traffic_list,
        std::vector<std::tuple<QString,QString>>& perf_list,
        std::vector<QString>& work_list,
        const QString web_server
    ) const;


private slots:

    void sliceClicked( QPieSlice* slice );


private:

    // quantity of information to display throught dialogs
    DialogsLevel dialogs_level{ DL_NORMAL };

    const QColor warning_color{ 255, 140, 0, 255 };

    DbQuery dbQuery;

    // collection of available dates
    // db_dates_t = std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::vector<int>>>>
    // { web_server_id : { year : { month : [ days ] } } }
    database_dates_t dates;

    qint64 speed_interval{ 1ll };
    QString speed_time_format{ "hh:mm" };

    qreal count_pie_size{ 0.6 };
    int count_max_slices{ 15 };

    QString relat_time_format{ "hh:mm" };


    // converr Web Servers names to  Web Server IDs
    const QHash<QString, WebServer> WebServer_s2e{
        {"apache",WS_APACHE}, {"nginx",WS_NGINX}, {"iis",WS_IIS} };

    // convert months names to months numbers
    const QHash<QString, int> Months_s2i{
        {QString::fromStdString(this->dbQuery.MONTHS.at(1)),   1},
        {QString::fromStdString(this->dbQuery.MONTHS.at(2)),   2},
        {QString::fromStdString(this->dbQuery.MONTHS.at(3)),   3},
        {QString::fromStdString(this->dbQuery.MONTHS.at(4)),   4},
        {QString::fromStdString(this->dbQuery.MONTHS.at(5)),   5},
        {QString::fromStdString(this->dbQuery.MONTHS.at(6)),   6},
        {QString::fromStdString(this->dbQuery.MONTHS.at(7)),   7},
        {QString::fromStdString(this->dbQuery.MONTHS.at(8)),   8},
        {QString::fromStdString(this->dbQuery.MONTHS.at(9)),   9},
        {QString::fromStdString(this->dbQuery.MONTHS.at(10)), 10},
        {QString::fromStdString(this->dbQuery.MONTHS.at(11)), 11},
        {QString::fromStdString(this->dbQuery.MONTHS.at(12)), 12}};

};


#endif // LOGDOCTOR__CRAPVIEW_H
