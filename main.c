//gcc -o main main.c
//main
#include <stdio.h>
#include <string.h>
#define lenght 6 //Valeur max de la string en input (vu qu'on a pas encore vu l'allocation dynamique)
#define octet 8
#define nb_octets (lenght*7/8)+1

void affichage_stockage(size_t stockage[nb_octets][octet]);
void affectation_stockage_intermed(size_t stockage[nb_octets][octet], size_t octet_intermed[8], size_t mode, int ind);

int main() {
    //On obtient la string de l'utilisateur dans str
    char str[lenght];
    printf("Enter a message (7 char max) :\n>>");
    scanf("%[^\n]s", &str);
    char tampon= getchar();
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
    printf("\n");
    size_t stockage[nb_octets][octet];
    //Initialisation du tableau avec valeur par default '0'
    for (int i = 0; i < nb_octets; ++i) {
        for (int k = 0; k < octet; ++k) {
            stockage[i][k] = 0;
        }
    }
    //Rangement du bordel de valeur dans un tableau scindant en plusieurs octects
    int m = 0; n = 0;
    for (int i = 0; i < lenght*7; ++i) {
        if (m == octet - 1){
            m = 0;
            n++;
        }
        stockage[n][m] = bin[i];
        m++;
    }
    //Affichage du tableau à double entrée avec les valeurs rangées en octets
    affichage_stockage(stockage);
    char key[20] = {0};
    char key_backup[20] = {0};
    printf("Enter the key :\n>>");
    scanf("%[^\n]s", &key);
    printf("%s\n", key);
    //Sauvegarde backup clé
    for (int i = 0; i < 20 ; ++i) {
        key_backup[i] = key[i];
    }
    //Permutation par transposition rectangulaire par ordre croissant ASCII
    for (int i = 0; i < strlen(key); ++i) {
        int k = i;
        char lettre = key[i];
        size_t octet_intermed[8];
        affectation_stockage_intermed(stockage, octet_intermed, 0, i);
        while ((key[i]< key[k-1]) && (k-1 >= 0)) k--;

        for (int p =  i; p > k ; --p) {
            key[p] = key[p-1];
            for (int l = 0; l < octet; ++l) {
                stockage[p][l] = stockage[p-1][l];
            }
        }
        key[k]=lettre;
        affectation_stockage_intermed(stockage, octet_intermed, 1, k);
    }

    printf("%s\n", key);
    affichage_stockage(stockage);
    return 0;
}

void affectation_stockage_intermed(size_t stockage[nb_octets][octet], size_t octet_intermed[8], size_t mode, int ind){
    //Mode def le sens d'affectation
    // 0 : octet_intermed <- stockage
    // 1 : stockage <- octet_intermed
    if ( mode == 0){
        for (int l = 0; l < octet; ++l) {
            octet_intermed[l] = stockage[ind][l];
        }
    }
    else if ( mode == 1){
        for (int l = 0; l < octet; ++l) {
            stockage[ind][l] = octet_intermed[l];
        }
    }
}

void affichage_stockage(size_t stockage[nb_octets][octet]){
    for (int i = 0; i < nb_octets; ++i) {
        for (int k = 0; k < octet; ++k) {
            printf("%d ", stockage[i][k]);
        }
        putchar('\n');
    }
}
