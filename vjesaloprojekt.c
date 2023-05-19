#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_WORD_LENGTH 50
#define MAX_WRONG_GUESSES 6

// Funkcije
void clear_screen();
void print_hangman(int num_wrong_guesses);
void print_word(char* word, int* guessed_letters);
void print_guessed_letters(char* guessed_letters);
char* choose_word(char** words, int num_words);
int is_game_over(char* word, int* guessed_letters, int num_wrong_guesses);
int is_guess_correct(char* word, int guess);
int* get_guessed_letters();
int get_guess();
void play_game(char* word);

// Riječi koje se koriste u igri
char* words[] = { "jabuka", "naranča", "limun", "mandarina", "kruška", "breskva", "šljiva", "smokva", "grožđe", "malina", "jagoda", "trešnja", "višnja", "borovnica",
"malina", "dinja", "lubenica", "ananas", "avokado", "banana", "kivi", "mango", "marakuja", "papaja", "šipak", "agrum", "đumbir",
"kardamom", "klinčić", "kumina", "muškatni oraščić", "papar", "paprika", "ružmarin", "timijan", "vanilija", "bosiljak", "češnjak", "peršin", "kopar", "majčina dušica" };
int num_words = sizeof(words) / sizeof(char*);

int main() {
	// Postavljanje sjemena za generator slučajnih brojeva
	srand(time(NULL));

	// Postavljanje nasumično odabrane riječi
	char* word = choose_word(words, num_words);

	// Početni zaslon igre
	printf("Welcome to Hangman!\n");
	printf("Press any key to start.\n");
	getchar();

	// Pokretanje igre
	play_game(word);

	// Oslobađanje memorije koju smo dinamički zauzeli za riječ
	free(word);

	return 0;
}

void clear_screen() {
	// Brisanje ekrana
	system("clear || cls");
}

void print_hangman(int num_wrong_guesses) {
	// Ispisivanje vješala i obješenog čovjeka ovisno o broju pogrešnih pogađanja
	printf("  +---+\n");
	printf("  |   |\n");
	printf("  %s   |\n", num_wrong_guesses > 0 ? "O" : " ");
	printf(" %s%s%s  |\n", num_wrong_guesses > 2 ? "/" : " ", num_wrong_guesses > 1 ? "|" : " ", num_wrong_guesses > 3 ? "\\" : " ");
	printf(" %s   |\n", num_wrong_guesses > 4 ? "/" : " ");
	printf(" %s   |\n", num_wrong_guesses > 5 ? "/" : " ");
	printf("      |\n");
	printf("=========\n");
}

void print_word(char* word, int* guessed_letters) {
	// Ispisivanje riječi s prikazom pogodaka
	for (int i = 0; i < strlen(word); i++) {
		if (guessed_letters[i]) {
			printf("%c ", word[i]);
		}
		else {
			printf("_ ");
		}
	}
	printf("\n");
}

void print_guessed_letters(char* guessed_letters) {
	// Ispisivanje slova koja su već pogađana
	printf("Guessed letters: ");
	for (int i = 0; i < 26; i++) {
		if (guessed_letters[i]) {
			printf("%c ", 'a' + i);
		}
	}
	printf("\n");
}

char* choose_word(char** words, int num_words) {
	// Nasumično odabiranje riječi iz niza riječi
	int index = rand() % num_words;
	// Dinamičko zauzimanje memorije za riječ
	char* word = (char*)malloc(MAX_WORD_LENGTH * sizeof(char));

	// Kopiranje odabrane riječi u dinamički zauzetu memoriju
	strcpy(word, words[index]);

	return word;
}

int is_game_over(char* word, int* guessed_letters, int num_wrong_guesses) {
	// Provjera je li igra završila
	if (num_wrong_guesses == MAX_WRONG_GUESSES) {
		printf("You lose! The word was %s.\n", word);
		printf("Last word was: %s\n", word);
		return 1;
	}
	for (int i = 0; i < strlen(word); i++) {
		if (!guessed_letters[i]) {
			return 0;
		}
	}

	printf("Congratulations! You win! The word was %s.\n", word);
	printf("Last word was: %s\n", word);
	return 1;
}
int is_guess_correct(char* word, int guess) {
	// Provjera je li pogodak ispravan
	for (int i = 0; i < strlen(word); i++) {
		if (word[i] == guess) {
			return 1;
		}
	}

	return 0;
}

int* get_guessed_letters() {
	// Dinamičko zauzimanje memorije za niz koji će pohranjivati informacije o pogocima
	int* guessed_letters = (int*)malloc(MAX_WORD_LENGTH * sizeof(int));
	// Postavljanje svih elemenata niza na 0
	for (int i = 0; i < MAX_WORD_LENGTH; i++) {
		guessed_letters[i] = 0;
	}

	return guessed_letters;
}

int get_guess() {
	// Učitavanje pogađanja od korisnika
	char c;
	do {
		printf("Guess a letter: ");
		c = getchar();
		c = tolower(c);
	} while (isspace(c)); // Ignoriranje razmaka
	return c;
}
void play_game(char* word) {
	// Inicijalizacija varijabli
	int num_wrong_guesses = 0;
	int* guessed_letters = get_guessed_letters();
	while (!is_game_over(word, guessed_letters, num_wrong_guesses)) {
		clear_screen();
		printf("Hangman\n");
		print_hangman(num_wrong_guesses);
		printf("Word: ");
		print_word(word, guessed_letters);
		printf("Guessed letters: ");
		print_guessed_letters(guessed_letters);

		int guess = get_guess();
		if (is_guess_correct(word, guess)) {
			for (int i = 0; i < strlen(word); i++) {
				if (word[i] == guess) {
					guessed_letters[i] = 1;
				}
			}
		}
		else {
			num_wrong_guesses++;
		}
	}

	// Oslobađanje dinamički zauzete memorije
	free(guessed_letters);


	srand(time(NULL));
	char* words[] = { "jabuka", "naranča", "limun", "mandarina", "kruška", "breskva", "šljiva", "smokva", "grožđe", "malina", "jagoda", "trešnja", "višnja", "borovnica",
"malina", "dinja", "lubenica", "ananas", "avokado", "banana", "kivi", "mango", "marakuja", "papaja", "šipak", "agrum", "đumbir",
"kardamom", "klinčić", "kumina", "muškatni oraščić", "papar", "paprika", "ružmarin", "timijan", "vanilija", "bosiljak", "češnjak", "peršin", "kopar", "majčina dušica" };
	int num_words = 5;

	word = choose_word(words, num_words);
	play_game(word);
	free(word);

	return 0;

}