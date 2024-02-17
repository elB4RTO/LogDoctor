#ifndef LOGDOCTOR__GLOBALS__GLOBAL_CONFIGS_H
#define LOGDOCTOR__GLOBALS__GLOBAL_CONFIGS_H


class QString;

typedef unsigned char themes_t;


//! Available themes fot the window
enum class WindowTheme : themes_t {
    Native = 0,
    Light  = 1,
    Dark   = 2
};

//! Available themes for the icons
enum class IconsTheme : themes_t {
    Auto  = 0,
    Light = 1,
    Dark  = 2
};

//! Available themes for the charts
enum class ChartsTheme : themes_t {
    Light = 0,
    Dark  = 1
};

//! Available color schemes for the text browsers
enum class ColorsScheme : themes_t {
    None    = 0,
    Breeze  = 1,
    Monokai = 2,
    Radical = 3
};


//! GlobalConfigs
/*!
    Globally shared configurations
*/
namespace GlobalConfigs
{

extern WindowTheme window_theme;

extern IconsTheme icons_theme;

extern ChartsTheme charts_theme;

extern QString icons_set;

extern ColorsScheme colors_scheme;

} // namespace GlobalConfigs


#endif // LOGDOCTOR__GLOBALS__GLOBAL_CONFIGS_H
