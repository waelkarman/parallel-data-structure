#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#ifndef N
#define N 5
#endif
#ifndef FS
#define FS 38
#endif

struct node {
   int data;
   int fibdata;
   struct node* next;
};

int fib(int n) {
   int x, y;
   if (n < 2) {
      return (n);
   } else {
      x = fib(n - 1);
      y = fib(n - 2);
	  return (x + y);
   }
}

void processwork(struct node* p) 
{
   int n;
   n = p->data;
   p->fibdata = fib(n);
}

void processworkinparallel(struct node* p, int i) 
{
   int j=0;
   while (j<i) {
      j++;
      p = p->next;
   }
   int n;
   n = p->data;
   p->fibdata = fib(n);
}

struct node* init_list(struct node* p) {
   int i;
   struct node* head = NULL;
   struct node* temp = NULL;
   
   head = malloc(sizeof(struct node));
   p = head;
   p->data = FS;
   p->fibdata = 0;
   for (i=0; i< N; i++) {
      temp  =  malloc(sizeof(struct node));
      p->next = temp;
      p = temp;
      p->data = FS + i + 1;
      p->fibdata = i+1;
   }
   p->next = NULL;
   return head;
}

int main(int argc, char *argv[]) {
   double start, end;
   struct node *p=NULL;
   struct node *temp=NULL;
   struct node *head=NULL;
   struct node *r=NULL;
   struct node *k=NULL;

   printf("Process linked list\n");
   printf("  Each linked list node will be processed by function 'processwork()'\n");
   printf("  Each ll node will compute %d fibonacci numbers beginning with %d\n",N,FS);      
 
   p = init_list(p);
   head = p;
   int algo = 2,count = 0;
   
   if (algo == 0){
      start = omp_get_wtime();
      {
         while (p != NULL) {
            processwork(p);
            p = p->next;
         }
      }
      end = omp_get_wtime();
   }else if (algo == 1){
      start = omp_get_wtime();
      {
         r = head;
         while (r != NULL) {
            count++;
            r = r->next;
         }

         #pragma omp parallel for
         for(int j=0; j<count; j++){
            processworkinparallel(p, j);
         }
      }
      end = omp_get_wtime();
   }else if (algo == 02){

      start = omp_get_wtime();
      {
         k=head;
         while (k != NULL) {
            count++;
            k = k->next;
         }
         
         struct node* arr[count];
         k=head;
         for(int i=0; i<count;i++){
            arr[i] = k;
            k = k->next;
         }

         #pragma omp parallel for
         for(int j=0; j<count; j++){
            processwork(arr[j]);
         }
      }
      end = omp_get_wtime();
   }

   p = head;
   while (p != NULL) {
      printf("%d : %d\n",p->data, p->fibdata);
      temp = p->next;
      free (p);
      p = temp;
   }  
   free (p);

   printf("Compute Time for algo %d : %f seconds\n",algo, end - start);

   return 0;
}

