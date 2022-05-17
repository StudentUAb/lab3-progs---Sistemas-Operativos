/*
**  file: pth05.c
**
**  Somar os primeiros N numeros naturais 1,2,3,...,N
**  Processo multi-tarefa.
**  Aceita um argumento com o numero n de tarefas, onde n= {1-15,20,30}
**  Cria n tarefas juntaveis com a tarefa principal.
**  A soma e' repartida igualmente por todas as tarefas.
**  Utiliza o argumento da tarefa para retornar a respectiva soma parcial.
**
**  Paulo Shirley @ UAb
**  v2008/04/10
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N      1081080   /* N=2*3*4*5*7*9*11*13, N divisivel por 2-15, 20, 30 */
#define MAXTRF 30        /* numero maximo de tarefas */

double soma(double x[], int n); /* funcao que soma os elementos de um vector */ 
void *tarefa(void *arg);        /* prototipo funcao tarefa */

double v[N],S;                                /* vector de dados e soma total */
typedef struct {double *x,s; int n,i;} trfarg_t; /* tipo do argumento tarefas */


int main(int argc, char *argv[])
{
   int i,r,n,va;
   pthread_t      trfid[MAXTRF];    /* variavel para ID das tarefas */
   pthread_attr_t trfatr;           /* variavel para atributos das tarefas */
   trfarg_t       trfarg[MAXTRF];   /* variavel para argumento das tarefas */
   
   /* verificar validade dos argumentos */
   va=1;    /* flag validade argumentos */
   if( argc!=2 )
      va=0;              /* numero de argumentos invalido */
   else {
      n= atoi(argv[1]);
      if( !((n>=1 && n<=15) || n==20 || n==30) )
         va=0;           /* numero de tarefas invalido */
   }
   if( !va ) {
      printf("\n Utilizacao:  pth05 n\n com n=1-15, 20, 30\n\n");
      exit(1);
   }

   printf("\nA iniciar a tarefa principal(%d).\n", n-1);
   for(i=0; i<N; i++)
      v[i]= i+1;
   /* inicializar variavel de atributos com valores por defeito */
   pthread_attr_init(&trfatr);
   /* modificar estado de desacoplamento para "joinable" */
   pthread_attr_setdetachstate(&trfatr, PTHREAD_CREATE_JOINABLE);
   /* inicializar estruturas de dados que servem de argumento das tarefas */
   for(i=0; i<n-1; i++) {
      trfarg[i].i= i;  /* usado para notacao das tarefas criadas 0,...,n-2 */
      trfarg[i].n= N/n;       /* cada tarefa soma N/n elementos */
      trfarg[i].x= &v[i*N/n]; /* inicio do sub-vector a somar */
      /* criar e iniciar execucao de tarefa */
      r= pthread_create(&trfid[i], &trfatr, tarefa, (void*) &trfarg[i]);
      if( r ) {
         /* erro ! */
         perror("Erro na criacao da tarefa!");
         exit(1);
      }
   }
   /* ultimo segmento da soma fica para a tarefa principal (n-1) */
   S= soma(&v[(n-1)*N/n], N/n);

   /* esperar que as tarefas criadas terminem e calcular soma total
      a partir das parciais */
   for(i=0; i<n-1; i++) {
      pthread_join( trfid[i], (void **) NULL);
      S+= trfarg[i].s;
   }
   printf("Soma= %.0lf\n",S);
   
   printf("A terminar a tarefa principal(%d).\n", n-1);
   return 0;
}

void *tarefa(void *arg)
{
   trfarg_t *a;
   
   a= (trfarg_t *) arg;     /* converter (void *) para o tipo certo */
   printf("A iniciar a sub-tarefa(%d).\n", a->i);
   a->s= soma( a->x, a->n); /* variaveis de entrada e saida na mesma estrutura*/
   printf("A terminar a sub-tarefa(%d).\n", a->i);
   return (void*) NULL;     /* mecanismo de retorno por apontador nao e' usado*/
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
