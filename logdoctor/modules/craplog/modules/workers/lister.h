#ifndef LOGDOCTOR__CRAPLOG__WORKERS__LISTER_H
#define LOGDOCTOR__CRAPLOG__WORKERS__LISTER_H


#include <QObject>

struct LogsFormat;
struct LogFile;
class HashOps;

enum class WorkerDialog;


class CraplogLister final : public QObject
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

    void showDialog( const WorkerDialog dialog_type,
                     const QStringList arg );

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


#endif // LOGDOCTOR__CRAPLOG__WORKERS__LISTER_H
