// Decompiled with JetBrains decompiler
// Type: JbnLib.Lang.Messages
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System;
using System.Threading;

namespace JbnLib.Lang
{
  public static class Messages
  {
    public static string ERROR;
    public static string WARNING;
    public static string EXPECTING;
    public static string GOT;
    public static string FILE;
    public static string COMPARE_STRING;
    public static string COMPARE_ARRAY;
    public static string CONVERT_BYTE;
    public static string CONVERT_INT;
    public static string SET_PROHIBITED;
    public static string PARAM_NOT_SET;
    public static string FILE_NOT_FOUND;
    public static string DIR_NOT_EXIST;
    public static string LINE;
    public static string TOKEN;
    public static string QUOTE_MISSING;
    public static string QCEVERSION_NULL;
    public static string POINT_XY;
    public static string POINT3D_XYZ;
    public static string COLOR_RGB;
    public static string EYEPOSITION_YXZ;
    public static string METHOD_OVERRIDE;
    public static string ANIMATION_TRIS;
    public static string VERSION_ONE;
    public static string INVALID_TRIS;
    public static string NOT_FOUND_IN_COLLECTION;
    public static string OFFSET_NOT_FOUND;
    public static string UPDATE_TO_SUPPORT;
    public static string VERSION_MISMATCH;
    public static string IDENTIFIER_MISMATCH;
    public static string QCE_VERSION_MISMATCH;
    public static string NOT_ROOT;
    public static string INCORRECT_ARCHIVE_ID;
    public static string FILE_NOT_FOUND_IN_ARCHIVE;
    public static string INPUT_CORRUPT;
    public static string NOT_ENOUGH_MEMORY;
    public static string BUFFER_TOO_SMALL;
    public static string UNEXPECTED_ERROR;
    public static string MISSING_LEFT_BRACKETS;
    public static string MISSING_RIGHT_BRACKETS;
    public static string CLASSNAME;
    public static string TWO_SECTIONS;
    public static string THREE_SECTIONS;
    public static string HEX_COLORS;
    public static string RGB_FORMAT;
    public static string NOT_COLOR255;
    public static string THREE_VALUES_SPACES;
    public static string NOT_INT;
    public static string NOT_SOUND;
    public static string NOT_SPRITE;
    public static string NOT_STRING;
    public static string NOT_STUDIO;
    public static string NOT_TARGET_DESTINATION;
    public static string NOT_TARGET_SOURCE;
    public static string SCORE_TEAM_NULL;
    public static string SCORE_NULL;
    public static string COLON_REQUIRED;
    public static string CANNOT_CONNECT;
    public static string CONNECTION_LOST;
    public static string COMMUNICATON_UNSUCCESFUL;
    public static string MAX_IMAGE;

