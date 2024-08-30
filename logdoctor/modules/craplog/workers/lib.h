#ifndef LOGDOCTOR__CRAPLOG__WORKERS__LIB_H
#define LOGDOCTOR__CRAPLOG__WORKERS__LIB_H


#include <QMetaType>


//! Signals which dialog to show
enum class WorkerDialog {
    errGeneric,                 // 1 arg
    errDirNotExists,            // 1 arg
    errPathHasSymlink,          // 1 arg
    errFailedDefiningLogType,   // 1 arg
    errFailedParsingLogs,       // 1 arg
    errDatabaseFileNotFound,    // 1 arg
    errDatabaseFileNotFile,     // 1 arg
    errDatabaseFileNotReadable, // 1 arg
    errDatabaseFileNotWritable, // 1 arg
    errDatabaseFailedOpening,   // 2 args
    errDatabaseFailedExecuting, // 3 args
    warnFileNotReadable,        // 1 arg
    warnEmptyFile,              // 1 arg
};

Q_DECLARE_METATYPE(WorkerDialog)


#endif // LOGDOCTOR__CRAPLOG__WORKERS__LIB_H
