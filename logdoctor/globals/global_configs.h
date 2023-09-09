#ifndef GLOBAL_CONFIGS_H
#define GLOBAL_CONFIGS_H


typedef unsigned char themes_t;


enum class WindowTheme : themes_t {
    Native = 0,
    Light  = 1,
    Dark   = 2
};

enum class IconsTheme : themes_t {
    Auto  = 0,
    Light = 1,
    Dark  = 2
};

enum class ChartsTheme : themes_t {
    Light = 0,
    Dark  = 1
};


namespace GlobalConfigs {

    extern WindowTheme window_theme;

    extern IconsTheme icons_theme;

    extern ChartsTheme charts_theme;

} // namespace GlobalConfigs

#endif // GLOBAL_CONFIGS_H
