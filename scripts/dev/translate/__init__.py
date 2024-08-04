import os
import argostranslate.package

from translator import Translator


TRANSLATIONS = {
    "ar_AR": ("en", "ar"),  # Arabic (classic)
    "az_AZ": ("en", "az"),  # Azerbaijani (Azerbaijan)
    "bg_BG": ("en", "bg"),  # Bulgarian (Bulgaria)
    "bn_BN": ("en", "bn"),  # Bengali (Bangladesh)
    "ca_SP": ("en", "ca"),  # Catalan (Spain)
    "cs_CZ": ("en", "cs"),  # Czech (Czech Republic)
    "da_DA": ("en", "da"),  # Danish (Denmark)
    "de_GE": ("en", "de"),  # German (Germany)
    "el_GR": ("en", "el"),  # Greek (modern) (Greece)
    "en_GB": ("en", "en"),  # English (United Kingdom)
    "eo_EO": ("en", "eo"),  # Esperanto
    "es_SP": ("en", "es"),  # Spanish (Spain)
    "et_ES": ("en", "et"),  # Estonian (Estonia)
    "fa_PE": ("en", "fa"),  # Persian (Persia)
    "fi_FI": ("en", "fi"),  # Finnish (Finland)
    "fr_FR": ("en", "fr"),  # French (France)
    "ga_IR": ("en", "ga"),  # Irish (Ireland)
    "he_HE": ("en", "he"),  # Hebrew
    "hi_IN": ("en", "hi"),  # Hindi (India)
    "hu_HU": ("en", "hu"),  # Hungarian (Hungary)
    "id_ID": ("en", "id"),  # Indonesian (Indonesia)
    "it_IT": ("en", "it"),  # Italian (Italy)
    "ja_JP": ("en", "ja"),  # Japanese (Japan)
    "ko_KO": ("en", "ko"),  # Korean (Korea)
    "lt_LI": ("en", "lt"),  # Lithuanian (Lithuania)
    "lv_LA": ("en", "lv"),  # Latvian (Latvia)
    "ms_MA": ("en", "ms"),  # Malay (Malaysia)
    "nb_NB": ("en", "nb"),  # Norwegian (Norway)
    "nl_NL": ("en", "nl"),  # Dutch (Netherland)
    "pl_PL": ("en", "pl"),  # Polish (Poland)
    "pt_BR": ("en", "pt"),  # Portuguese (Brazil)
    "pt_PT": ("en", "pt"),  # Portuguese (Portugal)
    "ro_RO": ("en", "ro"),  # Romanian (Romania)
    "ru_RU": ("en", "ru"),  # Russian (Russia)
    "sk_SK": ("en", "sk"),  # Slovak (Slovakia)
    "sl_SL": ("en", "sl"),  # Slovenian (Slovenia)
    "sq_AL": ("en", "sq"),  # Albanian (Albania)
    "sv_SV": ("en", "sv"),  # Swedish (Sweden)
    "th_TH": ("en", "th"),  # Thai (Thailand)
    "tl_PH": ("en", "tl"),  # Tagalog (Philippines)
    "tr_TR": ("en", "tr"),  # Turkish (Turkey)
    "uk_UK": ("en", "uk"),  # Ukranian (Ukraine)
    "ur_PA": ("en", "ur"),  # Urdu (Pakistan)
    "zh_CN": ("en", "zh"),  # Chinese (cantonese) (China)
    "zt_CN": ("en", "zt"),  # Chinese (traditional) (China)
}

PACKAGES = sorted([pkg.to_code
                   for pkg in argostranslate.package.get_installed_packages()
                   if pkg.from_code == "en"])


def main():
    print("*** STARTING TRANSLATION ***")
    project_root = os.path.abspath(os.path.abspath(__file__) + "/../../../..")
    translations_path = project_root + "/logdoctor/translations"
    for (locale, (code_from, code_to)) in TRANSLATIONS.items():
        if code_to != "en" and code_to not in PACKAGES:
            print(f"Failed to translate '{locale}': missing package")
            continue
        file_path = f"{translations_path}/LogDoctor_{locale}.ts"
        if not os.path.exists(file_path):
            print(f"Cannot translate '{locale}': ts file not found")
            continue
        print(f"Updating 'logdoctor/translations/LogDoctor_{locale}.ts'...")
        Translator(file_path, code_from, code_to).translate(locale)


if __name__ == "__main__":
    main()
