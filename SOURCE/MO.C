#include "refal.def"
#include <stdio.h>
extern REFAL refal;
extern char debug;

#ifdef PDP
void pcoden();
long gcoden();
#endif

static void p1_() {
 linkcb *p; long l;
   p = refal.preva->next;
   if ( (p->next != refal.nexta) || (p->tag != TAGN) ) {
NEOT: refal.upshot = 2;
      return;
   }
   l=gcoden(p)+1;
   if (l > 16777215l ) goto NEOT;
   pcoden(p,l);
   rftpl(refal.prevr, p->prev, p->next);
   return;
 }
static char p1_0[] = {Z2   'P','1','\002'};
G_L_B char p1 = '\122';    static void (*p1_1)() = p1_;

static void m1_() {
 linkcb *p; long l;
   p = refal.preva->next;
   if ( (p->next != refal.nexta) || (p->tag != TAGN) ) {
NEOT: refal.upshot = 2;
      return;
   }
   l=gcoden(p)-1;
   if (l<0) goto NEOT;
   pcoden(p,l);
   rftpl(refal.prevr, p->prev, p->next);
   return;
 }
static char m1_0[] = {Z2    'M','1','\002'};
G_L_B char m1 = '\122';     static void (*m1_1)() = m1_;

long atol();

static void numb_() {
 linkcb *p,*pz,*p1;
 char str[12],zn;
 register i; long l;
   p = refal.preva->next;
   zn = p->info.infoc;
   pz=p;
   if ((p->tag == TAGO) && ((zn=='-')||(zn=='+')) ) {
      p = p->next;
      if(zn=='+')  pz=p; 
   }
   p1=p;
   while ((p->tag==TAGO) && (p->info.infoc=='0')) p=p->next;
   for (i=0; p != refal.nexta; i++) {
      if( (p->tag != TAGO) || (i==11) ) {
NEOT:    refal.upshot = 2;
         return;
      }
      else {
         str[i] = p->info.infoc;
         if (str[i] < 48 || str[i] > 57 ) goto NEOT;
         if ((i==9) && (cmpstr(i+1,str,"2147483647") > 0) )
            goto NEOT;
         p=p->next;
      }
   }
   str[i]='\0';
   if(strlen(str)==0) {
      if (lrqlk(1)==0)
         if (lincrm() == 0) {
            refal.upshot = 3;
            return;
         }
      lins(refal.nexta->prev,1);
      p1=refal.nexta->prev; pz=p1;
   }
   l=atol(str);
   p1->tag = TAGN;
   if(l>16777215l) { pcoden(p1,l/16777216l);
      p1=p1->next; p1->tag = TAGN; l = l % 16777216l;
   }
   pcoden(p1,l);
   rftpl(refal.prevr,pz->prev, p1->next);
   return;
}
static char numb_0[] = {Z4   'N','U','M','B','\004'};
G_L_B char numb = '\122';    static void (*numb_1)() = numb_;

static void symb_() {
 linkcb *p,*pz,*p1;
 char str[12],zn;
 register i; long l; int j;
   p = refal.preva->next;
   zn = p->info.infoc;
   pz=p;
   if ((p->tag == TAGO) && ((zn=='-')||(zn=='+')) ) {
      p = p->next;
      if(zn=='+') pz=p;
   }
   p1=p;
   while ((p->tag==TAGN) && (gcoden(p)==0l)) p=p->next;
   for (i=0; p != refal.nexta; i++, p=p->next )
      if( (p->tag != TAGN) || (i==2) ) goto NEOT;
   p=p->prev;
   if( (i==2) && (gcoden(p1) >= 128) )  {
NEOT:    refal.upshot = 2;
         return;
   }
   l=gcoden(p);
   if(i==2) l=l+ 16777216l * gcoden(p1);
   if((i==0)||(l==0l)) { pz=p1; l=0l; }
   sprintf(str,"%ld",l);
   j=strlen(str);
   if (lrqlk(j)==0)
      if (lincrm() == 0) {
         refal.upshot = 3;
         return;
      }
   if(pz != refal.nexta)  lins(p1,j);
   else { pz=pz->prev; lins(pz,j); pz=pz->next; p1=pz; } 
   for(i=0, p=p1; i<j; i++, p=p->next) {
      p->tag=TAGO; pcoden(p,0l); p->info.infoc = str[i];
   }
   rftpl(refal.prevr,pz->prev, p);
   return;
}
static char symb_0[] = {Z4   'S','Y','M','B','\004'};
G_L_B char symb = '\122';    static void (*symb_1)() = symb_;

