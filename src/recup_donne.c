#include <stdio.h>
#include "../include/recup_donne.h"
#define B4f 15   		//pour 00001111 les 4bits de poids faible 
#define B4F 240			//pour 11110000 les 4bits de poids Fort

void	Open(char *nom_fich, mem_inst *m,short *mem_donn)
{
	unsigned char code_tmp[2]; // pour avoir 16 bits
	FILE *fich = fopen(nom_fich, "rb");
	int i;
	for(i = 0;i<256;i++)  //  ce qui represente 512 octets recupérés, la moitié de 1 kilooctet 
	{
		fread(code_tmp, sizeof(char), 2, fich);
		m[i].oppcode 	= code_tmp[0] & B4F;	
		m[i].oppcode	>>= 4;
		m[i].addr1 		= code_tmp[0] & B4f;
		m[i].addr2 		= code_tmp[1] & B4F;
		m[i].oppcode	>>= 4;
		m[i].addr3 		= code_tmp[1] & B4f;
	}//manipulation  permettant de recup les pack de 4 bits voulu dans chaque case
		(fread(&mem_donn[i], sizeof(short),256, fich));
	// recuperation des données
	fclose(fich);
}



/*________________________________________________*/




int	strcmp2(char *f1, char *f2)
{
	int i = 0;
	while(f2[i++]!='\0')
		if(f2[i] != f1[i])
			return 0;
	return 1;
}

/*________________________________________________*/


//j'ai voulu faire un lecteure de fichier texte pour le traduire en fichier binaire, j'ai cepandant un probleme. je ne sais pas d'ou vient le probleme. 
void	Assembler_to_binary(char *nom_fich, char *nom_fichier_donne, char *nom_fich_bin)
{
	FILE *fic_texte = fopen(nom_fich, "r");
	FILE *fic_binaire = fopen(nom_fich_bin, "wb");
	int fin = 256;
	int boolen = 1;
	short remplissage = 0;
	char tmp_oppcode[10000]; unsigned short tmp[3];
	while(fin-->0)
	{
		if(boolen == 0)
			fwrite(&remplissage, sizeof(short), 1, fic_binaire);
		else
		{
			fscanf(fic_texte,"%s",tmp_oppcode);
			printf("%s ",tmp_oppcode);
			if(strcmp2(tmp_oppcode,"j")==1)
			{
				fscanf(fic_texte, "%hd",tmp);
				tmp[0] += (3<<12);
				fwrite(tmp, sizeof(short), 1, fic_binaire);
			}
			else
			{
				fscanf(fic_texte, "%hd %hd %hd",tmp,tmp+1,tmp+2);
				tmp[2] += (tmp[1]<<4) +  (tmp[0]<<8);
				if(strcmp2(tmp_oppcode,"exit") == 1)
					boolen =0;
				if(strcmp2(tmp_oppcode,"add")==1) //je compose 
					tmp[2] += (4<<12);
				else if(strcmp2(tmp_oppcode,"sub")==1) 
					tmp[2] += (5<<12);
				else if(strcmp2(tmp_oppcode,"addi")==1) 
					tmp[2] += (6<<12);
				else if(strcmp2(tmp_oppcode,"mul")==1) 
					tmp[2] += (7<<12);
				else if(strcmp2(tmp_oppcode,"lw")==1) 
					tmp[2] += (8<<12);
				else if(strcmp2(tmp_oppcode,"sw")==1) 
					tmp[2] += (9<<12);
				else if(strcmp2(tmp_oppcode,"beq")==1) 
					tmp[2] += (1<<12);
				else if(strcmp2(tmp_oppcode,"bne")==1) 
					tmp[2] += (2<<12);
				fwrite(tmp+2, sizeof(short), 1, fic_binaire);
			}
		}
	}

	fclose(fic_texte);
	FILE *fic = fopen(nom_fichier_donne, "r");
	short tmpmp[256];
	int j = -1;
	do
		fscanf(fic,"%hd",tmpmp+(++j));
	while(tmpmp[j] != EOF);

	printf("j = %d\n",j);
	fwrite(tmpmp, sizeof(short), 1, fic_binaire);
	fclose(fic);
	fclose(fic_binaire);
}
