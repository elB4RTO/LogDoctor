
#include "count_stats.h"

#include "modules/database/database.h"
#include "modules/crapview/modules/query/utilities.h"

#include <QPieSeries>
#include <QTableWidget>

#include <ranges>


using namespace QueryPrivate;


bool CountData::buildFromQuery( QueryWrapper& query )
{
    QHash<QString, unsigned> aux_data;

    if ( const size_t size{ query.size() }; size > 0ul ) {
        aux_data.reserve( size * 3 );
    } else {
        return false;
    }

    while ( query->next() ) {
        const QString item{ toString( query[0] ) };
        if ( ! item.isEmpty() ) {
            ++ aux_data[ item ];
        }
    }

    // morph tha QHash into an ordered map
    QHashIterator iter{ aux_data };
    while ( iter.hasNext() ) {
        iter.next();
        data.emplace( iter.value(), iter.key() );
    }

    return true;
}

void CountData::insertInChart( QPieSeries*const pie, const int max_slices, const QString others_label ) const
{
    int n_slices{0}, others_count{0};

    for ( const auto& key_value : std::views::reverse( data ) ) {
        if ( n_slices < max_slices ) {
            pie->append( key_value.second, key_value.first );
        } else {
            others_count += key_value.first;
        }
        ++ n_slices;
    }

    if ( others_count > 0 ) {
        pie->append( others_label, others_count );
        pie->slices().at( max_slices )->setBrush( Qt::gray );
    }
}

void CountData::insertInTable( QTableWidget*const table ) const
{
    int n_rows{0};

    for ( const auto& key_value : std::views::reverse( data ) ) {
        table->insertRow( n_rows );
        auto*const ic{ new QTableWidgetItem() };
        ic->setData( Qt::DisplayRole, key_value.first );
        table->setItem( n_rows, 0, ic );
        table->setItem( n_rows, 1, new QTableWidgetItem( key_value.second ));
        ++ n_rows;
    }
}
