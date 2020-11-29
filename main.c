#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


void etape_3_cryptage (int bits_octet[]);
void etape_3_decryptage(int bits_octet[]);
void etape_4_cryptage(int *iteration, char *key, int bits_octect1[], int bits_octect2[], int bits_octect3[], int bits_octect4[]);
void etape_4_decryptage(int *iteration, char *key, int bits_octect1[], int bits_octect2[], int bits_octect3[], int bits_octect4[]);
void afficher_octet (int octect[]);
void decimal_octet(int decimal, int *octet);
void octet_decimal(int *decimal, int *bits_octet);
void obtenir_cle(char *key);
void test_des_etapes();

int main() {
    test_des_etapes();
    return 0;
}

void test_des_etapes(){
    char key[30] = {0}, tmp;
    obtenir_cle(key);
    int numero_iteration = 0, nb_iteration;  //numero de l'iteration et nombre d'iteration totale
    int dec_octet1, dec_octet2, dec_octet3, dec_octet4; //Variables pour stockage décimales des 4 octets
    int octet1_bits[8] = {0}, octet2_bits[8] = {0}, octet3_bits[8] = {0}, octet4_bits[8] = {0}; //Variables pour stockage bits des 4 octets
    printf("Enter the decimal values : [O1] [O2] [O3] [O4] :\n>>");
    scanf("%d %d %d %d", &dec_octet1, &dec_octet2, &dec_octet3, &dec_octet4); tmp = getchar();
    printf("Enter the number of iteration :\n>>");
    scanf("%d", &nb_iteration); tmp = getchar();
    printf("Initial Bytes : \n%d %d %d %d\n", dec_octet1, dec_octet2, dec_octet3, dec_octet4);
    //Conversion des valeurs décimales des octets en octets de bits
    decimal_octet(dec_octet1, octet1_bits);
    decimal_octet(dec_octet2, octet2_bits);
    decimal_octet(dec_octet3, octet3_bits);
    decimal_octet(dec_octet4, octet4_bits);
    for (int i = 0; i < nb_iteration; ++i) {
        etape_3_cryptage(octet1_bits);         //Faire un passage par adresse de dec_octectX pour pouvoir l'update et lui repasser une couche
        etape_3_cryptage(octet2_bits);
        etape_3_cryptage(octet3_bits);
        etape_3_cryptage(octet4_bits);
            printf("Crypted Bytes STEP 3 :\n");
            afficher_octet(octet1_bits);
            afficher_octet(octet2_bits);
            afficher_octet(octet3_bits);
            afficher_octet(octet4_bits);
        etape_4_cryptage(&numero_iteration, key, octet1_bits, octet2_bits, octet3_bits, octet4_bits);
            printf("Crypted Bytes STEP 4 :\n");
            afficher_octet(octet1_bits);
            afficher_octet(octet2_bits);
            afficher_octet(octet3_bits);
            afficher_octet(octet4_bits);
    }
    //rajouter le for et enlever le passage par adresse useless
    for (int i = 0; i < nb_iteration; ++i) {
        numero_iteration = i+1;
        etape_4_decryptage(&numero_iteration, key, octet1_bits, octet2_bits, octet3_bits, octet4_bits);
            printf("Uncrypted Bytes STEP 4 :\n");
            afficher_octet(octet1_bits);
            afficher_octet(octet2_bits);
            afficher_octet(octet3_bits);
            afficher_octet(octet4_bits);
        etape_3_decryptage(octet1_bits);
        etape_3_decryptage(octet2_bits);
        etape_3_decryptage(octet3_bits);
        etape_3_decryptage(octet4_bits);
    }
    octet_decimal(&dec_octet1, octet1_bits);
    octet_decimal(&dec_octet2, octet2_bits);
    octet_decimal(&dec_octet3, octet3_bits);
    octet_decimal(&dec_octet4, octet4_bits);
    printf("Uncrypted Bytes STEP 3 : \n%d %d %d %d", dec_octet1, dec_octet2, dec_octet3, dec_octet4);
}

