#include <stdio.h>

/*
*  Functia intoarce valoarea elementului de la coordonatele (l,c)
* din submatricea ce are coordonatele coltului stanga-sus (l0,c0)
* si lungimea lenurii len. neg indica daca elementele matricei au
* fost negate (neg = 1)sau nu (neg = 0).
*/
int getValue(int l0, int c0, int len, int l, int c, int neg) {
  int len2;

  if (len == 1) {
    return neg;
  } else {
      len2 = len >> 1;
      if (l < l0 + len2) {   // C1 sau C2
        if (c < c0 + len2) { // C1
          return getValue(l0, c0, len2, l, c, neg);
        } else {             // C2
            return getValue(l0, c0 + len2, len2, l, c, neg);
        }
      } else { // C3 sau C4
          if (c < c0 + len2) { // C3
            return getValue(l0 + len2, c0, len2, l, c, neg);
          } else {             // C4
              return getValue(l0 + len2, c0 + len2, len2, l, c, 1 - neg);
          }
      }
  }
}

int main() {
  FILE *fin, *fout;
  int n, k, x, y;

  fin = fopen("walsh.in", "r");
  fout = fopen("walsh.out", "w");

  fscanf(fin, "%d %d", &n, &k);

  while (k--) {
    fscanf(fin, "%d %d", &x, &y);

    fprintf(fout, "%d\n", getValue(1, 1, n, x, y, 0));
  }

  fclose(fin);
  fclose(fout);

  return 0;
}
