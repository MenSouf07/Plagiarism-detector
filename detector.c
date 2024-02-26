/***************************************************************************************************************


				PROJET C : Anas IBOUBKAREN & Mendel SOUFFIR

		--- Partie A : Brouillon : fonctions ou l'on reviendra plus tard
		--- Partie 0 : Directives de pré-processeur, variables globales, structures et fonctions auxiliaires
		--- Partie 1 : Pre-traitement
		--- Partie 2 : Découpage en blocs/segments
		--- Partie 3 : Similarite entre segments (version distance de Dice)
		--- Partie 4 : Couplage des segments
		--- Partie 5 : Post-filtrage
		--- Partie 6 : Output des resultats
		--- Partie 7 : Programme principal (main)


	I. Au niveau de la forme

	A) A faire avant de rendre le fichier :
	1. Supprimer tous les commentaires inutiles.
	2. Commenter davantage les parties centrales du code.
	3. Ecrire des lignes de débogage propre pour le jour de la soutenance.
	4. Si possible, se débarrasser du message d'erreur final "stack..."
	6. Faire un dernier tour global du fichier.

	B) Ameliorations possibles :
	1. Notation camelCase pour tous les identificateurs si on a la foi.

	ATTENTION : revoir transformerMotAvecBonus pour le cas où on a "int" sur une seule ligne.


***************************************************************************************************************/


/***************************************************************************************************************

		PARTIE 0 : Directives de pre-processeur, variables globales, stuctures et fonctions auxiliaires

	--- variable : listeMotsCles
	--- structure : Digramme
	--- structure : Ligne
	--- supprimerDigramme
	--- supprimerLigne
	--- affiche_Digramme
	--- affiche_ligne
	--- min
	--- max
	--- AfficheMatrice
	--- fichierPGM
	--- NomSansExtension

***************************************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#define DEBUG //Affiche les digrammes.
#define nombreMotsCles 27

int Ndigramme = 2;

char* listeMotsCles[nombreMotsCles] = {"break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern", "float", "for", "if", "int", "long", "return", "short", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "while"};


typedef struct Digramme {
	char* chaine;
	struct Digramme* next;
} Digramme;


typedef struct Ligne {
	char* ch;
	struct Ligne* next;
	struct Ligne* prev;
	int numligne;
	Digramme* d;
} Ligne;


//Prend une liste de chainee de Digramme et la supprime en libérant l'espace memoire correspondant.
void supprimerDigramme(Digramme* premierDigramme) {
	Digramme* DigrammeCourant = premierDigramme;

	while(DigrammeCourant != NULL) {
		Digramme* DigrammeSuivant = DigrammeCourant->next;
		free(DigrammeCourant->chaine);
		free(DigrammeCourant);
		DigrammeCourant = DigrammeSuivant;
	}

	return;
}


//Prend une liste de chainee de Liste et la supprime en libérant l'espace memoire correspondant.
void supprimerLigne(Ligne* L1) {
	Ligne* LigneCourante = L1;

	while(LigneCourante != NULL) {
		Ligne* LigneSuivante = LigneCourante->next;
		supprimerDigramme(LigneCourante->d);
		free(LigneCourante->ch);
		free(LigneCourante);
		LigneCourante = LigneSuivante;
	}

	return;
}


//Prend une liste chainee de Digramme et affiche tous les digrammes sur la même ligne.
void affiche_Digramme(Digramme* d) {
	if(d == NULL) {
		printf("Erreur : vous avez passe un Digramme NULL a affiche_Digramme\n");
		return;
	}

	Digramme* D = d;

	while(D != NULL) {
		printf("|%s|\t", D->chaine);
		D = D->next;
	}

	return;
}


//Prend une liste chainee de Ligne et affiche toutes les lignes.
void affiche_ligne(Ligne* l) {
	if(l == NULL) {
		printf("Erreur : vous avez passe une Ligne NULL a affiche_ligne\n");
		return;
	}

	else {
		printf("ligne numero %d : |%s| \n\t\tDigramme associe:", l->numligne, l->ch);
		affiche_Digramme(l->d);
		printf("\n");
	}
}


//Prend deux entiers et renvoie le minimum.
int min(int n, int m) {
	return (n < m) ? n : m;
}


//Prend deux entiers et renvoie le max.
int max(int n, int m) {
	return (n < m) ? m : n;
}


//Prend une matrice et l'affiche de manière a ce que chaque coefficient ait deux chiffres apres la virgule.
void AfficheMatrice(double* D, int n, int m) {
	if(D == NULL) {
		printf("Erreur : matrice NULL passee en argument de AfficheMatrice\n");
		return;
	}

	printf("\t");

	for(int i = 0; i < n*m; i++) {
		if((i%m == 0) && (i != 0)) {		//correspond au cas ou on est debut d'une ligne
			printf("\n\t");								//donc on retourne à la ligne
		}

		printf("%.2f\t", D[i]);
  	}

	printf("\n");
  	return;
}


//Prend une matrice et cree le fichier PGM associe a cette matrice.
void fichierPGM(char* nomFichier, double* D, int n, int m) {
	if(D == NULL) {
		printf("Erreur : matrice NULL passee en argument de fichier\n");
		return;
	}

	if(nomFichier == NULL) {
		printf("Erreur : chaine de caractere NULL passé en argument de fichierPGM\n");
		return;
	}

	FILE* fichier = fopen(nomFichier, "w");

	if(fichier == NULL) {
		printf("Erreur : ouverture du fichier %s impossible.\n", nomFichier);
		return;
	}

	fputs("P2\n", fichier); //correspond aux 3 lignes génériques des fichers PGM
	fprintf(fichier, "%d %d\n", m, n);
	fprintf(fichier, "255\n");
	int i = 0;
	int j = 0;

	for(i = 0; i < n; i++) {
		for(j = 0; j < m-1; j++) {
			fprintf(fichier, "%d ", (int) ((1-D[i*m+j])*255));
		}

		if(j == m-1) {
			fprintf(fichier, "%d", (int) ((1-D[i*m+j])*255));
		}

		if(i != n-1) {
			fputc('\n', fichier);
		}
	}

	fclose(fichier);
}


//Prend un nom de fichier et renvoie le nom du fichier sans extension.
char* NomSansExtension(char* nom) {
	if(nom == NULL) {
		printf("Erreur : chaine de caractere NULL passee en argument de NomSansExtension\n");
		exit(1);
	}

	for(int i = 0; i < strlen(nom); i++) {
		if(nom[i] == '.') { 		//on recopie le nom initial jusqu'au moment ou on rencontre un point a ce moment la on arrete de recopier la chaine initiale
			nom[i] = '\0';
			return nom;
		}
	}

	return nom;
}

//Cree un fichier appelé "copie.c" qui le même que celui passé en argument mais sans les commentaires du type /* */
void copieFichier(char* nom){
	FILE* f=fopen(nom,"r");
 	FILE* fp=fopen("copie.c","w");
	int cpt1=0,cpt2=0;
	char c=fgetc(f);
	char d;

	do {
		if((cpt1%2 == 0) && (cpt2%2 == 0)) {
			if(c == '/') {
				d = fgetc(f);

				if(d == '*') {
					cpt2++;
					c = fgetc(f);
					continue;
				}

				else {
					fprintf(fp,"%c",c);
					fprintf(fp,"%c",d);
					c = fgetc(f);
					continue;
				}
      		}

			else if (c=='\"'){
				cpt1++;
				fprintf(fp,"%c",c);
				c=fgetc(f);
				continue;
			}

			else {
				fprintf(fp, "%c", c);
				c = fgetc(f);
				continue;
			}
		}

		else if((cpt1%2 == 0) && (cpt2%2 == 1)) {
			if(c == '*') {
				d = fgetc(f);
				if(d == '/') {
					cpt2++;
					c = fgetc(f);
					continue;
				}

				else{
					c = d;
					continue;
				}
			}

			else {
				c = fgetc(f);
				continue;
			}
		}

		else if((cpt1%2 == 1) && (cpt2%2 == 0)) {
			fprintf(fp, "%c", c);

			if(c == '\\') {
				c = fgetc(f);
				fprintf(fp, "%c", c);
				c = fgetc(f);
				continue;
			}

			else if(c == '\"') {
				cpt1++;
				c = fgetc(f);
				continue;
			}

			else {
				c = fgetc(f);
				continue;
			}
		}
 	} while(c != EOF);

	fclose(f);
	fclose(fp);
	return ;
}