static void first_() {
 linkcb *p,*pn;
 int k;  long n;  /*eg*/
   pn = refal.preva->next;
   if ((pn == refal.nexta) || (pn->tag != TAGN))
                         { refal.upshot = 2; return; }; /* FAIL */
   n = gcoden(pn);
   p = pn;
   for ( k=1; k<=n; k++) {
      p = p->next;
      if ( p == refal.nexta) {
         pn->info.codef = NULL; pn->info.infoc = '*';
         pn->tag = TAGO;
         rftpl(refal.prevr, refal.preva, refal.nexta);
         return;
      }
      if (p->tag == TAGLB) p = p->info.codep;
   }
   p = p->next;
   refal.preva->info.codep = pn; refal.preva->tag = TAGLB;
   pn->info.codep = refal.preva; pn->tag = TAGRB;
   rftpl(refal.preva,pn,p);
   rftpl(refal.prevr,refal.nextr,refal.nexta); return;
}
static char first_0[] = {Z5  'F','I','R','S','T','\005'};
G_L_B char first = '\122';   static void (*first_1)() = first_;

static void last_() {
 linkcb *pn,*p;
 int k; long n;  /*eg*/
   pn = refal.preva->next;
   if((pn == refal.nexta) || (pn->tag != TAGN))
                        { refal.upshot = 2; return; }; /* FAIL */
   n = gcoden(pn);
   p = refal.nexta;
   for(k=1; k<=n; k++) {
      p = p->prev;
      if (p == pn) {
         pn->info.codef = NULL; pn->info.infoc = '*';
         pn->tag = TAGO;
         rftpl(refal.prevr,pn,refal.nexta);
         p = refal.nextr->prev;
         rftpl(p,refal.preva,refal.nexta);
         return;
      }
      if(p->tag == TAGRB) p = p->info.codep;
   }
   p = p->prev;
   refal.preva->info.codep = pn; refal.preva->tag = TAGLB;
   pn->info.codep = refal.preva; pn->tag = TAGRB;
   rftpl(refal.preva,p,refal.nexta);
   rftpl(refal.prevr,pn,refal.nexta);
   p = refal.nextr->prev;
   rftpl(p,refal.nextr,refal.nexta); return; 
}
static char last_0[] = {Z4   'L','A','S','T','\004'};
G_L_B char last = '\122';    static void (*last_1)() = last_;

static lengr_() {
 linkcb *p;
 long n; /* kras */
   n = NULL;
   p = refal.preva->next;
   while( p != refal.nexta) {
      n++;
      p = p->next;
   }
   refal.preva->info.codep = NULL;
   pcoden(refal.preva, n);
   refal.preva->tag = TAGN;
   rftpl(refal.prevr,refal.nextr,refal.nexta); return; 
}
static char lengr_0[] = {Z5'L','E','N','G','R','\005'};
G_L_B char lengr = '\122';
static (*lengr_1)() = lengr_;

static lengw_() {
 linkcb *p;
 long n;  /* kras  */
   n = NULL;
   p = refal.preva->next;
   while (p != refal.nexta){
      n++;
      if(p->tag == TAGLB) p = p->info.codep;
      p = p->next;
   }
   refal.preva->info.codef = NULL;
   pcoden(refal.preva, n);
   refal.preva->tag = TAGN;
   rftpl(refal.prevr,refal.nextr,refal.nexta); return; 
}
static char lengw_0[] = {Z5'L','E','N','G','W','\005'};
G_L_B char lengw = '\122';
static (*lengw_1)() = lengw_;

static multe_() {
 linkcb *p,*pn,*q;
 long n,k;
   pn = refal.preva->next;
   if((pn == refal.nexta) || (pn->tag != TAGN))
                     { refal.upshot = 2; return;};  /* FAIL */
   n = gcoden(pn);
   if(n == 0) return;
   p  = pn->next;
   if (p == refal.nexta) return;
   if(p->next != refal.nexta) {
      do {
         p = refal.nextr->prev;
         if (!lcopy(p,pn,refal.nexta))
            { refal.upshot = 3; return;};  /* LACK */
         n--;
      } while (n >= 1);
   }
   else {
      if(!lins(refal.prevr,n))
              { refal.upshot = 3; return;};  /*  LACK */
      q = refal.prevr;
      for (k=1; k<=n; k++) {
         q = q->next;
         pcoden(q, gcoden(p));
         q->tag = p->tag;
      }
   }
   return; 
}
static char multe_0[] = {Z5'M','U','L','T','E','\005'};
G_L_B char multe = '\122';
static (*multe_1)() = multe_;

