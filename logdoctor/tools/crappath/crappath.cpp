
#include "crappath.h"

#include "globals/global_configs.h"

#include "modules/exceptions.h"

#include <unordered_map>


namespace /*private*/
{

enum StyleId : uint32_t {
    BUTTONS_BASE,
    BUTTONS_BASE_HOVER,
    BUTTONS_BASE_FLAT,
    BUTTONS_BASE_DISABLED
};

using StyleMap = std::unordered_map<StyleId, QString>;

StyleMap makeStyleMap()
{
    switch ( GlobalConfigs::window_theme ) {
    case WindowTheme::Light:
        return {
            {BUTTONS_BASE,
             "rgb( 99, 188, 255 )"},
            {BUTTONS_BASE_HOVER,
             "rgb( 123, 201, 255 )"},
            {BUTTONS_BASE_FLAT,
             "rgb( 200, 219, 238 )"},
            {BUTTONS_BASE_DISABLED,
             "rgb( 200, 219, 238 )"}
        };
    case WindowTheme::Dark:
        return {
            {BUTTONS_BASE,
             "rgb( 10, 155, 10 )"},
            {BUTTONS_BASE_HOVER,
             "rgb( 33, 162, 33 )"},
            {BUTTONS_BASE_FLAT,
             "rgb( 21, 71, 21 )"},
            {BUTTONS_BASE_DISABLED,
             "rgb( 21, 71, 21 )"}
        };
    default:
        throw DoNotCatchException( "Unexpected WindowTheme", std::to_string(static_cast<themes_t>(GlobalConfigs::window_theme)) );
    }
}

QString makeStyleSheet()
{
    const StyleMap style{ makeStyleMap() };
    return
        "QPushButton {"
        "   border: 0px;"
        "   border-radius: 8px;"
        "   padding-left: 4px;"
        "   padding-right: 4px;"
        "   background-color: "+style.at(BUTTONS_BASE)+";"
                                   "}"
                                   "QPushButton:hover {"
                                   "   background-color: "+style.at(BUTTONS_BASE_HOVER)+";"
                                         "}"
                                         "QPushButton::flat {"
                                         "   background-color: "+style.at(BUTTONS_BASE_FLAT)+";"
                                        "}"
                                        "QPushButton::disabled {"
                                        "   background-color: "+style.at(BUTTONS_BASE_DISABLED)+";"
                                            "}";
}

} //namespace (private)



Crappath::Crappath( QWidget* parent )
    : QFileDialog(parent)
{
    QFileDialog::setViewMode( QFileDialog::Detail );

    QFileDialog::setFileMode( QFileDialog::Directory );

    QFileDialog::setOptions(
        QFileDialog::ShowDirsOnly                |
        QFileDialog::DontResolveSymlinks         |
        QFileDialog::ReadOnly                    |
        QFileDialog::DontUseCustomDirectoryIcons );

    QFileDialog::setFilter(
        QDir::Dirs        |
        QDir::NoDot       |
        QDir::NoSymLinks  |
        QDir::Hidden      );

    QFileDialog::setLabelText( QFileDialog::Accept, Crappath::tr("Choose") );
    QFileDialog::setLabelText( QFileDialog::Reject, Crappath::tr("Cancel") );

    switch ( GlobalConfigs::window_theme ) {
        case WindowTheme::Native:
            QFileDialog::setStyleSheet("");
            break;
        case WindowTheme::Light:
            [[fallthrough]];
        case WindowTheme::Dark:
            QFileDialog::setStyleSheet( makeStyleSheet() );
            break;
        default:
            // wrong
            throw DoNotCatchException( "Unexpected WindowTheme", std::to_string(static_cast<themes_t>(GlobalConfigs::window_theme)) );
            break;
    }
}