/***************************************************************************************************************

		PARTIE 1 : Pre-traitement

	--- 1. estAlphaNumeriqueOuUnderscore
	--- 2. selectionligne
	--- 3. supprimerChaine
	--- 4. supprimerCommentaire
	--- 5. transformerMotSansBonus
	--- 6. transformerMotAvecBonus
	--- 7. supprimerEspace

***************************************************************************************************************/

//Prend un caractere et renvoie 1 s'il s'agit d'un caractère alphanumerique ou du caractere souligne. Renvoie 0 sinon.
int estAlphanumeriqueOuUnderscore(char c) {
	if((('a' <= c) && (c <= 'z')) || (('A' <= c) && (c <= 'Z')) || (('0' <= c) && (c <= '9')) || (c == '_')) {
		return 1;
	}

	return 0;
}


//Prend un fichier et renvoie la ligne courante sous la forme d'une chaine de caracteres.
char* selectionligne(FILE* f) {
	if(f == NULL) {
		printf("Erreur : vous avez passe un FILE NULL a selectionligne\n");
		exit(1);
	}

	int size = 100; //On fera un realloc plus tard si la taille dépasse 100
	char* l = malloc(size*sizeof(char));

	if(l == NULL) {
		printf("Echec de l'allocation memoire.\n");
		exit(1);
	}

	char a;
	int i = 0;

	do {
		a = fgetc(f);
		l[i++] = a;

		if(i == size-1) { 		//quand on atteint la taille maximale on aggrandit la zone allouée
			size *= 2;
			l = realloc(l, size*sizeof(char));

			if(l == NULL) {
				printf("Echec de l'allocation memoire.\n");
				exit(1);
			}
		}
	} while((a != '\n') && (a != EOF)); // on s'arrete quand on rencontre un retour a la ligne ou quand c'est la fin du fichier

	l[i] = '\0';
	return l;
}