st *malloc();
static appl_() {
 st *s_st, *upst;
 linkcb *px,*pk,*pd;
   upst = refal.currst;
   if (!lins(refal.prevr,1)) goto LACK;
   px = refal.prevr->next;
   s_st=malloc(sizeof(st));
   if (!lcre(s_st)) goto LACK;
   if (!lins(s_st->view,2)) {
     rfcanc(s_st);
     goto LACK;
   }
   pk = s_st->view->next;
   pd = pk->next;
   pk->info.codep = NULL;
   pk->tag = TAGK;
   pd->info.codep = pk;
   pd->tag = TAGD;
   s_st->dot = pd;
   rftpl(pk,refal.preva,refal.nexta);
   rftpl(s_st->store,upst->store,upst->store);
   s_st->stop = -1;
   do {
      if(debug==0) rfrun(s_st);
      else         dbapp(s_st);
      if (s_st->state == 3)
        if (lincrm()) s_st->state = 1;
   } while ((s_st->state == 1) && (s_st->dot != NULL));
   rftpl(upst->store,s_st->store,s_st->store);
   switch (s_st->state) {
     case 1:
          px->info.infoc = 'N';
          rftpl (px,s_st->view,s_st->view);
          break;
     case 2:
          px->info.infoc = 'R';
          pd = s_st->dot;
          pk = pd->info.codep;
          rftpl(px,pk,pd);
          break;
     case 3:
          px->info.infoc = 'S';
          break;
   }
   rfcanc(s_st);
   free(s_st);
   return;
LACK: refal.upshot = 3; return; 
}
static char appl_0[] = { Z5'A','P','P','L','Y','\005'};
G_L_B char apply = '\122';
static (*appl_1)() = appl_;  

static delf_() {
 linkcb *dot,*dot1,*sk,*nd;
  if(refal.preva->next != refal.nexta){refal.upshot = 2;return;} /*FAIL*/
  dot = refal.nexta;
  dot1 = refal.nextr->info.codep;
REPEAT:
  sk = dot->info.codep;
  if (sk == NULL) rfabe("delf: sign '#' missing ");
  dot = sk->info.codep;
  nd = dot->next;
  if (nd->info.infoc != '#' ) goto REPEAT;
  while (dot1 != dot ) {
   sk = dot1->info.codep;
   nd = sk->info.codep;
   rfdel( sk->prev, dot1->next);
   dot1 = nd;
  }
  sk = dot1->info.codep;
  nd = sk->info.codep;
  dot1 = dot1->next;
  rfdel (sk->prev, dot1->next);
  refal.nextr->info.codep = nd; return; 
}
static char delf_0[] = { Z4'D','E','L','F','\004' };
G_L_B char delf = '\122';
static (*delf_1)() = delf_;

static VOID crel_() {
 char c;
 linkcb *p,*p1,*q,*q1;
   p = refal.preva->next;
   if ( p->tag != TAGLB ) goto FAIL;
   p1 = p->info.codep;
   p = p->next;
   q = p1->next;
   q1 = refal.nexta;
   c = '=';
   for(; c=='=' && p!=p1 && q!=q1; p=p->next, q=q->next) {
      if( p->tag==TAGLB ) {
         if( q->tag!=TAGLB ) goto FAIL;
         else break;      }
      if( p->tag==TAGRB ) {
         if( q->tag!=TAGRB ) goto FAIL;
         else break;      }
      if( p->info.coden < q->info.coden ) c = '<';
      if( p->info.coden > q->info.coden ) c = '>';
   }
   if( p==p1 && q!=q1 ) c = '<';
   if( q==q1 && p!=p1 ) c = '>';
   for(; p!=p1; p=p->next) if( p->tag==TAGLB ) goto FAIL;
   for(; q!=q1; q=q->next) if( q->tag==TAGLB ) goto FAIL;
   p = refal.preva->next;
   p->tag = TAGO;   p->info.coden = 0l;   p->info.infoc = c;
   q = p->next;
   rftpl(refal.prevr, refal.preva, q);
   return;
FAIL:
   refal.upshot = 2; return;  
}
static char crel_0[] = {Z4  'C','R','E','L','\004'};
G_L_B char crel = '\122';   static VOID (*crel_1)() = crel_;
