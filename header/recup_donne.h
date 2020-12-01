#ifndef _recup
#define _recup

struct mem_inst{
	unsigned char oppcode, addr1,addr2,addr3;//l'oppcode fait 4 bits, comme les addresse pour un total de 16bit
};typedef struct mem_inst mem_inst;
void	Open(char *nom_fich, mem_inst *m,short *mem_donn);
void	Assembler_to_binary(char *nom_fich,char *nom_fichier_donne,char *nom_fich_bin);
#endif 