//Prend une ligne d'un programme en C et renvoie une copie sans le contenu des chaines de caracteres et celui des caracteres.
char* supprimerChaine(char* ligne) {
	if (ligne == NULL) {
		printf("Erreur : chaine de caractere NULL passé en argument de supprimerChaine\n");
		exit(1);
	}

	int cpt1 = 0, cpt2=0, j = 0; // Si cpt1 est impair, on est entre 2 " (i.e. dans une chaine de caractere) et sinon il est pair
								// Si cpt2 est impair, on est entre 2 ' et sinon il est pair
								// La variable j est l'indice du prochain element qu'on rentrera dans ch
	char* ch = malloc((strlen(ligne)+1)*sizeof(char));

	if(ch == NULL) {
		printf("Echec de l'allocation memoire.\n");
		exit(1);
	}

	for(int i = 0; ligne[i] != '\0'; i++) {
		if(cpt1%2 == 1) {
			//le if est un peu compliqué mais on vérifie juste si ligne[i] est un vrai guillemet c'est-a-dire que ce n'est pas le caractère \" présent dans une chaine
			if((ligne[i]=='\"') && ((i == 0) || ((i == 1) && (ligne[i-1] != '\\')) || ((i >= 2) && ((ligne[i-1] != '\\') || ((ligne[i-1] == '\\') && (ligne[i-2] == '\\')))))) {
				cpt1++;
				ch[j++]=ligne[i];
 			}
		}

		else if(cpt2%2 == 1) {
			// meme commentaire que pour l'autre if mais cette fois avec les cote
			if ((ligne[i] == '\'') && ( (i == 0) || ((i == 1) && (ligne[i-1] != '\\')) || ((i >= 2) && ((ligne[i-1] != '\\') || ((ligne[i-1] == '\\') && (ligne[i-2] == '\\')))))){
				cpt2++;
				ch[j++]=ligne[i];
			}
		}

		else {
			ch[j++] = ligne[i];
			if(ligne[i] == '\"') cpt1++;
			if(ligne[i] == '\'') cpt2++;
		}
	}

	ch[j] = '\0';
	free(ligne);
	return ch;
}


//Prend une ligne d'un programme en C et renvoie une copie sans les commentaires.
char* supprimerCommentaire(char* ligne) {
	if(ligne == NULL) {
		printf("Erreur : chaine de caractere NULL passé en argument de supprimerCommentaire\n");
		exit(1);
	}

	int taille = strlen(ligne);
	char* nouvelleLigne = malloc(sizeof(char)*(taille+1));

	if(nouvelleLigne == NULL) {
		printf("Echec de l'allocation memoire.\n");
		exit(1);
	}

	if(taille == 0) nouvelleLigne[0] = '\0';

	else if(taille == 1) { //Si la ligne possède 1 caractère, alors on renvoie une copie de cette ligne car on sait qu'il n'y a pas de commentaire
		nouvelleLigne[0] = ligne[0];
		nouvelleLigne[1] = '\0';
	}

	else { //Si la ligne possède au moins 2 caractères, alors on renvoie la ligne sans les éventuelles commentaires
		int i = 0;

		//des qu'on rencontre 2 / on arrete de recopier ligne dans nouvelleLigne et on renvoie ce dernier
		while((ligne[i] != '\0') && ((ligne[i] != '/') || (ligne[i+1] != '/'))) {
			nouvelleLigne[i] = ligne[i];
			i++;
		}

		nouvelleLigne[i] = '\0';
	}

	free(ligne);
	return nouvelleLigne;
}


//Prend une ligne d'un programme en C et renvoie une copie en transformant tous les mots par w.
char* transformerMotSansBonus(char* ligne) {
	if(ligne == NULL) {
		printf("Erreur : chaine de caracteres NULL passé en argument de transformerMotSansBonus\n");
		exit(1);
	}

	int n = strlen(ligne);
	char* nouvelleLigne = malloc(sizeof(char)*(n+1));

	if(nouvelleLigne == NULL) {
		exit(1);
	}

	int presenceMot = 0; // Vaut 1 s'il y a un mot et 0 sinon
	int i = 0; // Parcourt ligne
	int j = 0; // Parcourt nouvelleLigne

	for(i = 0; i < n; i++) {
		if(estAlphanumeriqueOuUnderscore(ligne[i])) {
			presenceMot = 1;
		}

		else {
			if(presenceMot == 1) {
				nouvelleLigne[j] = 'w';
				j++;
			}

			nouvelleLigne[j] = ligne[i];
			j++;
			presenceMot = 0;
		}
	}

	if(presenceMot == 1) { // Cas où on aurait par exemple "int" sur la première ligne et  "a = 3;" sur la deuxième.
		nouvelleLigne[j] = 'w';
		j++;
	}

	nouvelleLigne[j] = '\0';
	free(ligne);
	return nouvelleLigne;
}

