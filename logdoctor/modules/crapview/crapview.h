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


    //! Returns le list of available years, for the given web server
    /*!
        \param web_server The printable Web Server name
        \return The list of yearss which are avaliable
    */
    QStringList getYears(  const QString& web_server ) const noexcept;

    //! Returns le list of available months in the given year, for the given web server
    /*!
        \param web_server The printable Web Server name
        \param year The year
        \return The list of printable month names which are avaliable
    */
    QStringList getMonths( const QString& web_server, const QString& year ) const noexcept;

    //! Returns le list of available days in the given month and year, for the given web server
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

    // charts theme ID
    int charts_theme{ 0 };

    const QColor warning_color{ 255, 140, 0, 255 };

    DbQuery dbQuery;

    // collection of available dates
    // db_dates_t = std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::vector<int>>>>
    // { web_server_id : { year : { month : [ days ] } } }
    database_dates_t dates;


    // converr Web Servers names to  Web Server IDs
    const QHash<QString, int> WebServer_s2i{
            {"apache",11}, {"nginx",12}, {"iis",13} };

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
