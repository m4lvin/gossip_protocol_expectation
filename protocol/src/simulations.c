#include <stdio.h>
#include "main.h"
#include "simulations.h"


void print_g(int g[MAXN_SIM][MAXN_SIM], int n) {
	int i,j;
	
	for (i=0; i<n; i++) {
		for (j=0; j<n; j++)
			printf("%d ", g[i][j]);
		
		printf("\n");	
	}
	
}

void init_sec(int secrets[MAXN_SIM][MAXN_SIM], int n)
{
	int i,j;
	
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
			secrets[i][j]=(i==j)?1:0;				
}

void init_avail_calls(int avail_calls[MAXN_SIM][MAXN_SIM], int n)
{
	int i,j;
	
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
			avail_calls[i][j]=(i==j)?0:1;					
}

int edges(int g[MAXN_SIM][MAXN_SIM], int n)
{
	int i,j, result=0;
	
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
			result+=g[i][j];
	
	return result;						
}

void execute_call(int caller, int callee, int secrets[MAXN_SIM][MAXN_SIM],
	int avail_calls[MAXN_SIM][MAXN_SIM], int n, int prot) 
{		
	int k;
		
	//~ print_g(secrets,n);
	
	//~ printf("\n");
		
	for(k=0;k<n;k++)
		secrets[caller][k] = secrets[callee][k] = (secrets[caller][k] || secrets[callee][k]);				
	
	switch (prot) {	
		case (LNS) :
			for(k=0;k<n;k++)
				if (secrets[caller][k])
					avail_calls[caller][k]= avail_calls[callee][k]=0;
		break;
		case (CO) :	
			avail_calls[caller][callee] = avail_calls[callee][caller] =0;
			break;
		case (ANY) :
			break;
		case (TOK) :
			for(k=0;k<n;k++) {
				avail_calls[caller][k] = 0;
				avail_calls[callee][k] = 1;
			}
			avail_calls[callee][callee] = 0;
			break;
		case (SPI) :
			for(k=0;k<n;k++) {
				avail_calls[callee][k]=0;
				avail_calls[caller][k]=1;
			}
			avail_calls[caller][caller] = 0;
			break;		
	}		
}

int count_callers(int avail_calls[MAXN_SIM][MAXN_SIM], int n) 
{
	int count=0;
	
	int i, j;
	
	for(i=0; i < n ; i++)
		for(j=0; j < n ; j++) 		
			if (avail_calls[i][j]) { 
				count++;
				break;
			}
	
	return count;
}

int count_callees(int avail_calls[MAXN_SIM][MAXN_SIM], int i, int n) {
	int count=0;
	
	int j;
	
	for(j=0; j < n ; j++) 		
		count+= avail_calls[i][j];
	
	return count;
}

int get_caller(int avail_calls[MAXN_SIM][MAXN_SIM], int r_caller, int n)
{
	int i;
	
	for(i=0; i < n ; i++) {
		if (count_callees(avail_calls, i, n) > 0)
			r_caller--;
		if (!r_caller)
			break;
	}
	
	return i;
}

int get_callee(int avail_calls[MAXN_SIM][MAXN_SIM], int caller, 
	int r_callee, int n){
	int j;
	
	for(j=0; j < n ; j++) {
		if (avail_calls[caller][j])
			r_callee--;
		if (!r_callee)
			break;
	}
	
	return j;
}

void get_call_parts
(int avail_calls[MAXN_SIM][MAXN_SIM], int r_call, int *caller,
 int *callee, int n)
{
	int i,j;
	
	*caller=*callee=0;
	
	for (i=0; i <  n; i++)
		for (j=0; j < n; j++) {
			r_call-=avail_calls[i][j];
			if (!r_call) {
				*caller = i;
				*callee =j;
				return;
			}
		}
}


float simulated(pars_t pars) {
	int s, r_caller, r_callee, duration=0;		
	int secrets[MAXN_SIM][MAXN_SIM];
	int avail_calls[MAXN_SIM][MAXN_SIM];
							
	for(s=0; s<pars.max_sim;s++)  {
		init_sec(secrets, pars.n);
		init_avail_calls(avail_calls, pars.n);
		
		while (1) {
			get_call_parts(avail_calls, 
			   rand() % edges(avail_calls,pars.n) + 1, &r_caller, 
			   &r_callee, pars.n);
					
			//~ printf("caller=%d\n", r_caller);
			//~ printf("callee=%d\n", r_callee);
					
			execute_call(r_caller, r_callee, secrets, avail_calls,
				pars.n, pars.prot);
			duration++;
			
			if (edges(secrets,pars.n) == pars.n * pars.n)
				break;
		}
	}
		
	printf("Agents %d done!\n", pars.n);
	return ((float) duration)/pars.max_sim;
}