    public static void SetDutch()
    {
      Messages.ERROR = "Fout: ";
      Messages.WARNING = "Waarschuwing: ";
      Messages.EXPECTING = "Verwacht: ";
      Messages.GOT = "Heb: ";
      Messages.COMPARE_STRING = "Bezig met vergelijken van tekst. ";
      Messages.COMPARE_ARRAY = "Bezig met het vergelijken van arrays. ";
      Messages.CONVERT_BYTE = "Waarde(n) kunnen niet omgezet worden naar een byte.";
      Messages.CONVERT_INT = "Waarde(n) kunnen niet omgezet worden naar een integer.";
      Messages.SET_PROHIBITED = "Deze waarde kan nu niet worden aangepast.";
      Messages.PARAM_NOT_SET = "De benodigde parameter is niet gegeven: ";
      Messages.FILE_NOT_FOUND = "Bestand niet gevonden: ";
      Messages.DIR_NOT_EXIST = "Map bestaat niet: ";
      Messages.LINE = "Lijn: ";
      Messages.TOKEN = "Token: ";
      Messages.QUOTE_MISSING = "Er ontbreekt een aanhalingsteken. ";
      Messages.QCEVERSION_NULL = "De variabele 'QceVersion' kan niet leeg zijn terwijl er een QCE bestand wordt geschreven.";
      Messages.POINT_XY = "De variabele 'xy' moet exact twee waarden bevatten.";
      Messages.POINT3D_XYZ = "De variable 'xyz' moet exact drie waarden bevatten.";
      Messages.COLOR_RGB = "De variable 'rgb' moet exact drie waarden bevatten.";
      Messages.EYEPOSITION_YXZ = "De variabele 'yxz' moet exact drie waarden bevatten.";
      Messages.METHOD_OVERRIDE = "Deze methode moet overschreden worden.";
      Messages.ANIMATION_TRIS = "Driehoeken kunnen niet toegevoegd worden aan een SMD-animatie.";
      Messages.VERSION_ONE = "Bestand moet versie 1 zijn.";
      Messages.INVALID_TRIS = "Driehoeken kloppen niet.";
      Messages.NOT_FOUND_IN_COLLECTION = "Niet gevonden in verzameling: ";
      Messages.OFFSET_NOT_FOUND = "Gegeven bij offset niet gevonden: ";
      Messages.UPDATE_TO_SUPPORT = "Update om te ondersteunen: ";
      Messages.VERSION_MISMATCH = "Versies komen niet overeen: ";
      Messages.IDENTIFIER_MISMATCH = "Identifiers komen niet overeen: ";
      Messages.QCE_VERSION_MISMATCH = "De toegewezen Qce is niet compatibel met deze methode.";
      Messages.NOT_ROOT = "De variabele 'root' moet een map genaamd 'ROOT' aanwijzen (hoofdletter-gevoelig).";
      Messages.INCORRECT_ARCHIVE_ID = "Incorrect archief ID.";
      Messages.FILE_NOT_FOUND_IN_ARCHIVE = "Bestand niet gevonden in archief: ";
      Messages.INPUT_CORRUPT = "De invoer is corrupt of onvolledig.";
      Messages.NOT_ENOUGH_MEMORY = "Er is niet genoeg werkgeheugen om de bewerking te voltooien.";
      Messages.BUFFER_TOO_SMALL = "Er is niet genoeg plaats in de output buffer.";
      Messages.UNEXPECTED_ERROR = "Onverwachte fout.";
      Messages.MISSING_LEFT_BRACKETS = "Ontbrekend '[' haakje: ";
      Messages.MISSING_RIGHT_BRACKETS = "Ontbrekend ']' haakje: ";
      Messages.CLASSNAME = "Classname ";
      Messages.TWO_SECTIONS = "Keuzes moeten exact twee secties hebben.";
      Messages.THREE_SECTIONS = "Keuzes moeten exact drie secties hebben.";
      Messages.HEX_COLORS = "Hexadecimale kleuren moeten beginnen met een # teken en moeten exact 7 of 9 tekens lang zijn.";
      Messages.RGB_FORMAT = "Tekst moet in 'R G B', 'R G B L', '#RRGGBB', of '#RRGGBBLL' formaat zijn.";
      Messages.THREE_VALUES_SPACES = "Moet drie waarden hebben,gescheiden door spaties.";
      Messages.NOT_COLOR255 = "Lijn is geen color255.";
      Messages.NOT_INT = "Lijn is geen integer.";
      Messages.NOT_SOUND = "Lijn is geen geluid.";
      Messages.NOT_SPRITE = "Lijn is geen sprite.";
      Messages.NOT_STRING = "Lijn is geen string.";
      Messages.NOT_STUDIO = "Lijn is geen studio.";
      Messages.NOT_TARGET_DESTINATION = "Lijn is geen 'target_destination'.";
      Messages.NOT_TARGET_SOURCE = "Lijn is geen 'target_source'.";
      Messages.SCORE_TEAM_NULL = "ScoreMI6, ScorePhoenix, en/of ScoreOther is leeg.  Heb je al 'RefreshScores()' gedaan?";
      Messages.SCORE_NULL = "Score is leeg.  Heb je al 'RefreshScores()' gedaan?";
      Messages.COLON_REQUIRED = "Het IP en de Port moeten gescheiden worden door een dubbelpunt.";
      Messages.CANNOT_CONNECT = "Kon niet verbinden met de server.";
      Messages.CONNECTION_LOST = "De verbinding is verbroken.";
      Messages.COMMUNICATON_UNSUCCESFUL = "Kon geen gegevens verkrijgen vanaf het gegeven eindpunt.";
      Messages.MAX_IMAGE = "Te veel afbeeldingen om in de sprite te kunnen passen.";
    }

