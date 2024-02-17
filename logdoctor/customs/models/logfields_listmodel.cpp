
#include "logfields_listmodel.h"

#include "modules/shared.h"

#include "modules/crapview/lib.h"

#include <algorithm>


LogFieldsListModel::LogFieldsListModel( real_data_t&& new_data, QObject* parent )
    : QAbstractListModel{ parent }
    , real_data{ std::move(new_data) }
    , view_data( this->real_data.size() )
{
    this->translate();
}

void LogFieldsListModel::translate()
{
    if ( this->real_data.empty() ) {
        return;
    }

    std::transform( this->real_data.cbegin(), this->real_data.cend(), this->view_data.begin(),
                    [](const data_pair_t& p){ return TR::tr( p.second.c_str() ); } );
}

int LogFieldsListModel::rowCount( const QModelIndex& parent ) const
{
    Q_UNUSED(parent)
    return this->view_data.count();
}

QVariant LogFieldsListModel::data( const QModelIndex& index, int role ) const
{
    if ( !index.isValid() || index.row() >= this->view_data.count() ) {
        return QVariant{};
    }

    switch ( role ) {
        case Qt::DisplayRole:
            return this->view_data.at( index.row() );
        case Qt::UserRole:
            return QVariant::fromValue<LogField>( this->real_data.at( index.row() ).first );
        default:
            return QVariant{};
    }
}



DaytimeLogFieldsListModel::DaytimeLogFieldsListModel( QObject* parent )
    : LogFieldsListModel(
        {
            {LogField::Protocol,     FIELDS__PROTOCOL},
            {LogField::Method,       FIELDS__METHOD},
            {LogField::Uri,          FIELDS__URI},
            {LogField::Query,        FIELDS__QUERY},
            {LogField::ResponseCode, FIELDS__RESPONSE_CODE},
            {LogField::UserAgent,    FIELDS__USER_AGENT},
            {LogField::Client,       FIELDS__CLIENT},
            {LogField::Cookie,       FIELDS__COOKIE},
            {LogField::Referrer,     FIELDS__REFERRER}
        },
        parent )
{

}



RelationslLogFieldsListModel::RelationslLogFieldsListModel( QObject* parent )
    : LogFieldsListModel(
        {
            {LogField::Protocol,      FIELDS__PROTOCOL},
            {LogField::Method,        FIELDS__METHOD},
            {LogField::Uri,           FIELDS__URI},
            {LogField::Query,         FIELDS__QUERY},
            {LogField::ResponseCode,  FIELDS__RESPONSE_CODE},
            {LogField::TimeTaken,     FIELDS__TIME_TAKEN},
            {LogField::BytesSent,     FIELDS__BYTES_SENT},
            {LogField::BytesReceived, FIELDS__BYTES_RECEIVED},
            {LogField::UserAgent,     FIELDS__USER_AGENT},
            {LogField::Client,        FIELDS__CLIENT},
            {LogField::Cookie,        FIELDS__COOKIE},
            {LogField::Referrer,      FIELDS__REFERRER}
        },
        parent )
{

}
