#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

//Definition d'operation
/**
 *@brief Permet de définir l'opération effectuée
*/
#define CRYPT     0
/**
 *@brief Permet de définir l'opération effectuée
*/
#define DECRYPT   1
/**
 *@brief Booléen pour contrôle sortie de certaines boucles
*/
#define FALSE     0
/**
 *@brief Booléen pour contrôle sortie de certaines boucles
*/
#define TRUE      1

//Nombre d'octet et de bits par octet (peut être facilement modifiable)
/**
 *@brief Valeur définie pour fixer le nombre d'octet par paquet
*/
#define NB_OCTET  4
/**
 *@brief Valeur définie pour fixer le nombre de bit par octet
*/
#define NB_BIT    8


/**
 *@fn pass_binary(int *oct, int **bits_oct)
 *@brief Passage de valeurs decimales a valeurs binaires
 *@param *oct             Passage par adresse tableau des octets
 *@param **bits_oct       Passage par adresse tableau octets de 8 bits
 *@return N/A
*/
void pass_binary(int *oct, int **bits_oct)
{
  // Initialisation de tous les bits des octets a 0
  for (int i1 = 0; i1 < NB_OCTET; i1++) {
    for (int i2 = 0; i2 < NB_BIT; i2++)
        bits_oct[i1][i2] = 0;
  }

  // Calcul valeur binaire bits par bits
  for (int i1 = 0; i1 < NB_OCTET; i1++) {
    int octet = oct[i1]; // pour ne pas modifier la valeur originale de l'octet
    for (int i2 = NB_BIT - 1; octet > 0 || i2 >= 0; i2--) {
      bits_oct[i1][i2] = octet % 2;
      octet = octet / 2;
    }
  }
}
/**
 *@fn pass_decimal(int **bits_oct, int *oct)
 *@brief Passage de valeurs binaires a valeurs decimales
 *@param **bits_oct       Passage par adresse tableau octets de 8 bits
 *@param *oct             Passage par adresse tableau des octets
 *@return N/A
*/
void pass_decimal(int **bits_oct, int *oct)
{
  // Initialisation de tous les octets a 0
  for (int i1 = 0; i1 < NB_OCTET; i1++)
    oct[i1] = 0;

  // Calcul valeur decimale
  for (int i1 = 0; i1 < NB_OCTET; i1++){
    for (int i2 = 0; i2 < NB_BIT; i2++){
      oct[i1] += bits_oct[i1][7 - i2] * (int)pow(2, i2);
      oct[i1] = oct[i1] % 256;
      while (oct[i1] < 0)
        oct[i1] = oct[i1] + 256;
    }
  }
}



/**
 *@fn etape_1_crypt(int *oct, const char *key)
 *@brief Etape de cryptage 1 - Associe à chaque valeur une autre valeur par ajout valeurs ASCII termes de clé
 *@param *oct       Passage par adresse tableau des octets
 *@param *key       Passage clé de chiffrement (const : car même si passage par adresse aucune modification sur la clé)
 *@return N/A
*/
void etape_1_crypt(int *oct, const char *key)
{
  int sum = 0, indx = 0;

  // Calcul écart en sommant valeurs ASCII clé de chiffrement
  while (key[indx] != '\0'){
    sum += (int)key[indx];
    indx++;
  }

  // Incremente les valeurs de chaque octet de l'écart calculé
  for (int i = 0; i < NB_OCTET; i++)
    oct[i] = (oct[i] + sum) % 256;
}
/**
 *@fn etape_1_decrypt(char *key)
 *@brief Etape de décryptage 1 - Associe à chaque valeur une autre valeur par retait valeur ASCII des termes de clé
 *@param *oct       Passage par adresse tableau des octets
 *@param *key       Passage clé de chiffrement (const : car même si passage par adresse aucune modification sur la clé)
 *@return N/A
*/
void etape_1_decrypt(int *oct, const char *key)
{
  int sum = 0, indx = 0;

  // Calcul écart en sommant valeurs ASCII clé de chiffrement
  while (key[indx] != '\0'){
    sum += (int)key[indx];
    indx++;
  }

  // Décremente les valeurs de chaque octet de l'écart calculé
  for (int i = 0; i < NB_OCTET; i++) {
    oct[i] = (oct[i] - sum) % 256;

    // pour toujours avoir un resultat positif
    while (oct[i] < 0)
      oct[i] = oct[i] + 256;
  }
}



