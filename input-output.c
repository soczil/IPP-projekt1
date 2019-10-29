#define _GNU_SOURCE
#define __STDC_FORMAT_MACROS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <errno.h>

#include "input-output.h"
#include "quantum-history-tree.h"

/**
 * Funkcja drukuje komunikat ERROR na wyjście diagnostyczne.
 */
static void printError(void) {
    fprintf(stderr, "ERROR\n");
}

/**
 * Funkcja drukuje komunikat OK na standardowe wyjście.
 */
static void printOk(void) {
    printf("OK\n");
}

/**
 * Funkcja drukuje komunikat YES na standardowe wyjście.
 */
static void printYes(void) {
    printf("YES\n");
}

/**
 * Funkcja drukuje komunikat NO na standardowe wyjście.
 */
static void printNo(void) {
    printf("NO\n");
}

/**
 * Funkcja daje w wyniku wskaźnik na następny znak napisu, jeśli znak o podanym
 * indeksie jest różny od znaku końca napisu.
 */
static char *pointerToTheNextSign(char *str, int index) {
    if (str[index] == '\0') {
        return NULL;
    }
    else {
        return &str[index + 1];
    }
}

/**
 * Funkcja przyjmuje jako argument historię cząstki oraz ostatni znak (biały
 * znak), jakim powinna kończyć się historia. Funkcja zwraca true, jeżeli
 * historia zawiera tylko znaki '0', '1', '2' lub '3'.
 */
static bool isHistoryCorrect(char *history, char lastSign) {
    int i = 0;
    char c = history[0];
    bool correct;

    if (c == lastSign) {
        correct = false;
    }
    else {
        correct = true;
    }

    while (correct && c != lastSign) {
        if ((c < '0') || (c > '3')) {
            correct = false;
        }
        i++;
        c = history[i];
    }

    return correct;
}

/**
 * Funkcja sprawdza, czy napis reprezentujący energię jest poprawny.
 */
static bool isEnergyANumber(char *energy) {
    int i = 0;
    bool correct = true;

    while (correct && (energy[i] != '\n') && (energy[i] != '\0')) {
        if ((energy[i] < '0') || (energy[i] > '9')) {
            correct = false;
        }
        i++;
    }

    if ((i == 0) || (energy[i] == '\0')) {
        correct = false;
    }

    return correct;
}

/**
 * Funkcja sprawdza, czy energia mieści się w dopuszczalnym zakresie.
 */
static bool isEnergyCorrect(uint64_t energyNumber) {
    if ((energyNumber > 0) && (energyNumber <= UINT64_MAX)) {
        return true;
    }
    else {
        return false;
    }
}

/**
 * Funkcja dostaje jako argumenty napis po komendzie oraz pierwszą literę
 * komendy, którą powinna wywołać (d - wywołanie funkcji dla polecenia DECLARE,
 * r - wywołanie funkcji REMOVE).
 */
static void decRemCommandFunction(HistoryTree t, char *strAfterCommand, char command) {
    char *history = pointerToTheNextSign(strAfterCommand, 0);

    if (isHistoryCorrect(history, '\n')) {
        if (command == 'd') {
            declareHistory(t, history);
        }
        else if (command == 'r') {
            removeHistory(t, history);
        }
        printOk();
    } else {
        printError();
    }
}

/** 
 * Funkcja odpowiada za obsługę polecenia VALID.
 */
static void validCommand(HistoryTree t, char *strAfterCommand) {
    char *history = pointerToTheNextSign(strAfterCommand, 0);

    if (isHistoryCorrect(history, '\n')) {
        if (isHistoryValid(t, history)) {
            printYes();
        }
        else {
            printNo();
        }
    }
    else {
        printError();
    }
}

/**
 * Funkcja drukuje energię przypisaną do podanej historii. Funkcja daje
 * w wyniku true, jeśli podana historia jest dopuszczalna.
 */
static bool printEnergy(HistoryTree t, char *history) {
    t = getHistoryNode(t, history, '\n');

    if (t != NULL && t->energy > 0) {
        printf("%"PRIu64"\n", t->energy);
        return true;
    }
    else {
        return false;
    }
}

