/*
**  file: pth02.c
**
**  Somar os primeiros N numeros naturais 1,2,3,...,N
**  Processo bi-tarefa. 
**  Cria tarefa desacopolada da tarefa principal que efectua a soma.
**
**  Paulo Shirley @ UAb
**  v2008/04/10
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define N      1081080   /* N=2*3*4*5*7*9*11*13 */

double soma(double x[], int n); /* funcao que soma os elementos de um vector */ 
void *tarefa(void *arg);        /* prototipo funcao tarefa */

double v[N],S;                  /* vector de dados e soma total */


int main()
{
   int i,r;
   pthread_t      trfid;     /* variavel para ID da tarefa */
   pthread_attr_t trfatr;    /* variavel para atributos da tarefa */
   
   printf("\nA iniciar a tarefa principal: PID=%d\n", (int) getpid());
   for(i=0; i<N; i++)
      v[i]= i+1;
   /* inicializar variavel de atributos com valores por defeito */
   pthread_attr_init(&trfatr);
   /* modificar estado de desacoplamento para "detached" */
   pthread_attr_setdetachstate(&trfatr, PTHREAD_CREATE_DETACHED);
   /* criar e iniciar execução de tarefa */
   r= pthread_create(&trfid, &trfatr, tarefa, (void*) NULL);
   if( r ) {
      /* erro ! */
      perror("Erro na criacao da tarefa!");
      exit(1);
   }
   /* dar tempo para a sub-tarefa terminar */
   sleep(4);     /* nota: versão original com sleep(4) */

   printf("A terminar a tarefa principal.\n\n");
   return 0;
}

void *tarefa(void *arg)
{
   printf("A iniciar a sub-tarefa: PID=%d\n", (int) getpid());
   S= soma(v,N);
   printf(" Soma= %.0lf\n",S);
   printf("A terminar a sub-tarefa.\n");
   return (void*) NULL;
}

double soma(double x[], int n)
{
   int i;
   double s=0;
   
   for(i=0; i<n; i++)
      s+= x[i];

   return s;
}

/* EOF */
