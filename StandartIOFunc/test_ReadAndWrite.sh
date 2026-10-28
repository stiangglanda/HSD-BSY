#!/bin/bash
# Skript zum Testen von ReadAndWrite.c


# entfernen erzeugter Dateien
rm -f -- ./readWrite ./pers_data.bin /personen daten.bin

ok=0
nok=0

# Exit-Status pruefen: $1 = tatsaechlich, $2 = erwartet
check_status()
{
    echo "Exit-Status: $1"
    if [ "$1" -eq "$2" ]; then
        echo "Test -> OK"
        ok=$((ok + 1))
    else
        echo "Test -> NOK (erwartet: $2)"
        nok=$((nok + 1))
    fi
}

# Programm erzeugen
echo "Kompilieren von ReadAndWrite.c"
gcc -Wall -o readWrite ReadAndWrite.c -Wall -Wextra
if [ $? -ne 0 ]; then
    echo "ERROR: Kompilierung fehlgeschlagen!"
    exit 1
fi

# Kommandozeile testen
echo
echo "Keine Argumente (Fehler erwartet):"
"./readWrite"
check_status $? 1

echo
echo "Zu viele Argumente (Fehler erwartet):"
"./readWrite" a b
check_status $? 1

# Fehler beim Oeffnen testen
echo
echo "Leerer Dateiname (Fehler erwartet):"
"./readWrite" ""
check_status $? 1

echo
echo "Verzeichnis als Dateiname (Fehler erwartet):"
"./readWrite" ".."
check_status $? 1

# Datei schreiben und lesen
echo
echo "Neue Datei schreiben und lesen:"
"./readWrite" "./pers_data.bin"
check_status $? 0

echo
echo "Pruefen, ob die Datei vorhanden und nicht leer ist:"
test -s "./pers_data.bin"
check_status $? 0

echo
echo "Bestehende Datei erneut schreiben und lesen:"
"./readWrite" "./pers_data.bin"
check_status $? 0

echo
echo "Dateiname mit Leerzeichen:"
"./readWrite" "./personen daten.bin"
check_status $? 0

echo
echo "Ergebnis: $ok OK | $nok NOK"
[ "$nok" -eq 0 ]