//Prend une ligne d'un programme en C et renvoie une copie en transformant les mots-cles par m et les mots simples par w.
char* transformerMotAvecBonus(char* ligne) {
	if(ligne == NULL) {
		printf("Erreur : chaine de caracteres NULL passé en argument de transformerMotAvecBonus\n");
		exit(1);
	}

	int n = strlen(ligne);
	char motCourant[9];
	char* nouvelleLigne = malloc(sizeof(char)*(n+1));

	if(nouvelleLigne == NULL) {
		printf("Echec de l'allocation memoire.\n");
		exit(1);
	}

	int presenceMot = 0; // Vaut 1 s'il y a un mot et 0 sinon
	int presenceMotCle = 0; // Vaut 1 s'il y a un mot-clé et 0 sinon
	int i = 0; // Parcourt ligne
	int j = 0; // Parcourt nouvelleLigne
	int k = 0; // Parcourt motCourant

	for(i = 0; i < n; i++) {
		if(estAlphanumeriqueOuUnderscore(ligne[i])) {
			presenceMot = 1;

			if(k < 8) {
				motCourant[k] = ligne[i];
				k++;
			}
		}

		else {
			if(presenceMot == 1) {
				motCourant[k] = '\0'; // On a alors notre mot-clé en entier
				k = 0; // On met k à 0 pour le prochain "remplissage" de la variable motCourant
				int l = 0;
				while((l < nombreMotsCles) && (presenceMotCle == 0)) { // On vérifie que motCourant est un mot-clé
					if(strcmp(motCourant, listeMotsCles[l]) == 0) {
						nouvelleLigne[j] = 'm';
						presenceMotCle = 1;
					}
					l++;
				}

				if(presenceMotCle == 0) nouvelleLigne[j] = 'w';
					j++;
				presenceMotCle = 0;
			}

			nouvelleLigne[j] = ligne[i];
			j++;
			presenceMot = 0;
		}
	}

	if(presenceMot == 1) { // Cas où on aurait par exemple "int" sur la première ligne et  "a = 3;" sur la deuxième.
		nouvelleLigne[j] = 'w';
		j++;
	}

	nouvelleLigne[j] = '\0';
	free(ligne);
	return nouvelleLigne;
}


//Prend une chaine de caracteres et renvoie une copie sans les les espaces et les tabulations
char* supprimerEspace(char* ligne) {
	if(ligne == NULL) {
		printf("Erreur : chaine de caractere NULL passé en argument de supprimerEspace\n");
		exit(1);
	}

	int j = 0;
	char* ch = malloc((strlen(ligne)+1)*sizeof(char));

	if(ch == NULL) {
		printf("Echec de l'allocation memoire.\n");
		exit(1);
	}

	for(int i = 0; ligne[i] != '\0'; i++){
		if((ligne[i] != ' ') && (ligne[i] != '\t') && (ligne[i] != '\n') && (ligne[i] != '\r')) {
			ch[j++] = ligne[i];
		}
	}

	ch[j] = '\0';
	free(ligne);
	return ch;
}


/***************************************************************************************************************

		PARTIE 2 : Découpage en blocs/segments

	--- listeDigrammes
	--- copier
	--- cree_ligne
	--- ajout_ligne

***************************************************************************************************************/


//Prend un segment et renvoie une liste de Digrammes.
Digramme* listeDigrammes(char* segment) {
	if(segment == NULL) {
		printf("Erreur : vous avez passe une chaine de caractere NULL en argument dans listeDigrammes.\n");
		exit(1);
	}

	if(strlen(segment) < Ndigramme) { //si on a une chaine qui n'est pas assez longue on la complete avec des espaces
		Digramme* d = malloc(sizeof(Digramme));

		if(d == NULL) {
			printf("Echec de l'allocation memoire.\n");
			exit(1);
		}

		char* ch = malloc((Ndigramme+1)*sizeof(char));

		if(ch == NULL) {
			printf("Echec de l'allocation memoire.\n");
			exit(1);
		}
		int i;

		for(i = 0; i < strlen(segment); i++){						//on copie les caractere de segment dans ch
			ch[i]=segment[i];
		}

		for(i = strlen(segment); i < Ndigramme; i++){		//on complete ch avec des espaces
			ch[i]=' '; //on a choisi de mettre des espaces dans ce cas la car on supprime les espaces au cours du pre-traitement
								// donc le seul cas ou on verra un Ngramme avec un ou des espaces c'est lorsqu'il ne sera pas assez long
		}

		ch[i]='\0';
		d->chaine = ch;
		d->next = NULL;
		return d;
	}

	Digramme* premierDigramme = malloc(sizeof(Digramme));

	if(premierDigramme == NULL) {
		printf("Echec de l'allocation memoire.\n");
		exit(1);
	}

	premierDigramme->next = NULL;
	char* chaine = malloc(sizeof(char)*(Ndigramme+1));

	if(chaine == NULL) {
		printf("Echec de l'allocation memoire.\n");
		exit(1);
	}

	for(int i = 0; i < Ndigramme; i++) { //on copie les Ndigramme premier caractere de segment dans ch
		chaine[i] = segment[i];
	}

	chaine[Ndigramme] = '\0';
	premierDigramme->chaine = chaine;
	Digramme* DigrammePrecedent = premierDigramme;
	int i = 1;

	while(segment[i+(Ndigramme-1)] != '\0') {
		char* chaine = malloc(sizeof(char)*(Ndigramme+1));

		if(chaine == NULL) {
			printf("Echec de l'allocation memoire.\n");
			exit(1);
		}

		for(int j = 0; j < Ndigramme; j++) {		//on copie Ndigramme caractere de segment dans ch
		chaine[j] = segment[j+i];
		}

		chaine[Ndigramme] = '\0';
		Digramme* DigrammeCourant = malloc(sizeof(Digramme));

		if(DigrammeCourant == NULL) {
			printf("Echec de l'allocation memoire.\n");
			exit(1);
		}

		DigrammeCourant->chaine = chaine;
		DigrammeCourant->next = NULL;
		DigrammePrecedent->next = DigrammeCourant;
		DigrammePrecedent = DigrammeCourant;
		i++;
	}

	return premierDigramme;
}


