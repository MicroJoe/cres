#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 2048
#define BYTES_PER_LINE 12 

void usage(void);

int main(int argc, const char *argv[])
{
    int i, j;
    int lineret;
    size_t l;
    unsigned char buffer[BUFSIZE];
    int *filelength;
    FILE *f, *out;

    if (argc < 2) {
        usage();
        return 0; 
    }

    if (!(out = fopen("out.c", "w"))) {
        printf("unable to open output file for writing");
        return 1;      
    }

    fprintf(out, "/*\n    fichier généré par cres\n*/\n\n"
                 "#include <stdio.h>\n\n");

    filelength = malloc(argc * sizeof(int));

    for (i = 1; i < argc; i++) {
        filelength[i] = 0;
    }

    for(i = 1; i < argc; ++i) {
        if(!(f = fopen(argv[i], "rb"))) {
            printf("unable to open input file %s", argv[i]);
            fclose(out);
            free(filelength);
            return 2;
        }

        fprintf(out, "\nunsigned char f%d[] = {\n    ", i);

        lineret = 0;
        while((l = fread(buffer, 1, BUFSIZE, f))) {
            filelength[i] += l;
            for(j = 0; j < l; ++j) {
                fprintf(out, "0x%.2x, ", buffer[j]);
                if(++lineret == BYTES_PER_LINE) {
                    fprintf(out, "\n    ");
                    lineret = 0;
                }
            }
        }
        fprintf(out, "};\n\n");
        fclose(f);
    }

    fprintf(out, "unsigned char *files[] = {\n");
    for (i = 1; i < argc; i++) {
        fprintf(out, "    f%d", i);
        if (i != (argc - 1))
            fprintf(out, ", ");
        else
            fprintf(out, "\n");
    }
    fprintf(out, "};\n\n");

    fprintf(out, "int filelength[] = {\n");
    for (i = 1; i < argc; i++) {
        fprintf(out, "%d", filelength[i]);
        if (i != (argc - 1)) {
            fprintf(out, ", ");
        }
    }
    fprintf(out, "\n};\n\n");

    fprintf(out, "char *filenames[] = {\n    ");
    for (i = 1; i < argc; i++) {
        fprintf(out, "\"%s\", ", argv[i]);
    }
    fprintf(out, "\"\"\n};\n\n");

    fprintf(out,
            "#define NUMFILES %d\n\n"
            "int main(void) {\n"
            "    int i, j;\n"
            "    FILE *fd;\n"
            "    for (i = 0; i < NUMFILES; ++i) {\n"
            "        printf(\"Extracting %%s...\", filenames[i]);\n"
            "        fd = fopen(filenames[i], \"w\");\n"
            "        for (j = 0; j < filelength[i]; j++) {\n"
            "           fputc(files[i][j], fd);\n"
            "        }\n"
            "        fclose(fd);\n"
                "    printf(\"\\n\");\n"
            "    }\n"
            "}\n",
            argc-1);

    fclose(out);

    free(filelength);

    return 0;
}

void usage(void) {
    printf("usage: cres <file1> <file2> ...\n");
}
