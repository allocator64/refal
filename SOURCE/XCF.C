#include <stdio.h>
#include "refal.def"
extern REFAL refal;
typedef char *adr;
short func_n=0;
adr *func_f=NULL;
adr *malloc();
adr *realloc();
char convert(char cm);

static VOID ftochar_() {
 union {char b[2];short w;} d;
 linkcb *p;
 char *u;
 short i;
   p = refal.preva->next;
   if (p->tag != TAGF)  goto HEOT;
   u = p->info.codef - 1;
   d.b[0] = *u; d.b[1]=0; /* d.w - dlina */
   u -= d.w;
   p = refal.prevr;
   if (lrqlk(d.w) ==0) 
      if (lincrm()==0) {
         rfdel(refal.prevr,refal.nextr);
         refal.upshot = 3;
         return;
      }
   lins(p,d.w);
   for(i=0; i<d.w; i++) {
      p = p->next;
      p->info.codep = NULL;
      p->tag = TAGO;
      p->info.infoc = *(u+i);
   }
   return;
HEOT:
   printf("\n ***** MO ftochar : o{ibka w obra}enii");
   refal.upshot = 2;  return;
}
static char ftochar_0[] = {Z7'F','T','O','C','H','A','R','\007'};
G_L_B char ftochar = '\122';
static VOID (*ftochar_1)() = ftochar_;

static VOID functab_() {
 linkcb *p;
 char *u;
 short i;
   p = refal.preva->next;
   if (p->tag != TAGF)  goto HEOT;
   u = p->info.codef;
   for(i=0; i<func_n; i++) if (u == func_f[i]) return;
   if( func_n == 0 ) 
      func_f = (adr *) malloc(sizeof(adr));
   else
      func_f = (adr *) realloc(func_f, (func_n+1)*sizeof(adr));
   func_f[func_n] = u;
   func_n++;
   return;
HEOT:
   printf("\n ***** MO functab : o{ibka w obra}enii");
   refal.upshot = 2;  return;
}
static char functab_0[] = {Z7'F','U','N','C','T','A','B','\007'};
G_L_B char functab = '\122';
static VOID (*functab_1)() = functab_;

static VOID chartof_() {
 union {char b[2];short w;} d;
 linkcb *p;
 char *u,*j;
 short i,k;
   p = refal.preva->next;
   if (p == refal.nexta) goto HEOT;
   for(i=0; p!= refal.nexta; i++, p = p->next)
      if( p->tag != TAGO) goto HEOT;
   if (i>255) goto HEOT;
   p = refal.preva->next;
   u = (char *) malloc(i+2);
   for(i=0; p!= refal.nexta; i++, p = p->next)
      u[i] = convert(p->info.infoc);
   u[i] = i++;
   u[i] = 2; /* HEOT */
   j = u + i;
   d.b[1]=0;
   for(k=0; k<func_n; k++) {
      d.b[0]= *(func_f[k]-1);
      if( (i == d.w+1) && (strncmp(u,func_f[k]-(d.w+1),d.w)==0) ) {
            /* identificator iz tablicy ne preobr. w zaglawnye!!! */
            /* poetomu w m.o. imja d.b. napisano zaglawnymi!      */       
         p = refal.preva->next;
         p->tag = TAGF; p->info.codef = func_f[k];
         if( p->next != refal.nexta ) rfdel(p, refal.nexta);
         rftpl(refal.prevr, p->prev, p->next);
         free(u);
         return;
      }
   }
   if( func_n == 0 ) 
      func_f = (adr *) malloc(sizeof(adr));
   else
      func_f = (adr *) realloc(func_f, (func_n+1)*sizeof(adr));
   func_f[func_n] = j;
   func_n++;
   p = refal.preva->next;
   p->tag = TAGF; p->info.codef = j;
   if( p->next != refal.nexta ) rfdel(p, refal.nexta);
   rftpl(refal.prevr, p->prev, p->next);
   return;
HEOT:
   printf("\n ***** MO chartof : o{ibka w obra}enii");
   refal.upshot = 2;  return;
}
static char chartof_0[] = {Z7'C','H','A','R','T','O','F','\007'};
G_L_B char chartof = '\122';
static VOID (*chartof_1)() = chartof_;
