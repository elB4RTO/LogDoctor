#ifndef LOGDOCTOR__CUSTOMS__LOGFILE_TREEWIDGETITEM_H
#define LOGDOCTOR__CUSTOMS__LOGFILE_TREEWIDGETITEM_H


#include <QTreeWidgetItem>


class LogFileTreeWidgetItem final : public QTreeWidgetItem {
public:
    LogFileTreeWidgetItem(QTreeWidget*parent=nullptr):QTreeWidgetItem(parent){}
    bool operator < (const QTreeWidgetItem& other) const {
        const int sort_column{ this->treeWidget()->sortColumn() };
        const QString s_str{ this->text(sort_column) };
        const QString o_str{ other.text(sort_column) };
        if ( sort_column == 0 ) {
            // sort by file name
            const qsizetype s_sz{ s_str.size() };
            const qsizetype o_sz{ o_str.size() };
            if ( s_sz != o_sz ) {
                return s_sz < o_sz;
            }
            return s_str < o_str;
        } else {
            // sort by file size
            std::vector<std::string> s_items;
            StringOps::splitrip( s_items, s_str.toStdString(), ' ' );
            std::vector<std::string> o_items;
            StringOps::splitrip( o_items, o_str.toStdString(), ' ' );
            if ( s_items[1] != o_items[1] ) {
                return s_items[1] < o_items[1];
            }
            return std::stof( s_items[0] ) < std::stof( o_items[0] );
        }
        return true;
    }
};


#endif // LOGDOCTOR__CUSTOMS__LOGFILE_TREEWIDGETITEM_H
