#ifndef LOGDOCTOR__CRAPLOG__WORKERS__LIB_H
#define LOGDOCTOR__CRAPLOG__WORKERS__LIB_H


enum class WorkerDialog {
    errGeneric,
    errDirNotExists,
    errFailedDefiningLogType,
    errFailedParsingLogs,
    errDatabaseFailedOpening,
    errDatabaseFailedExecuting,
    warnFileNotReadable,
    warnEmptyFile,
};

Q_DECLARE_METATYPE(WorkerDialog);


#endif // LOGDOCTOR__CRAPLOG__WORKERS__LIB_H