//Prend une liste de diagrammes et renvoie une copie de la liste
Digramme* copier(Digramme* listeDigrammes) {
	if(listeDigrammes == NULL) {
		printf("Erreur : digramme NULL passee en argument de listeDigrammes\n");
		exit(1);
	}

	Digramme* copieListeDigrammes = malloc(sizeof(Digramme));

	if(copieListeDigrammes == NULL) {
		printf("Echec de l'allocation memoire.\n");
		exit(1);
	}

	copieListeDigrammes->chaine = malloc(sizeof(char)*(strlen(listeDigrammes->chaine)+1));

	if(copieListeDigrammes->chaine == NULL) {
		printf("Echec de l'allocation memoire.\n");
		exit(1);
	}

	strcpy(copieListeDigrammes->chaine, listeDigrammes->chaine);
	copieListeDigrammes->next = NULL;
	Digramme* copieDigrammePrecedent = copieListeDigrammes; //Digramme copié
	Digramme* DigrammeCourant = listeDigrammes->next; // Digramme de listeDiagrammes
	while(DigrammeCourant != NULL) {
		Digramme* copieDigrammeCourant = malloc(sizeof(Digramme));

		if(copieDigrammeCourant == NULL) {
			printf("Echec de l'allocation memoire.\n");
			exit(1);
		}

		copieDigrammeCourant->chaine = malloc(sizeof(char)*(strlen(DigrammeCourant->chaine)+1));

		if(copieDigrammeCourant->chaine == NULL) {
			printf("Echec de l'allocation memoire.\n");
			exit(1);
		}

		strcpy(copieDigrammeCourant->chaine, DigrammeCourant->chaine);
		copieDigrammeCourant->next = NULL;
		copieDigrammePrecedent->next = copieDigrammeCourant;
		copieDigrammePrecedent = copieDigrammeCourant;
		DigrammeCourant = DigrammeCourant->next;
	}

	return copieListeDigrammes;
}


//Prend une chaine de caractere et un entier et renvoie une ligne initialisé grace a ces arguments
Ligne* cree_ligne(char* ch, int n) {
	if(ch == NULL) {
		printf("Erreur : chaine de caractere NULL passé en argument de cree_ligne\n");
		exit(1);
	}

	Ligne* l = malloc(sizeof(Ligne));

	if(l == NULL) {
		printf("Echec de l'allocation memoire.\n");
		exit(1);
	}

	l->ch = ch;
	l->next = NULL;
	l->prev = NULL;
	l->numligne = n;
	l->d = listeDigrammes(ch);
	return l;
}


//Ajoute la ligne nv a une liste chainee
void ajout_ligne(Ligne** premiere, Ligne** derniere, Ligne** nv) { //premiere designe la tete de la liste chaine et derniere la derniere ligne ajoute
	if (*nv == NULL) {
		printf("Erreur : vous avez passe une Ligne NULL a ajout_ligne\n");
		exit(1);
	}

	if ((*nv)->numligne == 0 ){ 		//si la liste chainee est vide nv devient le premier element
		*premiere = *nv;
		*derniere = *nv;
		return;
	}

	else {													//sinon on ajoute nv a la fin de la liste
		if(((*derniere) == NULL) || ((*premiere) == NULL)) {
			printf("Erreur : vous avez passe une Ligne NULL a ajout_ligne\n");
			exit(1);
		}
		(*derniere)->next = *nv;
		(*nv)->prev = *derniere;
		*derniere = *nv;
		return;
	}
}


/***************************************************************************************************************

		PARTIE 3 : Similarité entre segments (version distance de Dice)

	--- nombreDigrammesCommuns
	--- nbdigramme
	--- Dice
	--- levenshtein
	--- Matrice_Levenshtein
	--- coefficient

***************************************************************************************************************/


