#ifndef QUANTUM_HISTORY_TREE
#define QUANTUM_HISTORY_TREE

#include <stdint.h>
#include <stdbool.h>

#define NUMBER_OF_STATES 4

/**
 * Struktura przechowująca drzewo historii kwantowych.
 */
struct Node;

typedef struct Node* HistoryTree;

struct Node {
    uint64_t energy;
    HistoryTree relatedNode;
    HistoryTree quantumState[NUMBER_OF_STATES];
};

/**
 * Funkcja daje w wyniku wskaźnik na węzeł drzewa po przejściu przez podaną 
 * historię. Argument lastSign odpowiada za znak, do którego funkcja ma
 * przechodzić po argumencie history.
 */
extern HistoryTree getHistoryNode(HistoryTree, char *, char);

/**
 * Funkcja usuwa całe poddrzewo (razem z usuwaniem każdego węzła z relacji
 * zrównania energii) od podanego węzła włącznie.
 */
extern void removeAllWithRemovingFromRelatedList(HistoryTree);

/**
 * Funkcja usuwa z drzewa wszystkie historie, których prefiks został podany
 * jako argument funkcji.
 */
extern void removeHistory(HistoryTree, char *);

/**
 * Funkcja obsługuje błąd alokacji.
 */
extern void checkAllocationError(HistoryTree);

/**
 * Funkcja ustawia wartości węzła na podstawowe, czyli przypisuje każdemu
 * wskaźnikowi do syna wartość NULL oraz ustawia wartość energii na 0, co
 * oznacza, że energia nie jest przypisana.
 */
extern void setAllToBasicValues(HistoryTree);

/**
 * Funkcja tworzy w drzewie podaną jako argument historię, co oznacza, że
 * dana historia jest dopuszczalna.
 */
extern void declareHistory(HistoryTree, char *);

/**
 * Funkcja daje w wyniku true, jeśli podana jako argument historia jest
 * dopuszczalna.
 */
extern bool isHistoryValid(HistoryTree, char *);

/**
 * Funkcja przypisuje podanej historii podaną energię. Jeśli wszystko się
 * powiodło funkcja daje w wyniku true.
 */
extern bool assignEnergy(HistoryTree, char *, uint64_t);

/**
 * Funkcja odpowiada za łączenie dwóch podanych historii w relację zrównania
 * energii. Jeśli wszystko się powiodło funkcja daje w wyniku true.
 */
extern bool equalEnergies(HistoryTree, char *, char *);

#endif /* QUANTUM_HISTORY_TREE */
