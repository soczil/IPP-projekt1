#!/bin/bash

VALGRIND="valgrind --error-exitcode=15 --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all"

if [ $# -eq 2 ]; then

    PROGRAM=$1
    DIRECTORY=$2

    if [ -d $DIRECTORY ] && [ -f $PROGRAM ]; then
        for f in $DIRECTORY/*.in;
        do
            $VALGRIND -q ./$PROGRAM <$f 1>check.out 2>check.err;

            EXIT_CODE=$?

            if diff -q check.out ${f%in}out && diff -q check.err ${f%in}err; then
                echo "Test $f: OK. Kod zakończenia programu: $EXIT_CODE.";
            else
                echo "Plik $f: ERROR";
            fi;

            rm -f check.out
            rm -f check.err
        done
    else
        if [ -d $DIRECTORY ]; then
            echo "Nie ma takiego pliku.";
        else
            echo "Nie ma takiego katalogu.";
        fi;
    fi;
else
    if [ $# -gt 2 ]; then
        echo "Za dużo parametrów.";
    else
        echo "Za mało parametrów.";
    fi;
fi;