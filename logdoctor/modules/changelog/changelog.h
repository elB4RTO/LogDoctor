#ifndef LOGDOCTOR__CHANGELOG_H
#define LOGDOCTOR__CHANGELOG_H


#include <QWidget>

class QTreeWidgetItem;


namespace Ui {
    class Changelog;
}


//! Changelog
/*!
    Displays information about the changes between the various versions of LogDoctor
*/
class Changelog : public QWidget
{
    Q_OBJECT

public:
    explicit Changelog( const QFont& font, const QFont& tb_font, QWidget* parent=nullptr );
    Q_DISABLE_COPY_MOVE(Changelog)

    //! Sets the given font for the changelog info
    void setTextFont( const QFont& font ) noexcept;

private slots:
    void on_tree_Versions_itemClicked(QTreeWidgetItem* item, int index);

private:
    QSharedPointer<Ui::Changelog> ui;
};


#endif // LOGDOCTOR__CHANGELOG_H
