
#include "languages_listmodel.h"

#include "modules/exceptions.h"


LanguagesListModel::LanguagesListModel(QObject *parent)
    : QAbstractListModel{parent}
{
}

int LanguagesListModel::indexOfLanguage( const std::string& lang ) const
{
    int index{ 0 };
    for ( const auto& [code,_] : this->model_data ) {
        if ( code == lang ) {
            return index;
        }
        ++index;
    }
    throw DoNotCatchException( "Unexpected Language code", lang );
}

int LanguagesListModel::rowCount( const QModelIndex& parent ) const
{
    Q_UNUSED(parent)
    return static_cast<int>( this->model_data.size() );
}

QVariant LanguagesListModel::data( const QModelIndex& index, int role ) const
{
    if ( !index.isValid() || index.row() >= this->model_data.size() ) {
        return QVariant{};
    }

    const auto& item{ this->model_data.at( index.row() ) };

    switch ( role ) {
        case Qt::DisplayRole:
            return item.second;
        case Qt::UserRole:
            return QVariant::fromValue<std::string>( item.first );
        default:
            return QVariant{};
    }
}
