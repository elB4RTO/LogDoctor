
#include "stylesheets.h"

#include "globals/global_configs.h"

#include "modules/exceptions.h"

#include <QString>
#include <QtCore/QStringBuilder>

#include <unordered_map>


namespace /*private*/
{

enum StyleId : uint32_t {
    TEXT_PRIMARY,
    TEXT_DISABLED,
    TOOLTIPS_BASE,
    TOOLTIPS_TEXT,
    TOOLTIPS_BORDER,
    MENU_TEXT,
    MENU_BASE,
    MENU_BASE_HOVER,
    MENU_DROPDOWN_BASE,
    MENU_DROPDOWN_BASE_SELECTION,
    MENU_DROPDOWN_TEXT,
    WINDOW_BASE_PRIMARY,
    WINDOW_BASE_SECONDARY,
    SECTIONS_LOG_HOVER,
    SECTIONS_VIEW_HOVER,
    SECTIONS_CONF_HOVER,
    BORDER_PRIMARY,
    BORDER_SECONDARY,
    BORDER_TERTIARY,
    SEPARATORS_BASE,
    SEPARATORS_BORDER,
    BUTTONS_BASE,
    BUTTONS_BASE_HOVER,
    BUTTONS_BASE_FLAT,
    BUTTONS_BASE_DISABLED,
    HELP_BUTTONS_BASE,
    HELP_BUTTONS_BASE_HOVER,
    WEB_SERVERS_BUTTONS_BASE,
    WEB_SERVERS_BUTTONS_BASE_FLAT,
    WEB_SERVERS_BUTTONS_BASE_DISABLED,
    WEB_SERVERS_BUTTONS_BORDER_FLAT,
    WEB_SERVERS_BUTTONS_BORDER_FLAT_HOVER,
    WEB_SERVERS_BUTTONS_TEXT_FLAT,
    WEB_SERVERS_BUTTONS_TEXT_FLAT_HOVER,
    WEB_SERVERS_BUTTONS_TEXT_DISABLED,
    STATS_SWITCH_BUTTONS,
    STATS_SWITCH_BUTTONS_FLAT,
    STATS_SWITCH_BUTTONS_FLAT_HOVER,
    STATS_GLOBALS_FRAMES,
    BOXES_BASE_PRIMARY,
    BOXES_BASE_SECONDARY,
    BOXES_BASE_DISABLED,
    BOXES_BASE_SELECTION,
    BOXES_DECO,
    LINEDIT_TEXT,
    LINEDIT_BASE,
    LINEDIT_BASE_FOCUS,
    LINEDIT_BASE_SELECTION,
    LINEDIT_BASE_DISABLED,
    TABLES_HEADER,
    TABLES_HEADER_HOVER,
    TABLES_HEADER_SEPARATOR,
    TABLES_BASE,
    TABLES_BASE_DISABLED,
    TABLES_BASE_SELECTION,
    TABLES_GRIDLINE,
    TABLES_TEXT_SELECTION,
    TABLES_DECO,
    CHARTS_CONTOUR,
    TEXTBROWSER_TEXT,
    TEXTBROWSER_BASE,
    TEXTBROWSER_BORDER,
    TEXTBROWSER_TEXT_SELECTION,
    TEXTBROWSER_BASE_SELECTION,
    SCROLLBAR_BASE,
    SCROLLBAR_HANDLER,
    SCROLLBAR_CONTROLS,
    SLIDER_HANDLER,
    SLIDER_BAR_ACTIVE,
    SLIDER_BAR_INACTIVE,
    SLIDER_BAR_ACTIVE_BORDER,
    SLIDER_HANDLER_BORDER,
    SLIDER_HANDLER_BORDER_HOVER,
    BWLISTS_BUTTONS_BASE,
    BWLISTS_BUTTONS_BASE_HOVER,
    BWLISTS_BUTTONS_BASE_DISABLED,
    BWLISTS_BUTTONS_BASE_FLAT,
    CONFIGS_TREE_BASE
};

} //namespace (private)


using StyleMap = std::unordered_map<StyleId, QString>;


