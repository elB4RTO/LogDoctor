#ifndef CRAPLOGWORKER_H
#define CRAPLOGWORKER_H

#include <QObject>

class LogsFormat;
class LogFile;
class HashOps;


class CraplogLister : public QObject
{
    Q_OBJECT

public:

    explicit CraplogLister(
        const unsigned web_server_id,
        const unsigned dialogs_level,
        const std::string& logs_path,
        const LogsFormat& logs_format,
        const HashOps& hashOps,
        const std::function<bool(const std::string&)> check_filename,
        QObject* parent=nullptr
    );


signals:

    void pushLogFile( const LogFile log_file );

    void done();

    void retire();

public slots:

    void work();

private:

    const unsigned wsID;

    const unsigned dialogs_level;

    const std::string& logs_path;

    const LogsFormat& logs_format;

    const HashOps& hashOps;

    const std::function<bool(const std::string&)> check_filename;

    void quit();

};


#endif // CRAPLOGWORKER_H
