/*CSMA/CA Using BEB Algorithm

by: Irvanda (20127734 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h> 

#define BUSY 0
#define FREE 1
#define timeSlot 1 // 1 ms



//prototype for list
struct list_el {
	int expTime;
	struct list_el * next;
};

typedef struct list_el item;

int max_user; //Define initial max user
int it; //Define iteration
int ts; //Define Time Simulation
int suc = 0; //define counter success
int CW_min=0; //define CW_min
int CW_zero; //define counter of CW 0
int average; //define average successful packet
int rate, dropPacket=0, packet_gen=0;
int LOG = 1;
int mediumState = BUSY;
int mediumCnt = 1;
	
//bikin fungsi add
void add( item *(* head), item *(* tail), int expTime)
{
	item * curr = (item *)malloc(sizeof(item));
	//printf(" awalnya curr[user_num] = %p\n", curr  );
	curr->expTime = expTime;

	if(*tail)
		(*tail)->next  = curr;

	if (!*head)
		*head = curr;

	curr->next  = NULL;
	*tail = curr;
	//printf(" tail curr[user_num] = %p\n", tail  );
}

//bikin fungsi remove
void rem( item *(* head),item *(* tail))
{
	//printf(" jalan sampe sini" );
	if(*head == *tail)
	{
		*head = NULL;
		*tail = NULL;
	}
	else
	{ 
		*head = (*head)->next;
	}
}

void reduceCW(int CW_val[] ,int user_num,int  ts)
{
	CW_val[user_num]=CW_val[user_num]-1;
	
	if ( CW_val[user_num] <= 0)
		CW_val[user_num] = 0;
		
/*	if(LOG && CW_val[user_num] > -10 && CW_val[user_num] < 2 && user_num == 0)
		printf("DEC CW. id = %ld now = %ld CW=%ld \n", user_num, ts, CW_val[user_num]); */

}


