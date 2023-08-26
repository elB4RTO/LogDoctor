
#include "stylesheets.h"

#include "modules/exceptions.h"

#include <QString>


namespace StyleSec
{

namespace /*private*/
{

//! Builds the final stylesheet upon the generic model
/*!
    \param ss Will hold the stylesheet
    \param style The key-value pairs to be applied at the generic
    \param theme The theme selected for the Icons
*/
void makeStyleSheet( QString& ss, const std::unordered_map<std::string, QString> &style, const QString& theme )
{
    ss= ////////////////
        //// SHARED ////
        "* {"
        "   color: "+style.at("text_primary")+";"
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
        "   border: 1px solid "+style.at("tooltips_border")+";"
        "   border-radius: 6px;"
        "   color: "+style.at("tooltips_text")+";"
        "   background-color: "+style.at("tooltips_base")+";"
        "   font-size: 12px;"
        "}"
        // check boxes
        "QCheckBox::indicator {"
        "   width: 13px;"
        "   height: 13px;"
        "}"
        "QCheckBox::indicator:unchecked {"
        "   image: url(:/icons/icons/"+theme+"/checkbox_unchecked.png);"
        "}"
        "QCheckBox::indicator:indeterminate {"
        "   image: url(:/icons/icons/"+theme+"/checkbox_semichecked.png);"
        "}"
        "QCheckBox::indicator:checked {"
        "   image: url(:/icons/icons/"+theme+"/checkbox_checked.png);"
        "}"
        // radio buttons
        "QRadioButton::indicator {"
        "   width: 13px;"
        "   height : 13px;"
        "}"
        "QRadioButton::indicator:unchecked {"
        "   image: url(:/icons/icons/"+theme+"/radiobutton_unchecked.png);"
        "}"
        "QRadioButton::indicator:checked {"
        "   image: url(:/icons/icons/"+theme+"/radiobutton_checked.png);"
        "}"
        // push buttons
        "QPushButton {"
        "   border: 0px;"
        "   border-radius: 12px;"
        "   background-color: "+style.at("buttons_base")+";"
        "}"
        "QPushButton:hover {"
        "   background-color: "+style.at("buttons_base_hover")+";"
        "}"
        "QPushButton::flat {"
        "   background-color: "+style.at("buttons_base_flat")+";"
        "}"
        "QPushButton::disabled {"
        "   background-color: "+style.at("buttons_base_disabled")+";"
        "}"
        // line edits
        "QLineEdit {"
        "   padding-left: 4px;"
        "   padding-right: 4px;"
        "   border-radius: 8px;"
        "   color: "+style.at("linedit_text")+";"
        "   selection-color: "+style.at("linedit_text")+";"
        "   background-color: "+style.at("linedit_base")+";"
        "   selection-background-color: "+style.at("linedit_base_selection")+";"
        "}"
        "QLineEdit::focus {"
        "   background-color: "+style.at("linedit_base_focus")+";"
        "}"
        "QLineEdit:disabled {"
        "   background-color: "+style.at("linedit_base_disabled")+";"
        "}"
        // combo boxes
        "QComboBox {"
        "   border-radius: 8px;"
        "   background-color: "+style.at("boxes_base_primary")+";"
        "}"
        "QComboBox::disabled {"
        "   background-color: "+style.at("boxes_base_disabled")+";"
        "}"
        "QComboBox::drop-down {"
        "   border-top-right-radius: 8px;"
        "   border-bottom-right-radius: 8px;"
        "   border-left: 3px solid"
        "                QLinearGradient("
        "                   x0:0, y0:0, x1:0, y1:1,"
        "                   stop:0 "+style.at("boxes_base_primary")+","
        "                   stop:0.1 "+style.at("boxes_base_primary")+","
        "                   stop:0.5 "+style.at("boxes_deco")+","
        "                   stop:0.9 "+style.at("boxes_base_primary")+","
        "                   stop:1 "+style.at("boxes_base_primary")+");"
        "   background-color: "+style.at("boxes_base_primary")+";"
        "   selection-background-color: "+style.at("boxes_base_primary")+";"
        "}"
        "QComboBox::drop-down::disabled {"
        "   border-left-color: "+style.at("boxes_base_disabled")+";"
        "   background-color: "+style.at("boxes_base_disabled")+";"
        "}"
        "QComboBox::down-arrow {"
        "   image: url(:/icons/icons/"+theme+"/combobox_arrow.png);"
        "}"
        "QComboBox::down-arrow::disabled {"
        "   image: url();"
        "}"
        "QComboBox:on {"
        "   border-bottom-left-radius: 0px;"
        "   border-bottom: 2px solid "+style.at("boxes_deco")+";"
        "   background-color: "+style.at("boxes_base_secondary")+";"
        "}"
        "QComboBox::drop-down:on {"
        "   border-bottom-right-radius: 0px;"
        "   border-left-color: transparent;"
        "   background-color: "+style.at("boxes_base_secondary")+";"
        "}"
        "QComboBox::down-arrow:on {"
        "   image: url();"
        "}"
        "QComboBox QAbstractItemView {"
        "   border-bottom-left-radius: 8px;"
        "   border-bottom-right-radius: 8px;"
        "   background-color: "+style.at("boxes_base_primary")+";"
        "   selection-background-color: "+style.at("boxes_base_selection")+";"
        "}"
        // spin boxes
        "QSpinBox {"
        "   border-radius: 4px;"
        "   color: "+style.at("linedit_text")+";"
        "   background-color: "+style.at("linedit_base")+";"
        "   selection-color: "+style.at("linedit_text")+";"
        "   selection-background-color: "+style.at("linedit_base_selection")+";"
        "}"
        "QSpinBox::focus {"
        "   background-color: "+style.at("linedit_base_focus")+";"
        "}"
        "QSpinBox::disabled {"
        "   border-color: "+style.at("boxes_base_disabled")+";"
        "   color: "+style.at("text_disabled")+";"
        "   background-color: "+style.at("boxes_base_disabled")+";"
        "}"
        "QSpinBox::up-button,"
        "QSpinBox::down-button {"
        "   padding: 2px;"
        "   border-left: 2px solid "+style.at("boxes_deco")+";"
        "   background-color: "+style.at("boxes_base_secondary")+";"
        "}"
        "QSpinBox::up-button:hover,"
        "QSpinBox::down-button:hover {"
        "   background-color: "+style.at("boxes_base_selection")+";"
        "}"
        "QSpinBox::up-button:off,"
        "QSpinBox::down-button:off {"
        "   background-color: "+style.at("boxes_base_disabled")+";"
        "}"
        "QSpinBox::up-button:disabled,"
        "QSpinBox::down-button:disabled {"
        "   border-left-color: "+style.at("boxes_base_primary")+";"
        "   background-color: "+style.at("boxes_base_disabled")+";"
        "}"
        "QSpinBox::up-arrow {"
        "   width: 7px;"
        "   height: 7px;"
        "   padding: 2px;"
        "   image: url(:/icons/icons/"+theme+"/spinbox_add.png);"
        "}"
        "QSpinBox::up-arrow:off,"
        "QSpinBox::up-arrow:disabled {"
        "   image: url(:/icons/icons/midtone/spinbox_add.png);"
        "}"
        "QSpinBox::down-arrow {"
        "   width: 7px;"
        "   height: 7px;"
        "   padding: 2px;"
        "   image: url(:/icons/icons/"+theme+"/spinbox_sub.png);"
        "}"
        "QSpinBox::down-arrow:off,"
        "QSpinBox::down-arrow:disabled {"
        "   image: url(:/icons/icons/midtone/spinbox_sub.png);"
        "}"
        // scroll bars
        "QScrollBar:horizontal {"
        "   height: 12px;"
        "   background-color: "+style.at("scrollbar_base")+";"
        "}"
        "QScrollBar::handle:horizontal {"
        "   min-width: 16px;"
        "   margin: 5px 0px 5px 0px;"
        "   background-color: "+style.at("scrollbar_handler")+";"
        "}"
        "QScrollBar::handle:horizontal:hover {"
        "   margin: 4px 0px 4px 0px;"
        "   border-radius: 2px;"
        "}"
        "QScrollBar:vertical {"
        "   width: 12px;"
        "   background-color: "+style.at("scrollbar_base")+";"
        "}"
        "QScrollBar::handle:vertical {"
        "   min-height: 16px;"
        "   margin: 0px 5px 0px 5px;"
        "   background-color: "+style.at("scrollbar_handler")+";"
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
        "   background-color: "+style.at("scrollbar_controls")+";"
        "}"
        // slider
        "QSlider::groove:horizontal {"
        "   height: 2px;"
        "   border: 1px solid "+style.at("slider_bar_active_border")+";"
        "   background-color: "+style.at("slider_bar_active")+";"
        "}"
        "QSlider::add-page:horizontal {"
        "   border: 0px;"
        "   background-color: "+style.at("slider_bar_inactive")+";"
        "}"
        "QSlider::sub-page:horizontal {"
        "   border: 1px solid "+style.at("slider_bar_active_border")+";"
        "   background-color: "+style.at("slider_bar_active")+";"
        "}"
        "QSlider::handle:horizontal {"
        "   width: 12px;"
        "   margin: -5px 0px;"
        "   border: 1px solid "+style.at("slider_handler_border")+";"
        "   border-radius: 4px;"
        "   background-color: "+style.at("slider_handler")+";"
        "}"
        "QSlider::handle:hover:horizontal {"
        "   border: 1px solid "+style.at("slider_handler_border_hover")+";"
        "}"
        // charts
        "QGraphicsView {"
        "   border-radius: 8px;"
        "   background-color: "+style.at("charts_contour")+";"
        "}"
        // tables, trees and headers
        "QTreeView,"
        "QListView,"
        "QTableView {"
        "   border-radius: 8px;"
        "   background-color: "+style.at("tables_base")+";"
        "   selection-color: "+style.at("tables_text_selection")+";"
        "   selection-background-color: "+style.at("tables_base_selection")+";"
        "}"
        "QTableView {"
        "   gridline-color: "+style.at("tables_gridline")+";"
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
        "   height: 29px;"
        "   border: 0px;"
        "   border-top-right-radius: 8px;"
        "   border-bottom: 2px solid "+style.at("tables_deco")+";"
        "   background-color: "+style.at("tables_header")+";"
        "}"
        "QHeaderView::section:first {"
        "   padding-left: 6px;"
        "   border-top-left-radius: 6px;"
        "   border-right: 1px solid "+style.at("tables_header_separator")+";"
        "   border-bottom: 2px solid "+style.at("tables_deco")+";"
        "   background-color: "+style.at("tables_header")+";"
        "}"
        "QHeaderView::section:middle {"
        "   padding-left: 6px;"
        "   border-radius: 0px;" // leave this here or borders won't display
        "   border-right: 1px solid "+style.at("tables_header_separator")+";"
        "   border-bottom: 2px solid "+style.at("tables_deco")+";"
        "   background-color: "+style.at("tables_header")+";"
        "}"
        "QHeaderView::section:last {"
        "   padding-left: 6px;"
        "   border-top-right-radius: 0px;"
        "   border-bottom: 2px solid "+style.at("tables_deco")+";"
        "   background-color: "+style.at("tables_header")+";"
        "}"
        "QHeaderView::section:first::hover {"
        "   background-color: "+style.at("tables_header_hover")+";"
        "}"
        "QHeaderView::section:middle::hover {"
        "   background-color: "+style.at("tables_header_hover")+";"
        "}"
        "QHeaderView::section:last::hover {"
        "   background-color: "+style.at("tables_header_hover")+";"
        "}"
        // text browser
        "QTextBrowser {"
        "   border: 4px solid "+style.at("textbrowser_border")+";"
        "   border-radius: 8px;"
        "   color: "+style.at("textbrowser_text")+";"
        "   background-color: "+style.at("textbrowser_base")+";"
        "   selection-color: "+style.at("textbrowser_text_selection")+";"
        "   selection-background-color: "+style.at("textbrowser_base_selection")+";"
        "}"
        "QTextBrowser QScrollBar::handle:vertical {"
        "   padding: 12px;"
        "}"
        //////////////
        //// MENU ////
        "QMenuBar {"
        "   color: "+style.at("menu_text")+";"
        "   background-color: "+style.at("menu_base")+";"
        "}"
        "QMenuBar::item:selected {"
        "   color: "+style.at("menu_text")+";"
        "   background-color: "+style.at("menu_base_hover")+";"
        "   border-radius: 4px;"
        "}"
        "QMenuBar::item:pressed {"
        "   color: "+style.at("menu_dropdown_text")+";"
        "   background-color: "+style.at("menu_dropdown_base")+";"
        "   border-radius: 0px;"
        "   border-top-left-radius: 4px;"
        "   border-top-right-radius: 4px;"
        "}"
        // menu dropdown
        "QMenu {"
        "   border-top-right-radius: 4px;"
        "   border-bottom-right-radius: 4px;"
        "   border-bottom-left-radius: 4px;"
        "   color: "+style.at("menu_dropdown_text")+";"
        "   background-color: "+style.at("menu_dropdown_base")+";"
        "}"
        "QMenu::item:selected {"
        "   background-color: "+style.at("menu_dropdown_base_selection")+";"
        "}"
        //////////////
        //// MAIN ////
        // window
        "QWidget#mainwidget {"
        "   background-color: "+style.at("window_base_secondary")+";"
        "}"
        // tabs buttons
        "QFrame#frame_Tabs_Buttons {"
        "   border: 0px;"
        "   background-color: "+style.at("window_base_secondary")+";"
        "}"
        "QPushButton#button_Tab_Log,"
        "QPushButton#button_Tab_View,"
        "QPushButton#button_Tab_Conf {"
        "   border: 0px;"
        "   border-radius: 0px;"
        "   background-color: "+style.at("window_base_primary")+";"
        "}"
        "QPushButton#button_Tab_Log::flat,"
        "QPushButton#button_Tab_View::flat,"
        "QPushButton#button_Tab_Conf::flat {"
        "   background-color: "+style.at("window_base_secondary")+";"
        "}"
        "QPushButton#button_Tab_Log::flat:hover {"
        "   background-color: QLinearGradient("
        "      x1:0, y1:0.5, x2:1, y2:0.5,"
        "      stop:0 "+style.at("sections_log_hover")+","
        "      stop:0.4 "+style.at("sections_log_hover")+","
        "      stop:1 "+style.at("window_base_secondary")+");"
        "}"
        "QPushButton#button_Tab_View::flat:hover {"
        "   background-color: QLinearGradient("
        "      x1:0, y1:0.5, x2:1, y2:0.5,"
        "      stop:0 "+style.at("sections_view_hover")+","
        "      stop:0.4 "+style.at("sections_view_hover")+","
        "      stop:1 "+style.at("window_base_secondary")+");"
        "}"
        "QPushButton#button_Tab_Conf::flat:hover {"
        "   background-color: QLinearGradient("
        "      x1:0, y1:0.5, x2:1, y2:0.5,"
        "      stop:0 "+style.at("sections_conf_hover")+","
        "      stop:0.4 "+style.at("sections_conf_hover")+","
        "      stop:1 "+style.at("window_base_secondary")+");"
        "}"
        // tabs pages
        "QStackedWidget#stackedPages_Sections {"
        "   background-color: "+style.at("window_base_primary")+";"
        "}"
        //////////////
        //// LOGS ////
        // switcher buttons
        "QPushButton#button_Logs_Down {"
        "   border-radius: 16px;"
        "   background-color: "+style.at("window_base_primary")+";"
        "}"
        "QPushButton#button_Logs_Down:hover {"
        "   background-color: "+style.at("window_base_secondary")+";"
        "   icon: url(:/icons/icons/"+theme+"/down.png);"
        "}"
        "QPushButton#button_Logs_Up {"
        "   border-radius: 16px;"
        "   background-color: "+style.at("window_base_primary")+";"
        "}"
        "QPushButton#button_Logs_Up::hover {"
        "   background-color: "+style.at("window_base_secondary")+";"
        "   icon: url(:/icons/icons/"+theme+"/up.png);"
        "}"
        // web servers buttons
        "QPushButton#button_LogFiles_Apache,"
        "QPushButton#button_LogFiles_Nginx,"
        "QPushButton#button_LogFiles_Iis {"
        "   border-radius: 16px;"
        "   background-color: "+style.at("web_servers_buttons_base")+";"
        "}"
        "QPushButton#button_LogFiles_Apache::flat,"
        "QPushButton#button_LogFiles_Nginx::flat,"
        "QPushButton#button_LogFiles_Iis::flat {"
        "   border: 1px solid "+style.at("web_servers_buttons_border_flat")+";"
        "   color: "+style.at("web_servers_buttons_text_flat")+";"
        "   background-color: "+style.at("web_servers_buttons_base_flat")+";"
        "}"
        "QPushButton#button_LogFiles_Apache::flat:hover,"
        "QPushButton#button_LogFiles_Nginx::flat:hover,"
        "QPushButton#button_LogFiles_Iis::flat:hover {"
        "   border: 2px solid "+style.at("web_servers_buttons_border_flat_hover")+";"
        "   color: "+style.at("web_servers_buttons_text_flat_hover")+";"
        "}"
        "QPushButton#button_LogFiles_Apache::disabled,"
        "QPushButton#button_LogFiles_Nginx::disabled,"
        "QPushButton#button_LogFiles_Iis::disabled {"
        "   border: 0px;"
        "   color: "+style.at("web_servers_buttons_text_disabled")+";"
        "   background-color: "+style.at("web_servers_buttons_base_disabled")+";"
        "}"
        // logs list
        "QTreeWidget#listLogFiles::indicator {"
        "   width: 13px;"
        "   height: 13px;"
        "   margin-left: -4px;"
        "   margin-right: 4px;"
        "}"
        "QTreeWidget#listLogFiles::indicator:unchecked {"
        "   image: url(:/icons/icons/"+theme+"/checkbox_unchecked.png);"
        "}"
        "QTreeWidget#listLogFiles::indicator:checked {"
        "   image: url(:/icons/icons/"+theme+"/checkbox_checked.png);"
        "}"
        // view file button
        "QPushButton#button_LogFiles_ViewFile {"
        "   border-radius: 4px;"
        "}"
        // refresh button
        "QPushButton#button_LogFiles_RefreshList {"
        "   border: 1px solid "+style.at("border_primary")+";"
        "   border-radius: 16px;"
        "   background-color: transparent;"
        "}"
        "QPushButton#button_LogFiles_RefreshList:hover {"
        "   border: 1px solid "+style.at("border_secondary")+";"
        "}"
        // parse logs button
        "QPushButton#button_MakeStats_Start {"
        "   border-radius: 32px;"
        "}"
        "QPushButton#button_MakeStats_Start::disabled {"
        "   color: "+style.at("text_disabled")+";"
        "}"
        ///////////////
        //// STATS ////
        // statistics tabs buttons
        "QFrame#frame_Stats_Buttons {"
        "   border: 0px;"
        "   background-color: "+style.at("window_base_primary")+";"
        "}"
        "QPushButton#button_Tab_StatsWarn,"
        "QPushButton#button_Tab_StatsSpeed,"
        "QPushButton#button_Tab_StatsCount,"
        "QPushButton#button_Tab_StatsDay,"
        "QPushButton#button_Tab_StatsRelat,"
        "QPushButton#button_Tab_StatsGlob {"
        "   border-radius: 22px;"
        "   background-color: "+style.at("stats_switch_buttons")+";"
        "}"
        "QPushButton#button_Tab_StatsWarn::flat,"
        "QPushButton#button_Tab_StatsSpeed::flat,"
        "QPushButton#button_Tab_StatsCount::flat,"
        "QPushButton#button_Tab_StatsDay::flat,"
        "QPushButton#button_Tab_StatsRelat::flat,"
        "QPushButton#button_Tab_StatsGlob::flat {"
        "   background-color: "+style.at("stats_switch_buttons_flat")+";"
        "}"
        "QPushButton#button_Tab_StatsWarn::flat:hover,"
        "QPushButton#button_Tab_StatsSpeed::flat:hover,"
        "QPushButton#button_Tab_StatsCount::flat:hover,"
        "QPushButton#button_Tab_StatsDay::flat:hover,"
        "QPushButton#button_Tab_StatsRelat::flat:hover,"
        "QPushButton#button_Tab_StatsGlob::flat:hover {"
        "   background-color: "+style.at("stats_switch_buttons_flat_hover")+";"
        "}"
        // stacked pages
        "QStackedWidget#stackedPages_Stats {"
        "   border-top: 2px solid "+style.at("stats_switch_buttons")+";"
        "   border-radius: 12px;"
        "}"
        // draw buttons
        "QPushButton#button_StatsWarn_Update::disabled {"
        "   icon: url(:/icons/icons/midtone/save.png);"
        "}"
        // stats warn
        "QTableWidget#table_StatsWarn::indicator {"
        "   width: 13px;"
        "   height: 13px;"
        "}"
        "QTableWidget#table_StatsWarn::indicator:unchecked {"
        "   image: url(:/icons/icons/"+theme+"/checkbox_unchecked.png);"
        "}"
        "QTableWidget#table_StatsWarn::indicator:checked {"
        "   image: url(:/icons/icons/"+theme+"/checkbox_checked.png);"
        "}"
        "QTableWidget#table_StatsWarn QScrollBar::handle:vertical {"
        "   padding: 12px;"
        "}"
        // stats count
        "QScrollArea#scrollArea_StatsCount {"
        "   background-color: "+style.at("window_base_primary")+";"
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
        "   background-color: "+style.at("buttons_base_disabled")+";"
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
        "   background-color: "+style.at("window_base_secondary")+";"
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
        "   background-color: "+style.at("buttons_base")+";"
        "}"
        // stats globals
        "QPushButton#button_StatsGlob_Apache,"
        "QPushButton#button_StatsGlob_Nginx,"
        "QPushButton#button_StatsGlob_Iis {"
        "   border-radius: 16px;"
        "   background-color: "+style.at("web_servers_buttons_base")+";"
        "}"
        "QPushButton#button_StatsGlob_Apache::flat,"
        "QPushButton#button_StatsGlob_Nginx::flat,"
        "QPushButton#button_StatsGlob_Iis::flat {"
        "   border: 1px solid "+style.at("web_servers_buttons_border_flat")+";"
        "   color: "+style.at("web_servers_buttons_text_flat")+";"
        "   background-color: "+style.at("web_servers_buttons_base_flat")+";"
        "}"
        "QPushButton#button_StatsGlob_Apache::flat:hover,"
        "QPushButton#button_StatsGlob_Nginx::flat:hover,"
        "QPushButton#button_StatsGlob_Iis::flat:hover {"
        "   border: 2px solid "+style.at("web_servers_buttons_border_flat_hover")+";"
        "   color: "+style.at("web_servers_buttons_text_flat_hover")+";"
        "}"
        "QPushButton#button_StatsGlob_Apache::disabled,"
        "QPushButton#button_StatsGlob_Nginx::disabled,"
        "QPushButton#button_StatsGlob_Iis::disabled {"
        "   border: 0px;"
        "   color: "+style.at("web_servers_buttons_text_disabled")+";"
        "   background-color: "+style.at("web_servers_buttons_base_disabled")+";"
        "}"
        "QScrollArea#scrollArea_StatsGlob_Recur,"
        "QScrollArea#scrollArea_StatsGlob_Perf,"
        "QScrollArea#scrollArea_StatsGlob_Traffic,"
        "QScrollArea#scrollArea_StatsGlob_Work {"
        "   border-radius: 8px;"
        "   background-color: "+style.at("stats_globals_frames")+";"
        "}"
        // styled frames
        "QFrame#frame_StatsWarn_Logs,"
        "QFrame#frame_StatsDay_Date,"
        "QFrame#frame_StatsRelat_Field {"
        "   border: 1px solid "+style.at("border_tertiary")+";"
        "}"
        // lines separators
        "QFrame#gline_StatsWarn_1,"
        "QFrame#gline_StatsWarn_2,"
        "QFrame#gline_StatsWarn_3,"
        "QFrame#gline_StatsDay_1,"
        "QFrame#gline_StatsDay_2,"
        "QFrame#gline_StatsRelat_1 {"
        "   border: 1px solid "+style.at("separators_border")+";"
        "   background-color: "+style.at("separators_base")+";"
        "}"
        "QFrame#gline_StatsSpeed_1,"
        "QFrame#gline_StatsCount_1 {"
        "   border: 1px solid "+style.at("separators_border")+";"
        "   margin: 2px 0px 3px 0px;"
        "   background-color: "+style.at("separators_base")+";"
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
        "   border: 1px solid "+style.at("stats_globals_frames")+";"
        "   background-color: "+style.at("separators_base")+";"
        "}"
        /////////////////
        //// CONFIGS ////
        // sections
        "QTreeView#tree_ConfSections {"
        "   background-color: "+style.at("conf_list_base")+";"
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
        "   border: 2px solid "+style.at("separators_border")+";"
        "   margin: 13px 0px 14px 0px;"
        "   background-color: "+style.at("separators_base")+";"
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
        "   background-color: "+style.at("buttons_base")+";"
        "}"
        "QPushButton#button_ConfDatabases_Data_Save:hover,"
        "QPushButton#button_ConfDatabases_Hashes_Save:hover,"
        "QPushButton#button_ConfApache_Path_Save:hover,"
        "QPushButton#button_ConfApache_Format_Save:hover,"
        "QPushButton#button_ConfNginx_Path_Save:hover,"
        "QPushButton#button_ConfNginx_Format_Save:hover,"
        "QPushButton#button_ConfIis_Path_Save:hover,"
        "QPushButton#button_ConfIis_Format_Save:hover {"
        "   background-color: "+style.at("buttons_base_hover")+";"
        "}"
        "QPushButton#button_ConfDatabases_Data_Save::disabled,"
        "QPushButton#button_ConfDatabases_Hashes_Save::disabled,"
        "QPushButton#button_ConfApache_Path_Save::disabled,"
        "QPushButton#button_ConfApache_Path_Save::disabled,"
        "QPushButton#button_ConfNginx_Path_Save::disabled,"
        "QPushButton#button_ConfNginx_Format_Save::disabled,"
        "QPushButton#button_ConfIis_Path_Save::disabled,"
        "QPushButton#button_ConfIis_Format_Save::disabled {"
        "   background-color: "+style.at("buttons_base_disabled")+";"
        "   icon: url(:/icons/icons/midtone/save.png);"
        "}"
        // sample buttons
        "QPushButton#button_ConfApache_Format_Sample,"
        "QPushButton#button_ConfNginx_Format_Sample,"
        "QPushButton#button_ConfIis_Format_Sample {"
        "   border-radius: 8px;"
        "   background-color: "+style.at("buttons_base")+";"
        "}"
        "QPushButton#button_ConfApache_Format_Sample:hover,"
        "QPushButton#button_ConfNginx_Format_Sample:hover,"
        "QPushButton#button_ConfIis_Format_Sample:hover {"
        "   background-color: "+style.at("buttons_base_hover")+";"
        "}"
        // sample previews
        "QScrollArea#scrollArea_ConfApache_Format_Sample,"
        "QScrollArea#scrollArea_ConfNginx_Format_Sample,"
        "QScrollArea#scrollArea_ConfIis_Format_Sample {"
        "   border: 1px solid "+style.at("border_secondary")+";"
        "   border-top-left-radius: 4px;"
        "   border-top-right-radius: 4px;"
        "   background-color: "+style.at("textbrowser_base")+";"
        "}"
        // help buttons
        "QPushButton#button_ConfApache_Format_Help,"
        "QPushButton#button_ConfNginx_Format_Help,"
        "QPushButton#button_ConfIis_Format_Help {"
        "   border-radius: 8px;"
        "   background-color: "+style.at("help_buttons_base")+";"
        "}"
        "QPushButton#button_ConfApache_Format_Help:hover,"
        "QPushButton#button_ConfNginx_Format_Help:hover,"
        "QPushButton#button_ConfIis_Format_Help:hover {"
        "   background-color: "+style.at("help_buttons_base_hover")+";"
        "}"
        // warnlists / blacklists
        "QListWidget#list_ConfApache_Warnlist_List,"
        "QListWidget#list_ConfApache_Blacklist_List,"
        "QListWidget#list_ConfNginx_Warnlist_List,"
        "QListWidget#list_ConfNginx_Blacklist_List,"
        "QListWidget#list_ConfIis_Warnlist_List,"
        "QListWidget#list_ConfIis_Blacklist_List {"
        "   padding: 4px;"
        "   border: 1px solid "+style.at("border_primary")+";"
        "}"
        "QListWidget#list_ConfApache_Warnlist_List::disabled,"
        "QListWidget#list_ConfApache_Blacklist_List::disabled,"
        "QListWidget#list_ConfNginx_Warnlist_List::disabled,"
        "QListWidget#list_ConfNginx_Blacklist_List::disabled,"
        "QListWidget#list_ConfIis_Warnlist_List::disabled,"
        "QListWidget#list_ConfIis_Blacklist_List::disabled {"
        "   border: 0px;"
        "   color: "+style.at("text_disabled")+";"
        "   background-color: "+style.at("tables_base_disabled")+";"
        "   selection-background-color: "+style.at("tables_base_disabled")+";"
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
        "   background-color: "+style.at("bwlists_buttons_base")+";"
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
        "   background-color: "+style.at("bwlists_buttons_base_hover")+";"
        "}"
        "QPushButton#button_ConfApache_Warnlist_Add::disabled,"
        "QPushButton#button_ConfApache_Blacklist_Add::disabled,"
        "QPushButton#button_ConfNginx_Warnlist_Add::disabled,"
        "QPushButton#button_ConfNginx_Blacklist_Add::disabled,"
        "QPushButton#button_ConfIis_Warnlist_Add::disabled,"
        "QPushButton#button_ConfIis_Blacklist_Add::disabled {"
        "   background-color: "+style.at("bwlists_buttons_base_disabled")+";"
        "   icon: url(:/icons/icons/"+theme+"/list_add.png), #fff;"
        "}"
        "QPushButton#button_ConfApache_Warnlist_Remove::disabled,"
        "QPushButton#button_ConfApache_Blacklist_Remove::disabled,"
        "QPushButton#button_ConfNginx_Warnlist_Remove::disabled,"
        "QPushButton#button_ConfNginx_Blacklist_Remove::disabled,"
        "QPushButton#button_ConfIis_Warnlist_Remove::disabled,"
        "QPushButton#button_ConfIis_Blacklist_Remove::disabled {"
        "   background-color: "+style.at("bwlists_buttons_base_disabled")+";"
        "   icon: url(:/icons/icons/"+theme+"/list_rem.png), #fff;"
        "}"
        "QPushButton#button_ConfApache_Warnlist_Up::disabled,"
        "QPushButton#button_ConfApache_Blacklist_Up::disabled,"
        "QPushButton#button_ConfNginx_Warnlist_Up::disabled,"
        "QPushButton#button_ConfNginx_Blacklist_Up::disabled,"
        "QPushButton#button_ConfIis_Warnlist_Up::disabled,"
        "QPushButton#button_ConfIis_Blacklist_Up::disabled {"
        "   background-color: "+style.at("bwlists_buttons_base_disabled")+";"
        "   icon: url(:/icons/icons/"+theme+"/list_up.png), #fff;"
        "}"
        "QPushButton#button_ConfApache_Warnlist_Down::disabled,"
        "QPushButton#button_ConfApache_Blacklist_Down::disabled,"
        "QPushButton#button_ConfNginx_Warnlist_Down::disabled,"
        "QPushButton#button_ConfNginx_Blacklist_Down::disabled,"
        "QPushButton#button_ConfIis_Warnlist_Down::disabled,"
        "QPushButton#button_ConfIis_Blacklist_Down::disabled {"
        "   background-color: "+style.at("bwlists_buttons_base_disabled")+";"
        "   icon: url(:/icons/icons/"+theme+"/list_down.png), #fff;"
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
        "   background-color: "+style.at("bwlists_buttons_base_flat")+";"
        "   icon: none;"
        "}";
}

} // namespace (private)


void getStyleSheet( QString& stylesheet, const QString& icons_theme, const int theme_id )
{
    std::unordered_map<std::string, QString> style_map;
    switch ( theme_id ) {
        case 1: // light
            style_map = {
                {"text_primary",
                    "rgb( 22, 11, 0 )"},
                {"text_disabled",
                    "rgb( 128, 128, 128 )"},
                {"tooltips_base",
                    "rgb( 30, 35, 27 )"},
                {"tooltips_text",
                    "rgb( 220, 227, 213 )"},
                {"tooltips_border",
                    "rgb( 170, 176, 160 )"},
                {"menu_text",
                    "rgb( 32, 32, 32 )"},
                {"menu_base",
                    "rgb( 200, 200, 200 )"},
                {"menu_base_hover",
                    "rgb( 210, 210, 210 )"},
                {"menu_dropdown_base",
                    "rgb( 112, 112, 112 )"},
                {"menu_dropdown_base_selection",
                    "rgb( 128, 128, 128 )"},
                {"menu_dropdown_text",
                    "rgb( 250, 250, 255 )"},
                {"window_base_primary",
                    "rgb( 250, 250, 255 )"},
                {"window_base_secondary",
                    "rgb( 230, 230, 230 )"},
                {"sections_log_hover",
                    "rgb( 255, 188, 218 )"}, // 196, 7, 93
                {"sections_view_hover",
                    "rgb( 184, 255, 203 )"}, // 0, 226, 60
                {"sections_conf_hover",
                    "rgb( 179, 228, 255 )"}, // 0, 165, 255
                {"border_primary",
                    "rgb( 230, 230, 230 )"},
                {"border_secondary",
                    "rgb( 124, 119, 119 )"},
                {"border_tertiary",
                    "rgb( 88, 80, 80 )"},
                {"separators_base",
                    "rgb( 88, 80, 80 )"},
                {"separators_border",
                    "rgb( 250, 250, 255 )"},
                {"buttons_base",
                    "rgb( 99, 188, 255 )"},
                {"buttons_base_hover",
                    "rgb( 123, 201, 255 )"},
                {"buttons_base_flat",
                    "rgb( 200, 219, 238 )"},
                {"buttons_base_disabled",
                    "rgb( 200, 219, 238 )"},
                {"help_buttons_base",
                    "rgb( 230, 80, 80 )"},
                {"help_buttons_base_hover",
                    "rgb( 230, 112, 80 )"},
                {"web_servers_buttons_base",
                    "rgb( 146, 234, 146 )"},
                {"web_servers_buttons_base_flat",
                    "transparent"},
                {"web_servers_buttons_base_disabled",
                    "rgb( 246, 246, 246 )"},
                {"web_servers_buttons_border_flat",
                    "rgb( 177, 177, 177 )"},
                {"web_servers_buttons_border_flat_hover",
                    "rgb( 146, 234, 146 )"},
                {"web_servers_buttons_text_flat",
                    "rgb( 92, 92, 92 )"},
                {"web_servers_buttons_text_flat_hover",
                    "rgb( 40, 31, 7 )"},
                {"web_servers_buttons_text_disabled",
                    "rgb( 192, 192, 192 )"},
                {"stats_switch_buttons",
                    "rgb( 230, 80, 80 )"},
                {"stats_switch_buttons_flat",
                    "rgb( 250, 250, 255 )"},
                {"stats_switch_buttons_flat_hover",
                    "rgb( 230, 230, 230 )"},
                {"stats_globals_frames",
                    "rgb( 242, 242, 242 )"},
                {"boxes_base_primary",
                    "rgb( 230, 230, 230 )"},
                {"boxes_base_secondary",
                    "rgb( 220, 220, 220 )"},
                {"boxes_base_disabled",
                    "rgb( 200, 200, 200 )"},
                {"boxes_base_selection",
                    "rgb( 123, 201, 255 )"},
                {"boxes_deco",
                    "rgb( 99, 188, 255 )"},
                {"linedit_text",
                    "rgb( 88, 80, 80 )"},
                {"linedit_base",
                    "rgb( 236, 236, 236 )"},
                {"linedit_base_focus",
                    "rgb( 230, 230, 230 )"},
                {"linedit_base_selection",
                    "rgb( 197, 191, 185 )"},
                {"linedit_base_disabled",
                    "rgb( 247, 247, 247 )"},
                {"tables_header",
                    "rgb( 220, 220, 220 )"},
                {"tables_header_hover",
                    "rgb( 201, 239, 255 )"},
                {"tables_header_separator",
                    "rgb( 170, 170, 170 )"},
                {"tables_base",
                    "rgb( 242, 242, 242 )"},
                {"tables_base_disabled",
                    "rgb( 247, 247, 247 )"},
                {"tables_base_selection",
                    "rgb( 201, 239, 255 )"},
                {"tables_gridline",
                    "rgb( 170, 170, 170 )"},
                {"tables_text_selection",
                    "rgb( 22, 11, 0 )"},
                {"tables_deco",
                    "rgb( 99, 188, 255 )"},
                {"charts_contour",
                    "rgb( 220, 220, 220 )"},
                {"textbrowser_text",
                    "rgb( 88, 80, 80 )"},
                {"textbrowser_base",
                    "rgb( 236, 236, 236 )"},
                {"textbrowser_border",
                    "rgb( 236, 236, 236 )"},
                {"textbrowser_text_selection",
                    "rgb( 68, 60, 60 )"},
                {"textbrowser_base_selection",
                    "rgb( 207, 201, 195 )"},
                {"scrollbar_base",
                    "transparent"},
                {"scrollbar_handler",
                    "rgb( 40, 185, 40 )"},
                {"scrollbar_controls",
                    "transparent"},
                {"slider_handler",
                    "rgb( 100, 91, 87 )"},
                {"slider_bar_active",
                    "rgb( 99, 188, 255 )"},
                {"slider_bar_inactive",
                    "rgb( 210, 203, 199 )"},
                {"slider_bar_active_border",
                    "rgb( 123, 201, 255 )"},
                {"slider_handler_border",
                    "rgb( 220, 211, 187 )"},
                {"slider_handler_border_hover",
                    "rgb( 123, 201, 255 )"},
                {"bwlists_buttons_base",
                    "rgb( 99, 188, 255 )"},
                {"bwlists_buttons_base_hover",
                    "rgb( 123, 201, 255 )"},
                {"bwlists_buttons_base_disabled",
                    "rgb( 200, 219, 238 )"},
                {"bwlists_buttons_base_flat",
                    "rgb( 247, 247, 247 )"},
                {"conf_list_base",
                    "rgb( 245, 245, 247 )"}
            };
            break;
        case 2: // dark
            style_map = {
                {"text_primary",
                    "rgb( 248, 248, 248 )"},
                {"text_disabled",
                    "rgb( 128, 128, 128 )"},
                {"tooltips_base",
                    "rgb( 160, 160, 160 )"},
                {"tooltips_text",
                    "rgb( 32, 32, 32 )"},
                {"tooltips_border",
                    "rgb( 144, 144, 144 )"},
                {"menu_text",
                    "rgb( 174, 174, 174 )"},
                {"menu_base",
                    "rgb( 49, 54, 58 )"},
                {"menu_base_hover",
                    "rgb( 40, 45, 49 )"},
                {"menu_dropdown_base",
                    "rgb( 144, 144, 144 )"},
                {"menu_dropdown_base_selection",
                    "rgb( 128, 128, 128 )"},
                {"menu_dropdown_text",
                    "rgb( 13, 14, 15 )"},
                {"window_base_primary",
                    "rgb( 13, 14, 15 )"},
                {"window_base_secondary",
                    "rgb( 27, 30, 33 )"},
                {"sections_log_hover",
                    "rgb( 82, 0, 37 )"}, // 196, 7, 93
                {"sections_view_hover",
                    "rgb( 0, 69, 18 )"}, // 0, 226, 60
                {"sections_conf_hover",
                    "rgb( 0, 57, 89 )"}, // 0, 165, 255
                {"border_primary",
                    "rgb( 40, 45, 49 )"},
                {"border_secondary",
                    "rgb( 92, 92, 92 )"},
                {"border_tertiary",
                    "rgb( 128, 128, 128 )"},
                {"separators_base",
                    "rgb( 96, 96, 96 )"},
                {"separators_border",
                    "rgb( 13, 14, 15 )"},
                {"buttons_base",
                    "rgb( 10, 155, 10 )"},
                {"buttons_base_hover",
                    "rgb( 33, 162, 33 )"},
                {"buttons_base_flat",
                    "rgb( 21, 71, 21 )"},
                {"buttons_base_disabled",
                    "rgb( 21, 71, 21 )"},
                {"help_buttons_base",
                    "rgb( 221, 75, 14 )"},
                {"help_buttons_base_hover",
                    "rgb( 221, 113, 14 )"},
                {"web_servers_buttons_base",
                    "rgb( 0, 121, 115 )"},
                {"web_servers_buttons_base_flat",
                    "transparent"},
                {"web_servers_buttons_base_disabled",
                    "rgb( 18, 20, 22 )"},
                {"web_servers_buttons_border_flat",
                    "rgb( 54, 54, 54 )"},
                {"web_servers_buttons_border_flat_hover",
                    "rgb( 0, 121, 115 )"},
                {"web_servers_buttons_text_flat",
                    "rgb( 108, 108, 108 )"},
                {"web_servers_buttons_text_flat_hover",
                    "rgb( 188, 188, 188 )"},
                {"web_servers_buttons_text_disabled",
                    "rgb( 42, 42, 42 )"},
                {"stats_switch_buttons",
                    "rgb( 221, 75, 14 )"},
                {"stats_switch_buttons_flat",
                    "rgb( 13, 14, 15 )"},
                {"stats_switch_buttons_flat_hover",
                    "rgb( 27, 30, 33 )"},
                {"stats_globals_frames",
                    "rgb( 29, 29, 29 )"},
                {"boxes_base_primary",
                    "rgb( 39, 42, 45 )"},
                {"boxes_base_secondary",
                    "rgb( 45, 48, 51 )"},
                {"boxes_base_disabled",
                    "rgb( 27, 30, 33 )"},
                {"boxes_base_selection",
                    "rgb( 33, 162, 33 )"},
                {"boxes_deco",
                    "rgb( 10, 155, 10 )"},
                {"linedit_text",
                    "rgb( 210, 210, 210 )"},
                {"linedit_base",
                    "rgb( 41, 44, 44 )"},
                {"linedit_base_focus",
                    "rgb( 57, 60, 60 )"},
                {"linedit_base_selection",
                    "rgb( 47, 99, 47 )"},
                {"linedit_base_disabled",
                    "rgb( 18, 20, 22 )"},
                {"tables_header",
                    "rgb( 50, 52, 50 )"},
                {"tables_header_hover",
                    "rgb( 43, 82, 43 )"},
                {"tables_header_separator",
                    "rgb( 80, 80, 80 )"},
                {"tables_base",
                    "rgb( 29, 29, 29 )"},
                {"tables_base_disabled",
                    "rgb( 18, 20, 22 )"},
                {"tables_base_selection",
                    "rgb( 43, 82, 43 )"},
                {"tables_gridline",
                    "rgb( 80, 80, 80 )"},
                {"tables_text_selection",
                    "rgb( 248, 248, 248 )"},
                {"tables_deco",
                    "rgb( 10, 155, 10 )"},
                {"charts_contour",
                    "rgb( 50, 52, 50 )"},
                {"textbrowser_text",
                    "rgb( 210, 210, 210 )"},
                {"textbrowser_base",
                    "rgb( 32, 32, 32 )"},
                {"textbrowser_border",
                    "rgb( 32, 32, 32 )"},
                {"textbrowser_text_selection",
                    "rgb( 248, 248, 248 )"},
                {"textbrowser_base_selection",
                    "rgb( 64, 64, 64 )"},
                {"scrollbar_base",
                    "transparent"},
                {"scrollbar_handler",
                    "rgb( 69, 177, 255 )"},
                {"scrollbar_controls",
                    "transparent"},
                {"slider_handler",
                    "rgb( 112, 112, 112 )"},
                {"slider_bar_active",
                    "rgb( 10, 155, 10 )"},
                {"slider_bar_inactive",
                    "rgb( 64, 64, 64 )"},
                {"slider_bar_active_border",
                    "rgb( 33, 162, 33 )"},
                {"slider_handler_border",
                    "rgb( 128, 128, 128 )"},
                {"slider_handler_border_hover",
                    "rgb( 33, 162, 33 )"},
                {"bwlists_buttons_base",
                    "rgb( 10, 155, 10 )"},
                {"bwlists_buttons_base_hover",
                    "rgb( 33, 162, 33 )"},
                {"bwlists_buttons_base_disabled",
                    "rgb( 21, 71, 21 )"},
                {"bwlists_buttons_base_flat",
                    "rgb( 18, 20, 22 )"},
                {"conf_list_base",
                    "rgb( 20, 21, 22 )"}
            };
            break;
        default:
            throw GenericException( "Unexpected WindowTheme ID: "+std::to_string(theme_id), true );
            break;
    }
    makeStyleSheet( stylesheet, style_map, icons_theme );
}

} // namespace StyleSec