/**
 * Funkcja obsługuje polecenie ENERGY, gdy jest ono wpisywane z jednym
 * parametrem.
 */
static void energyWithOneParameter(HistoryTree t, char *history) {
    bool error = false;

    if (isHistoryCorrect(history, '\n')) {
        if (!printEnergy(t, history)) {
            error = true;
        }
    }
    else {
        error = true;
    }

    if (error) {
        printError();
    }
}

/**
 * Funkcja obsługuje polecenie ENERGY, gdy jest ono wpisywane z dwoma
 * parametrami.
 */
static void energyWithTwoParameters(HistoryTree t, char *history, char *energy) {
    bool error = false;
    uint64_t energyNumber;

    if (isHistoryCorrect(history, ' ') && isEnergyANumber(energy)) {
        energyNumber = strtoull(energy, NULL, 10);
        if ((errno == ERANGE) || !isEnergyCorrect(energyNumber)
            || !assignEnergy(t, history, energyNumber)) {
            errno = 0;
            error = true;
        }
    }
    else {
        error = true;
    }

    if (error) {
        printError();
    }
    else {
        printOk();
    }
}

/**
 * Funkcja sprawdza z iloma parametrami zostało wpisane polecenie ENERGY,
 * a następnie wywołuje odpowiednią funkcję.
 */
static void energyCommandFunction(HistoryTree t, char *strAfterCommand) {
    char *history = pointerToTheNextSign(strAfterCommand, 0);
    char *energy = strchr(history, ' ');

    if (energy == NULL) {
        energyWithOneParameter(t, history);
    }
    else {
        energy = pointerToTheNextSign(energy, 0);
        energyWithTwoParameters(t, history, energy);
    }
}

/**
 * Funkcja obsługuje polecenie EQUAL.
 */
static void equalCommandFunction(HistoryTree t, char *strAfterCommand) {
    bool error = false;
    char *histA = pointerToTheNextSign(strAfterCommand, 0);
    char *histB = strchr(histA, ' ');

    if (histB != NULL) {
        histB = pointerToTheNextSign(histB, 0);
        if (isHistoryCorrect(histA, ' ') && isHistoryCorrect(histB, '\n')) {
            if (equalEnergies(t, histA, histB)) {
                printOk();
            } else {
                error = true;
            }
        }
        else {
            error = true;
        }
    }
    else {
        error = true;
    }

    if (error) {
        printError();
    }
}

/**
 * Funkcja sprawdza jakie polecenie zostało wpisane, a następnie wywołuje
 * odpowiednią funkcję obsługującą to polecenie.
 */
static void chooseFunction(HistoryTree t, char *line) {
    char *strAfterCommand = strchr(line, ' ');
    if (strAfterCommand != NULL) {
        if (strstr(line, "DECLARE") == line) {
            decRemCommandFunction(t, strAfterCommand, 'd');
        }
        else if (strstr(line, "REMOVE") == line) {
            decRemCommandFunction(t, strAfterCommand, 'r');
        }
        else if (strstr(line, "VALID") == line) {
            validCommand(t, strAfterCommand);
        }
        else if (strstr(line, "ENERGY") == line) {
            energyCommandFunction(t, strAfterCommand);
        }
        else if (strstr(line, "EQUAL") == line) {
            equalCommandFunction(t, strAfterCommand);
        }
        else {
            printError();
        }
    }
    else {
        printError();
    }
}

void readAndDoCommands() {
    char *line = NULL;
    int charactersInString = 0;
    size_t length;
    HistoryTree t = NULL;

    t = (HistoryTree)malloc(sizeof(struct Node));
    checkAllocationError(t);
    setAllToBasicValues(t);

    while ((charactersInString = getline(&line, &length, stdin)) != -1) {
        // Pominięcie komentarzy i pustych linii.
        if (line[0] != '#' && line[0] != '\n') {
            if (line[charactersInString - 1] == '\n') {
                chooseFunction(t, line);
            }
            else {
                // Drukowanie błędu w przypadku niepoprawnej linii.
                printError();
            }
        }
    }

    free(line);
    removeAllWithRemovingFromRelatedList(t);
}
