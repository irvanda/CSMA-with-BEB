/*CSMA/CA Using BEB Algorithm

by: Irvanda (20127734) 

--2012--
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h> 

/*initiation*/

int max_user; //Define initial max user
int it; //Define iteration
int ts; //Define Time Simulation
int suc = 0; //define counter success
int CW_min=0; //define CW_min
int CW_zero; //define counter of CW 0
int average; //define average successful packet

/*Main program*/
int main(void)
{
	for (max_user=10; max_user<110; max_user+=10)
	{ 
		int succ[max_user];
    
		    //Define CW values
		int user_num, CW_val[max_user], CW_max[max_user];
		
		for (user_num=0; user_num<max_user; user_num++)
		{
			CW_max[user_num] = 8;
 			CW_val[user_num] = CW_min + rand() % ((CW_max[user_num]-1) - CW_min);
		}
		
		for (it=0;it<100;it++) //it means iteration
		{
			srand(time(NULL)); // seed random number
 			
			for (ts=0;ts<10000000;ts++) //ts means time simulation
			{
				CW_zero = 0;
				
				for (user_num=0; user_num<max_user; user_num++)
				{
					if (CW_val[user_num]==0)
						CW_zero=CW_zero+1; 
				}
				if (CW_zero>1)
				{
					for (user_num=0; user_num<max_user; user_num++)
					{
						if (CW_val[user_num]==0)
						{
							CW_max[user_num] = CW_max[user_num] * 2;
							if (CW_max[user_num] > 1024) 
								CW_max[user_num] = 1024;
						  CW_val[user_num]= CW_min + rand() % ((CW_max[user_num]-1) - CW_min);
						}
						
						else 
							CW_val[user_num]=CW_val[user_num]-1;
					}
          
					ts=ts+10; //increase Time Simulation by 10
				}
				else if (CW_zero<1) //no transmission
				{
					for (user_num=0; user_num<max_user; user_num++)
					{
						CW_val[user_num]=CW_val[user_num]-1;
					}
				}
        
				else // zero = 1, means success
				{
					for (user_num=0; user_num<max_user; user_num++) 
					{
						if (CW_val[user_num]==0)
						{
							CW_max[user_num] = 8;
							CW_val[user_num]= CW_min + rand() % ((CW_max[user_num]-1) - CW_min);
						}
						else 
					 		CW_val[user_num]=CW_val[user_num]-1;
					}
					if (ts>10000) //ignore first 10 second
						suc=suc+1; ts=ts+10;
				}      
			}
		}
			
		average=suc/100;
		printf("max_user %d success %d \n", max_user, average);
		suc=0; //reset for the next looping max_user
	
	}

getchar();
return 0;
}
