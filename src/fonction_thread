#include "../include/misc.h"

/*________________________________________________*/



void	*instruction_fetch(void *arg)
{
	ins_fetch *instru = (ins_fetch*)arg;

	while(*instru->ordre!=0);	//attend que instruction_decode finisse de lire if/id avant de modifier
	*instru->if_id = instru->mem[*instru->PC];
	*instru->PC += 1;
	*instru->ordre = 1;
	return NULL;
}

/*________________________________________________*/

void	*instruction_decode(void *arg)
{/* */
	ins_deco *instru = (ins_deco*)arg; 
	mem_inst tmp = *instru->if_id;
	*instru->ordre = 0; // verrou levé
	while(*instru->ordre1!=0);//attend que instruction_decode finisse de lire id/ex avant de modifier

	instru->id_ex[0] = tmp.oppcode;

	if(tmp.oppcode == 0) ;
	else if(tmp.oppcode <= 3) // gestion des jumps
	{
		if(tmp.oppcode == 3)
			instru->id_ex[1] = (tmp.addr1<<8) +(tmp.addr2<<4) +(tmp.addr3);
		else
			instru->id_ex[1] =tmp.addr3;
		if((tmp.addr1 == tmp.addr2 && tmp.oppcode == 1)||(tmp.addr1 != tmp.addr2 && tmp.oppcode == 2))
		{
			instru->id_ex[2] = 1; // annonce que le jump aura lieu car la condition est verifié
			while(*instru->ordre!=1);
			instru->if_id->oppcode = 10;
			*(instru->PC) -= 1;
		}
		else
			instru->id_ex[2] = 0; //annonce que le jump n'aura pas lieu car la condition n'est pas verifié
	}
	else if(tmp.oppcode <10)// le cas general
	{
		instru->id_ex[1] = instru->registre[tmp.addr1];
		instru->id_ex[2] = instru->registre[tmp.addr2];
		if(tmp.oppcode == 4 || tmp.oppcode ==  5 || tmp.oppcode == 7)
			instru->id_ex[3] = instru->registre[tmp.addr3];
		else
			instru->id_ex[3] = tmp.addr3;
	}
	else if(tmp.oppcode == 10) // cas special pour le deuxieme tours de no_op, puisque le jump se fait en "execute"
	{
		while(*instru->ordre!=1);
		instru->if_id->oppcode = 11;
	}


	*instru->ordre1 = 1;
	return NULL;
}


/*________________________________________________*/


void	*execute(void *arg)
{
	ins_exec *instru = (ins_exec*)arg;
	unsigned short tmp[4];
	for(int i = 0;i<4;i++)
	 	tmp[i] = instru->id_ex[i];  // je recuperre une copy puisque que id/ex va etre modifié par instruction_decode
	*instru->ordre1 = 0; // verrou levé

	while(*instru->ordre2!=0);

	instru->ex_mem[0] = tmp[0];
	instru->ex_mem[1] = tmp[1];
	if(tmp[0] == 0 )
		return NULL;
	else if((tmp[0] = 2 && tmp[2] == 1)||(tmp[0] = 1 && tmp[2] == 1)||(tmp[0] == 3)) // verifie condition jump validée
	{
		*instru->PC += tmp[1]; // j'ai pas compris le +4 dans le tableau en plus, je ne l'ai ducoup pas mis
	}
	else if (3<tmp[0] && tmp[0]<10)
	{
		instru->ex_mem[2] = U_A_L(tmp[2], tmp[3], tmp[0]);
	}
	*instru->ordre2 = 1;
	return NULL;
}



/*________________________________________________*/




void	*memory_acces(void *arg)
{
	ins_mem *instru = (ins_mem*)arg;
	unsigned short tmp[3];
	for(int i = 0;i<3;i++)
	 	tmp[i] = instru->ex_mem[i];  // je recuperre une copy puisque que ex/mem va etre modifié par instruction_decode
	*instru->ordre2 = 0; // verrou levé
	
	while(*instru->ordre3!=0);
	instru->mem_wb[0] = tmp[0];
	instru->mem_wb[1] = tmp[1];
	if(tmp[0] == 0)
		return NULL;
	if(tmp[0] == 8)
		instru->mem_wb[2]  = instru->mem_don[tmp[2]]; //load
	else if(tmp[0] == 9)
		instru->mem_don[tmp[2]] = tmp[1]; // store
	else if(tmp[0] <= 4 && 7>= tmp[0] )//arithmetique
		instru->mem_wb[2] = tmp[2];


	*instru->ordre3 = 1;
	return NULL;
}


/*________________________________________________*/




void	*write_back(void *arg)
{
	ins_wb *instru = (ins_wb*)arg;
	unsigned short tmp[3];
	for(int i = 0;i<3;i++)
	 	tmp[i] = instru->mem_wb[i];  // je recuperre une copy puisque que mem/wb va etre modifié par instruction_decode
	*instru->ordre3 = 0; // verrou levé
	if(tmp[0] <= 4 && 8>= tmp[0] )
		instru->registre[tmp[1]] = tmp[2];

	return NULL;
}
