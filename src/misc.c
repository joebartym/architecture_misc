
#include<stdio.h>
#include <pthread.h>
#include "../include/misc.h"


unsigned short	U_A_L(unsigned short nb1, unsigned short nb2,char mode)
{
	unsigned short res;
	if(mode == 4 || mode == 6 || mode == 8|| mode == 9) // add, addi, lw et sw
	{
		res = nb1 + nb2;
	}
	else if(mode == 5)
	{
		res = nb1 - nb2;
	}
	else if(mode == 7) // j'ai essayé de simulé l'algorythme de booth, j'ai repris celui que j'avais fait pour l'ias, 
	{
		struct booth{long int M:16,Q:16,A:16;};//multiplicant, multiplier et accumulateur, je suppose que l'ual dispose un accumulateur
		struct booth mul = {nb1,nb2,0};
		long int q0;
		long int k = 256;
		k = k*k;k>>=1;// 1 1 suivi de 15 0 utile pour la suite
		q0 = 0;
		for(int n = 16;n>0;n--)
		{
			if(mul.Q%2!=0 && q0==0)//ajout
				mul.A -= mul.M;
			else if(mul.Q%2==0 && q0!=0)
				mul.A += mul.M;
			q0 = mul.Q%2;
			mul.Q = mul.Q>>1;//decalage1
			if(mul.A%2!=0) // du bricolage pour pas  perdre le signe
				mul.Q = mul.Q|k;
			else
				mul.Q = mul.Q&(k-1);
			mul.A = mul.A>>1;//decalage
		}
		res = mul.Q;
	}

	return res;
}



/*________________________________________________*/



void	initialisation_misc(ins_fetch *p1,ins_deco *p2 ,ins_exec *p3,ins_mem *p4,ins_wb*p5,int *pc,int *ordre,mem_inst *mi,unsigned short *registre,mem_inst *if_id,unsigned short *id_ex,unsigned short *ex_mem ,unsigned short *mem_wb, unsigned short *md )
{
	p1->PC = pc; //initialisation ins_fetch
	p1->ordre = ordre;
	p1->mem = mi;
	p1->if_id = if_id;

	p2->ordre = ordre; //initialisation ins_decode
	p2->ordre1 = ordre+1;
	p2->if_id =if_id; 
	p2->registre = registre;
	p2->PC = pc;
	p2->id_ex=id_ex;

	p3->ordre1 = ordre+1;//initialisation ins_exec
	p3->ordre2 = ordre+2;
	p3->id_ex  =id_ex;
	p3->PC = pc;
	p3->ex_mem = ex_mem;
	
	p4->ordre2 = ordre+2;//initialisation ins_mem
	p4->ex_mem = ex_mem;
	p4->mem_don = md;
	p4->mem_wb = mem_wb;
	p4->ordre3 = ordre+3;

	p5->registre = registre;//initialisation ins_wb
	p5->ordre3 = ordre+3;
	p5->mem_wb = mem_wb;
}



/*________________________________________________*/



void	Misc(mem_inst *mi, unsigned short *md) //"mi" les instruction et "md" les données
{
	
  	pthread_t num_thread,num_thread1,num_thread2,num_thread3,num_thread4;
	unsigned short registre[16];
	int pc;
	int ordre[4];
	ordre[0] = 1;ordre[1] = 1;ordre[2] =1;ordre[3] = 1;
	mem_inst if_id; unsigned short id_ex[4],ex_mem[3] ,mem_wb[3];
	registre[0] = 0;
	ins_fetch 	*p1;
	ins_deco 	*p2;
	ins_exec	*p3;
	ins_mem		*p4;
	ins_wb		*p5;

	initialisation_misc(p1, p2 , p3, p4, p5, &pc, ordre, mi, registre, &if_id, id_ex, ex_mem, mem_wb, md); //fonction permettant d'initialiser les structure des arguments des fonction des modules
	int compte_a_rebourd = -1;
	while(compte_a_rebourd-- !=0)
	{

		pthread_create(&num_thread,NULL,instruction_fetch, (void *)p1);
		pthread_create(&num_thread1,NULL,instruction_decode, (void *)p2);
		pthread_create(&num_thread2,NULL,execute, (void *)p3);
		pthread_create(&num_thread3,NULL,memory_acces, (void *)p4);
		pthread_create(&num_thread4,NULL,write_back, (void *)p5);
		pthread_join(num_thread,NULL);//j'ai choisi de faire des thread pour les executer en simultané
		pthread_join(num_thread1,NULL);// un thread represente un module, 
		pthread_join(num_thread2,NULL);
		pthread_join(num_thread3,NULL); 
		pthread_join(num_thread4,NULL);
		if(if_id.oppcode == 0 && compte_a_rebourd<0) // oppcode = 0 -> instruction exite
			compte_a_rebourd = 4;
	}
}
