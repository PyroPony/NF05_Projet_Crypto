#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int oct[4]; // variable globale pour traiter les variables en decimal
int bits_oct[4][8]; // variable globale pour traiter les données en binaire



// Fonction passage de valeurs decimales a valeurs binaires
void pass_binary(){
  int i1, i2;

  // initialisation de tous les bits des octets a 0
  for (i1 = 0; i1 < 4; i1++){
    for (i2 = 0; i2 < 8; i2++){
      bits_oct[i1][i2] = 0;
    }
  }
  // calcul valeur binaire bits par bits
  for (i1 = 0; i1<4; i1++){
    int octet = oct[i1]; // pour ne pas modifier la valeur originale de l'octet
    for (i2 = 7; octet > 0 || i2 >= 0; i2--){
      bits_oct[i1][i2] = octet%2;
      octet = octet/2;
    }
  }
}
// Fonction passage de valeurs binaires a valeurs decimales
void pass_decimal(){
  int i1, i2;

  // Initialisation de tous les octets a 0
  for (i1 = 0; i1 < 4; i1++){
    oct[i1] = 0;
  }
  // Calcul valeur decimale
  for (i1 = 0; i1 < 4; i1++){
    for (i2 = 0; i2 < 8; i2++){
      oct[i1] += bits_oct[i1][7 - i2] * pow(2, i2);
      oct[i1] = oct[i1] % 256;
      if (oct[i1] < 0){
        do {oct[i1] = oct[i1] + 256;} while (oct[i1] < 0);
      }
    }
  }
}

// Fonction generant sous-cles pour fonction 4
void sous_cle(int mode, int nb_it_tot, int etape_it, char key[], int sub_key[]){
  int sum = 0;
  int i, div;


  // DETERMINATION SOUS CLE : somme valeurs caracteres cle chiffrement divisé en fontion etape d'iteration
  for (i = 0; i < strlen(key); i++){
    sum += (int)key[i];
  }

  // DISTINCTION ENTRE CRYPTAGE ET DECRYPTAGE : crypt -> 1 a nb d'iteration / decrypt -> nb d'iteration à 1
  if (mode == 0){
    div = etape_it + 1;
  }
  else{
    div = nb_it_tot - etape_it;
  }
  sum = (int) (sum / div); // prendre valeur entiere tronquee
  sum = sum % 256; // prendre valeur entre 0 et 255 car octet 8 bits

  // PASSAGE BINAIRE DE LA SOUS CLE
  for (i = 7; sum > 0 || i >= 0; i--){
    sub_key[i] = sum % 2;
    sum = sum / 2;
  }
}


//ETAPE 1 CRYPTAGE
void etape_1_crypt(char key[]){
  int i;

  for (i = 0; i < 4; i++){
    oct[i] = (oct[i] + (int)key[0])%256;
  }
}
//ETAPE 1 DECRYPTAGE
void etape_1_decrypt(char key[]){
  int i;

  for (i = 0; i < 4; i++){
    oct[i] = (oct[i] - (int)key[0])%256;

    // pour toujours avoir un resultat positif
    if (oct[i] < 0){
      do {oct[i] = oct[i] + 256;} while (oct[i] < 0);
    }
  }
}


