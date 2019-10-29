#include <stdio.h>
#include <stdlib.h>

#include "quantum-history-tree.h"

/**
 * Funkcja przyjmuje jako parametr znak c z historii cząstki, który może
 * być równy '0', '1', '2' lub '3'. Kody ASCII tych znaków wynoszą odpowiednio
 * 30, 31, 32, 33, a zatem odejmuje od nich znak '0', aby zwracała liczbę
 * całkowitą, równą 0, 1, 2 lub 3.
 */
static int correctIndex(char c) {
    return (int)(c - '0');
}

HistoryTree getHistoryNode(HistoryTree t, char *history, char lastSign) {
    int i = 0;
    int index;

    while ((t != NULL) && (history[i] != lastSign)) {
        index = correctIndex(history[i]);
        t = t->quantumState[index];
        i++;
    }
    return t;
}

/**
 * Funkcja usuwa węzeł t z relacji zrównania energii.
 */
static void removeFromRelatedList(HistoryTree t) {
    HistoryTree supportNode = t;

    while (t->relatedNode != supportNode) {
        t = t->relatedNode;
    }
    
    t->relatedNode = supportNode->relatedNode;
    supportNode->relatedNode = supportNode;
}

void removeAllWithRemovingFromRelatedList(HistoryTree t) {
    if (t != NULL) {
        for (int i = 0; i < NUMBER_OF_STATES; i++) {
            removeAllWithRemovingFromRelatedList(t->quantumState[i]);
        }
        removeFromRelatedList(t);
        free(t);
    }
}

void removeHistory(HistoryTree t, char *history) {
    int i = 0;
    int index;

    while ((t != NULL) && (history[i + 1] != '\n')) {
        index = correctIndex(history[i]);
        t = t->quantumState[index];
        i++;
    }

    if (t != NULL) {
        index = correctIndex(history[i]);
        removeAllWithRemovingFromRelatedList(t->quantumState[index]);
        t->quantumState[index] = NULL;
    }
}

void checkAllocationError(HistoryTree t) {
    if (t == NULL) {
        fprintf(stderr, "ERROR\n");
        removeAllWithRemovingFromRelatedList(t);
        exit(1);
    }
}

void setAllToBasicValues(HistoryTree t) {
    t->energy = 0;
    t->relatedNode = t;
    for (int i = 0; i < NUMBER_OF_STATES; i++) {
        t->quantumState[i] = NULL;
    }
}

void declareHistory(HistoryTree t, char *history) {
    HistoryTree supportNode = NULL;
    int i = 0;
    int index;

    while (history[i] != '\n') {
        index = correctIndex(history[i]);
        // Drzewo t ma atrapę, dlatego nie trzeba się martwić
        // o to, że na początku będzie NULLem.
        if (t->quantumState[index] == NULL) {
            supportNode = (HistoryTree)malloc(sizeof(struct Node));
            checkAllocationError(supportNode);
            setAllToBasicValues(supportNode);
            t->quantumState[index] = supportNode;
            supportNode = NULL;
        }
        t = t->quantumState[index];
        i++;
    }
}

bool isHistoryValid(HistoryTree t, char *history) {
    t = getHistoryNode(t, history, '\n');

    if (t != NULL) {
        return true;
    }
    else {
        return false;
    }
}

bool assignEnergy(HistoryTree t, char *history, uint64_t enteredEnergy) {
    HistoryTree supportNode;

    t = getHistoryNode(t, history, ' ');
    supportNode = t;
    if ((t != NULL) && (enteredEnergy > 0)) {
        t->energy = enteredEnergy;
        while (t->relatedNode != supportNode) {
            t = t->relatedNode;
            t->energy = enteredEnergy;
        }
        return true;
    }
    else {
        return false;
    }
} 

/**
 * Funkcja zrównuje energie obu podanych historii oraz wszystkich historii
 * z nimi związanych, a następnie łączy wszystkie te historie w jedną relację,
 * poza przypadkiem, gdy obie podane historie są już ze sobą w relacji.
 */
static void levelEnergies(HistoryTree tA, HistoryTree tB, uint64_t newEnergy) {
    HistoryTree supportNodeA = tA, supportNodeB = tB;

    tA->energy = newEnergy;
    tB->energy = newEnergy;
    while ((tA->relatedNode != supportNodeA)
           && (tA->relatedNode != supportNodeB)) {
        tA = tA->relatedNode;
        tA->energy = newEnergy;
    }

    if (tA->relatedNode == supportNodeA) {
        while (tB->relatedNode != supportNodeB) {
            tB = tB->relatedNode;
            tB->energy = newEnergy;
        }

        tA->relatedNode = supportNodeB;
        tB->relatedNode = supportNodeA;
    }
}

/**
 * Funkcja obsługuje przypadek, w którym tylko jedna historia ma przypisaną
 * energię, a więc sprawdza, która historia ma przypisaną energię i wywołuje
 * funkcję levelEnergies.
 */
static void oneEnergy(HistoryTree tA, HistoryTree tB, uint64_t eA, uint64_t eB) {
    uint64_t newEnergy;
    
    if (eA > 0) {
        newEnergy = eA;
    }
    else {
        newEnergy = eB;
    }

    levelEnergies(tA, tB, newEnergy);
}

/**
 * Funkcja obsługuje przypadek, w którym obie historie mają przypisane energie,
 * a więc oblicza ich średnią arytmetyczną i wywołuje funkcję levelEnergies.
 */
static void twoEnergies(HistoryTree tA, HistoryTree tB, uint64_t eA, uint64_t eB) {
    uint64_t newEnergy = 0;

    if (eA != eB) {
        newEnergy += eA / 2;
        newEnergy += eB / 2;
        if (((eA &1) != 0) && ((eB &1) != 0)) {
            newEnergy += 1;
        }
    }
    else {
        newEnergy = eA;
    }

    levelEnergies(tA, tB, newEnergy);
}

bool equalEnergies(HistoryTree t, char *histA, char *histB) {
    HistoryTree tA, tB;
    uint64_t energyA, energyB;
    bool correct = true;
    
    tA = getHistoryNode(t, histA, ' ');
    tB = getHistoryNode(t, histB, '\n');
    if ((tA != NULL) && (tB != NULL) && (tA != tB)) {
        // Obie historie są dopuszczalne i różne.
        energyA = tA->energy;
        energyB = tB->energy;
        if (energyA > 0 && energyB > 0) {
            // Obie historie mają przypisane energie.
            twoEnergies(tA, tB, energyA, energyB);
        }
        else if (energyA > 0 || energyB > 0) {
            // Jedna historia ma przypisaną energię.
            oneEnergy(tA, tB, energyA, energyB);
        }
        else {
            // Żadna historia nie ma przypisanej energii.
            correct = false;
        }
    }
    else if ((tA != NULL) && (tB != NULL) && (tA == tB)) {
        correct = true;
    }
    else {
        // Obie historie nie są dopuszczalne.
        correct = false;
    }

    return correct;
}
