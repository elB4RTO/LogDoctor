from os.path import exists
from argostranslate.translate import translate


MSG_DELIM_BEG = "<message>"
MSG_DELIM_END = "</message>"

SRC_DELIM_BEG = "<source>"
SRC_DELIM_END = "</source>"

TR_DELIM_BEG_FIN  = "<translation>"
TR_DELIM_BEG_UFIN = "<translation type=\"unfinished\">"
TR_DELIM_END      = "</translation>"

SANITIZATIONS = {
    "&amp;"  : "&",
    "&apos;" : "'",
    "&quot;" : '"',
    "&lt;"   : "<",
    "&gt;"   : ">",
}


class Translator:
    """
    Used to process a '.ts' file
    """

    def __init__(self, tr_file:str, tr_from:str, tr_to:str):
        """
        Initializes the translator

        Parameters
        ----------
        tr_file : str
            The path of the '.ts' file to parse and translate.
        tr_from : str
            The code of the language to translate from.
        tr_to : str
            The code of the language to translate to.
        """
        self.file_path : str = tr_file
        self.from_code : str = tr_from
        self.to_code   : str = tr_to
        self.content   : str = ""

    def translate(self, locale_code:str):
        """
        Runs the translation process

        Parameters
        ----------
        locale_code : str
            The code of the locale (for display purposes only).
        """
        if self._check_file_exists() is False:
            return
        self._read_file()
        n_translations = self._parse_content()
        if self._check_file_exists() is False:
            return
        self._write_file()
        print(f"    Translated {n_translations} new message(s)")

    def _check_file_exists(self) -> bool:
        """
        Checks whether the '.ts' file exists on disk

        Returns
        -------
        bool
            True if the file exists, False otherwise.
        """
        if exists(self.file_path) is False:
            print(f"    [Translator] File not found: '{self.file_path}'")
            return False
        return True

    def _read_file(self):
        """
        Reads the content of the file
        """
        with open(self.file_path, 'r') as file:
            self.content = file.read()

    def _write_file(self):
        """
        Writes the content on the file
        """
        with open(self.file_path, 'w') as file:
            file.write(self.content)

    def _parse_content(self) -> int:
        """
        Parses and translates the content

        Returns
        -------
        int
            The number of translated messages.
        """
        n_translations = 0
        start = stop = 0
        size = len(self.content)
        while start < size:
            (start,stop) = self._find_message(start)
            if start is None:
                break
            elif stop is None:
                continue
            message = Message(self.content[start:stop], self.file_path)
            new_content_slice = message.update(self.from_code, self.to_code)
            if new_content_slice is not None:
                self.content = self.content[:start] + new_content_slice + self.content[stop:]
                old_content_len = stop - start
                content_len_diff = len(new_content_slice) - old_content_len
                stop += content_len_diff
                size += content_len_diff
                n_translations += 1
            start = stop
        return n_translations

    def _find_message(self, idx:int) -> (int,int):
        """
        Searches the indexes that delimit the next message structure

        Parameters
        ----------
        idx : int
            The index to start searching from.

        Returns
        -------
        (int,int)
            The indexes representing the beginning and the end of the next message structure.
            Can be None.
        """
        start = self.content.find(MSG_DELIM_BEG, idx)
        if start == -1:
            return (None,None)
        stop = self.content.find(MSG_DELIM_END, start)
        if stop == -1:
            start += len(MSG_DELIM_BEG)
            print(f"    [Translator] Message end delimiter not found: '{self.file_path}'")
            return (start,None)
        stop += len(MSG_DELIM_END)
        dupl = self.content.find(MSG_DELIM_BEG, start+len(MSG_DELIM_BEG), stop)
        if dupl >= 0 and dupl < stop:
            print(f"    [Translator] Message end delimiter not found: '{self.file_path}'")
            return (dupl,None)
        return (start,stop)


class Message:
    """
    Represents a message structure of a '.ts' file
    """

    def __init__(self, message:str, file:str):
        """
        Initializes the message

        Parameters
        ----------
        message : str
            The original message structure.
        file : str
            The path of the origin file (for display purposes only).
        """
        self.file_path         : str  = file
        self.source            : str  = None
        self.needs_translation : bool = False
        self._process(message)

    def update(self, from_code:str, to_code:str) -> str:
        """
        Updates the message by translating the source text if needed

        Parameters
        ----------
        from_code : str
            The language code of the source text.
        to_code : str
            The language code of the translation text.

        Returns
        -------
        str
            The updated message structure if an update is needed,
            or None otherwise.
        """
        if self.needs_translation is False or self.source is None:
            return None
        if to_code == "en":
            translation = self.source
        else:
            _source = self._desanitize(self.source)
            _translation = translate(_source, from_code, to_code)
            translation = self._sanitize(_translation)
        message  = f"{MSG_DELIM_BEG}\n"
        message += f"        {SRC_DELIM_BEG}{self.source}{SRC_DELIM_END}\n"
        message += f"        {TR_DELIM_BEG_UFIN}{translation}{TR_DELIM_END}\n"
        message += f"    {MSG_DELIM_END}"
        return message

    def _process(self, message:str):
        """
        Processes a message structure

        Parameters
        ----------
        message : str
            The message structure.
        """
        self._find_source_text(message)
        if self.source is None:
            return
        self._check_translation(message)

    def _find_source_text(self, message:str):
        """
        Searches for the source text

        Parameters
        ----------
        message : str
            The message structure.
        """
        src_start = message.find(SRC_DELIM_BEG)
        if src_start == -1:
            print(f"    [Translator] Source begin delimiter not found: '{self.file_path}'")
            return
        src_start += len(SRC_DELIM_BEG)
        src_stop = message.find(SRC_DELIM_END, src_start)
        if src_stop == -1:
            print(f"    [Translator] Source end delimiter not found: '{self.file_path}'")
            return
        self.source = message[src_start:src_stop]

    def _check_translation(self, message:str):
        """
        Checks whether the message structure already contains a translation

        Parameters
        ----------
        message : str
            The message structure.
        """
        beg_delim_len = len(TR_DELIM_BEG_FIN)
        tr_start = message.find(TR_DELIM_BEG_FIN)
        if tr_start == -1:
            beg_delim_len = len(TR_DELIM_BEG_UFIN)
            tr_start = message.find(TR_DELIM_BEG_UFIN)
        if tr_start == -1:
            self.needs_translation = True
            return
        tr_stop = message.find(TR_DELIM_END)
        if tr_stop == -1:
            print(f"    [Translator] Translation end delimiter missing: '{self.file_path}'")
            return
        elif tr_start > tr_stop:
            print(f"    [Translator] Message is ill-formed: '{self.file_path}'")
            return
        tr_start += beg_delim_len
        if tr_start == tr_stop:
            self.needs_translation = True

    def _desanitize(self, text:str) -> str:
        """
        De-sanitizes the given text

        Parameters
        ----------
        text : str
            The text to de-sanitize.

        Returns
        -------
        str
            The de-sanitezed text.
        """
        for (code,plain) in SANITIZATIONS.items():
            text = text.replace(code, plain)
        return text

    def _sanitize(self, text:str) -> str:
        """
        Sanitizes the given text

        Parameters
        ----------
        text : str
            The text to sanitize.

        Returns
        -------
        str
            The sanitezed text.
        """
        for (code,plain) in SANITIZATIONS.items():
            text = text.replace(plain, code)
        return text