// ETAPE 3 CRYPTAGE
void etape_3_crypt (){
  int i1, i2;

  //Initialisation bits intermediaires de calcul
  int bits_inter[4][8];
  for (i1 = 0; i1 < 4; i1++){
    for (i2 = 0; i2 < 8; i2++){
        bits_inter[i1][i2] = bits_oct[i1][i2];
    }
  }

  //Initialisation des constantes
  int H[8][8]= {
            {1, 0, 0, 0, 1, 1, 1, 1},
            {1, 1, 0, 0, 0, 1, 1, 1},
            {1, 1, 1, 0, 0, 0, 1, 1},
            {1, 1, 1, 1, 0, 0, 0, 1},
            {1, 1, 1, 1, 1, 0, 0, 0},
            {0, 1, 1, 1, 1, 1, 0, 0},
            {0, 0, 1, 1, 1, 1, 1, 0},
            {0, 0, 0, 1, 1, 1, 1, 1}};
  int c[8] = {1, 1, 0, 0, 0, 1 , 1, 0};

  //[H*V] Boucle pour le produit matriciel avec modulo 2 (pas obligatoire, peut se faire etape suivante)
  for (i1 = 0; i1 < 4; i1++){
    for (i2 = 0; i2 < 8; i2++){
        int res = 0;
        for (int i3 = 0; i3 < 8; i3++){
            res += bits_inter[i1][i3] * H[i2][i3];
        }
        bits_oct[i1][i2] = res % 2;
    }
  }
  //[H*V+c] Addition de la var précédente avec c avec modulo 2
  for (i1 = 0; i1 < 4; i1++){
    for (i2 = 0; i2 < 8; i2++) {
        bits_oct[i1][i2] = (bits_oct[i1][i2] + c[i2])%2;
    }
  }

}
// ETAPE 3 DECRYPTAGE
void etape_3_decrypt(){
  int i1, i2;

  //passage des octets en binaire non obligatoire comme dans l'etape 4 du decryptage on travaille en binaire
  //Initialisation bits intermediaires de calcul
  int bits_inter[4][8];
  for (i1 = 0; i1 < 4; i1++){
    for (i2 = 0; i2 < 8; i2++){
        bits_inter[i1][i2] = bits_oct[i1][i2]; //initialisation des valeurs des bits intermediaires
    }
  }

  //Initialisation des constantes
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

  //[H'*V] Boucle pour le produit matriciel avec application modulo 2 (peut etre effectue etape suivante)
  for (i1 = 0; i1 < 4; i1++){
    for (i2 = 0; i2 < 8; i2++){
        int res = 0;
        for (int i3 = 0; i3 < 8; i3++){
            res += bits_inter[i1][i3] * H[i2][i3];
        }
        bits_oct[i1][i2] = res %2;
    }
  }

  //[H'*V+c'] Addition de la var précédente avec c' + application du modulo %2
  for (i1 = 0; i1 < 4; i1++){
    for (i2 = 0; i2 < 8; i2++){
        bits_oct[i1][i2] = (bits_oct[i1][i2] + c[i2])%2;
    }
  }

}


//ETAPE 4 CRYPTAGE
void etape_4_crypt(int num_iteration, int etape_iteration, char key[]){
  int i1, i2;
  int sub_key[8] = {0};

  //Generation sous_cle
  sous_cle(0,  num_iteration, etape_iteration, key, sub_key);

  // Application calcul XOR bit par bit
  for (i1 = 0; i1<4; i1++){
    for (i2 = 0; i2 < 8; i2++) {
        bits_oct[i1][i2] ^= sub_key[i2];
    }
  }
}
//ETAPE 4 DECRYPTAGE
void etape_4_decrypt(int nb_iteration, int etape_iteration, char key[]){
  int i1, i2;
  int sub_key[8] = {0};

  // Generation sous_cle
  sous_cle(1, nb_iteration, etape_iteration, key, sub_key);

  // Application calcul XOR bit par bit
  for (i1 = 0; i1 < 4; i1++){
    for(i2 = 0; i2 < 8; i2++){
      bits_oct[i1][i2] ^= sub_key[i2];
    }
  }
}


