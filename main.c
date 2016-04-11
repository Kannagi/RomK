#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int createrom8k(int argc, char** argv);
int createrom(int argc, char** argv);
int invert(int argc, char** argv);
int compare(int argc, char** argv);
int divfile(int argc, char** argv);
int divrom(int argc, char** argv);
int divf3(int argc, char** argv);
int add(int argc, char** argv);


int main(int argc, char** argv)
{
    createrom8k(argc,argv);
    createrom(argc,argv);

    invert(argc,argv);
    compare(argc,argv);

    divfile(argc,argv);
    divrom(argc,argv);
    divf3(argc,argv);

    add(argc,argv);

    return 0;

}

//------------------------------------------------------

int createrom8k(int argc, char** argv)
{
    if(argc < 3) return 0;
    if(strcmp(argv[1],"-rom8k") != 0) return 0;

    int octet;
    int n = 1,i;

    if(argc == 4) n = atoi(argv[3]);

    FILE *fichier;
    fichier = fopen(argv[2],"wb");
    if(fichier == NULL) return;

    octet = 0X8000*n;
    for(i = 0;i < octet;i++)
        fputc(0,fichier);

    fclose(fichier);

    return 0;

}

int createrom(int argc, char** argv)
{
    if(argc < 3) return 0;
    if(strcmp(argv[1],"-rom") != 0) return 0;

    int octet;
    int n = 1,i;

    if(argc == 4) n = atoi(argv[3]);

    FILE *fichier;
    fichier = fopen(argv[2],"wb");
    if(fichier == NULL) return;

    octet = n;
    for(i = 0;i < octet;i++)
        fputc(0,fichier);

    fclose(fichier);

    return 0;

}

//------------------------------------------------------

int invert(int argc, char** argv)
{
    if(argc < 3) return 0;
    if(strcmp(argv[1],"-invert") != 0) return 0;

    int octet,octet2;

    FILE *fichier;
    fichier = fopen(argv[2],"rb+");
    if(fichier == NULL) return;

    octet = 0;

    while(octet != -1)
    {
        octet = fgetc(fichier);
        octet2 = fgetc(fichier);


        fseek ( fichier , -2 , SEEK_CUR );
        if(octet != -1)
        {
            fputc(octet2,fichier);
            fputc(octet,fichier);
        }

    }

    fclose(fichier);

    return 0;

}

//------------------------------------------------------


int compare(int argc, char** argv)
{
    if(argc < 4) return 0;
    if(strcmp(argv[1],"-compare") != 0) return 0;

    int octet,octet2;
    char str[500];

    FILE *fichier1,*fichier2,*out;

    fichier1 = fopen(argv[2],"rb");
    fichier2 = fopen(argv[3],"rb");




    if(fichier1 == NULL) return;
    if(fichier2 == NULL) return;

    sprintf(str,"%s_out.txt",argv[2]);
    out = fopen(str,"wb");

    sprintf(str,"offset %s %s\n\n",argv[2],argv[3]);
    fputs(str,out);
    //printf("%s\n",str);

    octet = 0;
    int i = 0,n = 0,fst = 0,old_octet = -1;

    while(octet != -1)
    {

        octet = fgetc(fichier1);
        octet2 = fgetc(fichier2);


        if(octet != octet2 && octet != old_octet)
        {
            if(fst == 0) fst = i;
            sprintf(str,"%.2x %.2x %.2x\n",i,octet,octet2);
            fputs(str,out);
            //printf("%.2x %.2x %.2x\n",i,octet,octet2);
            n++;
        }

        old_octet = octet;
        i++;
    }

    //printf("%d $%x\n",n,fst);
    sprintf(str,"\nndif :%d \nbegin :%x\n",n,fst);
    fputs(str,out);

    fclose(fichier1);
    fclose(fichier2);

    fclose(out);

    return 0;
}



//------------------------------------------------------
int divrom(int argc, char** argv)
{
    if(argc < 4) return 0;
    if(strcmp(argv[1],"-divrom") != 0) return 0;

    FILE *file[8],*infile;
    int n = 1,i;
    int octet = 0;
    char str[200];

    infile = fopen(argv[2],"rb");
    n = atoi (argv[3]);

    for(i = 0;i < n;i++)
    {
        if(argc == 5) sprintf(str,"%s%d",argv[4],i);
        else sprintf(str,"rom%d.bin",i);
        file[i] = fopen(str,"wb");
    }

    while(octet != -1)
    {
        for(i = 0;i < n;i++)
        {
            octet = fgetc(infile);
            if(octet != -1)
            fputc(octet,file[i]);
        }
    }


    for(i = 0;i < n;i++)
        fclose(file[i]);

    fclose(infile);
}

int divfile(int argc, char** argv)
{
    if(argc < 4) return 0;
    if(strcmp(argv[1],"-div") != 0) return 0;

    FILE *file[8],*infile;
    int n = 1,i,l;
    int octet = 0;
    char str[200];

    infile = fopen(argv[2],"rb");
    n = atoi (argv[3]);

    for(i = 0;i < n;i++)
    {
        if(argc == 5) sprintf(str,"%s%d",argv[4],i);
        else sprintf(str,"file%d.bin",i);
        file[i] = fopen(str,"wb");
    }


    fseek (infile,0,SEEK_END);
    int size = ftell (infile)/n;
    fseek (infile,0,SEEK_SET);

    i = 0;
    int ioctet = 0;
    while(octet != -1)
    {
        octet = fgetc(infile);
        if(octet != -1)
            fputc(octet,file[i]);

        ioctet++;
        if(ioctet == size)
        {
            i++;
            ioctet = 0;
        }
    }


    for(i = 0;i < n;i++)
        fclose(file[i]);

    fclose(infile);
}

//for Taito F3
int divf3(int argc, char** argv)
{
    if(argc < 4) return 0;
    if(strcmp(argv[1],"-divf3") != 0) return 0;

    FILE *fichier,*fichier1,*fichier2,*fichier3,*fichier4;

    if(argc == 3) fichier = fopen(argv[2],"rb+");
    else          fichier = fopen("f3.bin","rb+");
    if(fichier == NULL) return;

    fichier1 = fopen("d49-13.20","wb");
    fichier2 = fopen("d49-14.19","wb");
    fichier3 = fopen("d49-16.18","wb");
    fichier4 = fopen("d49-20.17","wb");

    int octet = 0;
    while(octet != -1)
    {
        octet = fgetc(fichier);
        fputc(octet,fichier1);

        octet = fgetc(fichier);
        fputc(octet,fichier2);

        octet = fgetc(fichier);
        fputc(octet,fichier3);

        octet = fgetc(fichier);
        fputc(octet,fichier4);
    }

    fclose(fichier);

    fclose(fichier1);
    fclose(fichier2);
    fclose(fichier3);
    fclose(fichier4);

    return 0;
}
//------------------------------------------------------
int add(int argc, char** argv)
{
    if(argc < 4) return 0;
    if(strcmp(argv[1],"-add") != 0) return 0;

    int i,n;
    int octet = 0;
    n = atoi (argv[3]);

    FILE *fichier;
    fichier = fopen(argv[2],"ab");

    if(fichier == NULL) return 0;

    if(argc == 5) octet = atoi (argv[4]);

    for(i = 0;i < n;i++)
        fputc(octet,fichier);

    fclose(fichier);



    return 0;
}

