// CPU–GPU Compatibility CLI
// Author: Saba Karaman
// Description: Bidirectional command-line tool for recommending
//              CPU–GPU pairings based on performance tiers.

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef struct {
    char brand[10];   // Intel / AMD
    char name[10];    // i / ryzen
    int series;       // 3 / 5 / 7 / 9
    char suffix[10];  // optional
} cpu;

typedef struct {
    char series[10];  // RTX / RX
    int type;         // 4070, 7800, etc.
    char suffix[10];  // ti / super / xt / xtx / none
} gpu;


int case_equal(const char *a, const char *b) {
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) return 0;
        a++;
        b++;
    }
    return *a == '\0' && *b == '\0';
}

void to_lower_str(char *s) {
    for (; *s; s++) *s = (char)tolower((unsigned char)*s);
}

void clear_line(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {}
}

int choose_direction(void) {
    int choice;

    printf("What do you want to do?\n");
    printf("1) I have a CPU -> recommend GPU\n");
    printf("2) I have a GPU -> recommend CPU\n");
    printf("Enter choice (1 or 2): ");

    if (scanf("%d", &choice) != 1) {
        clear_line();
        return 0;
    }
    clear_line();
    return choice;
}

int parse_cpu_series_token(const char *token, char *name_out, int *series_out) {
    char temp[32];
    strncpy(temp, token, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';
    to_lower_str(temp);

    if (temp[0] == 'i' && isdigit((unsigned char)temp[1])) {
        strcpy(name_out, "i");
        *series_out = temp[1] - '0';
        return 1;
    }

    if (strncmp(temp, "ryzen", 5) == 0 && isdigit((unsigned char)temp[5])) {
        strcpy(name_out, "ryzen");
        *series_out = temp[5] - '0';
        return 1;
    }

    return 0;
}

void for_cpu(cpu *c) {
    printf("\n");
    printf("Which brand of CPU do you have (Intel/AMD)? ");
    scanf("%9s", c->brand);
    clear_line();

    if (!case_equal(c->brand, "intel") && !case_equal(c->brand, "amd")) {
        printf("Invalid CPU brand.\n");
        c->brand[0] = '\0';
        return;
    }

    printf("Which series do you have? (e.g., i5, Ryzen7): ");
    char tok[32];
    scanf("%31s", tok);
    clear_line();

    if (!parse_cpu_series_token(tok, c->name, &c->series)) {
        printf("Invalid CPU series format.\n");
        c->brand[0] = '\0';
        return;
    }

    strcpy(c->suffix, "none");
}

void recommend_cpu(const cpu *c) {
    if (c->brand[0] == '\0') return;

    if (c->series <= 3) {
        printf("CPU is entry level.\n");
    } else if (c->series == 5) {
        printf("CPU is solid midrange.\n");
    } else if (c->series == 7) {
        printf("CPU is high end.\n");
    } else {
        printf("CPU is enthusiast level.\n");
    }
}

int extract_suffix_from_stuck_number(const char *token, int *num_out, char *suffix_out) {
    int i = 0;
    while (isdigit((unsigned char)token[i])) i++;
    if (i == 0) return 0;

    char digits[16];
    memcpy(digits, token, i);
    digits[i] = '\0';
    *num_out = atoi(digits);

    if (token[i] == '\0') {
        strcpy(suffix_out, "none");
        return 1;
    }

    strncpy(suffix_out, token + i, 9);
    suffix_out[9] = '\0';
    to_lower_str(suffix_out);
    return 1;
}

void for_gpu(gpu *g) {
    printf("\n");
    printf("Which GPU do you have? (e.g., RTX 4070 Ti): ");

    char line[128];
    if (!fgets(line, sizeof(line), stdin)) {
        printf("Input error.\n");
        return;
    }

    char a[32] = {0}, b[32] = {0}, c[32] = {0};
    int n = sscanf(line, "%31s %31s %31s", a, b, c);

    if (n < 2) {
        printf("Invalid GPU format.\n");
        return;
    }

    strncpy(g->series, a, 9);
    g->series[9] = '\0';
    to_lower_str(g->series);

    to_lower_str(b);

    if (!extract_suffix_from_stuck_number(b, &g->type, g->suffix)) {
        printf("Invalid GPU number.\n");
        return;
    }

    if (n == 3) {
        strncpy(g->suffix, c, 9);
        g->suffix[9] = '\0';
        to_lower_str(g->suffix);
    }

}

void recommend_gpu_for_cpu(const cpu *c) {
    if (c->brand[0] == '\0') return;

    printf("Recommended GPUs for your CPU:\n");

    if (c->series <= 3) {
        printf("- RTX 3050 / 3060 or RX 6600\n");
    } else if (c->series == 5) {
        printf("- RTX 3060 Ti / 4060 / 4070 or RX 6700 XT / 7700 XT\n");
    } else if (c->series == 7) {
        printf("- RTX 4070 Super / 4070 Ti / 4080 or RX 7800 XT / 7900 XT\n");
    } else {
        printf("- RTX 4080 / 4090 or RX 7900 XTX\n");
    }
}

void recommend_gpu(const gpu *g) {
    int tier = g->type % 100;

    if (tier == 0) {
        if (g->type >= 7900) tier = 90;
        else if (g->type >= 7800) tier = 80;
        else if (g->type >= 7700) tier = 70;
    }

    if (tier == 50) {
        printf("Recommended CPUs: i3 or Ryzen 3\n");
    } else if (tier == 60) {
        printf("Recommended CPUs: i5 or Ryzen 5\n");
    } else if (tier == 70) {
        printf("Recommended CPUs: i7 or Ryzen 7\n");
    } else if (tier == 80) {
        printf("Recommended CPUs: i7 / i9 or Ryzen 7 / 9\n");
    } else if (tier == 90) {
        printf("Recommended CPUs: i9 or Ryzen 9\n");
    } else {
        printf("No rule for this GPU tier yet.\n");
    }
}

int main(void) {
    cpu c;
    gpu g;

    int choice = choose_direction();

    if (choice == 1) {
        for_cpu(&c);

        printf("\n--- CPU check ---\n");
        recommend_cpu(&c);

        printf("\n--- GPU recommendations ---\n");
        recommend_gpu_for_cpu(&c);
        printf("\n");

    } else if (choice == 2) {
        for_gpu(&g);

        printf("\n--- CPU recommendations ---\n");
        recommend_gpu(&g);
        printf("\n");

    } else {
        printf("Invalid selection.\n");
    }

    return 0;
}