//ETAPE 5 CRYPTAGE
void etape_5_crypt(){
  int inter[4]; // pour copier les anciennes valeurs des octets
  int i;

  // Boucle copie ancienne valeur
  for(i = 0; i < 4; i++){
    inter[i] = oct[i];
  }

  oct[0] = (inter[0] + inter[1]) % 256;
  oct[1] = (inter[0] + inter[1] + inter[2]) % 256;
  oct[2] = (inter[1] + inter[2]+ inter[3]) % 256;
  oct[3] = (inter[2] + inter[3]) % 256;

  // Boule verification valeures entre 0 et 255 car octet = 8 bit 
  for(i = 0; i < 4; i++){
    if (oct[i] < 0){
      do {oct[i] = oct[i] + 256;} while (oct[i] < 0);
    }
  }
}
//ETAPE 5 DECRYTAGE
void etape_5_decrypt(){
  int inter[4];
  int i;

  for(i = 0; i < 4; i++){
    inter[i] = oct[i];
  }

  oct[0] = (inter[0] - inter[2] + inter[3]) % 256;
  oct[1] = (inter[2] - inter[3]) % 256;
  oct[2] = (inter[1] - inter[0]) % 256;
  oct[3] = (inter[3] - inter[1] + inter[0]) % 256;

  for(i = 0; i < 4; i++){
    if (oct[i] < 0){
      do {oct[i] = oct[i] + 256;} while (oct[i] < 0);
    }
  }
}

int main() {
  int etape;
  int choix_ops;

  // Saisie de l'operation souhaitee avec un controle par l'algorithme du bon choix
  do{
    printf ("Quelle operation souhaitez vous effectuer : \nTapez 0 pour decrypter \nTapez 1 pour crypter\n\nChoix operation : ");
    scanf("%d", &choix_ops);
    } while (choix_ops != 1 && choix_ops != 0);

  // Saisie nombre iteration
  printf("\n\nCombien d'iteration votre cryptage/decryptage necessite-t-il?\nNombre iteration : ");
  int nb_iteration;
  scanf("%d", &nb_iteration);
  getchar(); // Vider Buffer Std In pour ne pas deranger lecture cle

  printf("\n\nEntrez votre cle, elle ne doit pas contenir d\'espaces\nCle : ");
  char key[20];
  scanf("%s", &key);

//Dans ce commentaire les valeurs ne servent que de test

// ICI LECTURE DES 4 Octects mais valeur juste pour tester !
  oct[0]= 2; oct[1]= 250; oct[2]= 255; oct[3]= 15;

/*
//LECTURE DES VALEUR BINAIRE JUSTE POUR TESTER !
  bits_oct[1][0] = 0; bits_oct[1][1] = 1; bits_oct[1][2] = 1; bits_oct[1][3] = 0; bits_oct[1][4] = 1; bits_oct[1][5] = 1; bits_oct[1][6] = 0; bits_oct[1][7] = 1;
  bits_oct[0][0] = 0; bits_oct[0][1] = 0; bits_oct[0][2] = 1; bits_oct[0][3] = 0; bits_oct[0][4] = 1; bits_oct[0][5] = 1; bits_oct[0][6] = 0; bits_oct[0][7] = 0;
  bits_oct[2][0] = 1; bits_oct[2][1] = 1; bits_oct[2][2] = 1; bits_oct[2][3] = 1; bits_oct[2][4] = 1; bits_oct[2][5] = 1; bits_oct[2][6] = 1; bits_oct[2][7] = 1;
  bits_oct[3][0] = 0; bits_oct[3][1] = 1; bits_oct[3][2] = 1; bits_oct[3][3] = 0; bits_oct[3][4] = 1; bits_oct[3][5] = 1; bits_oct[3][6] = 1; bits_oct[3][7] = 1;
// oct 1 = 44 - oct 2 = 109 - oct 3 = 255 - oct 4 = 111
*/

// il faudra ajouter lecture et écriture fichier
  for (etape = 0; etape < nb_iteration; etape++){
    // CRYPTAGE
    if (choix_ops == 1){
      etape_1_crypt(key);
      pass_binary();
      etape_3_crypt();
      etape_4_crypt(nb_iteration, etape, key);
      pass_decimal();
      etape_5_crypt();
    }

    // DECRYPTAGE
    if (choix_ops == 0){
      etape_5_decrypt();
      pass_binary();
      etape_4_decrypt(nb_iteration, etape, key);
      etape_3_decrypt();
      pass_decimal();
      etape_1_decrypt(key);
    }
  }


  return 0;
}
