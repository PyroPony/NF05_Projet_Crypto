#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

void pass_binary(){

}

void pass_decimal(){

}

void etape_1_crypt(){
  int i;
  char cle_1;
  int oct[4];
// Eventuellement voir pour que ecart = somme caractère 1 de la clé + carcatère 2 etc.. pour complexifier
  for (i = 0; i < 4; i++){
    oct[i] = (i + (int)cle_1)%256;
  }
}

void etape_1_decrypt(){
  int i;
  char cle_1;
  int oct[4];
// Eventuellement voir pour que ecart = somme caractère 1 de la clé + carcatère 2 etc.. pour complexifier
  for (i = 0; i < 4; i++){
    oct[i] = (i - (int)cle_1)%256;
  }
}

void etape_5_crypt(){
  int i;
  int Y_oct[4];
  int Z_oct[4];

  Z_oct[0] = Y_oct[0] + Y_oct[1];
  Z_oct[1] = Y_oct[0] + Y_oct[1] + Y_oct[2];
  Z_oct[2] = Y_oct[1] + Y_oct[2]+ Y_oct[3];
  Z_oct[3] = Y_oct[2] + Y_oct[3];
}

void etape_5_decrypt(){
  int i;
  int Y_oct[4];
  int Z_oct[4];
// Calcul a faire
  Y_oct[0] = Z_oct[0] - Z_oct[1] + Z_oct[3];
  Y_oct[1] = Z_oct[2] - Z_oct[3];
  Y_oct[2] = Z_oct[1] - Z_oct[0];
  Y_oct[3] = Z_oct[3] - Z_oct[1] + Z_oct[0];
}

int main() {
  int choix_ops, nb_iteration;

  printf ("Voulez-vous crypter : tapez 0 ou decrypter : tapez 1\n");
  scanf("%d", &choix_ops);

  printf("Combien d'iteration votre cryptage/decryptage necessite-t-il?\n");
  scanf("%d", &nb_iteration);

  for (i = 0; i < nb_iteration; i++){
    // CRYPTAGE
    if (choix_ops == 0){
      etape_1_crypt();
      etape_2_crypt();
      etape_3_crypt();
      etape_4_crypt();
      etape_5_crypt();
    }

    // DECRYPTAGE
    if (choix_ops == 1){
      etape_1_decrypt();
      etape_2_decrypt();
      etape_3_decrypt();
      etape_4_decrypt();
      etape_5_decrypt();
    }
  }

  return 0;
}