/**
 *@fn etape_2_crypt_decrypt(int *oct)
 *@brief Etape de cryptage et décryptage 2 - Associe à chaque x un y grâce au fichier permut_etape_2.txt
 *@param *oct       Passage par adresse tableau des octets
 *@return N/A
*/
void etape_2_crypt_decrypt(int *oct)
{
  // Declaration tableau de permutation et pointeur fichier
  int tab_permut[256][2];
  FILE *file_permut = NULL;

  // Ouverture fichier avec table de permutation -> doit ê dans même fichier que le code
  file_permut = fopen("permut_etap_2.txt", "r");

  // Vérification bonne ouverture du fichier et affichage en cas d'erreur
  if (file_permut == NULL) {
    printf("Erreur ouverture fichier permutation\n");
    exit(EXIT_FAILURE);
  }
  else {
    // Boucle de copie de la table de permutation
    for(int i1 = 0; i1 < 256; i1++)
      for (int i2 = 0; i2 < 2; i2++)
        fscanf(file_permut, "%d", &tab_permut[i1][i2]);

    //Recherche valeur dans table permutation puis permutation
    for(int i = 0; i < NB_OCTET; i++)
      for (int indx = 0; indx < 256; indx++)
        if(oct[i] == tab_permut[indx][0]) {
          oct[i] = tab_permut[indx][1];
          break;
        }

    //Fermeture du fichier de permutation
    fclose(file_permut);
  }
}



