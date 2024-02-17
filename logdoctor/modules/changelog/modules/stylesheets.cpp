
#include "stylesheets.h"

#include "globals/global_configs.h"

#include "modules/exceptions.h"

#include <QString>
#include <QtCore/QStringBuilder>

#include <unordered_map>


namespace /*private*/
{

enum StyleId : uint32_t {
    TEXT,
    WINDOW_PRIMARY,
    VERSIONS_TREE_BASE,
    VERSIONS_TREE_BASE_SELECTION,
    TEXTBROWSER_TEXT,
    TEXTBROWSER_BASE,
    TEXTBROWSER_BORDER,
    TEXTBROWSER_TEXT_SELECTION,
    TEXTBROWSER_BASE_SELECTION,
    SCROLLBAR_BASE,
    SCROLLBAR_HANDLER,
    SCROLLBAR_CONTROLS,
    TABLES_HEADER,
    TABLES_HEADER_HOVER,
    TABLES_DECO
};

using StyleMap = std::unordered_map<StyleId, QString>;

StyleMap makeStyleMap()
{
    switch ( GlobalConfigs::window_theme ) {
    case WindowTheme::Light:
        return {
            {TEXT,
                "rgb( 22, 11, 0 )"},
            {WINDOW_PRIMARY,
                "rgb( 230, 230, 230 )"},
            {VERSIONS_TREE_BASE,
                "rgb( 245, 245, 247 )"},
            {VERSIONS_TREE_BASE_SELECTION,
                "rgb( 220, 220, 222 )"},
            {TEXTBROWSER_TEXT,
                "rgb( 88, 80, 80 )"},
            {TEXTBROWSER_BASE,
                "rgb( 250, 250, 255 )"},
            {TEXTBROWSER_BORDER,
                "rgb( 236, 236, 236 )"},
            {TEXTBROWSER_TEXT_SELECTION,
                "rgb( 68, 60, 60 )"},
            {TEXTBROWSER_BASE_SELECTION,
                "rgb( 207, 201, 195 )"},
            {SCROLLBAR_BASE,
                "transparent"},
            {SCROLLBAR_HANDLER,
                "rgb( 40, 185, 40 )"},
            {SCROLLBAR_CONTROLS,
                "transparent"},
            {TABLES_HEADER,
                "rgb( 220, 220, 220 )"},
            {TABLES_HEADER_HOVER,
                "rgb( 201, 239, 255 )"},
            {TABLES_DECO,
                "rgb( 99, 188, 255 )"}
        };
    case WindowTheme::Dark:
        return {
            {TEXT,
                "rgb( 248, 248, 248 )"},
            {WINDOW_PRIMARY,
                "rgb( 27, 30, 33 )"},
            {VERSIONS_TREE_BASE,
                "rgb( 20, 21, 22 )"},
            {VERSIONS_TREE_BASE_SELECTION,
                "rgb( 40, 42, 44 )"},
            {TEXTBROWSER_TEXT,
                "rgb( 210, 210, 210 )"},
            {TEXTBROWSER_BASE,
                "rgb( 13, 14, 15 )"},
            {TEXTBROWSER_BORDER,
                "rgb( 32, 32, 32 )"},
            {TEXTBROWSER_TEXT_SELECTION,
                "rgb( 248, 248, 248 )"},
            {TEXTBROWSER_BASE_SELECTION,
                "rgb( 64, 64, 64 )"},
            {SCROLLBAR_BASE,
                "transparent"},
            {SCROLLBAR_HANDLER,
                "rgb( 69, 177, 255 )"},
            {SCROLLBAR_CONTROLS,
                "transparent"},
            {TABLES_HEADER,
                "rgb( 50, 52, 50 )"},
            {TABLES_HEADER_HOVER,
                "rgb( 43, 82, 43 )"},
            {TABLES_DECO,
                "rgb( 10, 155, 10 )"}
        };
    default:
        throw DoNotCatchException( "Unexpected WindowTheme", std::to_string(static_cast<themes_t>(GlobalConfigs::window_theme)) );
    }
}

} //namespace (private)


