#include <stdio.h>

#define BUFSIZE 2048 /* taille du buffer de lecture */
#define BYTES_PER_LINE 12 /* octets par ligne dans le fichier source */

/* petite fonction permettant d'afficher comment utiliser le programme */
void usage(void);

/* programme principal */
int main(int argc, const char *argv[])
{
    int i, j; /* compteurs de parcours (argv et octets) */
    int lineret; /* compteur pour le retour à la ligne */
    size_t l; /* nombre d'octets lus */
    unsigned char buffer[BUFSIZE]; /* buffer utilisé pour la lecture */
    FILE *f, *out; /* fichiers (entrée et sortie) */

    /* on vérifie qu'on a au moins un fichié passé en paramètre */
    if (argc < 2) {
        usage();
        return 0; 
    }

    /* on ouvre le fichier de sortie (source C) en mode texte */
    if (!(out = fopen("out.c", "w"))) {
        printf("unable to open output file for writing");
        return 1;      
    }

    /* on écrit une petite en-tête dans le fichier de sortie */
    fprintf(out, "/*\n    fichier généré par cres\n*/\n\n"
                 "#include <stdio.h>\n\n");

    /* pour chaque fichier passé en argument */
    for(i = 1; i < argc; ++i) {
        /* on ouvre le fichier d'entrée en binaire */
        if(!(f = fopen(argv[i], "rb"))) {
            printf("unable to open input file %s", argv[i]);
            fclose(out);
            return 2;
        }

        /* on déclare un tableau f<n> contenant les données */
        fprintf(out, "/* %s */\nunsigned char f%d[] = {\n    ", argv[i], i);

        /* on parcours le fichier avec le buffer */
        lineret = 0;
        while((l = fread(buffer, 1, BUFSIZE, f))) {
            /* on écrit chaque octet un à un dans sa forme hexa */
            for(j = 0; j < l; ++j) {
                fprintf(out, "0x%.2x, ", buffer[j]);
                /* si on a atteind le nombre d'octets par ligne on retourne à
                 * la ligne */
                if(++lineret == BYTES_PER_LINE) {
                    fprintf(out, "\n    ");
                    lineret = 0;
                }
            }
        }

        /* on a fini avec ce fichier, on clos le tableau */
        fprintf(out, "};\n\n");
        fclose(f);
    }

    /* écriture du main */
    fprintf(out,
            "#define NUMFILES %d\n\n"
            "int main(void) {\n"
            "    int i;\n"
            "    for (i = 0; i < NUMFILES; ++i) {\n"
            "    }\n"
            "}\n",
            argc-1);

    /* on a fini d'écrire le fichier source */
    fclose(out);

    return 0;
}

void usage(void) {
    printf("usage: cres <file1> <file2> ...\n");
}