/**
 *@fn etape_3_crypt(int **bits_oct)
 *@brief Etape de cryptage 3 - Calcul matriciel selon constantes imposées dans le sujet
 *@param **bits_oct       Passage par adresse tableau des octets de 8 bits
 *@return N/A
*/
void etape_3_crypt (int **bits_oct)
{
  //Initialisation bits intermediaires de calcul
  int bits_inter[NB_OCTET][NB_BIT];
  for (int i = 0; i < NB_OCTET; i++)
    for (int j = 0; j < NB_BIT; j++)
      bits_inter[i][j] = bits_oct[i][j];


  //Initialisation des constantes
  int H[8][8] = {{1, 0, 0, 0, 1, 1, 1, 1},
                {1, 1, 0, 0, 0, 1, 1, 1},
                {1, 1, 1, 0, 0, 0, 1, 1},
                {1, 1, 1, 1, 0, 0, 0, 1},
                {1, 1, 1, 1, 1, 0, 0, 0},
                {0, 1, 1, 1, 1, 1, 0, 0},
                {0, 0, 1, 1, 1, 1, 1, 0},
                {0, 0, 0, 1, 1, 1, 1, 1}};
  int c[8] = {1, 1, 0, 0, 0, 1 , 1, 0};

  //[H*V] Boucle pour le produit matriciel avec modulo 2 (pas obligatoire, peut se faire seulement etape suivante)
  for (int i = 0; i < NB_OCTET; i++)
    for (int j = 0; j < NB_BIT; j++) {
      int res = 0;
      for (int k = 0; k < NB_BIT; k++)
          res += bits_inter[i][k] * H[j][k];
      bits_oct[i][j] = res % 2;
    }

  //[H*V+c] Addition du résultat précédent avec c + application modulo 2
  for (int i = 0; i < NB_OCTET; i++)
    for (int j = 0; j < NB_BIT; j++)
      bits_oct[i][j] = (bits_oct[i][j] + c[j])%2;

}
/**
 *@fn etape_3_decrypt(int **bits_oct)
 *@brief Etape de décryptage 3 - Calcul matriciel selon constantes imposées dans le sujet
 *@param **bits_oct       Passage par adresse tableau des octets de 8 bits
 *@return N/A
*/
void etape_3_decrypt(int **bits_oct)
{
  //Initialisation bits intermediaires de calcul
  int bits_inter[4][8];
  for (int i = 0; i < NB_OCTET; i++)
    for (int j = 0; j < NB_BIT; j++)
      bits_inter[i][j] = bits_oct[i][j];

  //Initialisation des constantes
  int H[8][8] = {{0, 0, 1, 0, 0, 1, 0, 1},
                {1, 0, 0, 1, 0, 0, 1, 0},
                {0, 1, 0, 0, 1, 0, 0, 1},
                {1, 0, 1, 0, 0, 1, 0, 0},
                {0, 1, 0, 1, 0, 0, 1, 0},
                {0, 0, 1, 0, 1, 0, 0, 1},
                {1, 0, 0, 1, 0, 1, 0, 0},
                {0, 1, 0, 0, 1, 0, 1, 0}};
  int c[8] = { 1, 0, 1, 0, 0, 0, 0, 0};

  //[H'*V] Boucle pour le produit matriciel avec application modulo 2 (peut etre effectue seulement à l'étape suivante)
  for (int i = 0; i < NB_OCTET; i++)
    for (int j = 0; j < NB_BIT; j++) {
      int res = 0;
      for (int k = 0; k < NB_BIT; k++)
          res += bits_inter[i][k] * H[j][k];
      bits_oct[i][j] = res %2;
    }

  //[H'*V+c'] Addition du résultat précédent avec c' + application du modulo 2
  for (int i = 0; i < NB_OCTET; i++)
    for (int j = 0; j < NB_BIT; j++)
      bits_oct[i][j] = (bits_oct[i][j] + c[j])%2;

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
  for (int i = 0; i < strlen(key); i++)
    sum += (int)key[i];

  // DISTINCTION ENTRE CRYPTAGE ET DECRYPTAGE : crypt -> 1 a nb d'iteration / decrypt -> nb d'iteration à 1
  if (mode == CRYPT) div = etape_it + 1;
  else div = nb_it_tot - etape_it;

  // CALCUL FINAL VALEUR : somme de la clé / div : dépendant du mode de l'opération
  sum /= div;

  // PASSAGE BINAIRE DE LA SOUS CLE : seulement les 8 premiers bits sont conservés
  int i = 0;
  while (sum > 0 && i < NB_BIT){
      sub_key[(NB_BIT - 1) - i] = sum % 2;
      sum = sum / 2;
      i++;
  }
}
/**
 *@fn etape_4_crypt(int **bits_oct, int nb_iteration, int etape_iteration, char *key)
 *@brief Etape de cryptage 4 - Genere sous_clé via clé et étape pour ensuite faire XOR bit par bit
 *@param **bits_oct        Passage par adresse tableau des octets de 8 bits
 *@param nb_iteration      Nombre d'itération totale pour génération sous-clé
 *@param etape_iteration   Etape actuelle d'itération pour génération sous-clé
 *@param *key              Clé de chiffrement via son adresse
 *@return N/A
*/
void etape_4_crypt(int **bits_oct, int nb_iteration, int etape_iteration, char *key)
{
  int sub_key[8] = {0};

  //Generation sous_cle
  sous_cle(CRYPT,  nb_iteration, etape_iteration, key, sub_key);

  // Application calcul XOR bit par bit
  for (int i = 0; i < NB_OCTET; i++)
    for (int j = 0; j < NB_BIT; j++)
      bits_oct[i][j] ^= sub_key[j];

}
/**
 *@fn etape_4_decrypt(int **bits_oct, int nb_iteration, int etape_iteration, char *key)
 *@brief Etape de décryptage 4 - Genere sous_clé via clé et étape pour ensuite faire XOR bit par bit
 *@param **bits_oct        Passage par adresse tableau des octets de 8 bits
 *@param nb_iteration      Nombre d'itération totale pour génération sous-clé
 *@param etape_iteration   Etape actuelle d'itération pour génération sous-clé
 *@param *key              Clé de chiffrement via son adresse
 *@return N/A
*/
void etape_4_decrypt(int **bits_oct, int nb_iteration, int etape_iteration, char *key)
{
  int sub_key[8] = {0};

  // Generation sous_cle
  sous_cle(DECRYPT, nb_iteration, etape_iteration, key, sub_key);

  // Application calcul XOR bit par bit
  for (int i = 0; i < NB_OCTET; i++)
    for(int j = 0; j < NB_BIT; j++)
      bits_oct[i][j] ^= sub_key[j];

}



