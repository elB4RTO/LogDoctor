#ifndef LOGDOCTOR__CUSTOMS__LANGUAGES_LISTMODEL_H
#define LOGDOCTOR__CUSTOMS__LANGUAGES_LISTMODEL_H


#include <QAbstractListModel>


class LanguagesListModel final : public QAbstractListModel
{
    Q_OBJECT

    using data_pair_t = std::pair<std::string,QString>;
    using model_data_t = std::vector<data_pair_t>;

public:
    explicit LanguagesListModel(QObject* parent=nullptr);

    //! Returns the index in the model of the given language code
    /*!
        \param lang The language code
        \return The index in the model
        \throw DoNotCatchException
    */
    int indexOfLanguage( const std::string& lang ) const;

    int rowCount( const QModelIndex& parent=QModelIndex() ) const override;
    QVariant data( const QModelIndex& index, int role ) const override;

private:
    const model_data_t model_data{
        {"sq_AL", QStringLiteral("Albanian (Albania)")},
        {"ar_AR", QStringLiteral("Arabic (classic)")},
        {"az_AZ", QStringLiteral("Azerbaijani (Azerbaijan)")},
        {"bn_BN", QStringLiteral("Bengali (Bangladesh)")},
        {"bg_BG", QStringLiteral("Bulgarian (Bulgaria)")},
        {"ca_SP", QStringLiteral("Catalan (Spain)")},
        {"zh_CN", QStringLiteral("Chinese (cantonese) (China)")},
        {"zt_CN", QStringLiteral("Chinese (traditional) (China)")},
        {"cs_CZ", QStringLiteral("Czech (Czech Republic)")},
        {"da_DA", QStringLiteral("Danish (Denmark)")},
        {"nl_NL", QStringLiteral("Dutch (Netherland)")},
        {"en_GB", QStringLiteral("English (United Kingdom)")},
        {"eo_EO", QStringLiteral("Esperanto")},
        {"et_ES", QStringLiteral("Estonian (Estonia)")},
        {"fi_FI", QStringLiteral("Finnish (Finland)")},
        {"fr_FR", QStringLiteral("French (France)")},
        {"de_GE", QStringLiteral("German (Germany)")},
        {"el_GR", QStringLiteral("Greek (modern) (Greece)")},
        {"he_HE", QStringLiteral("Hebrew")},
        {"hi_IN", QStringLiteral("Hindi (India)")},
        {"hu_HU", QStringLiteral("Hungarian (Hungary)")},
        {"id_ID", QStringLiteral("Indonesian (Indonesia)")},
        {"ga_IR", QStringLiteral("Irish (Ireland)")},
        {"it_IT", QStringLiteral("Italian (Italy)")},
        {"ja_JP", QStringLiteral("Japanese (Japan)")},
        {"ko_KO", QStringLiteral("Korean (Korea)")},
        {"lt_LI", QStringLiteral("Lithuanian (Lithuania)")},
        {"lv_LA", QStringLiteral("Latvian (Latvia)")},
        {"ms_MA", QStringLiteral("Malay (Malaysia)")},
        {"nb_NB", QStringLiteral("Norwegian (Norway)")},
        {"fa_PE", QStringLiteral("Persian (Persia)")},
        {"pl_PL", QStringLiteral("Polish (Poland)")},
        {"pt_BR", QStringLiteral("Portuguese (Brazil)")},
        {"pt_PT", QStringLiteral("Portuguese (Portugal)")},
        {"ro_RO", QStringLiteral("Romanian (Romania)")},
        {"ru_RU", QStringLiteral("Russian (Russia)")},
        {"sk_SK", QStringLiteral("Slovak (Slovakia)")},
        {"sl_SL", QStringLiteral("Slovenian (Slovenia)")},
        {"es_SP", QStringLiteral("Spanish (Spain)")},
        {"sv_SV", QStringLiteral("Swedish (Sweden)")},
        {"tl_PH", QStringLiteral("Tagalog (Philippines)")},
        {"th_TH", QStringLiteral("Thai (Thailand)")},
        {"tr_TR", QStringLiteral("Turkish (Turkey)")},
        {"uk_UK", QStringLiteral("Ukranian (Ukraine)")},
        {"ur_PA", QStringLiteral("Urdu (Pakistan)")}
    };
};


#endif // LOGDOCTOR__CUSTOMS__LANGUAGES_LISTMODEL_H
