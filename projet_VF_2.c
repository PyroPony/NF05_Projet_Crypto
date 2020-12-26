#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//Definition d'operation
#define CRYPT     0
#define DECRYPT   1
#define FALSE     0
#define TRUE      1

// variables globales pour traiter les octets (decimal et binaire)
int oct[4];
int bits_oct[4][8];


/**
 *@fn pass_binary()
 *@brief Passage de valeurs decimales a valeurs binaires

 *@param N/A
 *@return N/A
*/
void pass_binary()
{
    // Initialisation de tous les bits des octets a 0
    for (int i1 = 0; i1 < 4; i1++) {
        for (int i2 = 0; i2 < 8; i2++)
            bits_oct[i1][i2] = 0;
    }

    // Calcul valeur binaire bits par bits
    for (int i1 = 0; i1<4; i1++) {
        int octet = oct[i1]; // pour ne pas modifier la valeur originale de l'octet
        for (int i2 = 7; octet > 0 || i2 >= 0; i2--) {
            bits_oct[i1][i2] = octet%2;
            octet = octet/2;
        }
    }
}
/**
 *@fn pass_decimal()
 *@brief Passage de valeurs binaires a valeurs decimales

 *@param N/A
 *@return N/A
*/
void pass_decimal()
{
    // Initialisation de tous les octets a 0
    for (int i1 = 0; i1 < 4; i1++){
        oct[i1] = 0;
    }
    // Calcul valeur decimale
    for (int i1 = 0; i1 < 4; i1++){
        for (int i2 = 0; i2 < 8; i2++){
            oct[i1] += bits_oct[i1][7 - i2] * (int)pow(2, i2);
            oct[i1] = oct[i1] % 256;
            while (oct[i1] < 0) oct[i1] = oct[i1] + 256;
        }
    }
}



/**
 *@fn etape_1_crypt(char *key)
 *@brief Etape de cryptage 1 - Associe à chaque valeur une autre valeur par ajout valeurs ASCII termes de clé

 *@param *key       Passage de la clé de chiffrement grâce à son adresse

 *@return N/A
*/
void etape_1_crypt(char *key)
{
    for (int i = 0; i < 4; i++)
      oct[i] = (oct[i] + (int)key[0])%256;
}
/**
 *@fn etape_1_decrypt(char *key)
 *@brief Etape de cryptage 1 - Associe à chaque valeur une autre valeur par retait valeur ASCII des termes de clé

 *@param *key       Passage de la clé de chiffrement grâce à son adresse

 *@return N/A
*/
void etape_1_decrypt(char *key)
{
    for (int i = 0; i < 4; i++) {
        oct[i] = (oct[i] - (int)key[0])%256;

        // pour toujours avoir un resultat positif
        while (oct[i] < 0)
          oct[i] = oct[i] + 256;
    }
}



/**
 *@fn etape_2_crypt_decrypt()
 *@brief Etape de cryptage et décryptage 2 - Associe à chaque x un y grâce au fichier permut_etape_2.txt

 *@param N/A
 *@return N/A
*/
void etape_2_crypt_decrypt()
{
  // Declaration tableau de permutation et pointeur fichier
  int tab_permut[256][2];
  FILE *file_permut;

  // Ouverture fichier avec table de permutation -> doit ê dans même fichier que le code
  file_permut = fopen("permut_etap_2.txt", "r");

  // Vérification bonne ouverture du fichier
  if (file_permut == NULL) {
      printf("Erreur ouverture fichier permutation\n");
      exit(EXIT_FAILURE);
  }
  else {
      // Boucle de copie de la table de permutation
      for(int i1 = 0; i1 < 256; i1++) {
        for (int i2 = 0; i2 < 2; i2++) {
          fscanf(file_permut, "%d", &tab_permut[i1][i2]);
        }
      }

      //Recherche valeur dans table permutation et permutation
      for(int i = 0; i < 4; i++) {
        for (int indx = 0; indx < 256; indx++) {
          if(oct[i] == tab_permut[indx][0]) {
            oct[i] = tab_permut[indx][1];
            break;
          }
        }
      }
      fclose(file_permut);
  }
}



