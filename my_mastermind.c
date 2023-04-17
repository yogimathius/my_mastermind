#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> /* time */
#include <unistd.h>

int get_from_user(char *buffer, size_t size) {
  size_t cnt = 0;
  char c;

  if (buffer == NULL || size == 0)
    return 0;

  while (read(STDIN_FILENO, &c, 1) == 1 && c != '\n') {
    if (c == '\n') {
      buffer[cnt] = 0;
      return 1;
    }
    if (c == 0) {
      printf("exit");
      return 0;
    }

    buffer[cnt++] = c;
  }

  buffer[cnt] = 0; // making sure it's 0-terminated
  return 1;
}

int is_digit(char *buffer) {
  int len = strlen(buffer);

  for (int i = 0; i < len; i++) {
    if (buffer[i] >= 48 && buffer[i] <= 57) {
      return 1;
    }
  }
  return 0;
}

int well_placed_pieces(char *buffer, int random_digits) {
  int len = strlen(buffer);
  int well_placed = 0;
  char str[5];
  sprintf(str, "%d", random_digits);
  for (int i = 0; i < len; i++) {
    if (buffer[i] == str[i]) {
      well_placed++;
    }
  }
  return well_placed;
}

int misplaced_pieces(char *buffer, int random_digits) {
  int len = strlen(buffer);
  int misplaced = 0;
  char str[5];
  sprintf(str, "%d", random_digits);
  for (int i = 0; i < len; i++) {
    for (int j = 0; j < len; j++) {
      if (buffer[i] == str[j] && i != j) {
        misplaced++;
      }
    }
  }
  return misplaced;
}

int parse_args(int ac, char **av, int *total_rounds, int *random_digits) {
  for (int i = 1; i < ac; i++) {
    if (i + 1 != ac) {
      if (strcmp(av[i], "-c") == 0) {
        *random_digits = atoi(av[i + 1]);
      }
      if (strcmp(av[i], "-t") == 0) {
        *total_rounds = atoi(av[i + 1]);
      }
      i++;
    }
  }
  return 0;
}

int main(int ac, char **av) {
  int current_round = 0;
  int total_rounds = 10;

  srand(time(NULL));
  int random_four_digits = rand() % 9000 + 1000;
  srand(1);
  if (ac >= 2) {
    parse_args(ac, av, &total_rounds, &random_four_digits);
  }

  printf("Will you find the secret code?\n");
  printf("Please enter a valid guess\n");
  bool word_guessed = false;

  while (!word_guessed && current_round < total_rounds) {
    printf("---");
    printf("Round %d\n", current_round);
    char buf[sizeof(int) * 4];

    get_from_user(buf, 5);
    if (*buf == 0) {
      printf("exit");
      return 0;
    }

    if (strlen(buf) != 4 || !is_digit((buf))) {
      printf("Wrong input!\n");
    } else {
      int well_placed = well_placed_pieces(buf, random_four_digits);
      if (well_placed == 4) {
        printf("Congratz! You did it!");
        word_guessed = true;
        return 0;
      }
      int misplaced = misplaced_pieces(buf, random_four_digits);
      printf("Well placed pieces: %d\n", well_placed);
      printf("Misplaced pieces: %d\n", misplaced);
      current_round++;
    }
  }

  return 0;
}