namespace StyleSec::Changelog
{

QString getStyleSheet()
{
    if ( GlobalConfigs::window_theme == WindowTheme::Native ) {
        return "";
    }
    const StyleMap style{ makeStyleMap() };
    return
        "QWidget#Changelog {"
        "   color: "% style.at(TEXT) %";"
        "   background-color: "% style.at(WINDOW_PRIMARY) %";"
        "}"
        "QFrame {"
        "   border: 0px;"
        "   background-color: transparent;"
        "}"
        // versions
        "QTreeView {"
        "   border-radius: 8px;"
        "   color: "% style.at(TEXT) %";"
        "   background-color: "% style.at(VERSIONS_TREE_BASE) %";"
        "   selection-color: "% style.at(TEXT) %";"
        "   selection-background-color: "% style.at(VERSIONS_TREE_BASE_SELECTION) %";"
        "}"
        "QTreeView QScrollBar::sub-line:vertical {"
        "   margin-top: -12px;"
        "   border: 0px;"
        "   border-top-right-radius: 8px;"
        "   background-color: transparent;"
        "}"
        "QTreeView QScrollBar::handle:vertical,"
        "QTreeView QScrollBar::handle:vertical:hover {"
        "   margin-top: 32px;"
        "}"
        "QTreeView QScrollBar::sub-line:vertical,"
        "QTableView QScrollBar::sub-line:vertical {"
        "   margin-top: -12px;"
        "   height: 24px;"
        "   border: 0px;"
        "   border-top-right-radius: 8px;"
        "   border-bottom: 2px solid "% style.at(TABLES_DECO) %";"
        "   background-color: "% style.at(TABLES_HEADER) %";"
        "}"
        // text browser
        "QTextBrowser {"
        "   border: 4px solid "% style.at(TEXTBROWSER_BORDER) %";"
        "   border-radius: 8px;"
        "   color: "% style.at(TEXTBROWSER_TEXT) %";"
        "   background-color: "% style.at(TEXTBROWSER_BASE) %";"
        "   selection-color: "% style.at(TEXTBROWSER_TEXT_SELECTION) %";"
        "   selection-background-color: "% style.at(TEXTBROWSER_BASE_SELECTION) %";"
        "}"
        "QTextBrowser QScrollBar::handle:vertical {"
        "   padding: 12px;"
        "}"
        // scroll-bars
        "QScrollBar {"
        "   border: 0px solid transparent;"
        "}"
        "QScrollBar:horizontal {"
        "   height: 12px;"
        "   background-color: "% style.at(SCROLLBAR_BASE) %";"
        "}"
        "QScrollBar::handle:horizontal {"
        "   min-width: 16px;"
        "   margin: 5px 0px 5px 0px;"
        "   background-color: "% style.at(SCROLLBAR_HANDLER) %";"
        "}"
        "QScrollBar::handle:horizontal:hover {"
        "   margin: 4px 0px 4px 0px;"
        "   border-radius: 2px;"
        "}"
        "QScrollBar:vertical {"
        "   width: 12px;"
        "   background-color: "% style.at(SCROLLBAR_BASE) %";"
        "}"
        "QScrollBar::handle:vertical {"
        "   min-width: 16px;"
        "   margin: 0px 5px 0px 5px;"
        "   background-color: "% style.at(SCROLLBAR_HANDLER) %";"
        "}"
        "QScrollBar::handle:vertical:hover {"
        "   margin: 0px 4px 0px 4px;"
        "   border-radius: 2px;"
        "}"
        "QScrollBar::add-line,"
        "QScrollBar::sub-line,"
        "QScrollBar::add-pae,"
        "QScrollBar::sub-pae,"
        "QScrollBar::up-arrow,"
        "QScrollBar::down-arrow,"
        "QScrollBar::left-arrow,"
        "QScrollBar::right-arrow {"
        "   border: 0px;"
        "   background-color: "% style.at(SCROLLBAR_CONTROLS) %";"
        "}"
        // tree header
        "QHeaderView::section {"
        "   height: 24px;"
        "   padding-left: 6px;"
        "   border-top-left-radius: 6px;"
        "   border-top-right-radius: 6px;"
        "   color: "% style.at(TEXT) %";"
        "   border-bottom: 2px solid "% style.at(TABLES_DECO) %";"
        "   background-color: "% style.at(TABLES_HEADER) %";"
        "}"
        "QHeaderView::section::hover {"
        "   background-color: "% style.at(TABLES_HEADER_HOVER) %";"
        "}";
}

} // namespace StyleSec::Changelog