//Prends deux listes chainees de digrammes et renvoie le nombre de digrammes communs.
int nombreDigrammesCommuns(Digramme* D1, Digramme* D2) {
	if((D1 == NULL) || (D2 == NULL)) {
		printf("Erreur : digramme nul passé en argument de nombreDigrammesCommuns\n");
		exit(1);
	}

	int n = 0; // Nombre de Digrammes communs
	Digramme* D3 = copier(D1);
	Digramme* D4 = copier(D2);
	Digramme* D5 = D4;
	Digramme* D6 = D3;

	while(D3 != NULL) {
		while(D4 != NULL) {
			if(strcmp(D3->chaine, D4->chaine) == 0) {
				n++;
				//pour etre sur que les 2 chaines ne seront plus comptees on les modifie
				D3->chaine[0] = '\0'; // Il n'y a pas de chaine vide dans D1 car on aura supprime les chaines vides à l'etape 2
				D4->chaine[0] = '\0';
				break;
			}

			else {
				D4 = D4->next;
			}
		}

		D3 = D3->next;
		D4 = D5;
	}

	D3 = D6;
	supprimerDigramme(D3);
	supprimerDigramme(D4);
	return n;
}


//Prend une chaine de caractere et renvoie son nombre de digrammes.
int nbdigramme(char* l) {
	if(l == NULL) {
		printf("Erreur : chaine de caractere NULL passé en argument de nbdigramme\n");
		exit(1);
	}

	if(strlen(l) < Ndigramme) {
		return 1;
	}

	else {
		return strlen(l)-Ndigramme+1;
	}
}


//Prend deux listes chainees de Ligne et renvoie la matrice dont les coefficients correspondent à la distance de Dice.
double* Dice(Ligne* l1, Ligne* l2, int n, int m) {
	if((l1 == NULL) || (l2 == NULL)) {
		printf("Erreur : Ligne NULL passee en argument de Dice\n");
		exit(1);
	}

	Ligne* Lignetampon = l2;
	double nc, nx, ny;
	double* D = malloc(sizeof(double)*(n*m));

	if(D == NULL) {
		printf("Echec de l'allocation memoire\n");
		exit(1);
	}

	for(int i = 0; i < n; i++) {
		l2 = Lignetampon;

		for(int j = 0; j < m; j++) {
			nc = (double) nombreDigrammesCommuns(l1->d, l2->d);
			//if (i>n-30){printf("nombre de digrammes communs entre   %s   et   %s   :  %d\n",l1->ch,l2->ch, (int) nc);}
			nx = (double) nbdigramme(l1->ch);
			ny = (double) nbdigramme(l2->ch);
			D[i*m+j] = 1 - (2*nc/(nx+ny));
			l2 = l2->next;
		}

		l1 = l1->next;
	}

	return D;
}


//Prend deux chaines de caractere et renvoie la distance de Levenshtein.
float levenshtein(char *s, char *t) {
	int ls = strlen(s), lt=strlen(t);
	int matrice[(ls+1)*(lt+1)];
	int i, j;

	for(i = 0; i <= ls; i++)
		matrice[i*(lt+1) + 0] = i;

	for(j = 0; j <= lt; j++)
		matrice[0*(lt+1) + j] = j;

	for(i = 1; i <= ls; i++) {
		for(j = 1; j <= lt; j++) {
			int cout = (s[i-1] == t[j-1]) ? 0 : 1;
			int effacement = matrice[(i-1)*(lt+1) + j] + 1;
			int insertion = matrice[i*(lt+1) + (j-1)] + 1;
			int substitution = matrice[(i-1)*(lt+1) + (j-1)] + cout;
			matrice[i*(lt+1) + j] = min(min(effacement, insertion), substitution);
		}
	}

	return ((float) matrice[ls*(lt+1) + lt])/((float) max(ls,lt)); //on divise par la longueur de chaine la plus longue entre ls et lt pour normaliser
}


//Prend deux listes chainees de Ligne et renvoie la matrice dont les coefficients correspondent à la distance de Levenshtein.
double* Matrice_Levenshtein(Ligne* l1, Ligne* l2, int n, int m) {
	Ligne* Lignetampon = l2;
	double dl;
	double* D = malloc(sizeof(double)*(n*m));

	if(D == NULL) {
		printf("Echec de l'allocation memoire.\n");
		exit(1);
	}

	for(int i = 0; i < n; i++) {
		l2 = Lignetampon;

		for(int j = 0; j < m; j++) {
			dl = levenshtein(l1->ch, l2->ch);
			D[i*m+j] = dl;
			l2 = l2->next;
		}

		l1 = l1->next;
	}

	return D;
}


//Prend la matrice de Dice et un couple (i,j) et rnvoie le coefficient de (i,j) de la matrice de Dice.
double coefficient(double* D, int n, int m, int i, int j) {
	return D[i*m+j];
}


