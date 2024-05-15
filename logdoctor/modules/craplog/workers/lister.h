#ifndef LOGDOCTOR__CRAPLOG__WORKERS__LISTER_H
#define LOGDOCTOR__CRAPLOG__WORKERS__LISTER_H


#include "main_lib.h"

#include <QObject>

struct LogsFormat;
struct LogFile;
class Hasher;

enum class WorkerDialog;


class CraplogLister final : public QObject
{
    Q_OBJECT

public:

    explicit CraplogLister(
        const WebServer web_server,
        const DialogsLevel dialogs_level,
        const std::string& logs_path,
        const LogsFormat& logs_format,
        const Hasher& hasher,
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

    const WebServer web_server;

    const DialogsLevel dialogs_level;

    const std::string& logs_path;

    const LogsFormat& logs_format;

    const Hasher& hasher;

    const std::function<bool(const std::string&)> check_filename;

    void quit();

};


#endif // LOGDOCTOR__CRAPLOG__WORKERS__LISTER_H
