#!/bin/bash

# Testskript für das Programm "readDir"
# Erwartet die ausführbare Datei unter ./build/Debug/outDebug.

EXEC="./build/Debug/outDebug"
if [[ ! -x "$EXEC" ]]; then
    echo "Fehler: $EXEC existiert nicht oder ist nicht ausführbar."
    exit 1
fi

echo "==> Starte Testläufe für readDir"
echo

# Hilfsfunktion: Trenner
sep() {
    echo "---------------------------------------------"
}

# Programm aufrufen und seinen Rückgabewert mit dem erwarteten Wert vergleichen.
TESTS_OK=0
TESTS_NOK=0
run_test() {
    local expected="$1"
    local actual
    shift

    if "$EXEC" "$@"; then
        actual=0
    else
        actual=$?
    fi

    if [[ "$actual" -eq "$expected" ]]; then
        echo "Test OK (Rückgabewert: $actual)"
        TESTS_OK=$((TESTS_OK + 1))
    else
        echo "Test NOK (Rückgabewert: $actual, erwartet: $expected)"
        TESTS_NOK=$((TESTS_NOK + 1))
    fi
}

# Arbeitsverzeichnis vorbereiten
TESTDIR="test_readDir_env"
rm -rf "$TESTDIR"
mkdir "$TESTDIR"

# ----------------------------------------------------
sep
echo "TEST 1: Standardverhalten – aktuelles Verzeichnis"
sep
run_test 0
echo

# ----------------------------------------------------
sep
echo "TEST 2: Leeres Verzeichnis"
sep
mkdir "$TESTDIR/empty"
run_test 0 "$TESTDIR/empty"
echo

# ----------------------------------------------------
sep
echo "TEST 3: Verzeichnis mit Dateien und Unterverzeichnissen"
sep
mkdir "$TESTDIR/mixed"
touch "$TESTDIR/mixed/a.txt"
touch "$TESTDIR/mixed/b.bin"
mkdir "$TESTDIR/mixed/subdir"
run_test 0 "$TESTDIR/mixed"
echo

# ----------------------------------------------------
sep
echo "TEST 4: Nicht-existierendes Verzeichnis"
sep
run_test 1 "$TESTDIR/does_not_exist"
echo

# ----------------------------------------------------
sep
echo "TEST 5: Pfad zeigt auf Datei statt Verzeichnis"
sep
echo "Hallo Welt" > "$TESTDIR/file.txt"
run_test 1 "$TESTDIR/file.txt"
echo

# ----------------------------------------------------
sep
echo "TEST 6: Verzeichnis ohne Leserechte (r-Bit fehlt)"
sep
mkdir "$TESTDIR/noread"
touch "$TESTDIR/noread/x"
chmod a-r "$TESTDIR/noread"

run_test 1 "$TESTDIR/noread"

# Rechte wiederherstellen, damit Cleanup möglich bleibt
chmod u+r "$TESTDIR/noread"
echo

# ----------------------------------------------------
sep
echo "TEST 7: Verzeichnis ohne Execute-Rechte (x-Bit fehlt)"
sep
mkdir "$TESTDIR/nosearch"
touch "$TESTDIR/nosearch/z"
chmod a-x "$TESTDIR/nosearch"  # a steht für alle (u,g,o)

# Das Auflisten der Namen benötigt Leserechte, aber keine Execute-Rechte.
run_test 0 "$TESTDIR/nosearch"

# Rechte wiederherstellen
chmod u+x "$TESTDIR/nosearch"
echo

# ----------------------------------------------------
sep
echo "TESTS FERTIG"
sep

if [[ "$TESTS_NOK" -eq 0 ]]; then
    echo "Alle $TESTS_OK Tests waren positiv."
    exit 0
else
    echo "Nicht alle Tests waren positiv: $TESTS_OK OK, $TESTS_NOK NOK."
    exit 1
fi
