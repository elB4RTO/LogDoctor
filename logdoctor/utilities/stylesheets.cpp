
#include "stylesheets.h"

#include "modules/exceptions.h"


StyleSec::StyleSec()
{

}


void StyleSec::getStyleSheet( QString& stylesheet, const QString& icons_theme, const int& theme_id )
{
    std::unordered_map<std::string, QString> style_map;
    switch ( theme_id ) {
        case 1:
            style_map = {
                {"text_primary",
                    "rgb( 248, 248, 248 )"},
                {"text_secondary",
                    "rgb( 144, 144, 144 )"},
                {"text_disabled",
                    "rgb( 128, 128, 128 )"},
                {"tooltips_base",
                    "rgb( 4, 4, 4 )"},
                {"tooltips_text",
                    "rgb( 248, 248, 248 )"},
                {"tooltips_border",
                    "rgb( 144, 144, 144 )"},
                {"menu_text",
                    "rgb( 144, 144, 144 )"},
                {"menu_base",
                    "rgb( 32, 32, 32 )"},
                {"menu_base_hover",
                    "rgb( 64, 64, 64 )"},
                {"menu_base_dropdown_selection",
                    "rgb( 192, 192, 192 )"},
                {"window_base_primary",
                    "rgb( 32, 32, 32 )"},
                {"window_base_secondary",
                    "rgb( 16, 16, 16 )"},
                {"window_base_tertiary",
                    "rgb( 64, 64, 64 )"},
                {"border_primary",
                    "rgb( 96, 96, 96 )"},
                {"border_secondary",
                    "rgb( 128, 128, 128 )"},
                {"border_tertiary",
                    "rgb( 128, 128, 128 )"},
                {"buttons_base",
                    "rgb( 64, 64, 64 )"},
                {"buttons_base_hover",
                    "rgb( 82, 82, 82 )"},
                {"buttons_base_disabled",
                    "rgb( 40, 40, 40 )"},
                {"logs_switch_buttons_base_hover",
                    "rgb( 24, 24, 24 )"},
                {"stats_switch_buttons_frame_base",
                    "rgb( 16, 16, 16 )"},
                {"stats_switch_buttons_frame_border",
                    "rgb( 32, 32, 32 )"},
                {"stats_switch_buttons_base",
                    "rgb( 64, 64, 64 )"},
                {"stats_switch_buttons_base_hover",
                    "rgb( 24, 24, 24 )"},
                {"stats_switch_buttons_base_border_hover",
                    "rgb( 48, 48, 48 )"},
                {"globals_frames_base",
                    "rgb( 16, 16, 16 )"},
                {"globals_frames_border",
                    "rgb( 128, 128, 128 )"},
                {"globals_frames_lines",
                    "rgb( 192, 192, 192 )"},
                {"boxes_base_primary",
                    "rgb( 64, 64, 64 )"},
                {"boxes_base_secondary",
                    "rgb( 96, 96, 96 )"},
                {"boxes_base_tertiary",
                    "rgb( 128, 128, 128 )"},
                {"boxes_border_focus",
                    "rgb( 192, 192, 192 )"},
                {"boxes_base_disabled",
                    "rgb( 40, 40, 40 )"},
                {"boxes_base_off",
                    "rgb( 48, 48, 48 )"},
                {"linedit_text",
                    "rgb( 16, 16, 16 )"},
                {"linedit_base",
                    "rgb( 216, 216, 216 )"},
                {"linedit_base_focus",
                    "rgb( 248, 248, 248 )"},
                {"linedit_base_selection",
                    "rgb( 192, 192, 192 )"},
                {"linedit_base_disabled",
                    "rgb( 96, 96, 96 )"},
                {"tables_header",
                    "rgb( 64, 64, 64 )"},
                {"tables_base",
                    "rgb( 32, 32, 32 )"},
                {"tables_text_selection",
                    "rgb( 248, 248, 248 )"},
                {"tables_base_selection",
                    "rgb( 64, 64, 64 )"},
                {"tables_border",
                    "rgb( 96, 96, 96 )"},
                {"tables_base_disabled",
                    "rgb( 96, 96, 96 )"},
                {"tables_text_disabled",
                    "rgb( 128, 128, 128 )"},
                {"tables_border_disabled",
                    "rgb( 64, 64, 64 )"},
                {"charts_contour",
                    "rgb( 32, 32, 32 )"},
                {"textbrowser_text",
                    "rgb( 192, 192, 192 )"},
                {"textbrowser_base",
                    "rgb( 32, 32, 32 )"},
                {"textbrowser_text_selection",
                    "rgb( 248, 248, 248 )"},
                {"textbrowser_base_selection",
                    "rgb( 64, 64, 64 )"},
                {"scrollbar_base",
                    "rgb( 96, 96, 96 )"},
                {"scrollbar_handler",
                    "rgb( 192, 192, 192 )"},
                {"slider_handler",
                    "rgb( 48, 48, 48 )"},
                {"slider_bar_active",
                    "rgb( 128, 128, 128 )"},
                {"slider_bar_inactive",
                    "rgb( 82, 82, 82 )"},
                {"slider_bar_active_border",
                    "rgb( 144, 144, 144 )"},
                {"slider_handler_border",
                    "rgb( 192, 192, 192 )"},
                {"slider_handler_border_hover",
                    "rgb( 248, 248, 248 )"},
                {"outer-tabs_base",
                    "rgb( 16, 16, 16 )"},
                {"inner-tabs_base",
                    "rgb( 32, 32, 32 )"},
                {"sub-tabs_base",
                    "rgb( 48, 48, 48 )"},
                {"outer-tabs_tab_text_selected",
                    "rgb( 248, 248, 248 )"},
                {"outer-tabs_tab_text_unselected",
                    "rgb( 128, 128, 128 )"},
                {"outer-tabs_tab_text_hover",
                    "rgb( 192, 192, 192 )"},
                {"outer-tabs_tab_border",
                    "rgb( 96, 96, 96 )"},
                {"outer-tabs_tab_border_unselected",
                    "rgb( 48, 48, 48 )"},
                {"inner-tabs_tab_base",
                    "rgb( 32, 32, 32 )"},
                {"inner-tabs_tab_base_unselected",
                    "rgb( 24, 24, 24 )"},
                {"inner-tabs_pane_base",
                    "rgb( 32, 32, 32 )"},
                {"inner-tabs_border",
                    "rgb( 96, 96, 96 )"},
                {"inner-tabs_lines",
                    "rgb( 192, 192, 192 )"},
                {"sub-tabs_tab_base",
                    "rgb( 48, 48, 48 )"},
                {"sub-tabs_tab_base_unselected",
                    "rgb( 40, 40, 40 )"},
                {"sub-tabs_pane_base",
                    "rgb( 48, 48, 48 )"},
                {"sub-tabs_border",
                    "rgb( 96, 96, 96 )"},
                {"sub-tabs_tab_border_unselected",
                    "rgb( 64, 64, 64 )"}
            };
            break;
        case 2:
            style_map = {
                {"text_primary",
                    "rgb( 45, 0, 30 )"},
                {"text_secondary",
                    "rgb( 115, 30, 70 )"},
                {"text_disabled",
                    "rgb( 255, 204, 143 )"},
                {"tooltips_base",
                    "rgb( 45, 0, 30 )"},
                {"tooltips_text",
                    "rgb( 255, 204, 143 )"},
                {"tooltips_border",
                    "rgb( 145, 100, 130 )"},
                {"menu_text",
                    "rgb( 45, 0, 30 )"},
                {"menu_base",
                    "rgb( 205, 154, 93 )"},
                {"menu_base_hover",
                    "rgb( 165, 114, 63 )"},
                {"menu_base_dropdown_selection",
                    "rgb( 145, 100, 130 )"},
                {"window_base_primary",
                    "rgb( 255, 204, 143 )"},
                {"window_base_secondary",
                    "rgb( 255, 140, 141 )"},
                {"window_base_tertiary",
                    "rgb( 245, 172, 142 )"},
                {"border_primary",
                    "rgb( 195, 80, 81 )"},
                {"border_secondary",
                    "rgb( 195, 80, 81 )"},
                {"border_tertiary",
                    "rgb( 255, 204, 143 )"},
                {"buttons_base",
                    "rgb( 114, 235, 115 )"},
                {"buttons_base_hover",
                    "rgb( 154, 255, 155 )"},
                {"buttons_base_disabled",
                    "rgb( 195, 80, 81 )"},
                {"logs_switch_buttons_base_hover",
                    "rgb( 245, 172, 142 )"},
                {"stats_switch_buttons_frame_base",
                    "rgb( 245, 172, 142 )"},
                {"stats_switch_buttons_frame_border",
                    "rgb( 255, 204, 143 )"},
                {"stats_switch_buttons_base",
                    "rgb( 255, 140, 141 )"},
                {"stats_switch_buttons_base_hover",
                    "rgb( 245, 172, 142 )"},
                {"stats_switch_buttons_base_border_hover",
                    "rgb( 255, 204, 143 )"},
                {"globals_frames_base",
                    "rgb( 255, 210, 221 )"},
                {"globals_frames_border",
                    "rgb( 195, 80, 81 )"},
                {"globals_frames_lines",
                    "rgb( 145, 100, 130 )"},
                {"boxes_base_primary",
                    "rgb( 164, 255, 165 )"},
                {"boxes_base_secondary",
                    "rgb( 114, 235, 115 )"},
                {"boxes_base_tertiary",
                    "rgb( 154, 245, 155 )"},
                {"boxes_border_focus",
                    "rgb( 195, 80, 81 )"},
                {"boxes_base_disabled",
                    "rgb( 195, 80, 81 )"},
                {"boxes_base_off",
                    "rgb( 195, 80, 81 )"},
                {"linedit_text",
                    "rgb( 45, 0, 30 )"},
                {"linedit_base",
                    "rgb( 113, 154, 225 )"},
                {"linedit_base_focus",
                    "rgb( 143, 184, 255 )"},
                {"linedit_base_selection",
                    "rgb( 145, 100, 130 )"},
                {"linedit_base_disabled",
                    "rgb( 195, 80, 81 )"},
                {"tables_header",
                    "rgb( 195, 140, 141 )"},
                {"tables_base",
                    "rgb( 255, 210, 221 )"},
                {"tables_text_selection",
                    "rgb( 45, 0, 30 )"},
                {"tables_base_selection",
                    "rgb( 145, 100, 130 )"},
                {"tables_border",
                    "rgb( 195, 80, 81 )"},
                {"tables_base_disabled",
                    "rgb( 195, 80, 81 )"},
                {"tables_text_disabled",
                    "rgb( 195, 140, 141 )"},
                {"tables_border_disabled",
                    "rgb( 165, 114, 63 )"},
                {"charts_contour",
                    "rgb( 113, 154, 225 )"},
                {"textbrowser_text",
                    "rgb( 45, 0, 30 )"},
                {"textbrowser_base",
                    "rgb( 255, 210, 221 )"},
                {"textbrowser_text_selection",
                    "rgb( 255, 204, 143 )"},
                {"textbrowser_base_selection",
                    "rgb( 145, 100, 130 )"},
                {"scrollbar_base",
                    "rgb( 145, 100, 130 )"},
                {"scrollbar_handler",
                    "rgb( 45, 0, 30 )"},
                {"slider_handler",
                    "rgb( 113, 154, 225 )"},
                {"slider_bar_active",
                    "rgb( 114, 235, 115 )"},
                {"slider_bar_inactive",
                    "rgb( 165, 114, 63 )"},
                {"slider_bar_active_border",
                    "rgb( 154, 245, 155 )"},
                {"slider_handler_border",
                    "rgb( 195, 140, 141 )"},
                {"slider_handler_border_hover",
                    "rgb( 255, 210, 221 )"},
                {"outer-tabs_base",
                    "rgb( 255, 140, 141 )"},
                {"inner-tabs_base",
                    "rgb( 255, 204, 143 )"},
                {"sub-tabs_base",
                    "rgb( 255, 140, 141 )"},
                {"outer-tabs_tab_text_selected",
                    "rgb( 45, 0, 30 )"},
                {"outer-tabs_tab_text_unselected",
                    "rgb( 115, 30, 70 )"},
                {"outer-tabs_tab_text_hover",
                    "rgb( 45, 0, 30 )"},
                {"outer-tabs_tab_border",
                    "rgb( 255, 204, 143 )"},
                {"outer-tabs_tab_border_unselected",
                    "rgb( 165, 114, 63 )"},
                {"inner-tabs_tab_base",
                    "rgb( 255, 204, 143 )"},
                {"inner-tabs_tab_base_unselected",
                    "rgb( 245, 172, 142 )"},
                {"inner-tabs_pane_base",
                    "rgb( 255, 204, 143 )"},
                {"inner-tabs_border",
                    "rgb( 195, 80, 81 )"},
                {"inner-tabs_lines",
                    "rgb( 195, 80, 81 )"},
                {"sub-tabs_tab_base",
                    "rgb( 255, 140, 141 )"},
                {"sub-tabs_tab_base_unselected",
                    "rgb( 245, 172, 142 )"},
                {"sub-tabs_pane_base",
                    "rgb( 255, 140, 141 )"},
                {"sub-tabs_border",
                    "rgb( 195, 80, 81 )"},
                {"sub-tabs_tab_border_unselected",
                    "rgb( 165, 114, 63 )"}
            };
            break;
        case 3:
            style_map = {
                {"text_primary",
                    "rgb( 220, 211, 187 )"},
                {"text_secondary",
                    "rgb( 193, 175, 129 )"},
                {"text_disabled",
                    "rgb( 163, 145, 99 )"},
                {"tooltips_base",
                    "rgb( 193, 175, 129 )"},
                {"tooltips_text",
                    "rgb( 14, 28, 0 )"},
                {"tooltips_border",
                    "rgb( 163, 145, 99 )"},
                {"menu_text",
                    "rgb( 163, 145, 99 )"},
                {"menu_base",
                    "rgb( 24, 48, 0 )"},
                {"menu_base_hover",
                    "rgb( 54, 78, 30 )"},
                {"menu_base_dropdown_selection",
                    "rgb( 74, 98, 50 )"},
                {"window_base_primary",
                    "rgb( 24, 48, 0 )"},
                {"window_base_secondary",
                    "rgb( 14, 28, 0 )"},
                {"window_base_tertiary",
                    "rgb( 54, 78, 30 )"},
                {"border_primary",
                    "rgb( 163, 145, 99 )"},
                {"border_secondary",
                    "rgb( 193, 175, 129 )"},
                {"border_tertiary",
                    "rgb( 193, 175, 129 )"},
                {"buttons_base",
                    "rgb( 94, 118, 70 )"},
                {"buttons_base_hover",
                    "rgb( 124, 148, 100 )"},
                {"buttons_base_disabled",
                    "rgb( 54, 78, 30 )"},
                {"logs_switch_buttons_base_hover",
                    "rgb( 24, 38, 0 )"},
                {"stats_switch_buttons_frame_base",
                    "rgb( 14, 28, 0 )"},
                {"stats_switch_buttons_frame_border",
                    "rgb( 24, 48, 0 )"},
                {"stats_switch_buttons_base",
                    "rgb( 54, 78, 30 )"},
                {"stats_switch_buttons_base_hover",
                    "rgb( 24, 48, 0 )"},
                {"stats_switch_buttons_base_border_hover",
                    "rgb( 94, 118, 70 )"},
                {"globals_frames_base",
                    "rgb( 4, 18, 0 )"},
                {"globals_frames_border",
                    "rgb( 128, 128, 128 )"},
                {"globals_frames_lines",
                    "rgb( 124, 148, 100 )"},
                {"boxes_base_primary",
                    "rgb( 94, 118, 70 )"},
                {"boxes_base_secondary",
                    "rgb( 104, 128, 80 )"},
                {"boxes_base_tertiary",
                    "rgb( 124, 148, 100 )"},
                {"boxes_border_focus",
                    "rgb( 193, 175, 129 )"},
                {"boxes_base_disabled",
                    "rgb( 54, 78, 30 )"},
                {"boxes_base_off",
                    "rgb( 74, 98, 50 )"},
                {"linedit_text",
                    "rgb( 4, 18, 0 )"},
                {"linedit_base",
                    "rgb( 193, 175, 129 )"},
                {"linedit_base_focus",
                    "rgb( 223, 215, 159 )"},
                {"linedit_base_selection",
                    "rgb( 173, 155, 109 )"},
                {"linedit_base_disabled",
                    "rgb( 54, 78, 30 )"},
                {"tables_header",
                    "rgb( 63, 65, 19 )"},
                {"tables_base",
                    "rgb( 33, 35, 0 )"},
                {"tables_text_selection",
                    "rgb( 33, 35, 0 )"},
                {"tables_base_selection",
                    "rgb( 133, 135, 89 )"},
                {"tables_border",
                    "rgb( 133, 135, 89 )"},
                {"tables_base_disabled",
                    "rgb( 54, 78, 30 )"},
                {"tables_text_disabled",
                    "rgb( 163, 145, 99 )"},
                {"tables_border_disabled",
                    "rgb( 83, 85, 39 )"},
                {"charts_contour",
                    "rgb( 94, 118, 70 )"},
                {"textbrowser_text",
                    "rgb( 193, 175, 129 )"},
                {"textbrowser_base",
                    "rgb( 33, 35, 0 )"},
                {"textbrowser_text_selection",
                    "rgb( 223, 205, 159 )"},
                {"textbrowser_base_selection",
                    "rgb( 133, 135, 89 )"},
                {"scrollbar_base",
                    "rgb( 94, 118, 70 )"},
                {"scrollbar_handler",
                    "rgb( 174, 198, 150 )"},
                {"slider_handler",
                    "rgb( 94, 118, 70 )"},
                {"slider_bar_active",
                    "rgb( 163, 145, 99 )"},
                {"slider_bar_inactive",
                    "rgb( 113, 95, 49 )"},
                {"slider_bar_active_border",
                    "rgb( 183, 165, 119 )"},
                {"slider_handler_border",
                    "rgb( 163, 145, 99 )"},
                {"slider_handler_border_hover",
                    "rgb( 193, 175, 129 )"},
                {"outer-tabs_base",
                    "rgb( 14, 28, 0 )"},
                {"inner-tabs_base",
                    "rgb( 24, 48, 0 )"},
                {"sub-tabs_base",
                    "rgb( 34, 58, 10 )"},
                {"outer-tabs_tab_text_selected",
                    "rgb( 220, 211, 187 )"},
                {"outer-tabs_tab_text_unselected",
                    "rgb( 163, 145, 99 )"},
                {"outer-tabs_tab_text_hover",
                    "rgb( 193, 175, 129 )"},
                {"outer-tabs_tab_border",
                    "rgb( 220, 211, 187 )"},
                {"outer-tabs_tab_border_unselected",
                    "rgb( 163, 145, 99 )"},
                {"inner-tabs_tab_base",
                    "rgb( 24, 48, 0 )"},
                {"inner-tabs_tab_base_unselected",
                    "rgb( 14, 28, 0 )"},
                {"inner-tabs_pane_base",
                    "rgb( 24, 48, 0 )"},
                {"inner-tabs_border",
                    "rgb( 220, 211, 187 )"},
                {"inner-tabs_lines",
                    "rgb( 124, 148, 100 )"},
                {"sub-tabs_tab_base",
                    "rgb( 34, 58, 10 )"},
                {"sub-tabs_tab_base_unselected",
                    "rgb( 24, 48, 0 )"},
                {"sub-tabs_pane_base",
                    "rgb( 34, 58, 10 )"},
                {"sub-tabs_border",
                    "rgb( 220, 211, 187 )"},
                {"sub-tabs_tab_border_unselected",
                    "rgb( 163, 145, 99 )"}
            };
            break;
        case 4:
            style_map = {
                {"text_primary",
                    "rgb( 30, 21, 0 )"},
                {"text_secondary",
                    "rgb( 40, 31, 7 )"},
                {"text_disabled",
                    "rgb( 60, 51, 27 )"},
                {"tooltips_base",
                    "rgb( 40, 31, 7 )"},
                {"tooltips_text",
                    "rgb( 170, 161, 137 )"},
                {"tooltips_border",
                    "rgb( 210, 201, 177 )"},
                {"menu_text",
                    "rgb( 40, 31, 7 )"},
                {"menu_base",
                    "rgb( 210, 201, 177 )"},
                {"menu_base_hover",
                    "rgb( 170, 161, 137 )"},
                {"menu_base_dropdown_selection",
                    "rgb( 70, 61, 37 )"},
                {"window_base_primary",
                    "rgb( 230, 221, 197 )"},
                {"window_base_secondary",
                    "rgb( 170, 161, 137 )"},
                {"window_base_tertiary",
                    "rgb( 210, 201, 177 )"},
                {"border_primary",
                    "rgb( 80, 71, 47 )"},
                {"border_secondary",
                    "rgb( 100, 91, 67 )"},
                {"border_tertiary",
                    "rgb( 100, 91, 67 )"},
                {"buttons_base",
                    "rgb( 130, 121, 97 )"},
                {"buttons_base_hover",
                    "rgb( 140, 131, 107 )"},
                {"buttons_base_disabled",
                    "rgb( 160, 151, 127 )"},
                {"logs_switch_buttons_base_hover",
                    "rgb( 160, 151, 127 )"},
                {"stats_switch_buttons_frame_base",
                    "rgb( 170, 161, 137 )"},
                {"stats_switch_buttons_frame_border",
                    "rgb( 230, 221, 197 )"},
                {"stats_switch_buttons_base",
                    "rgb( 200, 191, 167 )"},
                {"stats_switch_buttons_base_hover",
                    "rgb( 180, 171, 147 )"},
                {"stats_switch_buttons_base_border_hover",
                    "rgb( 220, 211, 187 )"},
                {"globals_frames_base",
                    "rgb( 210, 201, 177 )"},
                {"globals_frames_border",
                    "rgb( 80, 71, 47 )"},
                {"globals_frames_lines",
                    "rgb( 60, 51, 27 )"},
                {"boxes_base_primary",
                    "rgb( 130, 121, 97 )"},
                {"boxes_base_secondary",
                    "rgb( 140, 131, 107 )"},
                {"boxes_base_tertiary",
                    "rgb( 130, 121, 97 )"},
                {"boxes_border_focus",
                    "rgb( 80, 71, 47 )"},
                {"boxes_base_disabled",
                    "rgb( 160, 151, 127 )"},
                {"boxes_base_off",
                    "rgb( 150, 141, 117 )"},
                {"linedit_text",
                    "rgb( 30, 21, 0 )"},
                {"linedit_base",
                    "rgb( 210, 201, 177 )"},
                {"linedit_base_focus",
                    "rgb( 230, 221, 197 )"},
                {"linedit_base_selection",
                    "rgb( 170, 161, 137 )"},
                {"linedit_base_disabled",
                    "rgb( 160, 151, 127 )"},
                {"tables_header",
                    "rgb( 130, 121, 97 )"},
                {"tables_base",
                    "rgb( 210, 201, 177 )"},
                {"tables_text_selection",
                    "rgb( 230, 221, 197 )"},
                {"tables_base_selection",
                    "rgb( 60, 51, 27 )"},
                {"tables_border",
                    "rgb( 80, 71, 47 )"},
                {"tables_base_disabled",
                    "rgb( 160, 151, 127 )"},
                {"tables_text_disabled",
                    "rgb( 130, 121, 97 )"},
                {"tables_border_disabled",
                    "rgb( 130, 121, 97 )"},
                {"charts_contour",
                    "rgb( 40, 31, 7 )"},
                {"textbrowser_text",
                    "rgb( 30, 21, 0 )"},
                {"textbrowser_base",
                    "rgb( 210, 201, 177 )"},
                {"textbrowser_text_selection",
                    "rgb( 230, 221, 197 )"},
                {"textbrowser_base_selection",
                    "rgb( 80, 71, 47 )"},
                {"scrollbar_base",
                    "rgb( 100, 91, 67 )"},
                {"scrollbar_handler",
                    "rgb( 210, 201, 177 )"},
                {"slider_handler",
                    "rgb( 100, 91, 67 )"},
                {"slider_bar_active",
                    "rgb( 120, 111, 87 )"},
                {"slider_bar_inactive",
                    "rgb( 40, 31, 7 )"},
                {"slider_bar_active_border",
                    "rgb( 150, 141, 117 )"},
                {"slider_handler_border",
                    "rgb( 220, 211, 187 )"},
                {"slider_handler_border_hover",
                    "rgb( 250, 241, 217 )"},
                {"outer-tabs_base",
                    "rgb( 170, 161, 137 )"},
                {"inner-tabs_base",
                    "rgb( 180, 171, 147 )"},
                {"sub-tabs_base",
                    "rgb( 190, 181, 157 )"},
                {"outer-tabs_tab_text_selected",
                    "rgb( 30, 21, 0 )"},
                {"outer-tabs_tab_text_unselected",
                    "rgb( 90, 81, 57 )"},
                {"outer-tabs_tab_text_hover",
                    "rgb( 50, 41, 17 )"},
                {"outer-tabs_tab_border",
                    "rgb( 80, 71, 47 )"},
                {"outer-tabs_tab_border_unselected",
                    "rgb( 100, 91, 67 )"},
                {"inner-tabs_tab_base",
                    "rgb( 180, 171, 147 )"},
                {"inner-tabs_tab_base_unselected",
                    "rgb( 170, 161, 137 )"},
                {"inner-tabs_pane_base",
                    "rgb( 180, 171, 147 )"},
                {"inner-tabs_border",
                    "rgb( 80, 71, 47 )"},
                {"inner-tabs_lines",
                    "rgb( 60, 51, 27 )"},
                {"sub-tabs_tab_base",
                    "rgb( 190, 181, 157 )"},
                {"sub-tabs_tab_base_unselected",
                    "rgb( 180, 171, 147 )"},
                {"sub-tabs_pane_base",
                    "rgb( 190, 181, 157 )"},
                {"sub-tabs_border",
                    "rgb( 80, 71, 47 )"},
                {"sub-tabs_tab_border_unselected",
                    "rgb( 100, 91, 67 )"}
            };
            break;
        default:
            throw GenericException( "Unexpected WindowTheme ID: "+std::to_string(theme_id), true );
            break;
    }
    StyleSec::makeStyleSheet( stylesheet, style_map, icons_theme );
}



