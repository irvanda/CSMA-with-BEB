/*CSMA/CA Using BEB Algorithm

by: Irvanda (20127734 )*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h> 

#define BUSY 0
#define FREE 1
#define timeSlot 100 // 1 ms


#define max( a, b ) ( ((a) > (b)) ? (a) : (b) )

/*initiation*/

//prototype for list
struct list_el {
	int expTime;
	int packetSize;
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

	
//make add function
void add( item *(* head), item *(* tail), int expTime, int packetSize)
{
	item * curr = (item *)malloc(sizeof(item));
	curr->expTime = expTime;
	curr->packetSize = packetSize;
	
	if(*tail)	
		(*tail)->next  = curr;
	
	
	if (!*head)
		*head = curr;

	curr->next  = NULL;
	*tail = curr;
	
}


//make remove fuction
void rem( item *(* head),item *(* tail))
{
	
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
		
}

//pareto function
double Pareto( double k, double alpha, double m)
{
  double ret;
  int sign = 0;

  double randf = (double) rand()/RAND_MAX;
  
  ret = (double) ( k / pow(1-randf, 1/alpha) ) ;
  if ( ret >=m)
    ret = m;
  else if ( ret < k ) 
    ret = k;

  return ret;
}


/*Main program*/
int main(void)
{
	//export to file
	FILE* csv_file;
	csv_file = fopen("beb_log.txt", "a+");
	fprintf(csv_file, "======================================================\n");
	fprintf(csv_file, "number of user, packet generated, drop packet, success \n");
	fclose(csv_file);

	for (max_user=200; max_user<1100; max_user+=100)
	
	{ 
		
		int user_num, CW_val[max_user], CW_max[max_user];
		int sendingCnt[max_user];
		int switchStateCnt[max_user];
		bool isActive[max_user];
		bool busyCnt=0; // for medium
		item  * head[max_user], * tail[max_user]; //bikin head ama tail
		
		int startTime[10];
		int nextCnt[10];
		int packetCnt[10];
		double size[10];
		int transmit;
		
		
		
		for (it=0;it<10;it++) //it means iteration
		{

			srand(time(NULL)); // seed random number


			//video user initiation
			for (user_num=0; user_num<10 ; user_num++)
			{
				head[user_num] = NULL ;
				tail[user_num] = NULL;
				packetCnt[user_num] = 8;
				startTime[user_num]=(rand()%(100*timeSlot));
				CW_max[user_num] = 8;
				CW_val[user_num] = CW_min + rand() % ((CW_max[user_num]-1) - CW_min);
				sendingCnt[user_num] = 0;
	  		}
	  		
	  		
			//voice user initiation
			for (; user_num<max_user; user_num++)
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
			
			int mediumCnt = 1;
			
			for (ts=0;ts<timeSlot*100000;ts++) //simulation time
			{
				
				mediumCnt--;
				
				if (mediumCnt==0)
					mediumState=FREE; //medium is free
					
					
				CW_zero = 0;
				
				//traffic generator
				if (mediumState==FREE)
				{
					//Video Generator
					for (user_num=0; user_num<10 ; user_num++)
					{
						
						if( (ts)%(100*timeSlot)==startTime[user_num] )
		  				{
		  					packetCnt[user_num] = 8;
		  					nextCnt[user_num] = ts;
							
		  				}
		  				
		  				
		  				if (packetCnt[user_num] > 0 && nextCnt[user_num] == ts )
		  				{
		  					
		  					packetCnt[user_num]--;
		  					nextCnt[user_num] = Pareto(250, 1.2, 1250);
		  					nextCnt[user_num] += ts;
		  					size[user_num] = Pareto(20, 1.1, 125); // in bytes
		  					transmit =  size[user_num]*8; //in bit
		  					transmit/=80;
			 				transmit=ceil(transmit);
			 				
			 				//entry 1 packet to buffer
			 				add(&head[user_num],&tail[user_num], ts+(timeSlot*200), transmit);
			 				if (ts>(timeSlot*10000)) 
									packet_gen++;
			 				
		  				} 
		  				
		  				//expire time check
						if ( head[user_num] != NULL )
						{
							if ( (*head[user_num]).expTime < ts) //drop packet
							{
								rem(&head[user_num], &tail[user_num]);
								if (ts>(timeSlot*10000))
									dropPacket++;
							}
						}
						
						//Search CW value = 0
						if (CW_val[user_num]==0 &&  tail[user_num] != NULL && sendingCnt[user_num] <=0 )
						{ 
							CW_zero=CW_zero+1;
						}

						sendingCnt[user_num]--;
			
					}
					//Voice Generator
					for (; user_num<max_user; user_num++)
					{
						
						//Voice generator state (talking and silent)
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
							
						}
					
					
						switchStateCnt[user_num]--;


						//entry 1 packet to buffer
						if ((switchStateCnt[user_num])%(timeSlot*20)==0 && isActive[user_num])
							{

								add(&head[user_num],&tail[user_num], ts+(timeSlot*200), timeSlot*20);
								if (ts>(timeSlot*10000)) 
									packet_gen++;
							}


						//expire time check
						if ( head[user_num] != NULL )
						{
							if ( (*head[user_num]).expTime < ts) //drop packet
							{
	
								rem(&head[user_num], &tail[user_num]);
								if (ts>(timeSlot*10000))
									dropPacket++;
							}
						}
					
					
							
						//Search CW value = 0
						if (CW_val[user_num]==0 &&  tail[user_num] != NULL && sendingCnt[user_num] <=0 )
						{ 
							CW_zero=CW_zero+1;
						}

						sendingCnt[user_num]--;
					
					}//END TRAFFIC INITIATION
					
				}//END IF mediumState


//COLLISION
				if (CW_zero>1 && mediumState==FREE)
				{
					
					mediumState = BUSY; // set channel to busy
					
					for (user_num=0; user_num<max_user; user_num++)
					{
											
						//check user collision
						if (CW_val[user_num]==0 &&  tail[user_num] != NULL && sendingCnt[user_num] <=0  )  
						{
							mediumCnt = max(mediumCnt, (*head[user_num]).packetSize);
														
							CW_max[user_num] = CW_max[user_num] * 2;
							//set sending counter time 
							sendingCnt[user_num] = (*head[user_num]).packetSize;
														
							
							//Set maximum CW value
							if (CW_max[user_num] > 1024) 
							{
								CW_max[user_num] = 1024;
							}
							
							CW_val[user_num]= CW_min + rand() % ((CW_max[user_num]-1) - CW_min);
						}
						
						else if (tail[user_num] != NULL && sendingCnt[user_num] <=0 )
						{
							reduceCW(CW_val, user_num, ts); //reduce CW value
						}
					}
					
				}
				
				
//NO TRANSMISSION				
				else if (CW_zero<1 && mediumState==FREE) 
				{
				
					for (user_num=0; user_num<max_user; user_num++)
					{
						
						if ( tail[user_num] != NULL  && sendingCnt[user_num] <=0 )
						{
							reduceCW(CW_val, user_num, ts); //reduce CW value
						
						}
					}
					
				
				}//END of No Transmission
				
				
// zero = 1, means success				
				else if (mediumState==FREE)
				{
					mediumState = BUSY; // set channel to busy
					
					
					for (user_num=0; user_num<max_user; user_num++) 
					{
						
					
						//change CW value and remove form buffer
						if (CW_val[user_num]==0 &&  tail[user_num] != NULL && sendingCnt[user_num] <=0  )
						{
						
							mediumCnt = max(mediumCnt, (*head[user_num]).packetSize);
							rem(&head[user_num],&tail[user_num]);						
							CW_max[user_num] = 8;
							CW_val[user_num]= CW_min + rand() % ((CW_max[user_num]-1) - CW_min);
							
						}
						
						
						//if CW value is not 0
						else if (tail[user_num] != NULL && sendingCnt[user_num] <=0 )
						{
							reduceCW(CW_val, user_num, ts); //reduce CW value
						}
				
					}
				
					//Calculate Success	
					if (ts>timeSlot*10000)
					suc++;
					
				}//END for Success      
			
			
			}//END SIMUlATION
			
				
		}//END ITERATION
				
		
		printf("\nmax_user %d packet %d dropPacket %d success %d  \n", max_user, packet_gen/10, dropPacket/10, suc/10);
		
		
		
		//write the compiled result into file
		FILE* csv_file;
		csv_file = fopen("beb_log.txt", "a+");
		fprintf(csv_file, "%d, %d, %d, %d\n", max_user, packet_gen/10, dropPacket/10, suc/10);
		fclose(csv_file);
	
	
	
		suc=0; //reset for the next looping max_user
		dropPacket=0;
		packet_gen=0;
		
	}//END EACH USER
	getchar();
	return 0;
}