void obtenir_cle(char *key){
    int tmp;
    printf("Enter the key :\n>>");
    scanf("%[^\n]s", key);
    tmp = getchar();
}
void sous_cle(int mode, int *N, char *K, int *SK){
    if (mode == 0) (*N) ++;
    //else (*N) --;
    int sum = 0;
    for (int i = 0; i < strlen(K); ++i) sum += K[i];
    sum = sum / *N;
    int i = 0;
    while (sum > 0 && i<8){
        SK[7-i] = sum % 2;
        sum = sum / 2;
        i++;
    }
}
void etape_4_cryptage(int *iteration, char *key, int bits_octect1[], int bits_octect2[], int bits_octect3[], int bits_octect4[]){
    int second_key[8] = {0};
    sous_cle(0, iteration, key, second_key); //afficher_octet (second_key);
    for (int i = 0; i < 8; ++i) {
        bits_octect1[i] ^= second_key[i];
        bits_octect2[i] ^= second_key[i];
        bits_octect3[i] ^= second_key[i];
        bits_octect4[i] ^= second_key[i];
    }
}
void etape_4_decryptage(int *iteration, char *key, int bits_octect1[], int bits_octect2[], int bits_octect3[], int bits_octect4[]) { //A retenir que selon la génération de clé, pour décrypter => commencer avec itération = max et ensuite décrémenter
    int second_key[8] = {0};
    sous_cle(1, iteration, key, second_key); //afficher_octet (second_key);
    for (int i = 0; i < 8; ++i) {
        bits_octect1[i] ^= second_key[i];
        bits_octect2[i] ^= second_key[i];
        bits_octect3[i] ^= second_key[i];
        bits_octect4[i] ^= second_key[i];
    }
}

void etape_3_cryptage (int bits_octet[]){
    //Constantes
    int bits_inter[8] = {0};
    for (int i = 0; i < 8; ++i) {
        bits_inter[i] = bits_octet[i];
    }
    int H[8][8]= {
            {1, 0, 0, 0, 1, 1, 1, 1},
            {1, 1, 0, 0, 0, 1, 1, 1},
            {1, 1, 1, 0, 0, 0, 1, 1},
            {1, 1, 1, 1, 0, 0, 0, 1},
            {1, 1, 1, 1, 1, 0, 0, 0},
            {0, 1, 1, 1, 1, 1, 0, 0},
            {0, 0, 1, 1, 1, 1, 1, 0},
            {0, 0, 0, 1, 1, 1, 1, 1}};
    int c[8] = {1, 1, 0, 0, 0, 1 ,1,0};
    //[H*V] Boucle pour le produit matriciel
    for (int i = 0; i < 8; ++i) {
        int res = 0;
        for (int k = 0; k < 8; ++k) {
            //res += bits_inter[k] * H[i][k];
            res += bits_inter[k] * H[i][k];
        }
        bits_octet[i] = res;
    }
    //[H*V+c] Addition de la var précédente avec c + application du modulo %2
    for (int i = 0; i < 8; ++i) {
        bits_octet[i] = (bits_octet[i] + c[i])%2;
    }
}
void etape_3_decryptage(int bits_octet[]){
    //Constantes
    int bits_inter[8] = {0};
    int H[8][8] = {
            { 0, 0, 1, 0, 0, 1, 0, 1},
            { 1, 0, 0, 1, 0, 0, 1, 0},
            { 0, 1, 0, 0, 1, 0, 0, 1},
            { 1, 0, 1, 0, 0, 1, 0, 0},
            { 0, 1, 0, 1, 0, 0, 1, 0},
            { 0, 0, 1, 0, 1, 0, 0, 1},
            { 1, 0, 0, 1, 0, 1, 0, 0},
            { 0, 1, 0, 0, 1, 0, 1, 0}
    };
    int c[8] = { 1, 0, 1, 0, 0, 0, 0, 0};
    //[V*H'] Boucle pour le produit matriciel
    for (int i = 0; i < 8; ++i) {
        int res = 0;
        for (int k = 0; k < 8; ++k) {
            res += bits_octet[k] * H[i][k];
        }
        bits_inter[i] = res;
    }
    //[V*H'+c'] Addition de la var précédente avec c' + application du modulo %2
    for (int i = 0; i < 8; ++i) {
        bits_octet[i] = (bits_inter[i] + c[i])%2;
    }
}
void decimal_octet(int decimal, int *octet){
    //Boucle de transcription en binaire de la valeur décimal de l'octet
    int j = 0;
    while (decimal > 0){
        octet[7-j] = decimal% 2;
        decimal = decimal / 2;
        j++;
    }
}
void octet_decimal(int *decimal, int *bits_octet){
    //Boucle de transcription du binaire à une valeur décimale
    *decimal = 0;
    for (int i = 0; i < 8; ++i) {
        *decimal += bits_octet[7-i] * pow(2, i);
    }
}
void afficher_octet (int octect[]){
    for (int i = 0; i < 8; ++i) {
        printf("%d ", octect[i]);
    }
    putchar('\n');
}