/**
 *@fn etape_3_crypt()
 *@brief Etape de cryptage 3 - Calcul matriciel selon constantes imposées dans le sujet

 *@param N/A
 *@return N/A
*/
void etape_3_crypt ()
{
    //Initialisation bits intermediaires de calcul
    int bits_inter[4][8];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 8; j++) {
            bits_inter[i][j] = bits_oct[i][j];
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
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 8; j++) {
            int res = 0;
            for (int k = 0; k < 8; k++) {
                res += bits_inter[i][k] * H[j][k];
            }
            bits_oct[i][j] = res % 2;
        }
    }
    //[H*V+c] Addition de la var précédente avec c avec modulo 2
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 8; j++) {
            bits_oct[i][j] = (bits_oct[i][j] + c[j])%2;
        }
    }
}
/**
 *@fn etape_3_decrypt()
 *@brief Etape de décryptage 3 - Calcul matriciel selon constantes imposées dans le sujet

 *@param N/A
 *@return N/A
*/
void etape_3_decrypt()
{
    //passage des octets en binaire non obligatoire comme dans l'etape 4 du decryptage on travaille en binaire
    //Initialisation bits intermediaires de calcul
    int bits_inter[4][8];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 8; j++) {
            bits_inter[i][j] = bits_oct[i][j]; //initialisation des valeurs des bits intermediaires
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
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 8; j++) {
            int res = 0;
            for (int k = 0; k < 8; k++) {
                res += bits_inter[i][k] * H[j][k];
            }
            bits_oct[i][j] = res %2;
        }
    }

    //[H'*V+c'] Addition de la var précédente avec c' + application du modulo %2
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 8; j++) {
            bits_oct[i][j] = (bits_oct[i][j] + c[j])%2;
        }
    }
}


/**
 *@fn sous_cle(int mode, int nb_it_tot, int etape_it, char key[], int sub_key[])
 *@brief Generation sous-cles a partir clé de cryptage et etape d'itération pour étape 4 cryptage

 *@param mode            Permet de savoir si l'on est en cryptage (0) ou en décryptage (1)
 *@param nb_it_tot       Nombre total d'itérations
 *@param etape_it        Etape actuelle d'itération
 *@param *key            Pointeur de la clé de chiffrement
 *@param sub_key[]       Variable de la sous_cle

 *@return N/A
*/
void sous_cle(int mode, int nb_it_tot, int etape_it, char *key, int sub_key[])
{
    int sum = 0, div;

    // DETERMINATION SOUS CLE : somme valeurs caracteres cle chiffrement divisé en fontion etape d'iteration
    for (int i = 0; i < strlen(key); i++) sum += (int)key[i];

    // DISTINCTION ENTRE CRYPTAGE ET DECRYPTAGE : crypt -> 1 a nb d'iteration / decrypt -> nb d'iteration à 1
    if (mode == 0) div = etape_it + 1;
    else div = nb_it_tot - etape_it;

    sum /= div; // prendre valeur entiere tronquee

    // PASSAGE BINAIRE DE LA SOUS CLE
    int i = 0;
    while (sum > 0 && i<8){
        sub_key[7-i] = sum % 2;
        sum = sum / 2;
        i++;
    }
}
/**
 *@fn etape_4_crypt(int nb_iteration, int etape_iteration, char *key)
 *@brief Etape de cryptage 4 - Genere sous_clé via clé et étape pour ensuite faire XOR bit par bit

 *@param nb_iteration      Nombre d'itération totale pour génération sous-clé
 *@param etape_iteration   Etape actuelle d'itération pour génération sous-clé
 *@param *key              Clé de chiffrement via son adresse

 *@return N/A
*/
void etape_4_crypt(int nb_iteration, int etape_iteration, char *key)
{
    int sub_key[8] = {0};

    //Generation sous_cle
    sous_cle(CRYPT,  nb_iteration, etape_iteration, key, sub_key);

    // Application calcul XOR bit par bit
    for (int i = 0; i<4; i++) {
        for (int j = 0; j < 8; j++) {
            bits_oct[i][j] ^= sub_key[j];
        }
    }
}
/**
 *@fn etape_4_decrypt(int nb_iteration, int etape_iteration, char *key)
 *@brief Etape de cryptage 4 - Genere sous_clé via clé et étape pour ensuite faire XOR bit par bit

 *@param nb_iteration      Nombre d'itération totale pour génération sous-clé
 *@param etape_iteration   Etape actuelle d'itération pour génération sous-clé
 *@param *key              Clé de chiffrement via son adresse

 *@return N/A
*/
void etape_4_decrypt(int nb_iteration, int etape_iteration, char *key)
{
    int sub_key[8] = {0};

    // Generation sous_cle
    sous_cle(DECRYPT, nb_iteration, etape_iteration, key, sub_key);

    // Application calcul XOR bit par bit
    for (int i = 0; i < 4; i++) {
        for(int j = 0; j < 8; j++) {
            bits_oct[i][j] ^= sub_key[j];
        }
    }
}