StyleMap makeStyleMap()
{
    switch ( GlobalConfigs::window_theme ) {
        case WindowTheme::Light:
            return {
                {TEXT_PRIMARY,
                    "rgb( 22, 11, 0 )"},
                {TEXT_DISABLED,
                    "rgb( 128, 128, 128 )"},
                {TOOLTIPS_BASE,
                    "rgb( 30, 35, 27 )"},
                {TOOLTIPS_TEXT,
                    "rgb( 220, 227, 213 )"},
                {TOOLTIPS_BORDER,
                    "rgb( 170, 176, 160 )"},
                {MENU_TEXT,
                    "rgb( 32, 32, 32 )"},
                {MENU_BASE,
                    "rgb( 200, 200, 200 )"},
                {MENU_BASE_HOVER,
                    "rgb( 210, 210, 210 )"},
                {MENU_DROPDOWN_BASE,
                    "rgb( 112, 112, 112 )"},
                {MENU_DROPDOWN_BASE_SELECTION,
                    "rgb( 128, 128, 128 )"},
                {MENU_DROPDOWN_TEXT,
                    "rgb( 250, 250, 255 )"},
                {WINDOW_BASE_PRIMARY,
                    "rgb( 250, 250, 255 )"},
                {WINDOW_BASE_SECONDARY,
                    "rgb( 230, 230, 230 )"},
                {SECTIONS_LOG_HOVER,
                    "rgb( 255, 188, 218 )"}, // 196, 7, 93
                {SECTIONS_VIEW_HOVER,
                    "rgb( 184, 255, 203 )"}, // 0, 226, 60
                {SECTIONS_CONF_HOVER,
                    "rgb( 179, 228, 255 )"}, // 0, 165, 255
                {BORDER_PRIMARY,
                    "rgb( 230, 230, 230 )"},
                {BORDER_SECONDARY,
                    "rgb( 124, 119, 119 )"},
                {BORDER_TERTIARY,
                    "rgb( 88, 80, 80 )"},
                {SEPARATORS_BASE,
                    "rgb( 88, 80, 80 )"},
                {SEPARATORS_BORDER,
                    "rgb( 250, 250, 255 )"},
                {BUTTONS_BASE,
                    "rgb( 99, 188, 255 )"},
                {BUTTONS_BASE_HOVER,
                    "rgb( 123, 201, 255 )"},
                {BUTTONS_BASE_FLAT,
                    "rgb( 200, 219, 238 )"},
                {BUTTONS_BASE_DISABLED,
                    "rgb( 200, 219, 238 )"},
                {HELP_BUTTONS_BASE,
                    "rgb( 230, 80, 80 )"},
                {HELP_BUTTONS_BASE_HOVER,
                    "rgb( 230, 112, 80 )"},
                {WEB_SERVERS_BUTTONS_BASE,
                    "rgb( 146, 234, 146 )"},
                {WEB_SERVERS_BUTTONS_BASE_FLAT,
                    "transparent"},
                {WEB_SERVERS_BUTTONS_BASE_DISABLED,
                    "rgb( 246, 246, 246 )"},
                {WEB_SERVERS_BUTTONS_BORDER_FLAT,
                    "rgb( 177, 177, 177 )"},
                {WEB_SERVERS_BUTTONS_BORDER_FLAT_HOVER,
                    "rgb( 146, 234, 146 )"},
                {WEB_SERVERS_BUTTONS_TEXT_FLAT,
                    "rgb( 92, 92, 92 )"},
                {WEB_SERVERS_BUTTONS_TEXT_FLAT_HOVER,
                    "rgb( 40, 31, 7 )"},
                {WEB_SERVERS_BUTTONS_TEXT_DISABLED,
                    "rgb( 192, 192, 192 )"},
                {STATS_SWITCH_BUTTONS,
                    "rgb( 230, 80, 80 )"},
                {STATS_SWITCH_BUTTONS_FLAT,
                    "rgb( 250, 250, 255 )"},
                {STATS_SWITCH_BUTTONS_FLAT_HOVER,
                    "rgb( 230, 230, 230 )"},
                {STATS_GLOBALS_FRAMES,
                    "rgb( 242, 242, 242 )"},
                {BOXES_BASE_PRIMARY,
                    "rgb( 230, 230, 230 )"},
                {BOXES_BASE_SECONDARY,
                    "rgb( 220, 220, 220 )"},
                {BOXES_BASE_DISABLED,
                    "rgb( 200, 200, 200 )"},
                {BOXES_BASE_SELECTION,
                    "rgb( 123, 201, 255 )"},
                {BOXES_DECO,
                    "rgb( 99, 188, 255 )"},
                {LINEDIT_TEXT,
                    "rgb( 88, 80, 80 )"},
                {LINEDIT_BASE,
                    "rgb( 236, 236, 236 )"},
                {LINEDIT_BASE_FOCUS,
                    "rgb( 230, 230, 230 )"},
                {LINEDIT_BASE_SELECTION,
                    "rgb( 197, 191, 185 )"},
                {LINEDIT_BASE_DISABLED,
                    "rgb( 247, 247, 247 )"},
                {TABLES_HEADER,
                    "rgb( 220, 220, 220 )"},
                {TABLES_HEADER_HOVER,
                    "rgb( 201, 239, 255 )"},
                {TABLES_HEADER_SEPARATOR,
                    "rgb( 170, 170, 170 )"},
                {TABLES_BASE,
                    "rgb( 242, 242, 242 )"},
                {TABLES_BASE_DISABLED,
                    "rgb( 247, 247, 247 )"},
                {TABLES_BASE_SELECTION,
                    "rgb( 201, 239, 255 )"},
                {TABLES_GRIDLINE,
                    "rgb( 170, 170, 170 )"},
                {TABLES_TEXT_SELECTION,
                    "rgb( 22, 11, 0 )"},
                {TABLES_DECO,
                    "rgb( 99, 188, 255 )"},
                {CHARTS_CONTOUR,
                    "rgb( 220, 220, 220 )"},
                {TEXTBROWSER_TEXT,
                    "rgb( 88, 80, 80 )"},
                {TEXTBROWSER_BASE,
                    "rgb( 236, 236, 236 )"},
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
                {SLIDER_HANDLER,
                    "rgb( 100, 91, 87 )"},
                {SLIDER_BAR_ACTIVE,
                    "rgb( 99, 188, 255 )"},
                {SLIDER_BAR_INACTIVE,
                    "rgb( 210, 203, 199 )"},
                {SLIDER_BAR_ACTIVE_BORDER,
                    "rgb( 123, 201, 255 )"},
                {SLIDER_HANDLER_BORDER,
                    "rgb( 220, 211, 187 )"},
                {SLIDER_HANDLER_BORDER_HOVER,
                    "rgb( 123, 201, 255 )"},
                {BWLISTS_BUTTONS_BASE,
                    "rgb( 99, 188, 255 )"},
                {BWLISTS_BUTTONS_BASE_HOVER,
                    "rgb( 123, 201, 255 )"},
                {BWLISTS_BUTTONS_BASE_DISABLED,
                    "rgb( 200, 219, 238 )"},
                {BWLISTS_BUTTONS_BASE_FLAT,
                    "rgb( 247, 247, 247 )"},
                {CONFIGS_TREE_BASE,
                    "rgb( 245, 245, 247 )"}
            };
            break;
        case WindowTheme::Dark:
            return {
                {TEXT_PRIMARY,
                    "rgb( 248, 248, 248 )"},
                {TEXT_DISABLED,
                    "rgb( 128, 128, 128 )"},
                {TOOLTIPS_BASE,
                    "rgb( 160, 160, 160 )"},
                {TOOLTIPS_TEXT,
                    "rgb( 32, 32, 32 )"},
                {TOOLTIPS_BORDER,
                    "rgb( 144, 144, 144 )"},
                {MENU_TEXT,
                    "rgb( 174, 174, 174 )"},
                {MENU_BASE,
                    "rgb( 49, 54, 58 )"},
                {MENU_BASE_HOVER,
                    "rgb( 40, 45, 49 )"},
                {MENU_DROPDOWN_BASE,
                    "rgb( 144, 144, 144 )"},
                {MENU_DROPDOWN_BASE_SELECTION,
                    "rgb( 128, 128, 128 )"},
                {MENU_DROPDOWN_TEXT,
                    "rgb( 13, 14, 15 )"},
                {WINDOW_BASE_PRIMARY,
                    "rgb( 13, 14, 15 )"},
                {WINDOW_BASE_SECONDARY,
                    "rgb( 27, 30, 33 )"},
                {SECTIONS_LOG_HOVER,
                    "rgb( 82, 0, 37 )"}, // 196, 7, 93
                {SECTIONS_VIEW_HOVER,
                    "rgb( 0, 69, 18 )"}, // 0, 226, 60
                {SECTIONS_CONF_HOVER,
                    "rgb( 0, 57, 89 )"}, // 0, 165, 255
                {BORDER_PRIMARY,
                    "rgb( 40, 45, 49 )"},
                {BORDER_SECONDARY,
                    "rgb( 92, 92, 92 )"},
                {BORDER_TERTIARY,
                    "rgb( 128, 128, 128 )"},
                {SEPARATORS_BASE,
                    "rgb( 96, 96, 96 )"},
                {SEPARATORS_BORDER,
                    "rgb( 13, 14, 15 )"},
                {BUTTONS_BASE,
                    "rgb( 10, 155, 10 )"},
                {BUTTONS_BASE_HOVER,
                    "rgb( 33, 162, 33 )"},
                {BUTTONS_BASE_FLAT,
                    "rgb( 21, 71, 21 )"},
                {BUTTONS_BASE_DISABLED,
                    "rgb( 21, 71, 21 )"},
                {HELP_BUTTONS_BASE,
                    "rgb( 221, 75, 14 )"},
                {HELP_BUTTONS_BASE_HOVER,
                    "rgb( 221, 113, 14 )"},
                {WEB_SERVERS_BUTTONS_BASE,
                    "rgb( 0, 121, 115 )"},
                {WEB_SERVERS_BUTTONS_BASE_FLAT,
                    "transparent"},
                {WEB_SERVERS_BUTTONS_BASE_DISABLED,
                    "rgb( 18, 20, 22 )"},
                {WEB_SERVERS_BUTTONS_BORDER_FLAT,
                    "rgb( 54, 54, 54 )"},
                {WEB_SERVERS_BUTTONS_BORDER_FLAT_HOVER,
                    "rgb( 0, 121, 115 )"},
                {WEB_SERVERS_BUTTONS_TEXT_FLAT,
                    "rgb( 108, 108, 108 )"},
                {WEB_SERVERS_BUTTONS_TEXT_FLAT_HOVER,
                    "rgb( 188, 188, 188 )"},
                {WEB_SERVERS_BUTTONS_TEXT_DISABLED,
                    "rgb( 42, 42, 42 )"},
                {STATS_SWITCH_BUTTONS,
                    "rgb( 221, 75, 14 )"},
                {STATS_SWITCH_BUTTONS_FLAT,
                    "rgb( 13, 14, 15 )"},
                {STATS_SWITCH_BUTTONS_FLAT_HOVER,
                    "rgb( 27, 30, 33 )"},
                {STATS_GLOBALS_FRAMES,
                    "rgb( 29, 29, 29 )"},
                {BOXES_BASE_PRIMARY,
                    "rgb( 39, 42, 45 )"},
                {BOXES_BASE_SECONDARY,
                    "rgb( 45, 48, 51 )"},
                {BOXES_BASE_DISABLED,
                    "rgb( 27, 30, 33 )"},
                {BOXES_BASE_SELECTION,
                    "rgb( 33, 162, 33 )"},
                {BOXES_DECO,
                    "rgb( 10, 155, 10 )"},
                {LINEDIT_TEXT,
                    "rgb( 210, 210, 210 )"},
                {LINEDIT_BASE,
                    "rgb( 41, 44, 44 )"},
                {LINEDIT_BASE_FOCUS,
                    "rgb( 57, 60, 60 )"},
                {LINEDIT_BASE_SELECTION,
                    "rgb( 47, 99, 47 )"},
                {LINEDIT_BASE_DISABLED,
                    "rgb( 18, 20, 22 )"},
                {TABLES_HEADER,
                    "rgb( 50, 52, 50 )"},
                {TABLES_HEADER_HOVER,
                    "rgb( 43, 82, 43 )"},
                {TABLES_HEADER_SEPARATOR,
                    "rgb( 80, 80, 80 )"},
                {TABLES_BASE,
                    "rgb( 29, 29, 29 )"},
                {TABLES_BASE_DISABLED,
                    "rgb( 18, 20, 22 )"},
                {TABLES_BASE_SELECTION,
                    "rgb( 43, 82, 43 )"},
                {TABLES_GRIDLINE,
                    "rgb( 80, 80, 80 )"},
                {TABLES_TEXT_SELECTION,
                    "rgb( 248, 248, 248 )"},
                {TABLES_DECO,
                    "rgb( 10, 155, 10 )"},
                {CHARTS_CONTOUR,
                    "rgb( 50, 52, 50 )"},
                {TEXTBROWSER_TEXT,
                    "rgb( 210, 210, 210 )"},
                {TEXTBROWSER_BASE,
                    "rgb( 32, 32, 32 )"},
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
                {SLIDER_HANDLER,
                    "rgb( 112, 112, 112 )"},
                {SLIDER_BAR_ACTIVE,
                    "rgb( 10, 155, 10 )"},
                {SLIDER_BAR_INACTIVE,
                    "rgb( 64, 64, 64 )"},
                {SLIDER_BAR_ACTIVE_BORDER,
                    "rgb( 33, 162, 33 )"},
                {SLIDER_HANDLER_BORDER,
                    "rgb( 128, 128, 128 )"},
                {SLIDER_HANDLER_BORDER_HOVER,
                    "rgb( 33, 162, 33 )"},
                {BWLISTS_BUTTONS_BASE,
                    "rgb( 10, 155, 10 )"},
                {BWLISTS_BUTTONS_BASE_HOVER,
                    "rgb( 33, 162, 33 )"},
                {BWLISTS_BUTTONS_BASE_DISABLED,
                    "rgb( 21, 71, 21 )"},
                {BWLISTS_BUTTONS_BASE_FLAT,
                    "rgb( 18, 20, 22 )"},
                {CONFIGS_TREE_BASE,
                    "rgb( 20, 21, 22 )"}
            };
            break;
        default:
            throw DoNotCatchException( "Unexpected WindowTheme", std::to_string(static_cast<themes_t>(GlobalConfigs::window_theme)) );
            break;
    }
}


