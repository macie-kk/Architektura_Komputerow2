#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <string>

using namespace std;

struct vector {
    float x[4];
};

float randFloat() {
    return (float)(rand() % 100) / 10;
}

vector randVector() {
    vector random;
    random.x[0] = randFloat();
    random.x[1] = randFloat();
    random.x[2] = randFloat();
    random.x[3] = randFloat();

    return random;
}

void add_SISD(float x, float y) {
    asm(
        "FLDS %1 \n"
        "FADDS %2  \n"
        "FSTPS %0 \n"
        : "=m"(x)
        : "m"(x), "m"(y));
}

void sub_SISD(float x, float y) {
    asm(
        "FLDS %1 \n"
        "FSUBS %2 \n"
        "FSTPS %0 \n"
        : "=m"(x)
        : "m"(x), "m"(y));
}

void mul_SISD(float x, float y) {
    asm(
        "FLDS %1 \n"
        "FMULS %2 \n"
        "FSTPS %0 \n"
        : "=m"(x)
        : "m"(x), "m"(y));
}

void div_SISD(float x, float y) {
    asm(
        "FLDS %1 \n"
        "FDIVS %2 \n"
        "FSTPS %0 \n"
        : "=m"(x)
        : "m"(x), "m"(y));
}

void add_SIMD(vector x, vector y) {
    asm(
        "movups %1, %%xmm0 \n"
        "movups %2, %%xmm1 \n"
        "addps %%xmm1, %%xmm0 \n"
        "movups %%xmm0, %0 \n"
        : "=g"(x)
        : "g"(x), "g"(y));
}

void sub_SIMD(vector x, vector y) {
    asm(
        "movups %1, %%xmm0 \n"
        "movups %2, %%xmm1 \n"
        "subps %%xmm1, %%xmm0 \n"
        "movups %%xmm0, %0 \n"
        : "=g"(x)
        : "g"(x), "g"(y));
}

void div_SIMD(vector x, vector y) {
    asm(
        "movups %1, %%xmm0 \n"
        "movups %2, %%xmm1 \n"
        "divps %%xmm1, %%xmm0 \n"
        "movups %%xmm0, %0 \n"
        : "=g"(x)
        : "g"(x), "g"(y));
}

void mul_SIMD(vector x, vector y) {
    asm(
        "movups %1, %%xmm0 \n"
        "movups %2, %%xmm1 \n"
        "mulps %%xmm1, %%xmm0 \n"
        "movups %%xmm0, %0 \n"
        : "=g"(x)
        : "g"(x), "g"(y));
}

double time_SIMD(int n, char operation) {
    void (*result)(vector x, vector y);
    switch (operation) {
        case '+':
            result = &add_SIMD;
            break;
        case '-':
            result = &sub_SIMD;
            break;
        case '*':
            result = &mul_SIMD;
            break;
        case '/':
            result = &div_SIMD;
            break;
        default:
            cout << "Zly typ operacji" << endl;
    }

    double time = 0;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < n; j++) {
            clock_t start = clock();
            result(randVector(), randVector());
            clock_t end = clock() - start;
            time += ((double)end) / CLOCKS_PER_SEC;
        }
    }
    return (time / 10);
}

double time_SISD(int n, char operation) {
    void (*result)(float x, float y);
    switch (operation) {
        case '+':
            result = &add_SISD;
            break;
        case '-':
            result = &sub_SISD;
            break;
        case '*':
            result = &mul_SISD;
            break;
        case '/':
            result = &div_SISD;
            break;
        default:
            cout << "Zly typ operacji" << endl;
    }

    double time = 0;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < n; j++) {
            clock_t start = clock();
            result(randFloat(), randFloat());
            clock_t end = clock() - start;
            time += ((double)end) / CLOCKS_PER_SEC;
        }
    }
    return (time / 10);
}

void writeToCSV(FILE *file, int n, double add, double sub, double mul, double div) {
    fprintf(file, "%d;%f;%f;%f;%f\n", n, add, sub, mul, div);
}

void writeToConsoleAndFile(FILE *file, string type, int n, double add, double sub, double mul, double div) {
    printf("\nTyp obliczen: %s\n", type.c_str());
    printf("Ilosc liczb: %d\n", n);
    printf("Sredni czas [s]:\n");
    printf("\t+ %f\n", add);
    printf("\t- %f\n", sub);
    printf("\t* %f\n", mul);
    printf("\t/ %f\n", div);

    fprintf(file, "\nTyp obliczen: %s\n", type.c_str());
    fprintf(file, "Ilosc liczb: %d\n", n);
    fprintf(file, "Sredni czas [s]:\n");
    fprintf(file, "\t+ %f\n", add);
    fprintf(file, "\t- %f\n", sub);
    fprintf(file, "\t* %f\n", mul);
    fprintf(file, "\t/ %f\n", div);
}

int main() {
    FILE *plik_simd = fopen("lab3_simd.csv", "w");
    FILE *plik_sisd = fopen("lab3_sisd.csv", "w");
    FILE *plik_combined = fopen("lab3.txt", "w");

    if (plik_simd == NULL || plik_sisd == NULL) {
        fprintf(stderr, "Nie mozna utworzyc pliku\n");
        exit(1);
    }

    fprintf(plik_simd, "ilosc liczb;+;-;*;/\n");
    fprintf(plik_sisd, "ilosc liczb;+;-;*;/\n");

    srand(time(0));
    double time_add, time_sub, time_mul, time_div;
    for (int i = 0, n = 2048; i < 3; i++, n *= 2) {
        time_add = time_SIMD(n, '+');
        time_sub = time_SIMD(n, '-');
        time_mul = time_SIMD(n, '*');
        time_div = time_SIMD(n, '/');

        writeToConsoleAndFile(plik_combined, "SIMD", n, time_add, time_sub, time_mul, time_div);
        writeToCSV(plik_simd, n, time_add, time_sub, time_mul, time_div);

        time_add = time_SISD(n, '+');
        time_sub = time_SISD(n, '-');
        time_mul = time_SISD(n, '*');
        time_div = time_SISD(n, '/');

        writeToConsoleAndFile(plik_combined, "SISD", n, time_add, time_sub, time_mul, time_div);
        writeToCSV(plik_sisd, n, time_add, time_sub, time_mul, time_div);
    }

    fclose(plik_simd);
    fclose(plik_sisd);
    fclose(plik_combined);
    return 0;
}