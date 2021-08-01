#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>

#define MAX_PATH 1024
#define MAX_BUF  1024

void raler(char *message)
{
	perror(message);
	exit(EXIT_FAILURE);
}

void afficher_ligne(unsigned char *buf, size_t buf_taille)
{
	// Affiche le caractère tant que different d'un saut de ligne
	for (size_t i = 0; i < buf_taille && buf[i] != '\n'; i++) {
		fprintf(stdout, "%c", buf[i]);
	}
	fprintf(stdout, "\n");
}

bool comparer_ext(unsigned char *buf, unsigned char *ext,
		  size_t buf_taille, size_t ext_taille)
{
	// Comparaison de la taille
	if (buf_taille < ext_taille) {
		return false;
	}

	// Comparaison du contenu
	for (size_t i = 0; i < ext_taille; i++) {
		if (buf[i] != ext[i]) {
			return false;
		}
	}

	return true;
}

void afficher_ext(unsigned char *buf, size_t buf_taille)
{
	// Liste des extensions gérées
	unsigned char ext_zip[2]     = { 0x50, 0x4B };
	unsigned char ext_7z[6]      = { 0x37, 0x7A, 0xBC, 0xAF, 0x27, 0x1C };
	unsigned char ext_pdf[5]     = { 0x25, 0x50, 0x44, 0x46, 0x2D };
	unsigned char ext_mp3[3]     = { 0x49, 0x44, 0x33 };
	unsigned char ext_tiff_le[4] = { 0x49, 0x49, 0x2A, 0x00 };
	unsigned char ext_tiff_be[4] = { 0x4D, 0x4D, 0x00, 0X2A };
	unsigned char ext_sh[2]      = { 0x23, 0x21 };

	// Test de chaque extension
	if (comparer_ext(buf, ext_zip, buf_taille, sizeof(ext_zip))) {
		fprintf(stdout, "ZIP\n");
	} else if (comparer_ext(buf, ext_7z, buf_taille, sizeof(ext_7z))) {
		fprintf(stdout, "7z\n");
	} else if (comparer_ext(buf, ext_pdf, buf_taille, sizeof(ext_pdf))) {
		fprintf(stdout, "PDF\n");
	} else if (comparer_ext(buf, ext_mp3, buf_taille, sizeof(ext_mp3))) {
		fprintf(stdout, "MP3\n");
	} else if (comparer_ext(buf, ext_tiff_le, buf_taille, sizeof(ext_tiff_le))) {
		fprintf(stdout, "TIFF (little endian)\n");
	} else if (comparer_ext(buf, ext_tiff_be, buf_taille, sizeof(ext_tiff_be))) {
		fprintf(stdout, "TIFF (big endian)\n");
	} else if (comparer_ext(buf, ext_sh, buf_taille, sizeof(ext_sh))) {
		afficher_ligne(buf, buf_taille);
	} else {
		fprintf(stdout, "Format inconnu\n");
	}
}

void afficher_fichier(char *fichier, off_t fichier_taille)
{
	fprintf(stdout, "%-40s\t%6zd octets\t", fichier, fichier_taille);

	// Ouvre et lit le début du fichier, jusqu'à MAX_BUF caractères
	int fd;
	unsigned char buf[MAX_BUF];
	ssize_t buf_taille;

	if ((fd = open(fichier, O_RDONLY)) < 0) {
		raler("Erreur open");
	}

	if ((buf_taille = read(fd, buf, MAX_BUF)) < 0) {
		raler("Erreur read");
	}

	// Affiche l'extension du fichier basée sur le contenu de ses premiers caractères
	afficher_ext(buf, buf_taille);

	if (close(fd) < 0) {
		raler("Erreur close");
	}
}

void lister_dossier(char *dossier)
{
	DIR *dir;
	struct dirent *dp;
	struct stat sb;

	if ((dir = opendir(dossier)) == NULL) {
		raler("Erreur opendir");
	}

	// Mise à 0 de errno avant d'appeler readdir
	errno = 0;
	while ((dp = readdir(dir)) != NULL) {
		// Ignore les dossiers "." et ".."
		if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) {
			continue;
		}

		// Concatène le chemin du dossier et le nom de l'élément
		char chemin[MAX_PATH];
		int res = snprintf(chemin, MAX_PATH, "%s/%s", dossier, dp->d_name);
		if (res < 0 || res >= MAX_PATH) {
			raler("Erreur snprintf");
		}

		// Lecture des informations
		if (lstat(chemin, &sb) < 0) {
			raler("Erreur lstat");
		}

		// Appel recursivement la fonction, ou affiche le fichier selon le type
		switch (sb.st_mode & S_IFMT) {
			case S_IFDIR:
				lister_dossier(chemin);
				break;
			case S_IFREG:
				afficher_fichier(chemin, sb.st_size);
				break;
			default:
				raler("Type d'élément inconnu");
		}

		// Remise à 0 de errno avant de rappeler readdir
		errno = 0;
	}

	if (errno != 0) {
		raler("Erreur readdir");
	}

	if (closedir(dir) < 0) {
		raler("Erreur closedir");
	}
}

int main(int argc, char **argv)
{
	// Pas assez ou trop d'argument
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <chemin d'un dossier>\n", argv[0]);
		return EXIT_FAILURE;
	}

	// Commence le parcours recurssif du dossier
	lister_dossier(argv[1]);

	return EXIT_SUCCESS;
}