/***************************************************************************************************************

		PARTIE 4 : Couplage des segments

	--- Couplage

***************************************************************************************************************/

//Prend une matrice et renvoie la matrice de couplage.
double* Couplage(double* matrice, int n, int m) {
	if(matrice == NULL) {
		printf("Erreur : matrice NULL passee en argument de Couplage\n");
		exit(1);
	}

	int i = 0, j = 0;
	double Min = 2;
	int l = min(n,m);
	int indiceLigne = 0, indiceColonne = 0;
	double* nouvelleMatrice = calloc(n*m, sizeof(double));

	if(nouvelleMatrice == NULL) {
		printf("Echec de l'allocation memoire");
		exit(1);
	}

	for(i = 0; i < l; i++) {
		for(j = 0; j < n*m; j++) {		//on parcourt la matrice pour trouver le minimum et ses coordonnees
			if(matrice[j] < Min) {
				Min = matrice[j];
				indiceLigne = j/m;
				indiceColonne = j - indiceLigne*m;
			}
		}

		nouvelleMatrice[indiceLigne*m+indiceColonne] = Min;

		for(int k = 0; k < m; k++) {
			matrice[indiceLigne*m+k] = 2;				//la collone du minimum est changé en 2 dans la matrice en argument
																					//Ainsi on est sur de ne plus rentrer trouvé de minimum sur cette collone
			if(k != indiceColonne) {
				nouvelleMatrice[indiceLigne*m+k] = 1; // on fait pareil pour nouvelleMatrice sauf pour le coefficient se trouvant au coordonnee du minimum et on change par 1 au lieu de 2
			}
		}

		for(int k = 0; k < n; k++) {
			matrice[k*m+indiceColonne] = 2;			//meme commentaire que plus haut mais cette fois  on agit sur les lignes
			if (k != indiceLigne) {
				nouvelleMatrice[k*m+indiceColonne] = 1;
			}
		}

		Min = 2;
	}

	free(matrice);
	return nouvelleMatrice;
}


/***************************************************************************************************************

		PARTIE 5 : Post-filtrage

	--- moyenne
	--- filtrage

***************************************************************************************************************/

//Prend une matrice et un couple (i,j) et renvoie le coefficient (i,j) du produit de convolution de la matrice.
double moyenne(double* matrice, int n, int m, int i, int j) {
	if(matrice == NULL) {
		printf("Erreur : matrice NULL passee en argument de moyenne\n");
		exit(1);
	}

	double moy = 0, c = 0;

	for(int k = -2; k <= 2; k++) {
		if((0 <= i+k) && (i+k <n) && (0 <= j+k) && (j+k < m)) { //Si les coordonnees ne se trouve pas dans la matrice on ignore ce "coefficient" pour la moyenne
			moy += coefficient(matrice, n, m, i+k, j+k);
			c++;
		}
	}

	moy /= c;
	return moy;
}


//Prend une matrice et renvoie son produit de convolution en considérant que si un coefficient est supérieur à 0.7, alors il est égal à 1.
double* filtrage(double* matrice, int n, int m) {
	if(matrice == NULL) {
		printf("Erreur : matrice NULL passee en argument de filtrage\n");
		exit(1);
	}

	double* mat = malloc(n*m*sizeof(double));

	if(mat == NULL) {
		printf("Echec de l'allocation memoire.\n");
		exit(1);
	}

	double seuil;

	for(int i = 0; i < n; i++) {
		for(int j = 0; j < m; j++) {
			seuil = moyenne(matrice, n, m, i, j);

			if(seuil >= 0.7) {
				mat[i*m+j] = 1;
			}

			else {
				mat[i*m+j] = seuil;
			}
		}
	}

	free(matrice);
	return mat;
}


/***************************************************************************************************************

		PARTIE 6 : Output des résultats

	--- distance

***************************************************************************************************************/

//Prend une matrice post-filtrée et renvoie la distance entre les deux fichiers originels.
double distance(double* F, int n, int m) {
	if(F == NULL) {
		printf("Erreur : matrice NULL passee en argument de distance\n");
		exit(1);
	}

	double s = 0;

	/*for(int i = 0; i < n;i++){
		for(int j=0; j < m;j++){
			s += 1-F[i*m+j];
		}
	}*/

	for(int i = 0; i < n*m; i++) {
		s += (1 - F[i]);
	}

	s = 1 - (s/(float) min(n,m));
	free(F);

	return s;
}


/***************************************************************************************************************

		PARTIE 7 : Programme principal (main)

***************************************************************************************************************/