void StyleSec::makeStyleSheet( QString& ss, const std::unordered_map<std::string, QString> &style, const QString& theme )
{
    ss= ////////////////
        //// SHARED ////
        "* {"
        "   color: "+style.at("text_primary")+";"
        "}"
        // frames
        "QFrame,"
        "QWidget {"
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
        "   border: 1px solid "+style.at("tooltips_border")+";"
        "   color: "+style.at("tooltips_text")+";"
        "   background-color: "+style.at("tooltips_base")+";"
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
        // line edits
        "QLineEdit {"
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
        "   background-color: "+style.at("boxes_base_primary")+";"
        "   selection-background-color: "+style.at("boxes_base_secondary")+";"
        "}"
        "QComboBox::disabled {"
        "   background-color: "+style.at("boxes_base_disabled")+";"
        "}"
        "QComboBox QAbstractItemView {"
        "   border: 1px solid "+style.at("boxes_base_tertiary")+";"
        "   background-color: "+style.at("boxes_base_primary")+";"
        "}"
        // spin boxes
        "QSpinBox {"
        "   border: 1px solid "+style.at("boxes_base_secondary")+";"
        "   border-radius: 4px;"
        "   background-color: "+style.at("boxes_base_primary")+";"
        "   selection-background-color: "+style.at("boxes_base_secondary")+";"
        "}"
        "QSpinBox::focus {"
        "   border-color: "+style.at("boxes_border_focus")+";"
        "}"
        "QSpinBox::disabled {"
        "   border-color: "+style.at("boxes_base_off")+";"
        "   color: "+style.at("text_disabled")+";"
        "   background-color: "+style.at("boxes_base_disabled")+";"
        "}"
        "QSpinBox::up-button,"
        "QSpinBox::down-button {"
        "   padding: 2px;"
        "   border-left: 1px solid "+style.at("boxes_border_focus")+";"
        "   background-color: "+style.at("boxes_base_secondary")+";"
        "}"
        "QSpinBox::up-button:hover,"
        "QSpinBox::down-button:hover {"
        "   background-color: "+style.at("boxes_base_tertiary")+";"
        "}"
        "QSpinBox::up-button:off,"
        "QSpinBox::down-button:off {"
        "   background-color: "+style.at("boxes_base_off")+";"
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
        "   margin: 5px 12px 5px 12px;"
        "   background-color: "+style.at("scrollbar_handler")+";"
        "}"
        "QScrollBar::handle:horizontal:hover {"
        "   margin: 4px 12px 4px 12px;"
        "}"
        "QScrollBar:vertical {"
        "   width: 12px;"
        "   background-color: "+style.at("scrollbar_base")+";"
        "}"
        "QScrollBar::handle:vertical {"
        "   min-height: 16px;"
        "   margin: 12px 5px 12px 5px;"
        "   background-color: "+style.at("scrollbar_handler")+";"
        "}"
        "QScrollBar::handle:vertical:hover {"
        "   margin:12px 4px 12px 4px;"
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
        "   border-radius: 4px;"
        "   background-color: "+style.at("charts_contour")+";"
        "}"
        // tables, trees and headers
        "QTreeWidget,"
        "QListWidget,"
        "QTableWidget {"
        "   background-color: "+style.at("tables_base")+";"
        "   selection-color: "+style.at("tables_text_selection")+";"
        "   selection-background-color: "+style.at("tables_base_selection")+";"
        "}"
        "QTreeWidget QScrollBar::handle:vertical,"
        "QTableWidget QScrollBar::handle:vertical {"
        "   padding: 12px;"
        "}"
        "QHeaderView {"
        "   background-color: "+style.at("tables_header")+";"
        "}"
        // text browser
        "QTextBrowser {"
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
        "   color: "+style.at("menu_base")+";"
        "   background-color: "+style.at("menu_text")+";"
        "   border-radius: 0px;"
        "   border-top-left-radius: 4px;"
        "   border-top-right-radius: 4px;"
        "}"
        // menu dropdown
        "QMenu {"
        "   color: "+style.at("menu_base")+";"
        "   background-color: "+style.at("menu_text")+";"
        "}"
        "QMenu::item:selected {"
        "   background-color: "+style.at("menu_base_dropdown_selection")+";"
        "}"
        //////////////
        //// MAIN ////
        // window
        "QWidget#mainwidget {"
        "   background-color: "+style.at("window_base_primary")+";"
        "}"
        // tabs buttons
        "QFrame#frame_Tabs_Buttons {"
        "   background-color: "+style.at("window_base_primary")+";"
        "}"
        "QPushButton#button_Tab_Log,"
        "QPushButton#button_Tab_View,"
        "QPushButton#button_Tab_Conf {"
        "   border-top-left-radius: 8px;"
        "   border-bottom-left-radius: 8px;"
        "   background-color: "+style.at("window_base_secondary")+";"
        "}"
        "QPushButton#button_Tab_Log::flat,"
        "QPushButton#button_Tab_View::flat,"
        "QPushButton#button_Tab_Conf::flat {"
        "   background-color: "+style.at("window_base_primary")+";"
        "}"
        "QPushButton#button_Tab_Log::flat:hover,"
        "QPushButton#button_Tab_View::flat:hover,"
        "QPushButton#button_Tab_Conf::flat:hover {"
        "   background-color: "+style.at("window_base_tertiary")+";"
        "}"
        // tabs pages
        "QStackedWidget#stacked_Tabs_Pages {"
        "   background-color: "+style.at("window_base_secondary")+";"
        "}"
        //////////////
        //// LOGS ////
        // switcher buttons
        "QFrame#frame_Logs_Down {"
        "   background-color: "+style.at("window_base_secondary")+";"
        "}"
        "QPushButton#button_Logs_Down:hover {"
        "   border-radius: 16px;"
        "   background-color: "+style.at("logs_switch_buttons_base_hover")+";"
        "   icon: url(:/icons/icons/"+theme+"/down.png);"
        "}"
        "QFrame#frame_Logs_Up {"
        "   background-color: "+style.at("window_base_secondary")+";"
        "}"
        "QPushButton#button_Logs_Up::flat:hover {"
        "   border-radius: 16px;"
        "   background-color: "+style.at("logs_switch_buttons_base_hover")+";"
        "   icon: url(:/icons/icons/"+theme+"/up.png);"
        "}"
        // web servers buttons
        "QPushButton#button_LogFiles_Apache,"
        "QPushButton#button_LogFiles_Nginx,"
        "QPushButton#button_LogFiles_Iis {"
        "   border-radius: 4px;"
        "   background-color: "+style.at("buttons_base")+";"
        "}"
        "QPushButton#button_LogFiles_Apache::flat,"
        "QPushButton#button_LogFiles_Nginx::flat,"
        "QPushButton#button_LogFiles_Iis::flat {"
        "   border: 1px solid "+style.at("buttons_base")+";"
        "   color: "+style.at("text_secondary")+";"
        "   background-color: "+style.at("window_base_secondary")+";"
        "}"
        "QPushButton#button_LogFiles_Apache::flat:hover,"
        "QPushButton#button_LogFiles_Nginx::flat:hover,"
        "QPushButton#button_LogFiles_Iis::flat:hover {"
        "   border: 1px solid "+style.at("border_secondary")+";"
        "   color: "+style.at("text_primary")+";"
        "}"
        "QPushButton#button_LogFiles_Apache::disabled,"
        "QPushButton#button_LogFiles_Nginx::disabled,"
        "QPushButton#button_LogFiles_Iis::disabled {"
        "   color: "+style.at("text_disabled")+";"
        "   background-color: "+style.at("buttons_base_disabled")+";"
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
        "   background-color: "+style.at("buttons_base")+";"
        "}"
        "QPushButton#button_LogFiles_ViewFile:hover {"
        "   border: 1px solid "+style.at("border_primary")+";"
        "}"
        "QPushButton#button_LogFiles_ViewFile::disabled {"
        "   background-color: "+style.at("buttons_base_disabled")+";"
        "}"
        // refresh button
        "QPushButton#button_LogFiles_RefreshList {"
        "   border: 1px solid "+style.at("buttons_base_disabled")+";"
        "   border-radius: 16px;"
        "   background-color: transparent;"
        "}"
        "QPushButton#button_LogFiles_RefreshList:hover {"
        "   border: 1px solid "+style.at("border_primary")+";"
        "}"
        // parse logs button
        "QPushButton#button_MakeStats_Start {"
        "   border-radius: 4px;"
        "   background-color: "+style.at("buttons_base")+";"
        "}"
        "QPushButton#button_MakeStats_Start:hover {"
        "   border: 1px solid "+style.at("border_primary")+";"
        "   background-color: "+style.at("buttons_base_hover")+";"
        "}"
        "QPushButton#button_MakeStats_Start::disabled {"
        "   color: "+style.at("text_disabled")+";"
        "   background-color: "+style.at("buttons_base_disabled")+";"
        "}"
        ///////////////
        //// STATS ////
        // statistics tabs buttons
        "QFrame#frame_Stats_Buttons {"
        "   border: 2px solid "+style.at("stats_switch_buttons_frame_border")+";"
        "   border-bottom-left-radius: 4px;"
        "   border-bottom-right-radius: 4px;"
        "   background-color: "+style.at("stats_switch_buttons_frame_base")+";"
        "}"
        "QPushButton#button_Tab_StatsWarn,"
        "QPushButton#button_Tab_StatsSpeed,"
        "QPushButton#button_Tab_StatsCount,"
        "QPushButton#button_Tab_StatsDay,"
        "QPushButton#button_Tab_StatsRelat,"
        "QPushButton#button_Tab_StatsGlob {"
        "   border-radius: 4px;"
        "   background-color: "+style.at("stats_switch_buttons_base")+";"
        "}"
        "QPushButton#button_Tab_StatsWarn::flat,"
        "QPushButton#button_Tab_StatsSpeed::flat,"
        "QPushButton#button_Tab_StatsCount::flat,"
        "QPushButton#button_Tab_StatsDay::flat,"
        "QPushButton#button_Tab_StatsRelat::flat,"
        "QPushButton#button_Tab_StatsGlob::flat {"
        "   background-color: transparent;"
        "}"
        "QPushButton#button_Tab_StatsWarn::flat:hover,"
        "QPushButton#button_Tab_StatsSpeed::flat:hover,"
        "QPushButton#button_Tab_StatsCount::flat:hover,"
        "QPushButton#button_Tab_StatsDay::flat:hover,"
        "QPushButton#button_Tab_StatsRelat::flat:hover,"
        "QPushButton#button_Tab_StatsGlob::flat:hover {"
        "   border: 1px solid "+style.at("stats_switch_buttons_base_border_hover")+";"
        "   background-color: "+style.at("stats_switch_buttons_base_hover")+";"
        "}"
        // draw buttons
        "QPushButton#button_StatsWarn_Update,"
        "QPushButton#button_StatsWarn_Draw,"
        "QPushButton#button_StatsSpeed_Draw,"
        "QPushButton#button_StatsCount_Draw,"
        "QPushButton#button_StatsDay_Draw,"
        "QPushButton#button_StatsRelat_Draw,"
        "QPushButton#button_StatsGlob_Draw {"
        "   border-radius: 4px;"
        "   background-color: "+style.at("buttons_base")+";"
        "}"
        "QPushButton#button_StatsWarn_Update:hover,"
        "QPushButton#button_StatsWarn_Draw:hover,"
        "QPushButton#button_StatsSpeed_Draw:hover,"
        "QPushButton#button_StatsCount_Draw:hover,"
        "QPushButton#button_StatsDay_Draw:hover,"
        "QPushButton#button_StatsRelat_Draw:hover,"
        "QPushButton#button_StatsGlob_Draw:hover {"
        "   border: 1px solid "+style.at("border_secondary")+";"
        "   background-color: "+style.at("buttons_base_hover")+";"
        "}"
        "QPushButton#button_StatsWarn_Update::disabled {"
        "   border: 1px solid "+style.at("buttons_base")+";"
        "   background-color: "+style.at("buttons_base_disabled")+";"
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
        "   background-color: "+style.at("window_base_secondary")+";"
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
        "   border-radius: 4px;"
        "   background-color: "+style.at("border_primary")+";"
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
        "   border: 1px solid "+style.at("buttons_base")+";"
        "   background-color: "+style.at("buttons_base_disabled")+";"
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
        "   border: 1px solid "+style.at("border_primary")+";"
        "   background-color: "+style.at("buttons_base")+";"
        "}"
        // stats globals
        "QFrame#frame_StatsGlob_WebServers {"
        "   border-radius: 4px;"
        "   border: 1px solid "+style.at("buttons_base")+";"
        "}"
        "QPushButton#button_StatsGlob_Apache,"
        "QPushButton#button_StatsGlob_Nginx,"
        "QPushButton#button_StatsGlob_Iis {"
        "   border-radius: 4px;"
        "   background-color: "+style.at("buttons_base")+";"
        "}"
        "QPushButton#button_StatsGlob_Apache::flat,"
        "QPushButton#button_StatsGlob_Nginx::flat,"
        "QPushButton#button_StatsGlob_Iis::flat {"
        "   border: 1px solid "+style.at("buttons_base")+";"
        "   color: "+style.at("text_secondary")+";"
        "   background-color: "+style.at("window_base_secondary")+";"
        "}"
        "QPushButton#button_StatsGlob_Apache::flat:hover,"
        "QPushButton#button_StatsGlob_Nginx::flat:hover,"
        "QPushButton#button_StatsGlob_Iis::flat:hover {"
        "   border: 1px solid "+style.at("border_secondary")+";"
        "   color: "+style.at("text_primary")+";"
        "}"
        "QPushButton#button_StatsGlob_Apache::disabled,"
        "QPushButton#button_StatsGlob_Nginx::disabled,"
        "QPushButton#button_StatsGlob_Iis::disabled {"
        "   color: "+style.at("text_disabled")+";"
        "   background-color: "+style.at("buttons_base_disabled")+";"
        "}"
        "QScrollArea#scrollArea_StatsGlob_Recur,"
        "QScrollArea#scrollArea_StatsGlob_Perf,"
        "QScrollArea#scrollArea_StatsGlob_Traffic,"
        "QScrollArea#scrollArea_StatsGlob_Work {"
        "   border: 1px solid "+style.at("globals_frames_border")+";"
        "   background-color: "+style.at("globals_frames_base")+";"
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
        "QFrame#gline_StatsRelat_1,"
        "QFrame#gline_StatsGlob_Recur_1,"
        "QFrame#gline_StatsGlob_Recur_2,"
        "QFrame#gline_StatsGlob_Recur_3,"
        "QFrame#gline_StatsGlob_Perf_1,"
        "QFrame#gline_StatsGlob_Perf_2,"
        "QFrame#gline_StatsGlob_Traffic_1,"
        "QFrame#gline_StatsGlob_Traffic_2,"
        "QFrame#gline_StatsGlob_Work_1,"
        "QFrame#gline_StatsGlob_Work_2 {"
        "   border: 1px solid "+style.at("globals_frames_base")+";"
        "   background-color: "+style.at("globals_frames_lines")+";"
        "}"
        "QFrame#gline_StatsSpeed_1,"
        "QFrame#gline_StatsCount_1 {"
        "   border: 1px solid "+style.at("globals_frames_base")+";"
        "   margin: 2px 0px 3px 0px;"
        "   background-color: "+style.at("globals_frames_lines")+";"
        "}"
        /////////////////
        //// CONFIGS ////
        // outer tabs
        "QTabWidget#ConfTabs QTabBar::tab {"
        "   margin-bottom: 4px;"
        "   padding: 8px 6px;"
        "   border: 1px solid "+style.at("outer-tabs_tab_border")+";"
        "   border-left: 4px solid "+style.at("outer-tabs_tab_border")+";"
        "   border-right: 0px;"
        "   border-top-left-radius: 4px;"
        "   border-bottom-left-radius: 4px;"
        "}"
        "QTabWidget#ConfTabs QTabBar::tab:selected {"
        "   color: "+style.at("outer-tabs_tab_text_selected")+";"
        "}"
        "QTabWidget#ConfTabs QTabBar::tab:!selected {"
        "   border-color: "+style.at("outer-tabs_tab_border_unselected")+";"
        "   border-left: 1px solid "+style.at("outer-tabs_tab_border_unselected")+";"
        "   color: "+style.at("outer-tabs_tab_text_unselected")+";"
        "}"
        "QTabWidget#ConfTabs QTabBar::tab:!selected:hover {"
        "   border-color: "+style.at("outer-tabs_tab_border")+";"
        "   color: "+style.at("outer-tabs_tab_text_hover")+";"
        "}"
        // inner tabs
        "QTabWidget#tabs_ConfGeneral QTabBar::tab,"
        "QTabWidget#tabs_ConfLogs QTabBar::tab {"
        "   margin: 0px;"
        "   margin-right: 4px;"
        "   padding: 6px 8px;"
        "   border: 1px solid "+style.at("inner-tabs_border")+";"
        "   border-bottom: 0px;"
        "   border-top-left-radius: 4px;"
        "   border-bottom-left-radius: 0px;"
        "   border-top-right-radius: 4px;"
        "   background-color: "+style.at("inner-tabs_tab_base")+";"
        "}"
        "QTabWidget#tabs_ConfGeneral QTabBar::tab:!selected,"
        "QTabWidget#tabs_ConfLogs QTabBar::tab:!selected {"
        "   background-color: "+style.at("inner-tabs_tab_base_unselected")+";"
        "}"
        "QTabWidget#tabs_ConfGeneral::pane,"
        "QTabWidget#tabs_ConfLogs::pane {"
        "   border: 1px solid "+style.at("inner-tabs_border")+";"
        "   border-top-right-radius: 4px;"
        "   border-bottom-left-radius: 4px;"
        "   border-bottom-right-radius: 4px;"
        "   background-color: "+style.at("inner-tabs_pane_base")+";"
        "}"
        // sub tabs
        "QTabWidget#tabs_ConfApache QTabBar,"
        "QTabWidget#tabs_ConfNginx QTabBar,"
        "QTabWidget#tabs_ConfIis QTabBar {"
        "   border: 0px;"
        "}"
        "QTabWidget#tabs_ConfApache QTabBar::tab,"
        "QTabWidget#tabs_ConfNginx QTabBar::tab,"
        "QTabWidget#tabs_ConfIis QTabBar::tab {"
        "   background-color: "+style.at("sub-tabs_tab_base")+";"
        "}"
        "QTabWidget#tabs_ConfApache QTabBar::tab:!selected,"
        "QTabWidget#tabs_ConfNginx QTabBar::tab:!selected,"
        "QTabWidget#tabs_ConfIis QTabBar::tab:!selected {"
        "   border: 1px solid "+style.at("sub-tabs_tab_border_unselected")+";"
        "   background-color: "+style.at("sub-tabs_tab_base_unselected")+";"
        "}"
        "QTabWidget#tabs_ConfApache::pane,"
        "QTabWidget#tabs_ConfNginx::pane,"
        "QTabWidget#tabs_ConfIis::pane {"
        "   border: 1px solid "+style.at("sub-tabs_border")+";"
        "   background-color: "+style.at("sub-tabs_pane_base")+";"
        "}"
        // panes
        "QWidget#confGeneral,"
        "QWidget#confLogs {"
        "   border: 1px solid "+style.at("outer-tabs_tab_border")+";"
        "   border-top-right-radius: 4px;"
        "   border-bottom-left-radius: 4px;"
        "   border-bottom-right-radius: 4px;"
        "   background-color: "+style.at("outer-tabs_base")+";"
        "}"
        "QWidget#tab_confWindow,"
        "QWidget#tab_confTextBrowser,"
        "QWidget#tab_confCharts,"
        "QWidget#tab_confDatabases,"
        "QWidget#tab_confLogs_Default,"
        "QWidget#tab_confLogs_Control,"
        "QWidget#tab_confLogs_Apache,"
        "QWidget#tab_confLogs_Nginx,"
        "QWidget#tab_confLogs_Iis {"
        "   border: 0px;"
        "   background-color: "+style.at("inner-tabs_base")+";"
        "}"
        "QWidget#tab_confApache_Paths,"
        "QWidget#tab_confApache_Formats,"
        "QWidget#tab_confApache_Warnlists,"
        "QWidget#tab_confApache_Blacklists,"
        "QWidget#tab_confNginx_Paths,"
        "QWidget#tab_confNginx_Formats,"
        "QWidget#tab_confNginx_Warnlists,"
        "QWidget#tab_confNginx_Blacklists,"
        "QWidget#tab_confIis_Paths,"
        "QWidget#tab_confIis_Formats,"
        "QWidget#tab_confIis_Warnlists,"
        "QWidget#tab_confIis_Blacklists {"
        "   background-color: "+style.at("sub-tabs_base")+";"
        "}"
        // line separators
        "QFrame#gline_ConfTextBrowser {"
        "   border: 2px solid "+style.at("inner-tabs_base")+";"
        "   margin: 13px 0px 14px 0px;"
        "   background-color: "+style.at("inner-tabs_lines")+";"
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
        "   border: 1px solid "+style.at("border_primary")+";"
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
        "   border-color: "+style.at("border_secondary")+";"
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
        "   border-color: "+style.at("buttons_base")+";"
        "   background-color: "+style.at("buttons_base_disabled")+";"
        "   icon: url(:/icons/icons/midtone/save.png);"
        "}"
        // sample buttons
        "QPushButton#button_ConfApache_Format_Sample,"
        "QPushButton#button_ConfNginx_Format_Sample,"
        "QPushButton#button_ConfIis_Format_Sample {"
        "   border: 1px solid "+style.at("border_primary")+";"
        "   border-radius: 4px;"
        "   background-color: "+style.at("buttons_base")+";"
        "}"
        "QPushButton#button_ConfApache_Format_Sample:hover,"
        "QPushButton#button_ConfNginx_Format_Sample:hover,"
        "QPushButton#button_ConfIis_Format_Sample:hover {"
        "   border-color: "+style.at("border_secondary")+";"
        "   background-color: "+style.at("buttons_base_hover")+";"
        "}"
        // sample previews
        "QWidget#scrollAreaContent_ConfApache_Format_Sample,"
        "QWidget#scrollAreaContent_ConfNginx_Format_Sample,"
        "QWidget#scrollAreaContent_ConfIis_Format_Sample {"
        "   border: 1px solid "+style.at("border_primary")+";"
        "   background-color: "+style.at("textbrowser_base")+";"
        "}"
        // help buttons
        "QPushButton#button_ConfApache_Format_Help,"
        "QPushButton#button_ConfNginx_Format_Help,"
        "QPushButton#button_ConfIis_Format_Help {"
        "   border: 1px solid "+style.at("border_primary")+";"
        "   border-radius: 4px;"
        "   background-color: "+style.at("buttons_base")+";"
        "}"
        "QPushButton#button_ConfApache_Format_Help:hover,"
        "QPushButton#button_ConfNginx_Format_Help:hover,"
        "QPushButton#button_ConfIis_Format_Help:hover {"
        "   border-color: "+style.at("border_secondary")+";"
        "   background-color: "+style.at("buttons_base_hover")+";"
        "}"
        // warnlists / blacklists
        "QListWidget#list_ConfApache_Warnlist_List,"
        "QListWidget#list_ConfApache_Blacklist_List,"
        "QListWidget#list_ConfNginx_Warnlist_List,"
        "QListWidget#list_ConfNginx_Blacklist_List,"
        "QListWidget#list_ConfIis_Warnlist_List,"
        "QListWidget#list_ConfIis_Blacklist_List {"
        "   padding: 4px;"
        "   border: 1px solid "+style.at("tables_border")+";"
        "}"
        "QListWidget#list_ConfApache_Warnlist_List::disabled,"
        "QListWidget#list_ConfApache_Blacklist_List::disabled,"
        "QListWidget#list_ConfNginx_Warnlist_List::disabled,"
        "QListWidget#list_ConfNginx_Blacklist_List::disabled,"
        "QListWidget#list_ConfIis_Warnlist_List::disabled,"
        "QListWidget#list_ConfIis_Blacklist_List::disabled {"
        "   border: 1px solid "+style.at("tables_border_disabled")+";"
        "   color: "+style.at("tables_text_disabled")+";"
        "   background-color: "+style.at("tables_base_disabled")+";"
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
        "   border: 1px solid "+style.at("border_primary")+";"
        "   border-radius: 4px;"
        "   background-color: "+style.at("buttons_base")+";"
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
        "   border-color: "+style.at("border_secondary")+";"
        "   background-color: "+style.at("buttons_base_hover")+";"
        "}"
        "QPushButton#button_ConfApache_Warnlist_Add::disabled,"
        "QPushButton#button_ConfApache_Blacklist_Add::disabled,"
        "QPushButton#button_ConfNginx_Warnlist_Add::disabled,"
        "QPushButton#button_ConfNginx_Blacklist_Add::disabled,"
        "QPushButton#button_ConfIis_Warnlist_Add::disabled,"
        "QPushButton#button_ConfIis_Blacklist_Add::disabled {"
        "   border-color: "+style.at("buttons_base")+";"
        "   background-color: "+style.at("buttons_base_disabled")+";"
        "   icon: url(:/icons/icons/add.png), #fff;"
        "}"
        "QPushButton#button_ConfApache_Warnlist_Remove::disabled,"
        "QPushButton#button_ConfApache_Blacklist_Remove::disabled,"
        "QPushButton#button_ConfNginx_Warnlist_Remove::disabled,"
        "QPushButton#button_ConfNginx_Blacklist_Remove::disabled,"
        "QPushButton#button_ConfIis_Warnlist_Remove::disabled,"
        "QPushButton#button_ConfIis_Blacklist_Remove::disabled {"
        "   border-color: "+style.at("buttons_base")+";"
        "   background-color: "+style.at("buttons_base_disabled")+";"
        "   icon: url(:/icons/icons/rem.png), #fff;"
        "}"
        "QPushButton#button_ConfApache_Warnlist_Up::disabled,"
        "QPushButton#button_ConfApache_Blacklist_Up::disabled,"
        "QPushButton#button_ConfNginx_Warnlist_Up::disabled,"
        "QPushButton#button_ConfNginx_Blacklist_Up::disabled,"
        "QPushButton#button_ConfIis_Warnlist_Up::disabled,"
        "QPushButton#button_ConfIis_Blacklist_Up::disabled {"
        "   border-color: "+style.at("buttons_base")+";"
        "   background-color: "+style.at("buttons_base_disabled")+";"
        "   icon: url(:/icons/icons/up.png), #fff;"
        "}"
        "QPushButton#button_ConfApache_Warnlist_Down::disabled,"
        "QPushButton#button_ConfApache_Blacklist_Down::disabled,"
        "QPushButton#button_ConfNginx_Warnlist_Down::disabled,"
        "QPushButton#button_ConfNginx_Blacklist_Down::disabled,"
        "QPushButton#button_ConfIis_Warnlist_Down::disabled,"
        "QPushButton#button_ConfIis_Blacklist_Down::disabled {"
        "   border-color: "+style.at("buttons_base")+";"
        "   background-color: "+style.at("buttons_base_disabled")+";"
        "   icon: url(:/icons/icons/down.png), #fff;"
        "}";
}