/**
 *@fn etape_5_crypt()
 *@brief Etape de cryptage 5 - Calcul système imposé par le sujet

 *@param N/A

 *@return N/A
*/
void etape_5_crypt()
{
    int inter[4]; // pour copier les anciennes valeurs des octets

    // Boucle copie anciennes valeurs
    for(int i = 0; i < 4; i++)
      inter[i] = oct[i];

    oct[0] = (inter[0] + inter[1]) % 256;
    oct[1] = (inter[0] + inter[1] + inter[2]) % 256;
    oct[2] = (inter[1] + inter[2]+ inter[3]) % 256;
    oct[3] = (inter[2] + inter[3]) % 256;

    // Boule verification valeures entre 0 et 255 car octet = 8 bit
    for(int i = 0; i < 4; i++)
      while (oct[i] < 0)
        oct[i] = oct[i] + 256;
}
/**
 *@fn etape_5_crypt()
 *@brief Etape de cryptage 5 - Calcul inverse système etape 5 crypt imposée par le sujet
 *@param N/A

 *@return N/A
*/
void etape_5_decrypt()
{
    int inter[4];

    // Boucle copie anciennes valeurs
    for(int i = 0; i < 4; i++)
      inter[i] = oct[i];

    oct[0] = (inter[0] - inter[2] + inter[3]) % 256;
    oct[1] = (inter[2] - inter[3]) % 256;
    oct[2] = (inter[1] - inter[0]) % 256;
    oct[3] = (inter[3] - inter[1] + inter[0]) % 256;

    for(int i = 0; i < 4; i++)
      while (oct[i] < 0)
        oct[i] = oct[i] + 256;
}



