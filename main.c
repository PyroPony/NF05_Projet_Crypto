#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void afficher_octet (int octect[]);
void etape_3_cryptage (int octect, int bits_octect[]);
void etape_3_decryptage(int bits_octect[], int *octect);
void ETAPE_3 ();

int main() {
    ETAPE_3();
    return 0;
}

void afficher_octet (int octect[]){
    for (int i = 0; i < 8; ++i) {
        printf("%d ", octect[i]);
    }
    putchar('\n');
}
void ETAPE_3 (){
    //Valeurs fictives des octets
    int octect1 = 25, octect2 = 50, octect3 = 125, octect4 = 200;
    //Variables de stockages des octets en 8 bits
    int octect1_bits[8], octect2_bits[8], octect3_bits[8], octect4_bits[8];
    //Cryptaage 8 bits de chaque octect, input [valeur décimale octet + var de sotckage]
    etape_3_cryptage(octect1, octect1_bits);
    etape_3_cryptage(octect2, octect2_bits);
    etape_3_cryptage(octect3, octect3_bits);
    etape_3_cryptage(octect4, octect4_bits);
    afficher_octet(octect1_bits);
    afficher_octet(octect2_bits);
    afficher_octet(octect3_bits);
    afficher_octet(octect4_bits);
    //Remise à 0 des valeurs décimales des octets pour faire le chemin inverse (à partir des 4*8bits cryptés pour retrouver les valeurs fictives)
    octect1 = 0; octect2 = 0; octect3 = 0; octect4 = 0;
    //Decryptage 8 bits de chaque octets en décimal, input [8 bits octet + var de stockage]
    etape_3_decryptage(octect1_bits, &octect1);
    etape_3_decryptage(octect2_bits, &octect2);
    etape_3_decryptage(octect3_bits, &octect3);
    etape_3_decryptage(octect4_bits, &octect4);
}
void etape_3_cryptage (int octect, int bits_octect[]){
    //Constantes
    int bits_inter[8] = {0};
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
    //Boucle de transcription en binaire de la valeur décimal de l'octet
    int j = 0;
    while (octect > 0) {
        bits_inter[7-j] = octect % 2;
        octect = octect / 2;
        j++;
    }
    //[H*V] Boucle pour le produit matriciel
    for (int i = 0; i < 8; ++i) {
        int res = 0;
        for (int k = 0; k < 8; ++k) {
            res += bits_inter[k] * H[i][k];
        }
        bits_octect[i] = res;
    }
    //[H*V+c] Addition de la var précédente avec c + application du modulo %2
    for (int i = 0; i < 8; ++i) {
        bits_octect[i] = (bits_octect[i] + c[i])%2;
    }
}
void etape_3_decryptage(int bits_octect[], int *octect){
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
            res += bits_octect[k] * H[i][k];
        }
        bits_inter[i] = res;
    }
    //[V*H'+c'] Addition de la var précédente avec c' + application du modulo %2
    for (int i = 0; i < 8; ++i) {
        bits_octect[i] = (bits_inter[i] + c[i])%2;
    }
    //Boucle de transcription du binaire à une valeur décimale
    for (int i = 0; i < 8; ++i) {
        *octect += bits_octect[7-i] * pow(2, i);
    }
    printf("%d\n", *octect);
}