namespace StyleSec
{

QString getStyleSheet()
{
    const QString icons_theme{ GlobalConfigs::icons_set };
    const StyleMap style{ makeStyleMap() };
    return
        ////////////////
        //// SHARED ////
        "* {"
        "   color: "% style.at(TEXT_PRIMARY) %";"
        "}"
        // frames
        "QFrame,"
        "QWidget {"
        "   border: 0px;"
        "   background-color: transparent;"
        "}"
        // tab widgets
        "QTabWidget {"
        "   border: 0px;"
        "}"
        "QTabWidget::tab-bar {"
        "   alignment: left;"
        "}"
        "QTabBar {"
        "   background-color: transparent;"
        "}"
        "QTabBar::tab {"
        "   background-color: transparent;"
        "}"
        // tooltips
        "QToolTip {"
        "   padding: 2px;"
        "   border: 1px solid "% style.at(TOOLTIPS_BORDER) %";"
        "   border-radius: 6px;"
        "   color: "% style.at(TOOLTIPS_TEXT) %";"
        "   background-color: "% style.at(TOOLTIPS_BASE) %";"
        "   font-size: 12px;"
        "}"
        // check boxes
        "QCheckBox::indicator {"
        "   width: 13px;"
        "   height: 13px;"
        "}"
        "QCheckBox::indicator:unchecked {"
        "   image: url(:/icons/icons/"% icons_theme %"/checkbox_unchecked.png);"
        "}"
        "QCheckBox::indicator:indeterminate {"
        "   image: url(:/icons/icons/"% icons_theme %"/checkbox_semichecked.png);"
        "}"
        "QCheckBox::indicator:checked {"
        "   image: url(:/icons/icons/"% icons_theme %"/checkbox_checked.png);"
        "}"
        // radio buttons
        "QRadioButton::indicator {"
        "   width: 13px;"
        "   height : 13px;"
        "}"
        "QRadioButton::indicator:unchecked {"
        "   image: url(:/icons/icons/"% icons_theme %"/radiobutton_unchecked.png);"
        "}"
        "QRadioButton::indicator:checked {"
        "   image: url(:/icons/icons/"% icons_theme %"/radiobutton_checked.png);"
        "}"
        // push buttons
        "QPushButton {"
        "   border: 0px;"
        "   border-radius: 12px;"
        "   background-color: "% style.at(BUTTONS_BASE) %";"
        "}"
        "QPushButton:hover {"
        "   background-color: "% style.at(BUTTONS_BASE_HOVER) %";"
        "}"
        "QPushButton::flat {"
        "   background-color: "% style.at(BUTTONS_BASE_FLAT) %";"
        "}"
        "QPushButton::disabled {"
        "   background-color: "% style.at(BUTTONS_BASE_DISABLED) %";"
        "}"
        // tool buttons
        "QToolButton {"
        "   border: 1px solid "% style.at(BORDER_PRIMARY) %";"
        "   border-radius: 16px;"
        "   background-color: transparent;"
        "}"
        "QToolButton:hover {"
        "   border: 1px solid "% style.at(BORDER_SECONDARY) %";"
        "}"
        // line edits
        "QLineEdit {"
        "   padding-left: 4px;"
        "   padding-right: 4px;"
        "   border-radius: 8px;"
        "   color: "% style.at(LINEDIT_TEXT) %";"
        "   selection-color: "% style.at(LINEDIT_TEXT) %";"
        "   background-color: "% style.at(LINEDIT_BASE) %";"
        "   selection-background-color: "% style.at(LINEDIT_BASE_SELECTION) %";"
        "}"
        "QLineEdit::focus {"
        "   background-color: "% style.at(LINEDIT_BASE_FOCUS) %";"
        "}"
        "QLineEdit:disabled {"
        "   background-color: "% style.at(LINEDIT_BASE_DISABLED) %";"
        "}"
        // combo boxes
        "QComboBox {"
        "   border-radius: 8px;"
        "   background-color: "% style.at(BOXES_BASE_PRIMARY) %";"
        "}"
        "QComboBox::disabled {"
        "   background-color: "% style.at(BOXES_BASE_DISABLED) %";"
        "}"
        "QComboBox::drop-down {"
        "   border-top-right-radius: 8px;"
        "   border-bottom-right-radius: 8px;"
        "   border-left: 3px solid"
        "                QLinearGradient("
        "                   x0:0, y0:0, x1:0, y1:1,"
        "                   stop:0 "% style.at(BOXES_BASE_PRIMARY) %","
        "                   stop:0.1 "% style.at(BOXES_BASE_PRIMARY) %","
        "                   stop:0.5 "% style.at(BOXES_DECO) %","
        "                   stop:0.9 "% style.at(BOXES_BASE_PRIMARY) %","
        "                   stop:1 "% style.at(BOXES_BASE_PRIMARY) %");"
        "   background-color: "% style.at(BOXES_BASE_PRIMARY) %";"
        "   selection-background-color: "% style.at(BOXES_BASE_PRIMARY) %";"
        "}"
        "QComboBox::drop-down::disabled {"
        "   border-left-color: "% style.at(BOXES_BASE_DISABLED) %";"
        "   background-color: "% style.at(BOXES_BASE_DISABLED) %";"
        "}"
        "QComboBox::down-arrow {"
        "   image: url(:/icons/icons/"% icons_theme %"/combobox_arrow.png);"
        "}"
        "QComboBox::down-arrow::disabled {"
        "   image: url();"
        "}"
        "QComboBox:on {"
        "   border-bottom-left-radius: 0px;"
        "   border-bottom: 2px solid "% style.at(BOXES_DECO) %";"
        "   background-color: "% style.at(BOXES_BASE_SECONDARY) %";"
        "}"
        "QComboBox::drop-down:on {"
        "   border-bottom-right-radius: 0px;"
        "   border-left-color: transparent;"
        "   background-color: "% style.at(BOXES_BASE_SECONDARY) %";"
        "}"
        "QComboBox::down-arrow:on {"
        "   image: url();"
        "}"
        "QComboBox QAbstractItemView {"
        "   border-bottom-left-radius: 8px;"
        "   border-bottom-right-radius: 8px;"
        "   background-color: "% style.at(BOXES_BASE_PRIMARY) %";"
        "   selection-background-color: "% style.at(BOXES_BASE_SELECTION) %";"
        "}"
        // spin boxes
        "QSpinBox,"
        "QDoubleSpinBox {"
        "   border-radius: 4px;"
        "   color: "% style.at(LINEDIT_TEXT) %";"
        "   background-color: "% style.at(LINEDIT_BASE) %";"
        "   selection-color: "% style.at(LINEDIT_TEXT) %";"
        "   selection-background-color: "% style.at(LINEDIT_BASE_SELECTION) %";"
        "}"
        "QSpinBox::focus,"
        "QDoubleSpinBox::focus {"
        "   background-color: "% style.at(LINEDIT_BASE_FOCUS) %";"
        "}"
        "QSpinBox::disabled,"
        "QDoubleSpinBox::disabled {"
        "   border-color: "% style.at(BOXES_BASE_DISABLED) %";"
        "   color: "% style.at(TEXT_DISABLED) %";"
        "   background-color: "% style.at(BOXES_BASE_DISABLED) %";"
        "}"
        "QSpinBox::up-button,"
        "QSpinBox::down-button,"
        "QDoubleSpinBox::up-button,"
        "QDoubleSpinBox::down-button {"
        "   padding: 2px;"
        "   border-left: 2px solid "% style.at(BOXES_DECO) %";"
        "   background-color: "% style.at(BOXES_BASE_SECONDARY) %";"
        "}"
        "QSpinBox::up-button:hover,"
        "QSpinBox::down-button:hover,"
        "QDoubleSpinBox::up-button:hover,"
        "QDoubleSpinBox::down-button:hover {"
        "   background-color: "% style.at(BOXES_BASE_SELECTION) %";"
        "}"
        "QSpinBox::up-button:off,"
        "QSpinBox::down-button:off,"
        "QDoubleSpinBox::up-button:off,"
        "QDoubleSpinBox::down-button:off {"
        "   background-color: "% style.at(BOXES_BASE_DISABLED) %";"
        "}"
        "QSpinBox::up-button:disabled,"
        "QSpinBox::down-button:disabled,"
        "QDoubleSpinBox::up-button:disabled,"
        "QDoubleSpinBox::down-button:disabled {"
        "   border-left-color: "% style.at(BOXES_BASE_PRIMARY) %";"
        "   background-color: "% style.at(BOXES_BASE_DISABLED) %";"
        "}"
        "QSpinBox::up-arrow,"
        "QDoubleSpinBox::up-arrow {"
        "   width: 7px;"
        "   height: 7px;"
        "   padding: 2px;"
        "   image: url(:/icons/icons/"% icons_theme %"/spinbox_add.png);"
        "}"
        "QSpinBox::up-arrow:off,"
        "QSpinBox::up-arrow:disabled,"
        "QDoubleSpinBox::up-arrow:off,"
        "QDoubleSpinBox::up-arrow:disabled {"
        "   image: url(:/icons/icons/midtone/spinbox_add.png);"
        "}"
        "QSpinBox::down-arrow,"
        "QDoubleSpinBox::down-arrow {"
        "   width: 7px;"
        "   height: 7px;"
        "   padding: 2px;"
        "   image: url(:/icons/icons/"% icons_theme %"/spinbox_sub.png);"
        "}"
        "QSpinBox::down-arrow:off,"
        "QSpinBox::down-arrow:disabled,"
        "QDoubleSpinBox::down-arrow:off,"
        "QDoubleSpinBox::down-arrow:disabled {"
        "   image: url(:/icons/icons/midtone/spinbox_sub.png);"
        "}"
        // scroll bars
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
        "   min-height: 16px;"
        "   margin: 0px 5px 0px 5px;"
        "   background-color: "% style.at(SCROLLBAR_HANDLER) %";"
        "}"
        "QScrollBar::handle:vertical:hover {"
        "   margin: 0px 4px 0px 4px;"
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
        // slider
        "QSlider::groove:horizontal {"
        "   height: 2px;"
        "   border: 1px solid "% style.at(SLIDER_BAR_ACTIVE_BORDER) %";"
        "   background-color: "% style.at(SLIDER_BAR_ACTIVE) %";"
        "}"
        "QSlider::add-page:horizontal {"
        "   border: 0px;"
        "   background-color: "% style.at(SLIDER_BAR_INACTIVE) %";"
        "}"
        "QSlider::sub-page:horizontal {"
        "   border: 1px solid "% style.at(SLIDER_BAR_ACTIVE_BORDER) %";"
        "   background-color: "% style.at(SLIDER_BAR_ACTIVE) %";"
        "}"
        "QSlider::handle:horizontal {"
        "   width: 12px;"
        "   margin: -5px 0px;"
        "   border: 1px solid "% style.at(SLIDER_HANDLER_BORDER) %";"
        "   border-radius: 4px;"
        "   background-color: "% style.at(SLIDER_HANDLER) %";"
        "}"
        "QSlider::handle:hover:horizontal {"
        "   border: 1px solid "% style.at(SLIDER_HANDLER_BORDER_HOVER) %";"
        "}"
        // charts
        "QGraphicsView {"
        "   border-radius: 8px;"
        "   background-color: "% style.at(CHARTS_CONTOUR) %";"
        "}"
        // tables, trees and headers
        "QTreeView,"
        "QListView,"
        "QTableView {"
        "   border-radius: 8px;"
        "   background-color: "% style.at(TABLES_BASE) %";"
        "   selection-color: "% style.at(TABLES_TEXT_SELECTION) %";"
        "   selection-background-color: "% style.at(TABLES_BASE_SELECTION) %";"
        "}"
        "QTableView {"
        "   gridline-color: "% style.at(TABLES_GRIDLINE) %";"
        "}"
        "QTreeView QScrollBar::handle:vertical,"
        "QTableView QScrollBar::handle:vertical,"
        "QTreeView QScrollBar::handle:vertical:hover,"
        "QTableView QScrollBar::handle:vertical:hover {"
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
        "QHeaderView::section {"
        "   height: 24px;"
        "}"
        "QHeaderView::section:first {"
        "   padding-left: 6px;"
        "   border-top-left-radius: 6px;"
        "   border-right: 1px solid "% style.at(TABLES_HEADER_SEPARATOR) %";"
        "   border-bottom: 2px solid "% style.at(TABLES_DECO) %";"
        "   background-color: "% style.at(TABLES_HEADER) %";"
        "}"
        "QHeaderView::section:middle {"
        "   padding-left: 6px;"
        "   border-radius: 0px;" // leave this here or borders won't display
        "   border-right: 1px solid "% style.at(TABLES_HEADER_SEPARATOR) %";"
        "   border-bottom: 2px solid "% style.at(TABLES_DECO) %";"
        "   background-color: "% style.at(TABLES_HEADER) %";"
        "}"
        "QHeaderView::section:last {"
        "   padding-left: 6px;"
        "   border-top-right-radius: 0px;"
        "   border-bottom: 2px solid "% style.at(TABLES_DECO) %";"
        "   background-color: "% style.at(TABLES_HEADER) %";"
        "}"
        "QHeaderView::section:first::hover {"
        "   background-color: "% style.at(TABLES_HEADER_HOVER) %";"
        "}"
        "QHeaderView::section:middle::hover {"
        "   background-color: "% style.at(TABLES_HEADER_HOVER) %";"
        "}"
        "QHeaderView::section:last::hover {"
        "   background-color: "% style.at(TABLES_HEADER_HOVER) %";"
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
        //////////////
        //// MENU ////
        "QMenuBar {"
        "   color: "% style.at(MENU_TEXT) %";"
        "   background-color: "% style.at(MENU_BASE) %";"
        "}"
        "QMenuBar::item:selected {"
        "   color: "% style.at(MENU_TEXT) %";"
        "   background-color: "% style.at(MENU_BASE_HOVER) %";"
        "   border-radius: 4px;"
        "}"
        "QMenuBar::item:pressed {"
        "   color: "% style.at(MENU_DROPDOWN_TEXT) %";"
        "   background-color: "% style.at(MENU_DROPDOWN_BASE) %";"
        "   border-radius: 0px;"
        "   border-top-left-radius: 4px;"
        "   border-top-right-radius: 4px;"
        "}"
        // menu dropdown
        "QMenu {"
        "   border-top-right-radius: 4px;"
        "   border-bottom-right-radius: 4px;"
        "   border-bottom-left-radius: 4px;"
        "   color: "% style.at(MENU_DROPDOWN_TEXT) %";"
        "   background-color: "% style.at(MENU_DROPDOWN_BASE) %";"
        "}"
        "QMenu::item:selected {"
        "   background-color: "% style.at(MENU_DROPDOWN_BASE_SELECTION) %";"
        "}"
        //////////////
        //// MAIN ////
        // window
        "QWidget#mainwidget {"
        "   background-color: "% style.at(WINDOW_BASE_SECONDARY) %";"
        "}"
        // tabs buttons
        "QFrame#frame_Tabs_Buttons {"
        "   border: 0px;"
        "   background-color: "% style.at(WINDOW_BASE_SECONDARY) %";"
        "}"
        "QPushButton#button_Tab_Log,"
        "QPushButton#button_Tab_View,"
        "QPushButton#button_Tab_Conf {"
        "   border: 0px;"
        "   border-radius: 0px;"
        "   background-color: "% style.at(WINDOW_BASE_PRIMARY) %";"
        "}"
        "QPushButton#button_Tab_Log::flat,"
        "QPushButton#button_Tab_View::flat,"
        "QPushButton#button_Tab_Conf::flat {"
        "   background-color: "% style.at(WINDOW_BASE_SECONDARY) %";"
        "}"
        "QPushButton#button_Tab_Log::flat:hover {"
        "   background-color: QLinearGradient("
        "      x1:0, y1:0.5, x2:1, y2:0.5,"
        "      stop:0 "% style.at(SECTIONS_LOG_HOVER) %","
        "      stop:0.4 "% style.at(SECTIONS_LOG_HOVER) %","
        "      stop:1 "% style.at(WINDOW_BASE_SECONDARY) %");"
        "}"
        "QPushButton#button_Tab_View::flat:hover {"
        "   background-color: QLinearGradient("
        "      x1:0, y1:0.5, x2:1, y2:0.5,"
        "      stop:0 "% style.at(SECTIONS_VIEW_HOVER) %","
        "      stop:0.4 "% style.at(SECTIONS_VIEW_HOVER) %","
        "      stop:1 "% style.at(WINDOW_BASE_SECONDARY) %");"
        "}"
        "QPushButton#button_Tab_Conf::flat:hover {"
        "   background-color: QLinearGradient("
        "      x1:0, y1:0.5, x2:1, y2:0.5,"
        "      stop:0 "% style.at(SECTIONS_CONF_HOVER) %","
        "      stop:0.4 "% style.at(SECTIONS_CONF_HOVER) %","
        "      stop:1 "% style.at(WINDOW_BASE_SECONDARY) %");"
        "}"
        // tabs pages
        "QStackedWidget#stackedPages_Sections {"
        "   background-color: "% style.at(WINDOW_BASE_PRIMARY) %";"
        "}"
        //////////////
        //// LOGS ////
        // switcher buttons
        "QPushButton#button_Logs_Down {"
        "   border-radius: 16px;"
        "   background-color: "% style.at(WINDOW_BASE_PRIMARY) %";"
        "}"
        "QPushButton#button_Logs_Down:hover {"
        "   background-color: "% style.at(WINDOW_BASE_SECONDARY) %";"
        "   icon: url(:/icons/icons/"% icons_theme %"/down.png);"
        "}"
        "QPushButton#button_Logs_Up {"
        "   border-radius: 16px;"
        "   background-color: "% style.at(WINDOW_BASE_PRIMARY) %";"
        "}"
        "QPushButton#button_Logs_Up::hover {"
        "   background-color: "% style.at(WINDOW_BASE_SECONDARY) %";"
        "   icon: url(:/icons/icons/"% icons_theme %"/up.png);"
        "}"
        // web servers buttons
        "QPushButton#button_LogFiles_Apache,"
        "QPushButton#button_LogFiles_Nginx,"
        "QPushButton#button_LogFiles_Iis {"
        "   border-radius: 16px;"
        "   background-color: "% style.at(WEB_SERVERS_BUTTONS_BASE) %";"
        "}"
        "QPushButton#button_LogFiles_Apache::flat,"
        "QPushButton#button_LogFiles_Nginx::flat,"
        "QPushButton#button_LogFiles_Iis::flat {"
        "   border: 1px solid "% style.at(WEB_SERVERS_BUTTONS_BORDER_FLAT) %";"
        "   color: "% style.at(WEB_SERVERS_BUTTONS_TEXT_FLAT) %";"
        "   background-color: "% style.at(WEB_SERVERS_BUTTONS_BASE_FLAT) %";"
        "}"
        "QPushButton#button_LogFiles_Apache::flat:hover,"
        "QPushButton#button_LogFiles_Nginx::flat:hover,"
        "QPushButton#button_LogFiles_Iis::flat:hover {"
        "   border: 2px solid "% style.at(WEB_SERVERS_BUTTONS_BORDER_FLAT_HOVER) %";"
        "   color: "% style.at(WEB_SERVERS_BUTTONS_TEXT_FLAT_HOVER) %";"
        "}"
        "QPushButton#button_LogFiles_Apache::disabled,"
        "QPushButton#button_LogFiles_Nginx::disabled,"
        "QPushButton#button_LogFiles_Iis::disabled {"
        "   border: 0px;"
        "   color: "% style.at(WEB_SERVERS_BUTTONS_TEXT_DISABLED) %";"
        "   background-color: "% style.at(WEB_SERVERS_BUTTONS_BASE_DISABLED) %";"
        "}"
        // logs list
        "QTreeWidget#listLogFiles::indicator {"
        "   width: 13px;"
        "   height: 13px;"
        "   margin-left: -4px;"
        "   margin-right: 4px;"
        "}"
        "QTreeWidget#listLogFiles::indicator:unchecked {"
        "   image: url(:/icons/icons/"% icons_theme %"/checkbox_unchecked.png);"
        "}"
        "QTreeWidget#listLogFiles::indicator:checked {"
        "   image: url(:/icons/icons/"% icons_theme %"/checkbox_checked.png);"
        "}"
        // view file button
        "QPushButton#button_LogFiles_ViewFile {"
        "   border-radius: 4px;"
        "}"
        // refresh button
        "QPushButton#button_LogFiles_RefreshList {"
        "   border: 1px solid "% style.at(BORDER_PRIMARY) %";"
        "   border-radius: 16px;"
        "   background-color: transparent;"
        "}"
        "QPushButton#button_LogFiles_RefreshList:hover {"
        "   border: 1px solid "% style.at(BORDER_SECONDARY) %";"
        "}"
        // parse logs button
        "QPushButton#button_MakeStats_Start {"
        "   border-radius: 32px;"
        "}"
        "QPushButton#button_MakeStats_Start::disabled {"
        "   color: "% style.at(TEXT_DISABLED) %";"
        "}"
        ///////////////
        //// STATS ////
        // statistics tabs buttons
        "QFrame#frame_Stats_Buttons {"
        "   border: 0px;"
        "   background-color: "% style.at(WINDOW_BASE_PRIMARY) %";"
        "}"
        "QPushButton#button_Tab_StatsWarn,"
        "QPushButton#button_Tab_StatsSpeed,"
        "QPushButton#button_Tab_StatsCount,"
        "QPushButton#button_Tab_StatsDay,"
        "QPushButton#button_Tab_StatsRelat,"
        "QPushButton#button_Tab_StatsGlob {"
        "   border-radius: 22px;"
        "   background-color: "% style.at(STATS_SWITCH_BUTTONS) %";"
        "}"
        "QPushButton#button_Tab_StatsWarn::flat,"
        "QPushButton#button_Tab_StatsSpeed::flat,"
        "QPushButton#button_Tab_StatsCount::flat,"
        "QPushButton#button_Tab_StatsDay::flat,"
        "QPushButton#button_Tab_StatsRelat::flat,"
        "QPushButton#button_Tab_StatsGlob::flat {"
        "   background-color: "% style.at(STATS_SWITCH_BUTTONS_FLAT) %";"
        "}"
        "QPushButton#button_Tab_StatsWarn::flat:hover,"
        "QPushButton#button_Tab_StatsSpeed::flat:hover,"
        "QPushButton#button_Tab_StatsCount::flat:hover,"
        "QPushButton#button_Tab_StatsDay::flat:hover,"
        "QPushButton#button_Tab_StatsRelat::flat:hover,"
        "QPushButton#button_Tab_StatsGlob::flat:hover {"
        "   background-color: "% style.at(STATS_SWITCH_BUTTONS_FLAT_HOVER) %";"
        "}"
        // stacked pages
        "QStackedWidget#stackedPages_Stats {"
        "   border-top: 2px solid "% style.at(STATS_SWITCH_BUTTONS) %";"
        "   border-radius: 12px;"
        "}"
        // stats warn
        "QTableWidget#table_StatsWarn::indicator {"
        "   width: 13px;"
        "   height: 13px;"
        "}"
        "QTableWidget#table_StatsWarn::indicator:unchecked {"
        "   image: url(:/icons/icons/"% icons_theme %"/checkbox_unchecked.png);"
        "}"
        "QTableWidget#table_StatsWarn::indicator:checked {"
        "   image: url(:/icons/icons/"% icons_theme %"/checkbox_checked.png);"
        "}"
        "QTableWidget#table_StatsWarn QScrollBar::handle:vertical {"
        "   padding: 12px;"
        "}"
        // stats count
        "QScrollArea#scrollArea_StatsCount {"
        "   background-color: "% style.at(WINDOW_BASE_PRIMARY) %";"
        "}"
        "QWidget#scrollAreaContent_StatsCount {"
        "   background-color: transparent;"
        "}"
        "QPushButton#button_StatsCount_Protocol,"
        "QPushButton#button_StatsCount_Method,"
        "QPushButton#button_StatsCount_Uri,"
        "QPushButton#button_StatsCount_Query,"
        "QPushButton#button_StatsCount_Response,"
        "QPushButton#button_StatsCount_Referrer,"
        "QPushButton#button_StatsCount_Cookie,"
        "QPushButton#button_StatsCount_UserAgent,"
        "QPushButton#button_StatsCount_Client {"
        "   border-radius: 8px;"
        "   background-color: "% style.at(BUTTONS_BASE_DISABLED) %";"
        "}"
        "QPushButton#button_StatsCount_Protocol::flat,"
        "QPushButton#button_StatsCount_Method::flat,"
        "QPushButton#button_StatsCount_Uri::flat,"
        "QPushButton#button_StatsCount_Query::flat,"
        "QPushButton#button_StatsCount_Response::flat,"
        "QPushButton#button_StatsCount_Referrer::flat,"
        "QPushButton#button_StatsCount_Cookie::flat,"
        "QPushButton#button_StatsCount_UserAgent::flat,"
        "QPushButton#button_StatsCount_Client::flat {"
        "   background-color: "% style.at(WINDOW_BASE_SECONDARY) %";"
        "}"
        "QPushButton#button_StatsCount_Protocol::flat:hover,"
        "QPushButton#button_StatsCount_Method::flat:hover,"
        "QPushButton#button_StatsCount_Uri::flat:hover,"
        "QPushButton#button_StatsCount_Query::flat:hover,"
        "QPushButton#button_StatsCount_Response::flat:hover,"
        "QPushButton#button_StatsCount_Referrer::flat:hover,"
        "QPushButton#button_StatsCount_Cookie::flat:hover,"
        "QPushButton#button_StatsCount_UserAgent::flat:hover,"
        "QPushButton#button_StatsCount_Client::flat:hover {"
        "   background-color: "% style.at(BUTTONS_BASE) %";"
        "}"
        // stats globals
        "QPushButton#button_StatsGlob_Apache,"
        "QPushButton#button_StatsGlob_Nginx,"
        "QPushButton#button_StatsGlob_Iis {"
        "   border-radius: 16px;"
        "   background-color: "% style.at(WEB_SERVERS_BUTTONS_BASE) %";"
        "}"
        "QPushButton#button_StatsGlob_Apache::flat,"
        "QPushButton#button_StatsGlob_Nginx::flat,"
        "QPushButton#button_StatsGlob_Iis::flat {"
        "   border: 1px solid "% style.at(WEB_SERVERS_BUTTONS_BORDER_FLAT) %";"
        "   color: "% style.at(WEB_SERVERS_BUTTONS_TEXT_FLAT) %";"
        "   background-color: "% style.at(WEB_SERVERS_BUTTONS_BASE_FLAT) %";"
        "}"
        "QPushButton#button_StatsGlob_Apache::flat:hover,"
        "QPushButton#button_StatsGlob_Nginx::flat:hover,"
        "QPushButton#button_StatsGlob_Iis::flat:hover {"
        "   border: 2px solid "% style.at(WEB_SERVERS_BUTTONS_BORDER_FLAT_HOVER) %";"
        "   color: "% style.at(WEB_SERVERS_BUTTONS_TEXT_FLAT_HOVER) %";"
        "}"
        "QPushButton#button_StatsGlob_Apache::disabled,"
        "QPushButton#button_StatsGlob_Nginx::disabled,"
        "QPushButton#button_StatsGlob_Iis::disabled {"
        "   border: 0px;"
        "   color: "% style.at(WEB_SERVERS_BUTTONS_TEXT_DISABLED) %";"
        "   background-color: "% style.at(WEB_SERVERS_BUTTONS_BASE_DISABLED) %";"
        "}"
        "QScrollArea#scrollArea_StatsGlob_Recur,"
        "QScrollArea#scrollArea_StatsGlob_Perf,"
        "QScrollArea#scrollArea_StatsGlob_Traffic,"
        "QScrollArea#scrollArea_StatsGlob_Work {"
        "   border-radius: 8px;"
        "   background-color: "% style.at(STATS_GLOBALS_FRAMES) %";"
        "}"
        // styled frames
        "QFrame#frame_StatsWarn_Logs,"
        "QFrame#frame_StatsDay_Date,"
        "QFrame#frame_StatsRelat_Field {"
        "   border: 1px solid "% style.at(BORDER_TERTIARY) %";"
        "}"
        // lines separators
        "QFrame#gline_StatsWarn_1,"
        "QFrame#gline_StatsWarn_2,"
        "QFrame#gline_StatsWarn_3,"
        "QFrame#gline_StatsDay_1,"
        "QFrame#gline_StatsDay_2,"
        "QFrame#gline_StatsRelat_1 {"
        "   border: 1px solid "% style.at(SEPARATORS_BORDER) %";"
        "   background-color: "% style.at(SEPARATORS_BASE) %";"
        "}"
        "QFrame#gline_StatsSpeed_1,"
        "QFrame#gline_StatsCount_1 {"
        "   border: 1px solid "% style.at(SEPARATORS_BORDER) %";"
        "   margin: 2px 0px 3px 0px;"
        "   background-color: "% style.at(SEPARATORS_BASE) %";"
        "}"
        "QFrame#gline_StatsGlob_Recur_1,"
        "QFrame#gline_StatsGlob_Recur_2,"
        "QFrame#gline_StatsGlob_Recur_3,"
        "QFrame#gline_StatsGlob_Perf_1,"
        "QFrame#gline_StatsGlob_Perf_2,"
        "QFrame#gline_StatsGlob_Traffic_1,"
        "QFrame#gline_StatsGlob_Traffic_2,"
        "QFrame#gline_StatsGlob_Work_1,"
        "QFrame#gline_StatsGlob_Work_2 {"
        "   border: 1px solid "% style.at(STATS_GLOBALS_FRAMES) %";"
        "   background-color: "% style.at(SEPARATORS_BASE) %";"
        "}"
        /////////////////
        //// CONFIGS ////
        // sections
        "QTreeView#tree_ConfSections {"
        "   background-color: "% style.at(CONFIGS_TREE_BASE) %";"
        "}"
        "QTreeView#tree_ConfSections QScrollBar::sub-line:vertical {"
        "   margin-top: -12px;"
        "   border: 0px;"
        "   border-top-right-radius: 8px;"
        "   background-color: transparent;"
        "}"
        "QTreeView#tree_ConfSections QScrollBar::handle:vertical,"
        "QTreeView#tree_ConfSections QScrollBar::handle:vertical:hover {"
        "   margin-top: 0px;"
        "}"
        // line separators
        "QFrame#gline_ConfTextBrowser {"
        "   border: 2px solid "% style.at(SEPARATORS_BORDER) %";"
        "   margin: 13px 0px 14px 0px;"
        "   background-color: "% style.at(SEPARATORS_BASE) %";"
        "}"
        // save buttons
        "QPushButton#button_ConfDatabases_Data_Save,"
        "QPushButton#button_ConfDatabases_Hashes_Save,"
        "QPushButton#button_ConfApache_Path_Save,"
        "QPushButton#button_ConfApache_Format_Save,"
        "QPushButton#button_ConfNginx_Path_Save,"
        "QPushButton#button_ConfNginx_Format_Save,"
        "QPushButton#button_ConfIis_Path_Save,"
        "QPushButton#button_ConfIis_Format_Save {"
        "   border-radius: 4px;"
        "   background-color: "% style.at(BUTTONS_BASE) %";"
        "}"
        "QPushButton#button_ConfDatabases_Data_Save:hover,"
        "QPushButton#button_ConfDatabases_Hashes_Save:hover,"
        "QPushButton#button_ConfApache_Path_Save:hover,"
        "QPushButton#button_ConfApache_Format_Save:hover,"
        "QPushButton#button_ConfNginx_Path_Save:hover,"
        "QPushButton#button_ConfNginx_Format_Save:hover,"
        "QPushButton#button_ConfIis_Path_Save:hover,"
        "QPushButton#button_ConfIis_Format_Save:hover {"
        "   background-color: "% style.at(BUTTONS_BASE_HOVER) %";"
        "}"
        "QPushButton#button_ConfDatabases_Data_Save::disabled,"
        "QPushButton#button_ConfDatabases_Hashes_Save::disabled,"
        "QPushButton#button_ConfApache_Path_Save::disabled,"
        "QPushButton#button_ConfApache_Path_Save::disabled,"
        "QPushButton#button_ConfNginx_Path_Save::disabled,"
        "QPushButton#button_ConfNginx_Format_Save::disabled,"
        "QPushButton#button_ConfIis_Path_Save::disabled,"
        "QPushButton#button_ConfIis_Format_Save::disabled {"
        "   background-color: "% style.at(BUTTONS_BASE_DISABLED) %";"
        "   icon: url(:/icons/icons/midtone/save.png);"
        "}"
        // sample buttons
        "QPushButton#button_ConfApache_Format_Sample,"
        "QPushButton#button_ConfNginx_Format_Sample,"
        "QPushButton#button_ConfIis_Format_Sample {"
        "   border-radius: 8px;"
        "   background-color: "% style.at(BUTTONS_BASE) %";"
        "}"
        "QPushButton#button_ConfApache_Format_Sample:hover,"
        "QPushButton#button_ConfNginx_Format_Sample:hover,"
        "QPushButton#button_ConfIis_Format_Sample:hover {"
        "   background-color: "% style.at(BUTTONS_BASE_HOVER) %";"
        "}"
        // sample previews
        "QScrollArea#scrollArea_ConfApache_Format_Sample,"
        "QScrollArea#scrollArea_ConfNginx_Format_Sample,"
        "QScrollArea#scrollArea_ConfIis_Format_Sample {"
        "   border: 1px solid "% style.at(BORDER_SECONDARY) %";"
        "   border-top-left-radius: 4px;"
        "   border-top-right-radius: 4px;"
        "   background-color: "% style.at(TEXTBROWSER_BASE) %";"
        "}"
        // help buttons
        "QPushButton#button_ConfApache_Format_Help,"
        "QPushButton#button_ConfNginx_Format_Help,"
        "QPushButton#button_ConfIis_Format_Help {"
        "   border-radius: 8px;"
        "   background-color: "% style.at(HELP_BUTTONS_BASE) %";"
        "}"
        "QPushButton#button_ConfApache_Format_Help:hover,"
        "QPushButton#button_ConfNginx_Format_Help:hover,"
        "QPushButton#button_ConfIis_Format_Help:hover {"
        "   background-color: "% style.at(HELP_BUTTONS_BASE_HOVER) %";"
        "}"
        // warnlists / blacklists
        "QListWidget#list_ConfApache_Warnlist_List,"
        "QListWidget#list_ConfApache_Blacklist_List,"
        "QListWidget#list_ConfNginx_Warnlist_List,"
        "QListWidget#list_ConfNginx_Blacklist_List,"
        "QListWidget#list_ConfIis_Warnlist_List,"
        "QListWidget#list_ConfIis_Blacklist_List {"
        "   padding: 4px;"
        "   border: 1px solid "% style.at(BORDER_PRIMARY) %";"
        "}"
        "QListWidget#list_ConfApache_Warnlist_List::disabled,"
        "QListWidget#list_ConfApache_Blacklist_List::disabled,"
        "QListWidget#list_ConfNginx_Warnlist_List::disabled,"
        "QListWidget#list_ConfNginx_Blacklist_List::disabled,"
        "QListWidget#list_ConfIis_Warnlist_List::disabled,"
        "QListWidget#list_ConfIis_Blacklist_List::disabled {"
        "   border: 0px;"
        "   color: "% style.at(TEXT_DISABLED) %";"
        "   background-color: "% style.at(TABLES_BASE_DISABLED) %";"
        "   selection-background-color: "% style.at(TABLES_BASE_DISABLED) %";"
        "}"
        "QListWidget#list_ConfApache_Warnlist_List::item,"
        "QListWidget#list_ConfApache_Blacklist_List::item,"
        "QListWidget#list_ConfNginx_Warnlist_List::item,"
        "QListWidget#list_ConfNginx_Blacklist_List::item,"
        "QListWidget#list_ConfIis_Warnlist_List::item,"
        "QListWidget#list_ConfIis_Blacklist_List::item {"
        "   padding: 4px;"
        "}"
        // lists buttons
        "QPushButton#button_ConfApache_Warnlist_Add,"
        "QPushButton#button_ConfApache_Warnlist_Remove,"
        "QPushButton#button_ConfApache_Warnlist_Up,"
        "QPushButton#button_ConfApache_Warnlist_Down,"
        "QPushButton#button_ConfApache_Blacklist_Add,"
        "QPushButton#button_ConfApache_Blacklist_Remove,"
        "QPushButton#button_ConfApache_Blacklist_Up,"
        "QPushButton#button_ConfApache_Blacklist_Down,"
        "QPushButton#button_ConfNginx_Warnlist_Add,"
        "QPushButton#button_ConfNginx_Warnlist_Remove,"
        "QPushButton#button_ConfNginx_Warnlist_Up,"
        "QPushButton#button_ConfNginx_Warnlist_Down,"
        "QPushButton#button_ConfNginx_Blacklist_Add,"
        "QPushButton#button_ConfNginx_Blacklist_Remove,"
        "QPushButton#button_ConfNginx_Blacklist_Up,"
        "QPushButton#button_ConfNginx_Blacklist_Down,"
        "QPushButton#button_ConfIis_Warnlist_Add,"
        "QPushButton#button_ConfIis_Warnlist_Remove,"
        "QPushButton#button_ConfIis_Warnlist_Up,"
        "QPushButton#button_ConfIis_Warnlist_Down,"
        "QPushButton#button_ConfIis_Blacklist_Add,"
        "QPushButton#button_ConfIis_Blacklist_Remove,"
        "QPushButton#button_ConfIis_Blacklist_Up,"
        "QPushButton#button_ConfIis_Blacklist_Down {"
        "   border-radius: 4px;"
        "   background-color: "% style.at(BWLISTS_BUTTONS_BASE) %";"
        "}"
        "QPushButton#button_ConfApache_Warnlist_Add:hover,"
        "QPushButton#button_ConfApache_Warnlist_Remove:hover,"
        "QPushButton#button_ConfApache_Warnlist_Up:hover,"
        "QPushButton#button_ConfApache_Warnlist_Down:hover,"
        "QPushButton#button_ConfApache_Blacklist_Add:hover,"
        "QPushButton#button_ConfApache_Blacklist_Remove:hover,"
        "QPushButton#button_ConfApache_Blacklist_Up:hover,"
        "QPushButton#button_ConfApache_Blacklist_Down:hover,"
        "QPushButton#button_ConfNginx_Warnlist_Add:hover,"
        "QPushButton#button_ConfNginx_Warnlist_Remove:hover,"
        "QPushButton#button_ConfNginx_Warnlist_Up:hover,"
        "QPushButton#button_ConfNginx_Warnlist_Down:hover,"
        "QPushButton#button_ConfNginx_Blacklist_Add:hover,"
        "QPushButton#button_ConfNginx_Blacklist_Remove:hover,"
        "QPushButton#button_ConfNginx_Blacklist_Up:hover,"
        "QPushButton#button_ConfNginx_Blacklist_Down:hover,"
        "QPushButton#button_ConfIis_Warnlist_Add:hover,"
        "QPushButton#button_ConfIis_Warnlist_Remove:hover,"
        "QPushButton#button_ConfIis_Warnlist_Up:hover,"
        "QPushButton#button_ConfIis_Warnlist_Down:hover,"
        "QPushButton#button_ConfIis_Blacklist_Add:hover,"
        "QPushButton#button_ConfIis_Blacklist_Remove:hover,"
        "QPushButton#button_ConfIis_Blacklist_Up:hover,"
        "QPushButton#button_ConfIis_Blacklist_Down:hover {"
        "   background-color: "% style.at(BWLISTS_BUTTONS_BASE_HOVER) %";"
        "}"
        "QPushButton#button_ConfApache_Warnlist_Add::disabled,"
        "QPushButton#button_ConfApache_Blacklist_Add::disabled,"
        "QPushButton#button_ConfNginx_Warnlist_Add::disabled,"
        "QPushButton#button_ConfNginx_Blacklist_Add::disabled,"
        "QPushButton#button_ConfIis_Warnlist_Add::disabled,"
        "QPushButton#button_ConfIis_Blacklist_Add::disabled {"
        "   background-color: "% style.at(BWLISTS_BUTTONS_BASE_DISABLED) %";"
        "   icon: url(:/icons/icons/"% icons_theme %"/list_add.png), #fff;"
        "}"
        "QPushButton#button_ConfApache_Warnlist_Remove::disabled,"
        "QPushButton#button_ConfApache_Blacklist_Remove::disabled,"
        "QPushButton#button_ConfNginx_Warnlist_Remove::disabled,"
        "QPushButton#button_ConfNginx_Blacklist_Remove::disabled,"
        "QPushButton#button_ConfIis_Warnlist_Remove::disabled,"
        "QPushButton#button_ConfIis_Blacklist_Remove::disabled {"
        "   background-color: "% style.at(BWLISTS_BUTTONS_BASE_DISABLED) %";"
        "   icon: url(:/icons/icons/"% icons_theme %"/list_rem.png), #fff;"
        "}"
        "QPushButton#button_ConfApache_Warnlist_Up::disabled,"
        "QPushButton#button_ConfApache_Blacklist_Up::disabled,"
        "QPushButton#button_ConfNginx_Warnlist_Up::disabled,"
        "QPushButton#button_ConfNginx_Blacklist_Up::disabled,"
        "QPushButton#button_ConfIis_Warnlist_Up::disabled,"
        "QPushButton#button_ConfIis_Blacklist_Up::disabled {"
        "   background-color: "% style.at(BWLISTS_BUTTONS_BASE_DISABLED) %";"
        "   icon: url(:/icons/icons/"% icons_theme %"/list_up.png), #fff;"
        "}"
        "QPushButton#button_ConfApache_Warnlist_Down::disabled,"
        "QPushButton#button_ConfApache_Blacklist_Down::disabled,"
        "QPushButton#button_ConfNginx_Warnlist_Down::disabled,"
        "QPushButton#button_ConfNginx_Blacklist_Down::disabled,"
        "QPushButton#button_ConfIis_Warnlist_Down::disabled,"
        "QPushButton#button_ConfIis_Blacklist_Down::disabled {"
        "   background-color: "% style.at(BWLISTS_BUTTONS_BASE_DISABLED) %";"
        "   icon: url(:/icons/icons/"% icons_theme %"/list_down.png), #fff;"
        "}"
        "QPushButton#button_ConfApache_Warnlist_Add::flat,"
        "QPushButton#button_ConfApache_Warnlist_Remove::flat,"
        "QPushButton#button_ConfApache_Warnlist_Up::flat,"
        "QPushButton#button_ConfApache_Warnlist_Down::flat,"
        "QPushButton#button_ConfApache_Blacklist_Add::flat,"
        "QPushButton#button_ConfApache_Blacklist_Remove::flat,"
        "QPushButton#button_ConfApache_Blacklist_Up::flat,"
        "QPushButton#button_ConfApache_Blacklist_Down::flat,"
        "QPushButton#button_ConfNginx_Warnlist_Add::flat,"
        "QPushButton#button_ConfNginx_Warnlist_Remove::flat,"
        "QPushButton#button_ConfNginx_Warnlist_Up::flat,"
        "QPushButton#button_ConfNginx_Warnlist_Down::flat,"
        "QPushButton#button_ConfNginx_Blacklist_Add::flat,"
        "QPushButton#button_ConfNginx_Blacklist_Remove::flat,"
        "QPushButton#button_ConfNginx_Blacklist_Up::flat,"
        "QPushButton#button_ConfNginx_Blacklist_Down::flat,"
        "QPushButton#button_ConfIis_Warnlist_Add::flat,"
        "QPushButton#button_ConfIis_Warnlist_Remove::flat,"
        "QPushButton#button_ConfIis_Warnlist_Up::flat,"
        "QPushButton#button_ConfIis_Warnlist_Down::flat,"
        "QPushButton#button_ConfIis_Blacklist_Add::flat,"
        "QPushButton#button_ConfIis_Blacklist_Remove::flat,"
        "QPushButton#button_ConfIis_Blacklist_Up::flat,"
        "QPushButton#button_ConfIis_Blacklist_Down::flat {"
        "   background-color: "% style.at(BWLISTS_BUTTONS_BASE_FLAT) %";"
        "   icon: none;"
        "}";
}

} // namespace StyleSec