/**
 *@fn etape_5_crypt()
 *@brief Saisie une chaîne dynamiquement sans connaître sa taille à l'avance

 *@param N/A
 *@return chaine      Chaine de caractère saisie
*/
char *saisie_chaine_dynam()
{
  char *chaine = NULL ;
  int n = 2;//un caractere et un \0
   chaine = malloc(n*sizeof(char));

  //Lecture du premier caractère
  scanf("%c", &chaine[n-2]);

  // pour éviter les retours de lignes des autres saisies
  while (chaine[n-2] == '\n') {
  scanf("%c", &chaine[n-2]);
  }

  //Boucle de reallocation dynamique tq pas fin de saisie
  while (chaine[n-2] != '\n') {
    n++; //on augmente la taille
    chaine = realloc(chaine, n * sizeof(char));
    scanf("%c", &chaine[n-2]);
  }
  // Caractère de fin de chaine
 chaine[n-2] = '\0';

 return chaine ;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////
// \fn main
// \param N/A
// \return N/A
// \brief saisie des informations necessaires et effecture lecture et écriture dans les fichiers
////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main() {
  // Variables paramètres cryptage/decryptage
  int choix_ops, nb_iteration;
  int choix_name = FALSE;
  char *key;
  // Variables fichiers et contrôle
  char *extension, *name_in, *name_out;
  char add_crypted[9] = {"_crypted"};
  char add_decrypted[11] = {"_decrypted"};
  int lenght_ext; int j;
  FILE *file_in, *file_out;
  int arret = FALSE, ignore_oct = FALSE;
  int *oct, **bits_oct;

  oct = (int*) malloc (4 * sizeof(int));
  *bits_oct = (int **) malloc (4 * sizeof(int*));
  bits_oct = (int*) malloc (8 * sizeof(int));

  // Variables octets bourrage
  int nb_bourrage = 0, rech_bourrage[2];


  // Saisie de l'operation souhaitee avec un controle bon choix
  do{
      printf ("Quelle operation souhaitez vous effectuer : \nCrypter [0] ou Decrypter [1]\nChoix operation :\n  >> ");
      scanf("%d", &choix_ops);
  } while (choix_ops != CRYPT && choix_ops != DECRYPT);

  // Saisie nombre iteration avec contrôle bon choix (pas de résultat = 0)
  do {
    printf("\nVeuillez saisir le nombre d'itération, attention, il ne peut pas etre nul :\n  >> ");
    scanf("%d", &nb_iteration);
  } while (nb_iteration == 0);

  // Saisie clé de chiffrement (dynamique)
  printf("\n\nVeuillez saisir votre cle, elle ne doit pas contenir d\'espaces :\n  >> ");
  key = saisie_chaine_dynam();


  // Saisie nom chemin d'accès + nom + extension fichier dynamique
  do {
    printf("\n\nVeuillez saisir le chemin d'acces complet du fichier /x/x/x.ext :\n  >>");
    name_in = saisie_chaine_dynam ();

    // Rercherche à partir d'où commence l'extension
    j = strlen(name_in);
    while (j > 0 && name_in[j] != '.') {
      j--;
    }
    if (j == 0 || j == strlen(name_in))
        printf("Erreur de saisie!\n");
  } while(j == 0 || j == strlen(name_in));

  // Copie de l'extension dans la variable qui lui est attitrée avec allocation dynamique
  lenght_ext = strlen(name_in) - j;
  extension = (char *) malloc ( lenght_ext * sizeof(char) );
  strncpy(extension, name_in + j, lenght_ext);
  extension[lenght_ext] = '\0';

  // Nommage fichier de sortie
  printf("Souhaitez-vous choisir le chemin d'acces complet du fichier en sortie\n Non [0] ou Oui [1]\n  >>");
  scanf("%d", &choix_name);
  if (choix_name == TRUE) {
    printf("Veuillez saisir le chemin et nom voulu pour le fichier en sortie :\n  >>");
    name_out = saisie_chaine_dynam();
  }
  else {
    name_out = (char *) malloc ((strlen(name_in) + 12) * sizeof(char));
    strncpy(name_out, name_in, strlen(name_in) - (lenght_ext));

    if (choix_ops == CRYPT)
      strcat(name_out, add_crypted);
    else strcat(name_out, add_decrypted);

    strcat(name_out, extension);
  }

  // Ouverture des fichiers en binaire
  file_in = fopen(name_in, "rb");
  file_out = fopen(name_out, "wb");


  // Verification ouverture des fichiers
  if (file_in == NULL){
      printf("Erreur d\'ouverture du fichier entrant\n");
      exit(EXIT_FAILURE);}
  else if (file_out == NULL){
      printf("Erreur d\'ouverture du fichier sortant\n");
      exit(EXIT_FAILURE);
  }

  // CRYPTAGE : lecture et écriture fichier jusqu'à sa fin (4 par 4 octets)
  if (choix_ops == CRYPT) {
    while (arret == FALSE) {
        for (int i = 0; i < 4; i++) {
          //Lecture d'un octet
          oct[i] = fgetc(file_in);

          // Condition pour savoir si fin fichier
          if (oct[i] == EOF) {
              arret = TRUE;
              //Condition pour savoir si on est au premier octet d'un paquet
              if (i == 0) {
                // Condition vérifiée :ignore paquet octet
                ignore_oct = TRUE;
                break;
              }
              // Condition non vérifiée : octets de bourrage
              oct[i] = 0;
              // Variable pour enregistrer le nombre d'octet de bourrage
              if (nb_bourrage == 0 ){
                nb_bourrage = 4-i;
              }
          }
        }

        // Condition pour vérifier si le paquet est ignoré
        if (ignore_oct != TRUE) {

          //CRYPTAGE ITERATIF avec contrôle des passages binaire/decimal
          for (int etape = 0; etape < nb_iteration; etape++){

              etape_1_crypt(key);
              etape_2_crypt_decrypt();
              pass_binary();
              etape_3_crypt();
              etape_4_crypt(nb_iteration, etape, key);
              pass_decimal();
              etape_5_crypt();
          }


          //Ecriture des octets dans file_out
          for (int i = 0; i < 4; i++) {
            fprintf(file_out, "%c", oct[i]);
          }
      }
    }
  }
  // DECRYPTAGE : lecture et écriture fichier jusqu'à sa fin (4 par 4 octets)
  else {
    while (arret == FALSE) {
        // Lecture des octets 4 par 4
        for (int i = 0; i < 4; i++)
          oct[i] = fgetc(file_in);
        //Lecture des 2 octets du prochain paquet pour savoir si c'est la fin
        for(int i = 0; i < 2; i++)
          rech_bourrage[i] = fgetc(file_in);
        // condition fin vérifiée : note nombre d'octets pour ne pas les recopier
        if (rech_bourrage[1] == EOF) {
            //pour ne pas relire 4 octet
            arret = TRUE;
            nb_bourrage = rech_bourrage[0];
        }
        //Retour du curseur 2 crans en arrière pour pouvoir continuer tranquillement
        else fseek (file_in, -2, SEEK_CUR);

          //DECRYPTAGE ITERATIF avec contrôle des passages binaire/decimal
          for (int etape = 0; etape < nb_iteration; etape++){
              etape_5_decrypt();
              pass_binary();
              etape_4_decrypt(nb_iteration, etape, key);
              etape_3_decrypt();
              pass_decimal();
              etape_2_crypt_decrypt();
              etape_1_decrypt(key);
          }

          //Ecriture des octets dans file_out tout en enlevant les octets de bourrage s'il y en a
          for (int i = 0; i < 4 - nb_bourrage; i++) {
              fprintf(file_out, "%c", oct[i]);
          }
      }
    }


    // Noter le nombre d'octet de bourrage en fin de fichier durant le cryptage
    if (choix_ops == CRYPT)
      fprintf (file_out, "%c", nb_bourrage);


  //Fermeture des fichiers
  fclose(file_in);
  fclose(file_out);

  // Libération espaces alloués dynamiquement
  free(key);
  free(name_in);
  free(name_out);
  free(extension);
  printf("L'operation s'est parfaitement bien passee\n");


  return EXIT_SUCCESS;
}
