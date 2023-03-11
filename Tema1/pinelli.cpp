#include <stdio.h>
#include <fstream>

using namespace std;

ifstream fin("prinel.in");
ofstream fout("prinel.out");

#define NMAX 1000       // numarul maxim de valori
#define VMAX 100000     // valoarea maxima a unui numar
#define KMAX 1000000    // numarul maxim de transformari

int target[NMAX];     // valorile care ar trebui obtinut prin transformari 1 -> target[i]
int p[NMAX];          // p[i] - numarul de puncte acordat daca target[i] 
                      //        poate fi obtinut din valoarea 1 prin transformari 
int a[VMAX + 1];      // a[i] - numarul minim de transformari prin care valoarea
                      //        i poate fi obtinuta din 1 prin tranformarile 
                      //        descrise de problema
int gain[KMAX + 1];   // gain[i] - 

/*
*   Functia intoarce valoarea maxima dintre doua valori intregi.
*/
int max(int u, int v) {
  return (u > v) ? u : v;
}

/*
*  Functia calculeaza valorile tabloului A. Pentru fiecare valoare
*  din intervalul [1, limit] calculeaza a[i] - numarul minim de 
*  transformari prin care se poate ajunge de la valoarea 1 la i.
*  Complexitate O(limit * sqrt(limit)).
*  limit este valoarea cea mai mare din tabloul target.
*/
void build(int limit) {
  int i, d;

  for (i = 0; i <= limit; i++) {
    a[i] = KMAX;
  }

  a[1] = 0;
  a[2] = 1;
  for (i = 2; i <= limit; i++) {
    // daca valoarea i a fost obtinuta prin transformari pornind de la 1
    if (a[i] < KMAX) {
      // determinam divizorii lui i
      for (d = 1; d * d <= i; d++) {
        // daca d este divizor al lui i
        if (i % d == 0) {
          // verificam daca i + d nu depaseste valoare limit
          // si daca a[i + d] se poate obtine cu un numar mai mic
          // de transformari
          if ((i + d <= limit) && (a[i] + 1 < a[i + d])) {
            a[i + d] = a[i] + 1;
          }

          // verificam daca d si i/d sunt divizori distincti
          if (d * d < i) {
            // aceleasi conditii ca mai sus pentru i + i/d
            if ((i + i / d <= limit) && (a[i] + 1 < a[i + i /d])) {
              a[i + i / d] = a[i] + 1;
            }
          }
        }
      }
    }
  }
}

/*
*   Functia compacteaza tablourile target si p.
*  Practic acele valori target[i] pentru care numarul de transformari
*  1 -> target[i] este mai mare de k, sunt sterse din lista de valori.
*  Complexitate O(n).
*/
int compact(int n, int k) {
  int i, j;

  for (i = j = 0; i < n; i++) {
    if (a[target[i]] <= k) {
      if (j < i) {
        target[j] = target[i];
        p[j] = p[i];
      }
      j++;
    }
  }

  return j;
}

/*
*  Se aplica tehnica de la problema discreta a rucsacului.
*  Volumul maxim al rucsacului este K, "greutatile" produselor sunt
*  in vectorul a[] iar "castigurile" sunt in vectorul p[].
*  Algoritmul implementeaza metoda programarii dinamice.
*  Se calculeaza valorile vectorului gain[]: gain[i] - castigul maxim
*  ce se poate obtine prin alegerea unor elemente din vectorul target[]
*  elemente la care s-au folosit in total i transformari.
*  Complexitate O(n * k)
*/
int compute(int n, int k) {
  int i, j, limit, vmax;
  
  for (i = 0; i < k; i++) {
    gain[i] = -1;
  }

  // calculam castigul total pentru elementele target[i] = 1
  // un astfel de element se obtine in urma a 0 transformari
  gain[0] = 0;
  for (i = 0; i < n; i++) {
    if (a[target[i]] == 0) {
      gain[0] += p[i];
    }
  }

  limit = 0;  // valoarea maxima la care s-a ajuns la pasul curent
  // se parcurge multimea de valori
  for (i = 0; i < n; i++) {
    // daca numarul de transformari 1 -> target[i] este strict pozitiv
    if (a[target[i]] > 0) {
      for (j = limit; j >= 0; j--) {
        // verificam ca j + a[target[i]] sa nu depaseasca limita K,
        // j sa se fi obtinut anterior
        // iar castigul obtinut prin selectarea valorii target[i] (gain[j] + p[i])
        // sa fie mai mare decat castigul obtinut anterior (gain[j + a[target[i]]])
        if ((j + a[target[i]] <= k) && (gain[j] > -1) 
            && (gain[j] + p[i] > gain[j + a[target[i]]])) {
          gain[j + a[target[i]]] = gain[j] + p[i];

          limit = max(limit, j + a[target[i]]);  // se actualizeaza valoare maxima
        }
      }
    }
  }

  // calculam castigul maxim obtinut din cel mult k transformari
  vmax = gain[0];
  for (i = 1; i <= limit; i++) {
    vmax = max(vmax, gain[i]);
  }

  return vmax;
}

int main() {
  // FILE *fin, *fout;
  int n, k, i, vmax;

  // fin = fopen("prinel.in", "r");
  // fout = fopen("prinel.out", "w");

  //fscanf(fin, "%d %d", &n, &k);
  fin >> n >> k;

  for (i = 0, vmax = 0; i < n; i++) {
    // fscanf(fin, "%d", &target[i]);
    fin >> target[i];
    vmax = max(vmax, target[i]);
  }

  for (i = 0; i < n; i++) {
    // fscanf(fin, "%d", &p[i]);
    fin >> p[i];
  }

  build(vmax);

  n = compact(n, k);

  // fprintf(fout, "%d\n", compute(n, k));
    fout << compute(n,k);
  fclose(fin);
  fclose(fout);

  return 0;
}
