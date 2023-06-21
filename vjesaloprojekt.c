#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>
// Pravilo 1: Odabir konkretnih primitivnih tipova podataka za rad s cjelobrojnim i realnim konstantama.
int score = 0;
float average = 0.0;

// Pravilo 2: Odabir konkretnih složenih tipova podataka za rad sa specifičnim objektima.
struct player {
    char name[50];
    int level;
};

// Pravilo 3: Primjena typedef sa strukturama i unijama, po potrebi s enum tipovima podataka.
typedef struct player Player;

// Pravilo 4: Imenovanje identifikatora (varijabli, konstanti, polja, funkcija, pokazivača…) – upotreba camelCase, PascalCase i snake_case konzistentno kroz cijeli projekt.
int max_attempts = 5;
char secret_word[50];
char guessed_letters[26];
int num_guessed_letters = 0;

// Pravilo 5: Primjena ključne riječi static za globalne i lokalne varijable.
static int num_players = 0;

// Pravilo 6: Organizacija izvornog kôda.

// Pravilo 7: Ako su funkcije jednostavne koristiti makro funkcije ili inline funkcije.
#define MAX(x, y) ((x) > (y) ? (x) : (y))

// Pravilo 8: Izbornik/podizbornici.
void display_menu() {
    printf("Hangman Game\n");
    printf("1. Play Game\n");
    printf("2. Exit\n");
}

// Pravilo 9: Generalno upotreba pokazivača tamo gdje su potrebni.
void initialize_game() {
    num_guessed_letters = 0;
    memset(guessed_letters, 0, sizeof(guessed_letters));
}

// Pravilo 10: Generalno upotreba struktura i funkcija.
void update_score(Player* p, int points) {
    p->level++;
    score += points;
}

// Pravilo 11: Zaštita parametara kod svih funkcija.
int play_game(const char* word) {
    int attempts = 0;
    int word_length = strlen(word);
    char guessed_word[50];

    // Pravilo 12: Koristiti statički zauzeta polja.
    static const char hangman_images[][28] = {
        "+---+\n"
        "   |\n"
        "   |\n"
        "   |\n"
        "   ===\n",

        // Ostatak slika hangmana
    };

    // Pravilo 13: Koristiti dinamičko zauzimanje memorije za bilo koji tip podatka, osobito za složene tipove podataka.
    char* temp_word = malloc(word_length * sizeof(char));
    if (temp_word == NULL) {
        perror("Memory allocation failed");
        return 0;
    }
    strcpy(temp_word, word);

    // Pravilo 14: Koristiti funkcije malloc(), calloc(), realloc(), free() – neku od njih, ako ne i sve.
    char* guess = malloc((word_length + 1) * sizeof(char));
    if (guess == NULL) {
        perror("Memory allocation failed");
        free(temp_word);
        return 0;
    }

    // Pravilo 15: Sigurno brisanje memorije koja je dinamički zauzeta, anuliranje memorijskog prostora, provjera pokazivača kako se ne bi dogodila pogreška double free() i anuliranje svih pokazivača koji su bili usmjereni na memorijski prostor koji se dinamički zauzeo.
    memset(guessed_word, 0, sizeof(guessed_word));

    // Pravilo 16: Datoteke, koristiti tekstualnu ili binarnu, provjera pokazivača i zatvaranje datoteke.
    FILE* file = fopen("scores.txt", "a");
    if (file == NULL) {
        perror("Failed to open file");
        free(temp_word);
        free(guess);
        return 0;
    }

    // Pravilo 17: Upotreba funkcija fseek(), ftell() i rewind() za manipulaciju datotekama.
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = (char*)malloc(size + 1);
    if (buffer == NULL) {
        perror("Memory allocation failed");
        fclose(file);
        free(temp_word);
        free(guess);
        return 0;
    }

    fread(buffer, 1, size, file);
    buffer[size] = '\0';

    printf("File contents:\n%s\n", buffer);

    free(buffer);

    // Logika igre hangman

    fclose(file);
    free(temp_word);
    free(guess);
    return 1;
}

int compare_numbers(const void* a, const void* b) {
    int num1 = *(int*)a;
    int num2 = *(int*)b;

    if (num1 < num2)
        return -1;
    else if (num1 > num2)
        return 1;
    else
        return 0;
}

// Pravilo 18: Koristiti funkcije remove(), rename(), po potrebi implementirati funkciju za kopiranje datoteka.
void remove_scores_file() {
    remove("scores.txt");
}

// Pravilo 19: Upravljati s pogreškama, errno, perror(), strerror(), feof(), ferror() – neku od njih ako ne sve.
void display_error(const char* message) {
    fprintf(stderr, "Error: %s\n", message);
}
int main() {
    char* words[] = { "hangman", "computer", "programming" };
    remove_scores_file();

    Player player;
    strcpy(player.name, "John Doe");
    player.level = 1;

    // Pravilo 20: Sortiranje – pokušati koristiti ugrađenu qsort() funkciju.
    int numbers[] = { 5, 2, 8, 1, 9 };
    int num_numbers = sizeof(numbers) / sizeof(numbers[0]);
    qsort(numbers, num_numbers, sizeof(int), compare_numbers);

    // Pravilo 21: Pretraživanje – pokušati koristiti ugrađenu bsearch() funkciju.
    int key = 8;
    int* found = bsearch(&key, numbers, num_numbers, sizeof(int), compare_numbers);
    if (found != NULL) {
        printf("Number %d found!\n", *found);
    } else {
        printf("Number not found!\n");
    }

    int choice = 0;
    while (choice != 2) {
        display_menu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Starting new game...\n");

                // Odabir nasumične riječi
                srand(time(NULL));
                int random_index = rand() % 3;
                strcpy(secret_word, words[random_index]);

                initialize_game();
                int result = play_game(secret_word);

                if (result) {
                    printf("Game finished!\n");
                } else {
                    display_error("Failed to play the game.");
                }

                break;
            case 2:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
