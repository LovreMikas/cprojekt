#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_WORD_LENGTH 50
#define MAX_WRONG_GUESSES 6  // 4

// Funkcije
void clear_screen();
void print_hangman(int num_wrong_guesses);// 6
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

int main() {// 7
	// Postavljanje sjemena za generator slučajnih brojeva
	srand(time(NULL));

	while (1) {
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

		// Provjera hoće li se igrati nova igra ili izaći iz programa
		printf("Do you want to play again? (Y/N)\n");
		char c = getchar();
		if (tolower(c) != 'y') {
			break;
		}
		// Očisti ulazni bafer nakon unosa slova 'Y' ili 'N'
		while ((c = getchar()) != '\n' && c != EOF);
	}

	return 0;
}

void clear_screen() {
	// Brisanje ekrana
	system("clear || cls");
}

void print_hangman(int num_wrong_guesses) {
	// Ispisivanje vješala i obješenog čovjeka ovisno o broju pogrešnih pogađanja
	printf(" +---+\n");
	printf(" |   |\n");
	printf(" %s   |\n", num_wrong_guesses > 0 ? "O" : " ");
	printf(" %s%s%s |\n", num_wrong_guesses > 2 ? "/" : " ", num_wrong_guesses > 1 ? "|" : " ", num_wrong_guesses > 3 ? "\\" : " ");
	printf(" %s   |\n", num_wrong_guesses > 4 ? "/" : " ");
	printf(" %s   |\n", num_wrong_guesses > 5 ? "\\" : " ");
	printf(" |\n");
	printf("=========\n");
}

void print_word(char* word, int* guessed_letters) { // 6
	// Ispisivanje riječi s prikazom mjesta za neotkrivene slova
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
	// Ispisivanje već pogađanih slova
	printf("Guessed letters: ");
	for (int i = 0; i < strlen(guessed_letters); i++) {
		if (guessed_letters[i]) {
			printf("%c ", 'a' + i);
		}
	}
	printf("\n");
}

char* choose_word(char** words, int num_words) { // 3, 5
	// Odabir nasumične riječi iz niza riječi
	int random_index = rand() % num_words;
	char* word = (char*)malloc(sizeof(char) * (strlen(words[random_index]) + 1));
	strcpy(word, words[random_index]);
	return word;
}

int is_game_over(char* word, int* guessed_letters, int num_wrong_guesses) {//4
	// Provjera je li igra gotova
	for (int i = 0; i < strlen(word); i++) {
		if (!guessed_letters[i]) {
			break;
		}
		if (i == strlen(word) - 1) {
			// Sva slova su otkrivena, igrač je pobijedio
			return 1;
		}
	}
	if (num_wrong_guesses >= MAX_WRONG_GUESSES) {
		// Igrač je potrošio sve pokušaje, izgubio je
		return 1;
	}
	return 0;
}

int is_guess_correct(char* word, int guess) { // 4
	// Provjera je li pogađanje ispravno
	for (int i = 0; i < strlen(word); i++) {
		if (word[i] == guess) {
			return 1;
		}
	}
	return 0;
}

int* get_guessed_letters() {// 4
	// Inicijalizacija niza za praćenje već pogađanih slova
	int* guessed_letters = (int*)malloc(sizeof(int) * MAX_WORD_LENGTH);
	for (int i = 0; i < MAX_WORD_LENGTH; i++) {
		guessed_letters[i] = 0;
	}
	return guessed_letters;
}

int get_guess() {  // 4
	// Učitavanje pogađanja od igrača
	printf("Guess a letter: ");
	char c = getchar();
	// Očisti ulazni bafer nakon unosa slova
	while ((getchar()) != '\n');
	return tolower(c);
}

void play_game(char* word) { // 2, 6, 7, 9, 10, 11, 12, 13
	// Inicijalizacija potrebnih varijabli i nizova
	int num_wrong_guesses = 0;
	int* guessed_letters = get_guessed_letters();

	while (!is_game_over(word, guessed_letters, num_wrong_guesses)) {
		clear_screen();
		printf("Hangman\n");
		print_hangman(num_wrong_guesses);
		printf("\n");
		print_word(word, guessed_letters);
		printf("\n");
		print_guessed_letters(guessed_letters);
		printf("\n");

		// Učitavanje i provjera pogađanja
		int guess = get_guess();
		if (is_guess_correct(word, guess)) {
			printf("Correct!\n");
			for (int i = 0; i < strlen(word); i++) {
				if (word[i] == guess) {
					guessed_letters[i] = 1;
				}
			}
		}
		else {
			printf("Wrong!\n");
			num_wrong_guesses++;
		}
	}

	clear_screen();
	printf("Hangman\n");
	print_hangman(num_wrong_guesses);
	printf("\n");
	print_word(word, guessed_letters);
	printf("\n");
	if (num_wrong_guesses >= MAX_WRONG_GUESSES) {
		printf("Youlost! The word was %s.\n", word);
	}
	else {
		printf("Congratulations, you won!\n");
	}

	free(guessed_letters);

	return 0;

}
