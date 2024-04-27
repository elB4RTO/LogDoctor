
set(MODULES_SOURCES

    modules/shared.h

    modules/exceptions.h
    modules/exceptions.cpp
    modules/tb.h
    modules/tb.cpp

    modules/database/database.h
    modules/database/database.cpp

    modules/dialogs.h
    modules/dialogs.cpp
    modules/dialogs/message_dialog.ui
    modules/dialogs/message_dialog.h
    modules/dialogs/message_dialog.cpp
    modules/dialogs/boolean_dialog.ui
    modules/dialogs/boolean_dialog.h
    modules/dialogs/boolean_dialog.cpp
    modules/dialogs/ida_dialog.ui
    modules/dialogs/ida_dialog.h
    modules/dialogs/ida_dialog.cpp

    modules/blacklists/blacklists.h
    modules/blacklists/blacklists.cpp
    modules/blacklists/modules/lib.h
    modules/blacklists/modules/blacklist.h
    modules/blacklists/modules/blacklist.cpp
    modules/blacklists/modules/blacklist_item.h
    modules/blacklists/modules/blacklist_item.cpp

    modules/warnlists/warnlists.h
    modules/warnlists/warnlists.cpp
    modules/warnlists/modules/lib.h
    modules/warnlists/modules/warnlist.h
    modules/warnlists/modules/warnlist.cpp
    modules/warnlists/modules/warnlist_item.h
    modules/warnlists/modules/warnlist_item.cpp

    modules/crapconf/lib.h

    modules/craplog/lib.h
    modules/craplog/craplog.h
    modules/craplog/craplog.cpp
    modules/craplog/modules/lib.h
    modules/craplog/modules/donuts.h
    modules/craplog/modules/donuts.cpp
    modules/craplog/modules/formats.h
    modules/craplog/modules/formats.cpp
    modules/craplog/modules/hasher.h
    modules/craplog/modules/hasher.cpp
    modules/craplog/modules/hasher/sha256.h
    modules/craplog/modules/hasher/sha256.cpp
    modules/craplog/utilities/datetime.h
    modules/craplog/utilities/datetime.cpp
    modules/craplog/utilities/logs.h
    modules/craplog/utilities/logs.cpp
    modules/craplog/workers/lib.h
    modules/craplog/workers/lister.h
    modules/craplog/workers/lister.cpp
    modules/craplog/workers/parser.h
    modules/craplog/workers/parser.cpp
    modules/craplog/workers/parser/logs_data.h
    modules/craplog/workers/parser/logs_data.cpp

    modules/crapview/lib.h
    modules/crapview/crapview.h
    modules/crapview/crapview.cpp
    modules/crapview/datatypes/fwd.h
    modules/crapview/datatypes/count_stats.h
    modules/crapview/datatypes/count_stats.cpp
    modules/crapview/datatypes/daytime_stats.h
    modules/crapview/datatypes/daytime_stats.cpp
    modules/crapview/datatypes/global_stats.h
    modules/crapview/datatypes/relational_stats.h
    modules/crapview/datatypes/relational_stats.cpp
    modules/crapview/datatypes/speed_stats.h
    modules/crapview/datatypes/speed_stats.cpp
    modules/crapview/datatypes/warning_stats.h
    modules/crapview/datatypes/warning_stats.cpp
    modules/crapview/datatypes/typedefs.h
    modules/crapview/modules/fetcher.h
    modules/crapview/modules/fetcher.cpp
    modules/crapview/modules/fetcher/date_time.h
    modules/crapview/modules/fetcher/utilities.h
    modules/crapview/modules/fetcher/utilities.cpp
    modules/crapview/utilities/charts.h
    modules/crapview/utilities/charts.cpp
    modules/crapview/utilities/datetime.h
    modules/crapview/utilities/datetime.cpp
    modules/crapview/utilities/filters.h
    modules/crapview/utilities/filters.cpp
    modules/crapview/utilities/globals.h

    modules/craphelp/craphelp.ui
    modules/craphelp/craphelp.h
    modules/craphelp/craphelp.cpp

    modules/crapup/crapup.ui
    modules/crapup/crapup.h
    modules/crapup/crapup.cpp
    modules/crapup/modules/stylesheets.h
    modules/crapup/modules/stylesheets.cpp

    modules/crapinfo/crapinfo.ui
    modules/crapinfo/crapinfo.h
    modules/crapinfo/crapinfo.cpp
    modules/crapinfo/modules/stylesheets.h
    modules/crapinfo/modules/stylesheets.cpp

    modules/changelog/changelog.ui
    modules/changelog/changelog.h
    modules/changelog/changelog.cpp
    modules/changelog/modules/stylesheets.h
    modules/changelog/modules/stylesheets.cpp
)
