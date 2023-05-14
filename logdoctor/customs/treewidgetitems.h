#ifndef CUSTOM_TREEWIDGETITEMS_H
#define CUSTOM_TREEWIDGETITEMS_H

#include <QTreeWidgetItem>


class LogFileTreeWidgetItem : public QTreeWidgetItem {
public:
    LogFileTreeWidgetItem(QTreeWidget*parent=nullptr):QTreeWidgetItem(parent){}
    bool operator < (const QTreeWidgetItem& other) const {
        const int sort_column{ this->treeWidget()->sortColumn() };
        const std::string s_str{ this->text(sort_column).toStdString() };
        const std::string o_str{ other.text(sort_column).toStdString() };
        if ( sort_column == 0 ) {
            // sort by file name
            const size_t s_sz{ s_str.size() - 1ul };
            const size_t o_sz{ o_str.size() - 1ul };
            if ( s_sz != o_sz ) {
                return s_sz < o_sz;
            }
            for (size_t i{0ul}; i<=s_sz; i++) {
                if ( s_str[i] != o_str[i] ) {
                    return s_str[i] < o_str[i];
                }
            }
        } else {
            // sort by file size
            const std::unordered_map<std::string,int> cmp{
                {"B",  0},
                {"KiB",1},
                {"MiB",2}
            };
            std::vector<std::string> s_items;
            StringOps::splitrip( s_items, s_str, ' ' );
            std::vector<std::string> o_items;
            StringOps::splitrip( o_items, o_str, ' ' );
            if ( s_items[1] != o_items[1] ) {
                return cmp.at( s_items[1] ) < cmp.at( o_items[1] );
            }
            return std::stof( s_items[0] ) < std::stof( o_items[0] );
        }
        return true;
    }
};


#endif // CUSTOM_TREEWIDGETITEMS_H