/**
 *@fn etape_5_crypt(int *oct)
 *@brief Etape de cryptage 5 - Calcul système imposé par le sujet
 *@param *oct       Passage par adresse tableau des octets
 *@return N/A
*/
void etape_5_crypt(int *oct)
{
  int inter[NB_OCTET];

  // Boucle copie anciennes valeurs
  for(int i = 0; i < NB_OCTET; i++)
      inter[i] = oct[i];

  //Application système imposé dans le sujet
  oct[0] = (inter[0] + inter[1]) % 256;
  oct[1] = (inter[0] + inter[1] + inter[2]) % 256;
  oct[2] = (inter[1] + inter[2]+ inter[3]) % 256;
  oct[3] = (inter[2] + inter[3]) % 256;

  // Boule verification valeures entre 0 et 255 car octet = 8 bit
  for(int i = 0; i < NB_OCTET; i++)
      while (oct[i] < 0)
          oct[i] = oct[i] + 256;
}
/**
 *@fn etape_5_decrypt(int *oct)
 *@brief Etape de décryptage 5 - Calcul inverse système etape 5 crypt imposée par le sujet
 *@param *oct       Passage par adresse tableau des octets
 *@return N/A
*/
void etape_5_decrypt(int *oct)
{
  int inter[NB_OCTET];

  // Boucle copie anciennes valeurs
  for(int i = 0; i < NB_OCTET; i++)
      inter[i] = oct[i];

  //Application système imposé dans le sujet
  oct[0] = (inter[0] - inter[2] + inter[3]) % 256;
  oct[1] = (inter[2] - inter[3]) % 256;
  oct[2] = (inter[1] - inter[0]) % 256;
  oct[3] = (inter[3] - inter[1] + inter[0]) % 256;

  // Boule verification valeures entre 0 et 255 car octet = 8 bit
  for(int i = 0; i < NB_OCTET; i++)
      while (oct[i] < 0)
          oct[i] = oct[i] + 256;

}



