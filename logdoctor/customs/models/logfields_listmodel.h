#ifndef LOGDOCTOR__CUSTOMS__LOGFIELDS_LISTMODEL_H
#define LOGDOCTOR__CUSTOMS__LOGFIELDS_LISTMODEL_H


#include <QAbstractListModel>

enum class LogField;


class LogFieldsListModel : public QAbstractListModel
{
    Q_OBJECT

protected:
    using data_pair_t = std::pair<LogField, std::string>;
    using real_data_t = std::vector<data_pair_t>;

public:
    explicit LogFieldsListModel( real_data_t&& new_data, QObject* parent=nullptr );
    virtual ~LogFieldsListModel() = default;

    void translate();

    int rowCount( const QModelIndex& parent=QModelIndex() ) const override;
    QVariant data( const QModelIndex& index, int role ) const override;

private:
    real_data_t  real_data;
    QVector<QString> view_data;

};


class DaytimeLogFieldsListModel final : public LogFieldsListModel
{
    Q_OBJECT
public:
    DaytimeLogFieldsListModel( QObject* parent=nullptr );
};


class RelationslLogFieldsListModel final : public LogFieldsListModel
{
    Q_OBJECT
public:
    RelationslLogFieldsListModel( QObject* parent=nullptr );
};


#endif // LOGDOCTOR__CUSTOMS__LOGFIELDS_LISTMODEL_H