    public static void SetEnglish()
    {
      Messages.ERROR = "Error: ";
      Messages.WARNING = "Warning: ";
      Messages.EXPECTING = "Expecting: ";
      Messages.GOT = "Got: ";
      Messages.FILE = " File: ";
      Messages.COMPARE_STRING = "Comparing strings. ";
      Messages.COMPARE_ARRAY = "Comparing arrays. ";
      Messages.CONVERT_BYTE = "Value(s) cannot be converted to a byte.";
      Messages.CONVERT_INT = "Value(s) cannot be converted to an integer.";
      Messages.SET_PROHIBITED = "This value cannot be altered at this time.";
      Messages.PARAM_NOT_SET = "The required parameter is not set: ";
      Messages.FILE_NOT_FOUND = "File not found: ";
      Messages.DIR_NOT_EXIST = "Directory does not exist: ";
      Messages.LINE = "Line: ";
      Messages.TOKEN = "Token: ";
      Messages.QUOTE_MISSING = "There is a quote missing. ";
      Messages.QCEVERSION_NULL = "Variable 'QceVersion' cannot be null while writing a QCE file.";
      Messages.POINT_XY = "Variable 'xy' must contain exactly two values.";
      Messages.POINT3D_XYZ = "Variable 'xyz' must contain exactly three values.";
      Messages.COLOR_RGB = "Variable 'rgb' must contain exactly three values.";
      Messages.EYEPOSITION_YXZ = "Variable 'yxz' must contain exactly three values.";
      Messages.METHOD_OVERRIDE = "This method must be overridden.";
      Messages.ANIMATION_TRIS = "Triangles cannot be added to an animation SMD.";
      Messages.VERSION_ONE = "File must be version 1.";
      Messages.INVALID_TRIS = "Triangles are not valid.";
      Messages.NOT_FOUND_IN_COLLECTION = "Not found in collection: ";
      Messages.OFFSET_NOT_FOUND = "Item at offset not found: ";
      Messages.UPDATE_TO_SUPPORT = "Update to support: ";
      Messages.VERSION_MISMATCH = "Version mismatch: ";
      Messages.IDENTIFIER_MISMATCH = "Identifier mistmatch: ";
      Messages.QCE_VERSION_MISMATCH = "Provided Qce is not compatible with this method.";
      Messages.NOT_ROOT = "The variable 'root' must point to a folder named 'ROOT' (case-sensitive).";
      Messages.INCORRECT_ARCHIVE_ID = "Incorrect archive ID.";
      Messages.FILE_NOT_FOUND_IN_ARCHIVE = "File not found in the archive: ";
      Messages.INPUT_CORRUPT = "The input data is corrupt or incomplete.";
      Messages.NOT_ENOUGH_MEMORY = "There is not enough memory to complete the operation.";
      Messages.BUFFER_TOO_SMALL = "There is not enough room in the output buffer.";
      Messages.UNEXPECTED_ERROR = "Unexpected error.";
      Messages.MISSING_LEFT_BRACKETS = "Missing '[' brackets: ";
      Messages.MISSING_RIGHT_BRACKETS = "Missing ']' brackets: ";
      Messages.CLASSNAME = "Classname ";
      Messages.TWO_SECTIONS = "Choices must have exactly two sections.";
      Messages.THREE_SECTIONS = "Choices must have exactly three sections.";
      Messages.HEX_COLORS = "Hex colors must start with a # sign and be exactly seven or nine chars in length.";
      Messages.RGB_FORMAT = "String must be in 'R G B', 'R G B L', '#RRGGBB', or '#RRGGBBLL' format.";
      Messages.THREE_VALUES_SPACES = "Must have three values separated by spaces.";
      Messages.NOT_COLOR255 = "Line is not a color255.";
      Messages.NOT_INT = "Line is not an integer.";
      Messages.NOT_SOUND = "Line is not a sound.";
      Messages.NOT_SPRITE = "Line is not a sprite.";
      Messages.NOT_STRING = "Line is not a string.";
      Messages.NOT_STUDIO = "Line is not a studio.";
      Messages.NOT_TARGET_DESTINATION = "Line is not a target_destination.";
      Messages.NOT_TARGET_SOURCE = "Line is not a target_source.";
      Messages.SCORE_TEAM_NULL = "ScoreMI6, ScorePhoenix, and/or ScoreOther is null.  Did you 'RefreshScores()' yet?";
      Messages.SCORE_NULL = "Score is null.  Did you 'RefreshScores()' yet?";
      Messages.COLON_REQUIRED = "The IP and Port must be separated by a colon.";
      Messages.CANNOT_CONNECT = "Could not connect to server.";
      Messages.CONNECTION_LOST = "The connection has been lost.";
      Messages.COMMUNICATON_UNSUCCESFUL = "Could not retrieve any data from the given end point.";
      Messages.MAX_IMAGE = "Too many images to fit in sprite.";
    }