/**
 *@fn *saisie_chaine_dynam()
 *@brief Saisie une chaîne dynamiquement sans connaître sa taille à l'avance
 *@param N/A
 *@return chaine      Chaine de caractère saisie
*/
char *saisie_chaine_dynam()
{
  char *chaine = NULL ;
  // Un caractere et un '\0'
  int n = 2;

  //Allocation et lecture premier caractère
  chaine = malloc(n*sizeof(char));
  scanf("%c", &chaine[n-2]);

  // Eviter les retours de lignes des autres saisies
  while (chaine[n-2] == '\n')
    scanf("%c", &chaine[n-2]);

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


/**
 *@fn main()
 *@brief Saisie des informations necessaires et effecture lecture et écriture dans les fichiers
 *@param N/A
 *@return N/A
*/
int main()
{
  // Variables concernant les octets (forme décimale et forme binaire)
  int *octet, **bits_octet;

  // Variables principales des paramètres cryptage/decryptage
  int choix_ops, nb_iteration, choix_name = FALSE;
  char *key;

  // Variables noms des fichiers
  char *extension = NULL, *name_in = NULL, *name_out = NULL;
  int lenght_ext, j;
  char add_crypted[9] = {"_crypted"}, add_decrypted[11] = {"_decrypted"};
  // Variables fichier et leur contrôle
  FILE *file_in = NULL, *file_out = NULL;
  int arret = FALSE, ignore_oct = FALSE;

  // Variables octets bourrage
  int nb_bourrage = 0, rech_bourrage[2];

  // Variables pour affichage progression opération cryptage ou décryptage
  int tot_size, cur_size, percent = 20;

  // Allocation dynamique des variables concernant les octets
  octet = (int*) malloc ( NB_OCTET * sizeof(int));
  bits_octet = (int **) malloc( NB_OCTET * sizeof(int *) );

  for (int i = 0; i < NB_OCTET; i++)
    bits_octet[i] = (int *) malloc(NB_BIT * sizeof(int));


  // Saisie de l'operation souhaitee avec un controle bon choix
  do{
    printf ("Quelle operation souhaitez vous effectuer : \nCrypter [0] ou Decrypter [1]\nChoix operation :\n  >> ");
    scanf("%d", &choix_ops);
  } while (choix_ops != CRYPT && choix_ops != DECRYPT);

  // Saisie nombre iteration avec contrôle bon choix (pas de résultat = 0)
  do {
    printf("\n\nVeuillez saisir le nombre d'iteration, attention, il ne peut pas etre nul :\n  >> ");
    scanf("%d", &nb_iteration);
  } while (nb_iteration == 0);

  // Saisie clé de chiffrement (dynamique)
  printf("\n\nVeuillez saisir votre cle, elle ne doit pas contenir d\'espaces :\n  >> ");
  key = saisie_chaine_dynam();

  // Saisie nom chemin d'accès + nom + extension fichier dynamique
  do {
    printf("\n\nVeuillez saisir le chemin d'acces complet du fichier \\x\\x\\x.ext :\n  >> ");
    name_in = saisie_chaine_dynam ();

    // Rercherche à partir d'où commence l'extension
    j = (int)strlen(name_in);
    while (j > 0 && name_in[j] != '.') j--;
    if (j == 0 || j == strlen(name_in))
      printf("Erreur de saisie!\n");
  } while(j == 0 || j == strlen(name_in));

  // Copie de l'extension dans la variable qui lui est attitrée avec allocation dynamique
  lenght_ext = (int)strlen(name_in) - j;
  extension = (char *) malloc ( lenght_ext * sizeof(char) );
  strncpy(extension, name_in + j, lenght_ext);
  extension[lenght_ext] = '\0';

  // Nommage fichier de sortie
  printf("\n\nSouhaitez-vous choisir le chemin d\'acces complet du fichier en sortie\n Non [0] ou Oui [1]\n  >>");
  scanf("%d", &choix_name);
  if (choix_name == TRUE) {
    printf("Veuillez saisir le chemin et nom voulu pour le fichier en sortie :\n  >> ");
    name_out = saisie_chaine_dynam();
  }
  else {
    // Allocation dynamique nom fichier sortie avec taille chaine entrée + 12 caractère pour
    name_out = (char *) malloc ((strlen(name_in) + 12) * sizeof(char));

    // Copie nom fichier en entrée dans nom fichier en sortie mais sans l'extension
    strncpy(name_out, name_in, strlen(name_in) - (lenght_ext));

    // Ajout chaine de caractère en fonction de l'opération effectuée pour différencier name_in de name_out
    if (choix_ops == CRYPT)
        strcat(name_out, add_crypted);
    else strcat(name_out, add_decrypted);

    // Ajout extension
    strcat(name_out, extension);
  }

    //Début calcul temps
    clock_t debut = clock();

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

  fseek(file_in, 0L, SEEK_END);
  tot_size = ftell(file_in);
  fseek(file_in, 0L, SEEK_SET);
  printf("Fichier converti : 0 %%\n");

  // CRYPTAGE : lecture et écriture fichier jusqu'à sa fin (4 par 4 octets)
  if (choix_ops == CRYPT) {
    while (arret == FALSE) {

      //Affichage progression cryptage en pourcentage
      cur_size = ftell(file_out);
      if ((cur_size * 100 / tot_size) / percent == 1 ){
        printf("Fichier converti : %d %%\n", cur_size * 100 / tot_size);
        percent += 20;
      }

      for (int i = 0; i < NB_OCTET; i++) {
        //Lecture d'un octet
        octet[i] = fgetc(file_in);

        // Condition pour savoir si fin fichier
        if (octet[i] == EOF) {
          arret = TRUE;
          //Condition pour savoir si on est au premier octet d'un paquet
          if (i == 0) {
            // Condition vérifiée :ignore paquet octet
            ignore_oct = TRUE;
            break;
          }
          // Condition non vérifiée : octets de bourrage
          octet[i] = 0;
          // Variable pour enregistrer le nombre d'octet de bourrage
          if (nb_bourrage == 0 )
            nb_bourrage = NB_OCTET - i;
        }
      }

      // Condition pour vérifier si le paquet est ignoré
      if (ignore_oct != TRUE) {

        //CRYPTAGE ITERATIF avec contrôle des passages binaire/decimal
        for (int etape = 0; etape < nb_iteration; etape++){
          etape_1_crypt(octet, key);
          etape_2_crypt_decrypt(octet);
          pass_binary(octet, bits_octet);
          etape_3_crypt(bits_octet);
          etape_4_crypt(bits_octet, nb_iteration, etape, key);
          pass_decimal(bits_octet, octet);
          etape_5_crypt(octet);
        }


        //Ecriture des octets dans file_out
        for (int i = 0; i < NB_OCTET; i++)
          fprintf(file_out, "%c", octet[i]);

      }
    }
  }

  // DECRYPTAGE : lecture et écriture fichier jusqu'à sa fin (4 par 4 octets)
  else {
    while (arret == FALSE) {

      //Affichage progression décryptage
      cur_size = ftell(file_out);
      if ((cur_size * 100 / tot_size) / percent == 1 ){
        printf("Fichier converti : %d %%\n", cur_size * 100 / tot_size);
        percent += 20;
      }

      // Lecture des octets 4 par 4
      for (int i = 0; i < NB_OCTET; i++)
        octet[i] = fgetc(file_in);

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
        etape_5_decrypt(octet);
        pass_binary(octet, bits_octet);
        etape_4_decrypt(bits_octet, nb_iteration, etape, key);
        etape_3_decrypt(bits_octet);
        pass_decimal(bits_octet, octet);
        etape_2_crypt_decrypt(octet);
        etape_1_decrypt(octet, key);
      }

      //Ecriture des octets dans file_out tout en enlevant les octets de bourrage s'il y en a
      for (int i = 0; i < NB_OCTET - nb_bourrage; i++)
        fprintf(file_out, "%c", octet[i]);
    }
  }

  //Affichage manuel 100% du fichier converti pour décryptage (2 derniers octets non lu : nb bourrage et EOF)
  if(choix_ops == DECRYPT)
    printf("Fichier converti : 100 %%\n");

  // Noter le nombre d'octet de bourrage en fin de fichier durant le cryptage
  if (choix_ops == CRYPT)
    fprintf (file_out, "%c", nb_bourrage);

  //Fermeture des fichiers
  fclose(file_in);
  fclose(file_out);

  // Libération espaces alloués dynamiquement
  free(octet);
  for(int i = 0; i < NB_OCTET; i++)
    free(bits_octet[i]);
  free(bits_octet);
  free(key);
  free(name_in);
  free(name_out);
  free(extension);

  // Affichage temps d'éxécution de l'opération
  clock_t fin = clock();
  int temps = (fin - debut) / CLOCKS_PER_SEC;
  int heures = (int) temps / 3600;
  int minutes = ((int) temps - 3600 * heures)/ 60;
  int secondes = (int) temps - 3600 * heures - minutes * 60;
  printf("\n\nTemps d'execution de %d heures, %d minutes, %d secondes\n\n", heures,minutes,secondes);

  printf("L'operation s'est parfaitement bien passee\n");

  return EXIT_SUCCESS;
}
