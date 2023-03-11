
#include <stdio.h>
#include <stdlib.h>
#include <fstream>

using namespace std;

ifstream fin("statistics.in");
ofstream fout("statistics.out");

#define NMAX 100000
#define ALPHABETLEN 26
#define FIRSTLETTER 'a' 

typedef struct word {
          int length;
          int f[ALPHABETLEN];
        } WORD;

char buffer[NMAX + 1];
WORD t[NMAX];
int ch;

int cmp(const void* a, const void* b) {
  WORD* pa = (WORD*)a;
  WORD* pb = (WORD*)b;
  int wa = 2 * pa->f[ch] - pa->length;
  int wb = 2 * pb->f[ch] - pb->length;

  return (wb - wa);
}

/*
*   Functia returneaza numarul maxim de cuvinte din lista astfel incat, 
* in urma concatenarii lor, sa existe o litera dominanta.
*   Se parcurge alfabetul (literele de la 'a' la 'z'), si pentru fiecare
* litera ch, se calculeaza lungimea celei mai mari subsecvente astfel
* incat litera ch sa fie dominanta in sirul rezultat in urma concatenarii
* cuvintelor din subsecventa.
*   Pentru un caracter ch, si pentru un cuvant w se calculeaza valoarea
* (2 * numarul de aparitii al caracterului ch in w - lungimea cuvantului w).
* Se ordoneaza descrescator sirul de cuvinte dupa aceste valori calculate. 
* Se aleg cuvinte atata timp cat suma acestor valori este strict pozitiva
* (cat timp caracterul ch este dominant in sirul obtinut prin concatenarea
* cuvintelor de la primul cuvant pana la cuvantul curent).
*  Se returneaza valoarea maxima determinata.
*  Complexitatea algoritmului este O(ALPHABETLEN * (nlogn + n)) = O(n log n)
*  Se poate inlocui ordonarea cu metoda quicksort cu o alta metoda de ordonare 
* ce are complexitatea pentru cazul cel mai defavorabil mai buna decat
* quicksort (O(n^2)), de exemplu mergesort.
*/
int getlongestsubsequencelength(int n) {
  int i, balance, maxcount;

  maxcount = 0;          // numarul maxim de cuvinte ale unui subsir
  // pentru fiecare caracter
  for (ch = 0; ch < ALPHABETLEN; ch++) {
    // Caracterul curent este ch. Se ordoneaza descrescator cuvintele
    // sirului dupa metrica: (numarul de caractere ch din cuvantul 
    // curent * 2 - lungimea cuvantului)
    qsort(t, n, sizeof(WORD), cmp);

    // se parcurge secventa de cuvinte ordonate atata timp cat caracterul 
    // curent ch este dominant in sirul obtinut prin concatenarea cuvintelor 
    // de la primul cuvant pana la cuvintul curent i 
    // (atata timp cat valoarea variabilei balance > 0)
    balance = 2 * t[0].f[ch] - t[0].length;
    for (i = 1; (i < n) && (balance > 0); i++) {
      balance = balance + (2 * t[i].f[ch] - t[i].length);
    }

    // daca balance <= 0 adica daca caracterul ch nu mai este dominant
    // atunci se scoate din secventa ultimul cuvant
    if (balance <= 0) {
      i--;
    }

    // se verifica daca lungimea secventei actuale este mai mare
    // decat cea mai lunga secventa ce are un caracter dominant
    if (maxcount < i) {
      maxcount = i;
    }
  }

  return maxcount;
}

int main() {
  //FILE *fin, *fout;
  int n, i, l;
  char* p;

  //fin = fopen("statistics.in", "r");
  //fout = fopen("statistics.out", "w");

  // fscanf(fin, "%d", &n);
  fin >> n;
  // se citesc cele n cuvinte
  for (i = 0; i < n; i++) {
    // se citeste un cuvant
    // fscanf(fin, "%s", buffer);
    fin >> buffer;
    // se parcurg literele cuvantului curent
    for (p = buffer; *p; p++) {
      // se calculeaza frecventa de aparitie a fiecarei litere
      t[i].f[*p - 'a']++;
    }

    // lungimea cuvantului
    t[i].length = p - buffer;
  }

  l = getlongestsubsequencelength(n);
  if (l == 0) {
    // fprintf(fout, "-1\n");
    fout << -1;
  } else {
    // fprintf(fout, "%d\n", l);
    fout << l;
  }

  fclose(fin);
  fclose(fout);

  return 0;
}
