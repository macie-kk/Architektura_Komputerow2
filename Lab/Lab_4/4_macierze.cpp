#include <stdio.h>

struct vect {
    float v[4];
};

float detM(struct vect x0, struct vect x1, struct vect x2, struct vect x3) {
    vect result;
    asm(
        // generowanie wektorow A'i B' & xmm3 = A'B'
        "movups %1, %%xmm3 \n"
        "movups %2, %%xmm6 \n"
        "shufps $0x48, %%xmm3, %%xmm3 \n"  // 1 0 2 1
        "shufps $0xb1, %%xmm6, %%xmm6 \n"  // 2 3 0 1
        "mulps %%xmm6, %%xmm3 \n"

        // generowanie wektorow A" i B" & xmm6 = A"B"
        "movups %1, %%xmm6 \n"
        "movups %2, %%xmm7 \n"
        "shufps $0xb1, %%xmm6, %%xmm6 \n"  // 2 3 0 1
        "shufps $0x48, %%xmm7, %%xmm7 \n"  // 1 0 2 0
        "mulps %%xmm7, %%xmm6 \n"

        // xmm3 = A'B'-A"B"
        "subps %%xmm6, %%xmm3 \n"

        // generowanie wektorow C' i D' & xmm4 = C'D'
        "movups %3, %%xmm4 \n"
        "movups %4, %%xmm6 \n"
        "shufps $0x16, %%xmm4, %%xmm4  \n"
        "shufps $0xEF, %%xmm6, %%xmm6 \n"
        "mulps %%xmm6, %%xmm4 \n"

        // generowanie wektorow C" i D" & xmm6 = C"D"
        "movups %3, %%xmm6 \n"
        "movups %4, %%xmm7 \n"
        "shufps $0xEF, %%xmm6, %%xmm6 \n"
        "shufps $0x16, %%xmm7, %%xmm7 \n"
        "mulps %%xmm7, %%xmm6 \n"

        // xmm4 = C'D'-C"D"
        "subps %%xmm6, %%xmm4 \n"

        // xmm3 = (A'B'-A"B")*(C'D'-C"D")
        "mulps %%xmm4, %%xmm3 \n"

        // generowanie wektorow E' i F' & xmm2 = E'F'
        "movups %1, %%xmm2 \n"
        "movups %2, %%xmm6 \n"
        "shufps $0xFE, %%xmm2, %%xmm2 \n"
        "shufps $0xF7, %%xmm6, %%xmm6 \n"
        "mulps %%xmm6, %%xmm2 \n"

        // generowanie wektorow E" i F" & xmm6 = E"F"
        "movups %1, %%xmm6 \n"
        "movups %2, %%xmm7 \n"
        "shufps $0xF7, %%xmm6, %%xmm6 \n"
        "shufps $0xFE, %%xmm7, %%xmm7 \n"
        "mulps %%xmm7, %%xmm6 \n"

        // xmm2 = E'F'-E"F"
        "subps %%xmm6, %%xmm2 \n"

        // generowanie wektorow G' i H' & xmm2 = G'H'
        "movups %3, %%xmm4 \n"
        "movups %4, %%xmm7 \n"
        "shufps $0xF0, %%xmm4, %%xmm4 \n"
        "shufps $0xF9, %%xmm7, %%xmm7 \n"
        "mulps %%xmm7, %%xmm4 \n"

        // generowanie wektorow G" i H" gdzie xmm6 = G"H"
        "movups %3, %%xmm6 \n"
        "movups %4, %%xmm7 \n"
        "shufps $0xF9, %%xmm6, %%xmm6 \n"
        "shufps $0xF0, %%xmm7, %%xmm7 \n"
        "mulps %%xmm7, %%xmm6 \n"

        // xmm4 = G'H' - G"H"
        "subps %%xmm6, %%xmm4 \n"

        // xmm2 = (E'F' - E"F")(G'H' - G"H")
        "mulps %%xmm4, %%xmm2 \n"

        // xmm3 = xmm3 + xmm2
        "addps %%xmm2,%%xmm3 \n"

        // result = xmm3
        "movups %%xmm3, %0 \n"

        // result = xmm0
        : "=m"(result)
        : "m"(x0), "m"(x1), "m"(x2), "m"(x3));

    float det = 0.0;
    for (int i = 0; i < 4; i++) {
        det += result.v[i];
    }

    return det;
}

int main() {
    float M[4][4] = {
        {4.0, 2.0, -5.0, 8.0},
        {1.0, 1.0, -2.0, 0.0},
        {4.0, 0.0, 0.0, 0.0},
        {3.0, -1.0, -2.0, 4.0}};

    struct vect matrixV[4];

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%.1f\t", M[i][j]);
            matrixV[i].v[j] = M[i][j];
        }
        printf("\n");
    }

    printf("\nWyznacznik = %.2f \n", detM(matrixV[0], matrixV[1], matrixV[2], matrixV[3]));
    return 0;
}
