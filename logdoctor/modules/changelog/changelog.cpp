
#include "changelog.h"
#include "ui_changelog.h"

#include "modules/stylesheets.h"

#include "modules/exceptions.h"


Changelog::Changelog( const QFont& font, const QFont& tb_font, QWidget *parent )
    : QWidget{ parent }
    , ui{ new Ui::Changelog }
{
    this->ui->setupUi(this);

    this->setFont( font );
    this->ui->tree_Versions->setFont( font );
    this->ui->text_Logs->setFont( tb_font );

    this->setStyleSheet( StyleSec::Changelog::getStyleSheet() );
}

void Changelog::setTextFont( const QFont& font ) noexcept
{
    this->ui->text_Logs->setFont( font );
}

void Changelog::on_tree_Versions_itemClicked(QTreeWidgetItem* item, int index)
{
    Q_UNUSED(index)

    static const QRegularExpression regex{ R"(^v[0-9]{1}\.[0-9]{2,2}$)" };

    const QString version{ item->text(0) };
    if ( ! regex.match( version ).hasMatch() ) {
        return;
    }

    QString content{ QStringLiteral(R"(<!DOCTYPE html><html><head><style>li.elem{margin-top:10px;}</style></head><body><h2>%1</h2><ul>)").arg( version ) };

    if ( version == "v1.00" ) {

        content.append( R"(<li class="elem">)" );
        content.append( tr("C++17") );
        content.append( R"(</li><li class="elem">)" );
        content.append( tr("Qt5") );
        content.append( R"(</li><li class="elem">)" );
        content.append( tr("Build scripts") );
        content.append( R"(</li><li class="elem">)" );
        content.append( tr("Cross-platform compatibility:") );
            content.append( R"(<ul><li>)" );
            content.append( tr("Linux") );
            content.append( R"(</li><li>)" );
            content.append( tr("BSD") );
            content.append( R"(</li><li>)" );
            content.append( tr("Windows") );
            content.append( R"(</li><li>)" );
            content.append( tr("OSX") );
            content.append( R"(</li></ul>)" );
        content.append( R"(</li><li class="elem">)" );
        content.append( tr("Statistics:") );
            content.append( R"(<ul><li>)" );
            content.append( tr("Warnings") );
            content.append( R"(</li><li>)" );
            content.append( tr("Speed") );
            content.append( R"(</li><li>)" );
            content.append( tr("Counts") );
            content.append( R"(</li><li>)" );
            content.append( tr("Daytime") );
            content.append( R"(</li><li>)" );
            content.append( tr("Relational") );
            content.append( R"(</li><li>)" );
            content.append( tr("Globals") );
            content.append( R"(</li></ul>)" );
        content.append( R"(</li><li class="elem">)" );
        content.append( tr("Translations:") );
            content.append( R"(<ul><li>)" );
            content.append( tr("Italian") );
            content.append( R"(</li><li>)" );
            content.append( tr("Spanish") );
            content.append( R"(</li><li>)" );
            content.append( tr("French") );
            content.append( R"(</li></ul>)" );
        content.append( R"(</li><li class="elem">)" );
        content.append( tr("Tools:") );
            content.append( R"(<ul><li>)" );
            content.append( tr("Block note") );
            content.append( R"(</li></ul>)" );
        content.append( R"(</li><li class="elem">)" );
        content.append( tr("Utilities:") );
            content.append( R"(<ul><li>)" );
            content.append( tr("Infos viewer") );
            content.append( R"(</li><li>)" );
            content.append( tr("Updates checker") );
            content.append( R"(</li></ul>)" );
        content.append( R"(</li><li class="elem">)" );
        content.append( tr("Themes:") );
            content.append( R"(<ul><li>)" );
            content.append( tr("Dark") );
            content.append( R"(</li><li>)" );
            content.append( tr("Light") );
            content.append( R"(</li></ul>)" );
        content.append( R"(</li><p></p>)" ); // leave the p tag

    } else if ( version == "v1.01" ) {

        content.append( R"(<li class="elem">)" );
        content.append( tr("Improvements and fixes") );
        content.append( R"(</li><li class="elem">)" );
        content.append( tr("New themes:") );
            content.append( R"(<ul><li>)" );
            content.append( tr("Ash") );
            content.append( R"(</li><li>)" );
            content.append( tr("Candy") );
            content.append( R"(</li><li>)" );
            content.append( tr("Forest").append(" ← <i>").append(tr("Dark")).append("</i>") );
            content.append( R"(</li><li>)" );
            content.append( tr("Powder").append(" ← <i>").append(tr("Light")).append("</i>") );
            content.append( R"(</li></ul>)" );
        content.append( R"(</li><p></p>)" ); // leave the p tag

    } else if ( version == "v2.00" ) {

        content.append( R"(<li class="elem">)" );
        content.append( tr("Improvements and fixes") );
        content.append( R"(</li><li class="elem">)" );
        content.append( tr("Restyled GUI") );
        content.append( R"(</li><li class="elem">)" );
        content.append( tr("Restyled dialogs") );
        content.append( R"(</li><li class="elem">)" );
        content.append( tr("Doxygen documentation") );
        content.append( R"(</li><li class="elem">)" );
        content.append( tr("Mini-Games:") );
            content.append( R"(<ul><li>)" );
            content.append( tr("Criss-cross") );
            content.append( R"(</li><li>)" );
            content.append( tr("Snake") );
            content.append( R"(</li></ul>)" );
        content.append( R"(</li><p></p>)" ); // leave the p tag

    } else if ( version == "v2.01" ) {

        content.append( R"(<li class="elem">)" );
        content.append( tr("Improvements and fixes") );
        content.append( R"(</li><li class="elem">)" );
        content.append( tr("Changed default paths") );
        content.append( R"(</li><li class="elem">)" );
        content.append( tr("New game modes for Snake") );
            content.append( R"(<ul><li>)" );
            content.append( tr("Hunt") );
            content.append( R"(</li><li>)" );
            content.append( tr("Battle") );
            content.append( R"(</li></ul>)" );
        content.append( R"(</li><p></p>)" ); // leave the p tag

    } else if ( version == "v2.02" ) {

        content.append( R"(<li class="elem">)" );
        content.append( tr("Improvements and fixes") );
        content.append( R"(</li><li class="elem">)" );
        content.append( tr("New translations:") );
            content.append( R"(<ul><li>)" );
            content.append( tr("Japanese") );
            content.append( R"(</li><li>)" );
            content.append( tr("Portuguese") );
            content.append( R"(</li></ul>)" );
        content.append( R"(</li><p></p>)" ); // leave the p tag

    } else if ( version == "v2.03" ) {

        content.append( R"(<li class="elem">)" );
        content.append( tr("Improvements and fixes") );
        content.append( R"(</li><p></p>)" ); // leave the p tag

    } else if ( version == "v2.04" ) {

        content.append( R"(<li class="elem">)" );
        content.append( tr("Code improvements") );
        content.append( R"(</li><li class="elem">)" );
        content.append( tr("Performance improvements") );
        content.append( R"(</li><li class="elem">)" );
        content.append( tr("Added tests suite:") );
            content.append( R"(<ul><li>)" );
            content.append( tr("white box tests") );
            content.append( R"(</li></ul>)" );
        content.append( R"(</li><li class="elem">)" );
        content.append( tr("Customized charts themes") );
        content.append( R"(</li><p></p>)" ); // leave the p tag

    } else if ( version == "v2.05" ) {

        content.append( R"(<li class="elem">)" );
        content.append( tr("Fixes") );
        content.append( R"(</li><li class="elem">)" );
        content.append( tr("Code improvements") );
        content.append( R"(</li><li class="elem">)" );
        content.append( tr("Performance improvements") );
        content.append( R"(</li><li class="elem">)" );
        content.append( tr("Tests improvements") );
        content.append( R"(</li><li class="elem">)" );
        content.append( tr("Docker support") );
        content.append( R"(</li><p></p>)" ); // leave the p tag

    } else if ( version == "v3.00" ) {

        content.append( R"(<li class="elem">)" );
        content.append( tr("Upgrade to C++20") );
        content.append( R"(</li><li class="elem">)" );
        content.append( tr("Upgrade to Qt6") );
        content.append( R"(</li><li class="elem">)" );
        content.append( tr("Restyled themes") );
        content.append( R"(</li><li class="elem">)" );
        content.append( tr("Redesigned configs section") );
        content.append( R"(</li><li class="elem">)" );
        content.append( tr("Code improvements") );
        content.append( R"(</li><li class="elem">)" );
        content.append( tr("Tests improvements") );
        content.append( R"(</li><li class="elem">)" );
        content.append( tr("Fixes") );
        content.append( R"(</li><p></p>)" ); // leave the p tag

    } else if ( version == "v4.00" ) {

        content.append( R"(<li class="elem">)" );
        content.append( tr("Fixes") );
        content.append( R"(</li><li class="elem">)" );
        content.append( tr("Code improvements") );
        content.append( R"(</li><li class="elem">)" );
        content.append( tr("Performance improvements") );
        content.append( R"(</li><li class="elem">)" );
        content.append( tr("Stability improvements") );
        content.append( R"(</li><li class="elem">)" );
        content.append( tr("Tests improvements") );
        content.append( R"(</li><li class="elem">)" );
        content.append( tr("Generating warnings dinamically") );
        content.append( R"(</li><li class="elem">)" );
        content.append( tr("New tool:") );
            content.append( R"(<ul><li>)" );
            content.append( tr("Changelog") );
            content.append( R"(</li></ul>)" );
        content.append( R"(</li><p></p>)" ); // leave the p tag

    } else if ( version == "v4.01" ) {

        content.append( R"(<li class="elem">)" );
        content.append( tr("Tweakable parameters for some of the charts:") );
            content.append( R"(<ul><li>)" );
            content.append( tr("Speed") );
            content.append( R"(</li><li>)" );
            content.append( tr("Counts") );
            content.append( R"(</li><li>)" );
            content.append( tr("Relational") );
            content.append( R"(</li></ul>)" );
        content.append( R"(</li><li class="elem">)" );
        content.append( tr("Improved project structure") );
        content.append( R"(</li><li class="elem">)" );
        content.append( tr("Modernized Cmake usage") );
        content.append( R"(</li><li class="elem">)" );
        content.append( tr("Code improvements") );
        content.append( R"(</li><li class="elem">)" );
        content.append( tr("Tests improvements") );
        content.append( R"(</li><p></p>)" ); // leave the p tag

    } else if ( version == "v4.02" ) {

        content.append( R"(<li class="elem">)" );
        content.append( tr("Upgrade to C++23") );
        content.append( R"(</li><li class="elem">)" );
        content.append( tr("Automatized translations") );
        content.append( R"(</li><li class="elem">)" );
        content.append( tr("Security improvements") );
        content.append( R"(</li><li class="elem">)" );
        content.append( tr("Tests improvements") );
        content.append( R"(</li><li class="elem">)" );
        content.append( tr("Fixes") );
        content.append( R"(</li><p></p>)" ); // leave the p tag

    } else {
        throw DoNotCatchException( "Unexpected changelog version", version.toStdString() );
    }

    content.append( QStringLiteral("</ul></body></html>") );

    this->ui->text_Logs->setText( content );
}