    public static void SetGerman()
    {
      Messages.ERROR = "Fehler: ";
      Messages.WARNING = "Warnung: ";
      Messages.EXPECTING = "Erwartung: ";
      Messages.GOT = "Ergebnis: ";
      Messages.COMPARE_STRING = "vergleiche Worte/Files. ";
      Messages.COMPARE_ARRAY = "Vergleiche Bereiche. ";
      Messages.CONVERT_BYTE = "Inhalte konnten nicht zu einem Byte-Wert konvertiert werden.";
      Messages.CONVERT_INT = "Inhalte passen nicht zu Integer-Inhalten.";
      Messages.SET_PROHIBITED = "Dieser Wert kann hier nicht verändert werden.";
      Messages.PARAM_NOT_SET = "Der erforderliche Parameter fehlt: ";
      Messages.FILE_NOT_FOUND = "Datei nicht gefunden: ";
      Messages.DIR_NOT_EXIST = "Verzeichnis existiert nicht: ";
      Messages.LINE = "Zeile: ";
      Messages.TOKEN = "Link: ";
      Messages.QUOTE_MISSING = "Ein Zeichen fehlt. ";
      Messages.QCEVERSION_NULL = "Variable 'QceVersion' kann nicht 0 sein, weil eine QCE Datei geschrieben wird.";
      Messages.POINT_XY = "Variable 'xy' muss genau 2 Zeichen beinhalten.";
      Messages.POINT3D_XYZ = "Variable 'xyz' muss genau 3 Zeichen beinhalten.";
      Messages.COLOR_RGB = "Variable 'rgb' muss genau 3 Zeichen beinhalten.";
      Messages.EYEPOSITION_YXZ = "Variable 'yxz' muss genau 3 Zeichen beinhalten.";
      Messages.METHOD_OVERRIDE = "Diese Funktion wird geändert.";
      Messages.ANIMATION_TRIS = "Triangles können keiner Animation zugeordnet werden SMD.";
      Messages.VERSION_ONE = "Datei sollte Version 1 sein.";
      Messages.INVALID_TRIS = "Triangles passen nicht.";
      Messages.NOT_FOUND_IN_COLLECTION = "Nicht in der Auflistung: ";
      Messages.OFFSET_NOT_FOUND = "Teil ist nicht im Verzeichnis: ";
      Messages.UPDATE_TO_SUPPORT = "Update to support: ";
      Messages.VERSION_MISMATCH = "Version falsch: ";
      Messages.IDENTIFIER_MISMATCH = "Identifier falsch: ";
      Messages.QCE_VERSION_MISMATCH = "Unterstütztes Qce is passt nicht zu diesem Verfahren.";
      Messages.NOT_ROOT = "Die Vaiable 'root' muss auf ein Verzeichnis namens 'ROOT' (case-sensitive) zeigen.";
      Messages.INCORRECT_ARCHIVE_ID = "Falsche Archiv-ID.";
      Messages.FILE_NOT_FOUND_IN_ARCHIVE = "Datei wurde nicht in den Archiven gefunden: ";
      Messages.INPUT_CORRUPT = "Die erhaltenenen Dateien sind beschädigt oder haben das falsche Format.";
      Messages.NOT_ENOUGH_MEMORY = "Nicht genug Speicher.";
      Messages.BUFFER_TOO_SMALL = "Keine Puffer.";
      Messages.UNEXPECTED_ERROR = "Irgendein scheiss unbekannter Fehler.";
      Messages.MISSING_LEFT_BRACKETS = "Es fehlt '[' klammer: ";
      Messages.MISSING_RIGHT_BRACKETS = "Es fehlt ']' klammer: ";
      Messages.CLASSNAME = "Gruppierung ";
      Messages.TWO_SECTIONS = "Die Auswahl MUSS 2 Sektionen umfassen.";
      Messages.THREE_SECTIONS = "Die Auswahl MUSS 3 Sektionen umfassen.";
      Messages.HEX_COLORS = "Hexfarben müssen mit ein # beginnen und haben genau 7 oder 9 Zeichen Länge.";
      Messages.RGB_FORMAT = "String hat 'R G B', 'R G B L', '#RRGGBB', oder '#RRGGBBLL' format.";
      Messages.THREE_VALUES_SPACES = "Brauche 3 Zähler, getrennt mit Leerzeichen.";
      Messages.NOT_COLOR255 = "Zeile ist nicht color255.";
      Messages.NOT_INT = "Zeile ist nicht integer.";
      Messages.NOT_SOUND = "Zeile ist kein sound.";
      Messages.NOT_SPRITE = "Zeile ist kein sprite.";
      Messages.NOT_STRING = "Zeile ist kein string.";
      Messages.NOT_STUDIO = "Zeile ist kein studio.";
      Messages.NOT_TARGET_DESTINATION = "Zeile ist kein target_destination.";
      Messages.NOT_TARGET_SOURCE = "Zeile ist kein target_source.";
      Messages.SCORE_TEAM_NULL = "ScoreMI6, ScorePhoenix, und/oder ScoreOther ist null.  Willst Du  'RefreshScores()' jetzt?";
      Messages.SCORE_NULL = "Score ist null.  Willst Du 'RefreshScores()' ?";
      Messages.COLON_REQUIRED = "Die IP und der Port müssen mit einem Semikolon getrennt werden.";
      Messages.CANNOT_CONNECT = "Kann den Server nicht erreichen.";
      Messages.CONNECTION_LOST = "Die Verbindung wrde abgebrochen.";
      Messages.COMMUNICATON_UNSUCCESFUL = "Keine Daten vom Clienten erhalten.";
      Messages.MAX_IMAGE = "Kein Platz, um alle Inhalte darzustellen.";
    }

    public static void ThrowException(string location, string message) => throw new Exception(Messages.ERROR + location + " - " + message);

    public static void SetFromCulture()
    {
      switch (Thread.CurrentThread.CurrentCulture.TwoLetterISOLanguageName)
      {
        case "de":
          Messages.SetGerman();
          break;
        case "nl":
          Messages.SetDutch();
          break;
        default:
          Messages.SetEnglish();
          break;
      }
    }
  }
}
