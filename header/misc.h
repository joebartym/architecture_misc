#ifndef _misc
#define _misc

/*	OPPCODE sommaire :

exit		0000 	0  _
beq			0001 	1 |	
bne			0010 	2 |jumps
je			0011 	3 |_
add			0100 	4 |
sub			0101 	5 |arithmetique
addi 		0110 	6 |
mult		0111 	7 |_
lw			1000 	8 |data transfert
sw			1001 	9 |_


*/
#include "recup_donne.h"
#include <pthread.h>
struct ins_fetch{
	mem_inst *mem;
	mem_inst *if_id;
	int *PC;
	int *ordre; // puisque je les fais dans un thread, et que if/id etc.. sont en commun : je met des verrou pour evité les erreurs d'ecriture lecture
};typedef struct ins_fetch ins_fetch;


struct ins_deco{
	unsigned short *registre; // tableau de taille 16
	mem_inst *if_id;
	unsigned short *id_ex; // tableau de taille 4, pour contenire justqu'a 3 valeurs et l'oppcode
	int *ordre,*ordre1;
	int *PC;
	int *blocage;
	int *t_blocage; // permet de gerer les depandance 
};typedef struct ins_deco ins_deco;

struct ins_exec{
	unsigned short *id_ex; // tableau de taille 4, pour contenire justqu'a 3 valeurs et l'oppcode
	unsigned short *ex_mem; // tableau de taille 3 pour , l'oppcode, l'adresse et la valeur 
	int *ordre2,*ordre1;
	int *PC;
};typedef struct ins_exec ins_exec;

struct ins_mem{
	unsigned short *ex_mem; // tableau de taille 3 pour , l'oppcode, l'adresse et la valeur 
	int *ordre2,*ordre3;
	unsigned short *mem_don;// memoir de donné, donc de taille 256(512 octect)
	unsigned short *mem_wb; // tableau de taille 3 pour , l'oppcode, l'adresse et la valeur 
};typedef struct ins_mem ins_mem;

struct ins_wb{
	unsigned short *registre; //tableau de taille 16 
	int *ordre3;
	unsigned short *mem_wb; // tableau de taille 3 pour , l'oppcode, l'adresse et la valeur 
};typedef struct ins_wb ins_wb;





unsigned short	U_A_L(unsigned short nb1, unsigned short nb2,char mode);

void	*instruction_fetch(void *arg);
void	*instruction_decode(void *arg);
void	*execute(void *arg);
void	*memory_acces(void *arg);
void	*write_back(void *arg);

void	Misc(mem_inst *mi, unsigned short *md);

void	initialisation_misc(ins_fetch *p1,ins_deco *p2 ,ins_exec *p3,ins_mem *p4,ins_wb*p5,int *pc,int *ordre,mem_inst *mi,unsigned short *registre,mem_inst *if_id,unsigned short *id_ex,unsigned short *ex_mem ,unsigned short *mem_wb, unsigned short *md );



#endif
