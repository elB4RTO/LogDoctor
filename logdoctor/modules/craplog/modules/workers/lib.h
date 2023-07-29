#ifndef WORKERS_LIB_H
#define WORKERS_LIB_H

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

#endif // WORKERS_LIB_H
