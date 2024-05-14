#ifndef LOGDOCTOR__CRAPLOG__MODULES__FORMATS__SAMPLES_H
#define LOGDOCTOR__CRAPLOG__MODULES__FORMATS__SAMPLES_H


struct LogsFormat;

class QString;


namespace FormatOps::Private
{

//! Returns a log line sample based on the given format
/*!
    \param logs_format The logs format to use to build the sample
    \return The sample line
    \see LogsFormat, Craplog::getLogsFormatSample()
*/
QString getApacheLogSample( const LogsFormat& logs_format );

//! Returns a log line sample based on the given format
/*!
    \param logs_format The logs format to use to build the sample
    \return The sample line
    \see LogsFormat, Craplog::getLogsFormatSample()
*/
QString getNginxLogSample( const LogsFormat& logs_format );

//! Returns a log line sample based on the given format
/*!
    \param logs_format The logs format to use to build the sample
    \return The sample line
    \see LogsFormat, Craplog::getLogsFormatSample()
*/
QString getIisLogSample( const LogsFormat& logs_format );

} // namespace FormatOps::Private


#endif // LOGDOCTOR__CRAPLOG__MODULES__FORMATS__SAMPLES_H
