# Translating

<br/>

## Translations

Most of the available translations are automatically generated through a script and are hence imprecise.

If you have a request for a missing language or you're willing to contribute in improving existing translations, please refer to [this issue](https://github.com/elB4RTO/LogDoctor/issues/10).

<br/><br/>

## How to contribute to translations

Since the whole application is build upon Qt, translations are made throught `.ts` [translation files](https://github.com/elB4RTO/LogDoctor/tree/main/logdoctor/translations).

<br/>

The easiest way to go is to use **Qt Linguist**:
- Download or clone this repo<br/>
- Open the `.ts` translation file of your language using QtLinguist<br/>
- Translate (*don't know how? Follow [this video](https://www.youtube.com/watch?v=GNyfkuDchNQ)*)<br/>
- **Push only the `.ts` files, you don't have to release them**: pull request containing `.qm` binaries won't be accepted.<br/>

<br/>

If you don't want to install QtLinguist, you can do it the hard way, by opening the files with a text editor and do it manually:
- Here is a sample of some text waiting for translation<br/>
  You can see the original text enclosed in the `<source>` tags. Don't edit it.<br/>
  ```
      <message>
          <location filename="..." line="n"/>
          <source>This is the original text</source>
      </message>
  ```
- Everything you need to do is to add the missing line containing the translated text, without editing the other lines.<br/>
  Here is a sample after having translated it<br/>
  ```
      <message>
          <location filename="..." line="n"/>
          <source>Original text</source>
          <translation type="unfinished">Translated text goes here</translation>
      </message>
  ```

<br/>

If you don't feel comfortable with any of the above solutions, please open an [issue](https://github.com/elB4RTO/LogDoctor/issues) and write your translations/corrections there, in a clear way.

<br/><br/>

## Translation guidelines

*[hints from Qt](https://doc.qt.io/qt-6/linguist-overview.html)*

Just follow some simple guidelines to ensure a correct and clear contribution:

- Respect the structure of the phrase: if *(for example)* it starts or ends with a whitespace or something, please do the same.

- Please leave your translations marked as *unfinished*, they will be checked and un-marked after having been verified.

- If you see a translation marked as *finished*, please edit it only if you're sure that your definition suits better.

<br/>