/*Main program*/
int main(void)
{

	FILE* csv_file;
	csv_file = fopen("beb_log.txt", "a+");
	fprintf(csv_file, "======================================================\n");
	fprintf(csv_file, "number of user, packet generated, drop packet, success \n");
	fclose(csv_file);

	for (max_user=10; max_user<110; max_user+=10)
	
	{ //int succ[max_user];
		//Define CW values
		
		int user_num, CW_val[max_user], CW_max[max_user];
		int sendingCnt[max_user];
		int switchStateCnt[max_user];
		bool isActive[max_user];
		bool busyCnt=0; // for medium
		item  * head[max_user], * tail[max_user]; //bikin head ama tail
		
		
		for (it=0;it<1;it++) //it means iteration
		{
		
			srand(it); // seed random number


			//Inisiasi awal
			for (user_num=0; user_num<max_user; user_num++)
			{
				head[user_num] = NULL ;
				tail[user_num] = NULL;
				isActive[user_num] = false;
				CW_max[user_num] = 8;
				switchStateCnt[user_num] = 0;
				CW_val[user_num] = CW_min + rand() % ((CW_max[user_num]-1) - CW_min);
				//printf("setelah function add keluar > tail[user_num] = %p\n", tail[user_num]);
				sendingCnt[user_num] = 0;
			}
			
			
			for (ts=0;ts<timeSlot*100000;ts++) //waktu simulasi
			{
			
				mediumCnt--;
				
				if (mediumCnt==0)
					mediumState=FREE;
					
					
				CW_zero = 0;
				
				//inisiasi traffic
				if (mediumState==FREE)
				{
					for (user_num=0; user_num<max_user; user_num++)
					{
				
						if (switchStateCnt[user_num]==0)
						{
							if (isActive[user_num])
							{
								isActive[user_num] = false;
								rate = timeSlot*1350;
							}
							else
							{
								isActive[user_num] = true;
								rate = timeSlot*1000;
							}
							switchStateCnt[user_num]=-rate*log((double)(rand())/(double)(RAND_MAX));
							//printf("%ld\n", switchStateCnt[user_num]);
						}
					
					
						//packet = round(switchStateCnt[user_num]/20);
						//packet_gen = packet_gen+packet;
						switchStateCnt[user_num]--;


						//masukin 1 paket ke buffer
						if ((switchStateCnt[user_num])%(timeSlot*20)==0 && isActive[user_num])
							{
	//							if(LOG) 								printf("add. id = %ld now = %ld exp time=%ld \n", user_num, ts, ts+200);
								add(&head[user_num],&tail[user_num], ts+(timeSlot*200));
								if (ts>(timeSlot*10000)) 
									packet_gen++;
							}


						//cek expire time
						if ( head[user_num] != NULL )
						{
							if ( (*head[user_num]).expTime < ts) //kalo exp < ts drop aja
							{
	//							if(LOG)								printf("rem. id = %ld now = %ld exp time=%ld dropPacket=%ld \n", user_num, ts, (*head[user_num]).expTime, dropPacket);
								rem(&head[user_num], &tail[user_num]);
								if (ts>(timeSlot*10000))
									dropPacket++;
							}
						}
					
					
					
						//printf("sending CNT = %ld\n", sendingCnt[user_num]);
					
						//Cari yang CW nya 0
						if (CW_val[user_num]==0 &&  tail[user_num] != NULL && sendingCnt[user_num] <=0 )
						{ 
							CW_zero=CW_zero+1;
						}

						sendingCnt[user_num]--;
					
					}//END INISIASI TRAFFIC
					
				}//END IF mediumState


//Kalo collision
				if (CW_zero>1 && mediumState==FREE)
				{
					mediumCnt = timeSlot*10;
					mediumState = BUSY;
					
					
					for (user_num=0; user_num<max_user; user_num++)
					{
						
						//cek user yang collision
						if (CW_val[user_num]==0 &&  tail[user_num] != NULL && sendingCnt[user_num] <=0  )  
						{
						
							CW_max[user_num] = CW_max[user_num] * 2;
							//tambah 10 slot
							sendingCnt[user_num] = timeSlot*10;
							
							//batas max CW_Val
							if (CW_max[user_num] > 1024) 
							{
								CW_max[user_num] = 1024;
							}
							
							CW_val[user_num]= CW_min + rand() % ((CW_max[user_num]-1) - CW_min);
						}
						
						else if (tail[user_num] != NULL && sendingCnt[user_num] <=0 )
						{
							reduceCW(CW_val, user_num, ts);
						}
					}
					//ts=ts+10; //increase Time Simulation by 10
				}
				
				
//no transmission				
				else if (CW_zero<1 && mediumState==FREE) 
				{
				
					for (user_num=0; user_num<max_user; user_num++)
					{
						//cek kalo tail ga kosong dan waktunya dikirim
						if ( tail[user_num] != NULL  && sendingCnt[user_num] <=0 )
						{
							reduceCW(CW_val, user_num, ts);
						
					//		if(LOG)								printf("DEC. id = %ld now = %ld CW=%ld \n", user_num, ts, CW_val[user_num]);
						}
					}
					
				
				}//END of No Transmission
				
				
// zero = 1, means success				
				else if (mediumState==FREE)
				{
					mediumCnt = timeSlot*10;
					mediumState = BUSY;
					
					
					for (user_num=0; user_num<max_user; user_num++) 
					{
					
						//untuk yang VW_val=0, ganti nilai CW dan remove dari buffer
						if (CW_val[user_num]==0 &&  tail[user_num] != NULL && sendingCnt[user_num] <=0  )
						{
							CW_max[user_num] = 8;
							CW_val[user_num]= CW_min + rand() % ((CW_max[user_num]-1) - CW_min);
							//printf(" tail sebelum diremove = %p\n", tail[user_num]);
							//printf(" head sebelum diremove = %p\n", head[user_num]);
							rem(&head[user_num],&tail[user_num]);
							//printf(" tail setelah diremove = %p\n", tail[user_num]);
							//printf(" head setelah diremove = %p\n", head[user_num]);
//							if(LOG)								printf("suc. id = %ld now = %ld CW=%ld \n", user_num, ts, CW_val[user_num]);
						}
						
						
						//kalo CW bukan 0, kurangin CW
						else if (tail[user_num] != NULL && sendingCnt[user_num] <=0 )
						{
							reduceCW(CW_val, user_num, ts);
						}
				
					}
				
					//if (ts>10000) //ignore first 10 second
					//{
						//printf(" ts = %ld\n", ts);
						if (ts>timeSlot*10000)
						suc++;
					//}
				}//END for Success      
			
			
			}//END SIMULASI
			
			
			
			
		//printf("1 = max_user %d packet %d dropPacket %d  \n", max_user, packet_gen, dropPacket);
		
		
		
		}//END ITERASI
		//average=suc/100;
		
		
		
		printf("max_user %d packet %d dropPacket %d success %d  \n", max_user, packet_gen, dropPacket, suc);
		
		//write the compiled result into file
	FILE* csv_file;
	csv_file = fopen("beb_log.txt", "a+");
	fprintf(csv_file, "%d, %d, %d, %d\n", max_user, packet_gen, dropPacket, suc);
	fclose(csv_file);
	
	
	
		suc=0; //reset for the next looping max_user
		dropPacket=0;
		packet_gen=0;
		
	}//END EACH USER
	getchar();
	return 0;
}