int main(int argc, char* argv[]) {
	if(argc < 3) {
		printf("Erreur : nombre de fichiers insuffisant\n");
		return 0;
	}
	int choixDistance = -1;
	do {
		printf("Entrez 1 pour utiliser la distance de Levenshtein. Sinon entrez 0 pour utiliser la distance de Dice : ");
		scanf("%d", &choixDistance);
	} while ((choixDistance != 0) && (choixDistance != 1));
	if(choixDistance == 0) {
		printf("Choisir la longueur de n-gramme que vous voulez utiliser : ");
		scanf("%d", &Ndigramme);
	}
	int choixPretraitement = -1;
	do {
		printf("Entrez 1 pour utiliser la traduction des mots-cles par m et des mots simples par w.\nSinon entrez 0 pour utiliser la traduction de tous les mots par w : ");
		scanf("%d", &choixPretraitement);
	} while ((choixPretraitement != 0) && (choixPretraitement != 1));

	Ligne* programme[argc-1]; //programme sera un tableau ou l'element i sera une liste chaine composee des lignes du i-eme fichier mis en argument
	int nbligne[argc-1]; //tableau ou l'element i sera le nombre de lignes du i-eme fichier mis en argument
	double matrice_distance[(argc-1)*(argc-1)];

	for(int i = 0; i < (argc-1); i++) {
		matrice_distance[i*(argc-1)+i] = 0;
	}

	for(int i = 1; i < argc; i++) { //remplit les tableaux programme et nbligne
		copieFichier(argv[i]);
		FILE* f = fopen("copie.c", "r");
		

		if(f == NULL) {
			printf("Erreur : Il n'y a pas de fichier portant le nom : %s\n", argv[i]);
			exit(1);
		}

		Ligne* ligne = NULL;
		Ligne* premiere = ligne;
		Ligne* derniere = ligne;
		char* l = selectionligne(f);
		int n = 0;

		do {
			l = supprimerChaine(l);
			l = supprimerCommentaire(l);
			if(choixPretraitement == 1) l = transformerMotAvecBonus(l);
			else l = transformerMotSansBonus(l);
			l = supprimerEspace(l);

			if(l[0] != '\0') {
				ligne = cree_ligne(l, n++);
				ajout_ligne(&premiere, &derniere, &ligne);
			}

			else {
				free(l);
			}

			l = selectionligne(f);
		} while((*l) != EOF);

		free(l);
		programme[i-1] = premiere;
		nbligne[i-1] = n;
		fclose(f);
	}
				// a enlevé dans la version finale
	#ifdef DEBUG
	for(int j = 0; j < argc-1; j++) { // affiche les lignes de chaque programme
		printf("\n\n\n---------------------PROGRAMME NUMERO %d(%d lignes)---------------------\n", j, nbligne[j]);
		Ligne* premiere = programme[j];

		while(premiere != NULL) {
			affiche_ligne(premiere);
			printf("%ld\n", strlen(premiere->ch));
			premiere = premiere->next;
		}
	}
	#endif

	for(int i = 0; i < argc-2; i++) { 		//compare les fichiers 2 a 2
		for(int j = i+1; j < argc-1; j++) {
			int n = nbligne[i], m = nbligne[j];
			double* D;
			if(choixDistance == 1) {
				D = Matrice_Levenshtein(programme[i], programme[j], n, m);
			}
			else {
				D = Dice(programme[i], programme[j], n, m);
			}
			char ch1[] = "Similarite_";
			strcat(ch1, NomSansExtension(argv[i+1])); //on cherche a avoir un nom du type "Similarite_fichier1_fichier2.pgm"
			strcat(ch1, "_");
			strcat(ch1, NomSansExtension(argv[j+1]));
			strcat(ch1, ".pgm");
			fichierPGM(ch1, D, n, m);
			double* C = Couplage(D, n, m);
			char ch2[] = "Couplage_";
			strcat(ch2, NomSansExtension(argv[i+1])); //on cherche a avoir un nom du type "Couplage_fichier1_fichier2.pgm"
			strcat(ch2, "_");
			strcat(ch2, NomSansExtension(argv[j+1]));
			strcat(ch2, ".pgm");
			fichierPGM(ch2, C, n, m);

			double* F = filtrage(C, n, m);
			char ch3[] = "Filtrage_";
			strcat(ch3, NomSansExtension(argv[i+1])); //on cherche a avoir un nom du type "Filtrage_fichier1_fichier2.pgm"
			strcat(ch3, "_");
			strcat(ch3, NomSansExtension(argv[j+1]));
			strcat(ch3, ".pgm");
			fichierPGM(ch3, F, n, m);
			double d = distance(F, n, m);
			printf("********************************************************************************\n\n\t\t\t\tRESULTAT\n\n\tLa distance entre les fichiers \"%s\" et \"%s\" est %.2f\n\n\n********************************************************************************\n", argv[i+1], argv[j+1] , d);
			matrice_distance[i*(argc-1)+j] = d;
			matrice_distance[j*(argc-1)+i] = d;

		}
	}

	for(int i = 0; i < argc-1; i++) {
		supprimerLigne(programme[i]);
	}

	printf("********************************************************************************\n\n\t\t\t\tMATRICE\n\n");
	AfficheMatrice(matrice_distance, (argc-1), (argc-1)); //affiche la matrice de toute les distances obtenues
	printf("\n\n********************************************************************************\n");
	return 0;
}


/***************************************************************************************************************

		FIN : Anas IBOUBKAREN & Mendel SOUFFIR

***************************************************************************************************************/
