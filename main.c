//gcc -o main main.c
//main
#include <stdio.h>
#include <string.h>
#define lenght 100 //Valeur max de la string en input (vu qu'on a pas encore vu l'allocation dynamique)


int main() {
    //On obtient la string de l'utilisateur dans str
    char str[lenght];
    scanf("%[^\n]s", &str);
    //printf("%s\n", str);
    //printf("%d\n", strlen(str));
    //On convertit la string de caractere en une array de binaire
    size_t n = strlen(str);
    int bin[lenght*7] = {0}; //valeur par defaut
    int j = 0;
    for (int i = 0; i < n; ++i) { //pour chaque caractere -> obtention de la valeur decimale -> val, puis conversion de val en binaire et intégration de val dans l'array binaire
        size_t val = str[i];
        while (val > 0) {
            bin[j] = val % 2;
            val = val / 2;
            j++;
        }
    }
    //Affichage de la array binaire représentant
    for (int i = 0; i < lenght*7 ; ++i) {
        printf("%d", bin[i]);
    }
    //Initialisation de variables pour le stockages
    size_t octet = 8;
    int nb_octects = (lenght*7/8)+1;
    size_t stockage[8][nb_octects];
    //Initialisation du tableau avec valeur par default '0'
    for (int i = 0; i < nb_octects; ++i) {
        for (int k = 0; k < 8; ++k) {
            stockage[k][i] = 0;
        }
    }
    //Rangement du bordel de valeur dans un tableau scindant en plusieurs octects
    int m = 0; n = 0;
    for (int i = 0; i < lenght*7; ++i) {
        if (m == octet){
            m = 0;
            n++;
        }
        stockage[m][n] = bin[i];
        m++;
    }
    //Affichage du tableau à double entrée avec les valeurs rangées en octets
    for (int i = 0; i < nb_octects; ++i) {
        for (int k = 0; k < octet; ++k) {
            printf("%d ", stockage[k][i]);
        }
        putchar('\n');
    }
    return 0;
}
