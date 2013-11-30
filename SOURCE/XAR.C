#include "refal.def"
#include <stdio.h>
extern REFAL refal;

#ifdef PDP
long gcoden();
void pcoden();
#endif

#define Oadd 1
#define Osub 2
#define Omul 3
#define Odr  5

#define HMAX 4096L
#define MAX 16777216L
#define SMAX 24

static VOID add_() {  oper(Oadd,0); }
static char add_0[] = {Z3'A','D','D','\003'};
G_L_B char add = '\122';
static VOID (*add_1)() = add_;

static VOID sub_() { oper(Osub,0); }
static char sub_0[] = {Z3'S','U','B','\003'};
G_L_B char sub = '\122';
static VOID (*sub_1)() = sub_;

static VOID mul_() { oper(Omul,0); }
static char mul_0[] = {Z3'M','U','L','\003'};
G_L_B char mul = '\122';
static VOID (*mul_1)() = mul_;

static VOID dr_() { oper(Odr,0); }
static char dr_0[] = {Z2'D','R','\002'};
G_L_B char dr = '\122';
static VOID (*dr_1)() = dr_;

static VOID divv_() { oper(Odr,2); }
static char div_0[] = {Z3 'D','I','V','\003'};
G_L_B char div_ = '\122';
static VOID (*div_1)() = divv_;

static VOID addn_() {  oper(Oadd,1); }
static char addn_0[] = {Z4'A','D','D','N','\004'};
G_L_B char addn = '\122';
static VOID (*addn_1)() = addn_;

static VOID subn_() { oper(Osub,1); }
static char subn_0[] = {Z4'S','U','B','N','\004'};
G_L_B char subn = '\122';
static VOID (*subn_1)() = subn_;

static VOID muln_() { oper(Omul,1); }
static char muln_0[] = {Z4'M','U','L','N','\004'};
G_L_B char muln = '\122';
static VOID (*muln_1)() = muln_;

static VOID drn_() { oper(Odr,1); }
static char drn_0[] = {Z3'D','R','N','\003'};
G_L_B char drn = '\122';
static VOID (*drn_1)() = drn_;

static VOID divn_() { oper(Odr,3); }
static char divn_0[] = {Z4'D','I','V','N','\004'};
G_L_B char divn = '\122';
static VOID (*divn_1)() = divn_;

static linkcb *x,*y,*nach,*kon,*Xn,*Yn,*Xk,*Yk;
static int dl,Xdl,Ydl;
static char zn,Xzn,Yzn;

static dajch() {
   zn = '+';
   kon = y->prev;
   if (x == kon) {  /* pustoe chislo */
      dl = 0; return (TRUE);  }
   x = x->next;
   if ( (x->tag == TAGO) &&
      ( (x->info.infoc == '+') || (x->info.infoc == '-') ) ) {
      zn = x->info.infoc;
      x = x->next;
      if (x == y) return (FALSE);  /*  w chisle - lish znak */
   }
   for (; (x->tag == TAGN) && (gcoden(x) == 0l); x = x->next);
   if (x == y) dl=0;  /*  wse cifry - nuli */
   else {
      for (dl=0, nach=x; (x->tag == TAGN);  x = x->next, dl++);
      if (x != y) return (FALSE);  /* ne makrocifra */
   }
   return (TRUE);
}

static dajarg() {
   x = refal.preva->next;
   if (x->tag != TAGLB) return (FALSE);
   y = x->info.codep;
   if ( dajch() ) {
      Xn=nach; Xk=kon; Xzn=zn; Xdl=dl;  }
   else  return (FALSE);
   x=y;
   y=refal.nexta;
   if ( dajch() ) {
      Yn=nach; Yk=kon; Yzn=zn; Ydl=dl; return (TRUE); }
   else  return (FALSE);
}

static void obmen() {
    linkcb *p; int i; char c;

    p=Xn; Xn=Yn; Yn=p;
    p=Xk; Xk=Yk; Yk=p;
    i=Xdl; Xdl=Ydl; Ydl=i;
    c=Xzn; Xzn=Yzn; Yzn=c;
}

static xmy() {  /*  if X < Y then TRUE  ( po modulju) */
   if (Xdl < Ydl) return (TRUE);
   if (Xdl > Ydl) return (FALSE);
   for (x=Xn, y=Yn; (x != Xk->next); x = x->next, y = y->next) {
      if (gcoden(x) < gcoden(y))  return (TRUE);
      if (gcoden(x) > gcoden(y)) return (FALSE);
   }
   return (2);  /* x=y */
}

static void ymn(a,b) long *a,*b; { /* rez.: a - st., b - ml */
   long a1,a2,b1,b2,r,r1,r2,r3,r4;
   if( *a==0l ) { *b=0l; return; }
   if( *b==0l ) { *a=0l; return; }
   a1 = (*a)/HMAX;  b1 = (*b)/HMAX;
   a2 = (*a)%HMAX;  b2 = (*b)%HMAX;
   r = a2*b2;  r4 = r%HMAX;  r3 = r/HMAX;
   r = a1*b2;  r3 += r%HMAX; r2 = r/HMAX;
   r = a2*b1;  r3 += r%HMAX; r2 += r/HMAX;
   r = a1*b1;  r2 += r%HMAX; r1 = r/HMAX;
   *a = r1*HMAX + r2 + r3/HMAX; *b = (r3%HMAX)*HMAX + r4;
}

static void norm(X,dl,S,j)  /*  normaliz. posledov. makrocifr */
   linkcb *X;               /*  X - ukaz. na konec            */
   int dl,j;                /*  j - sdwig (vlevo)             */
   long S; {                /*  S=2**j                        */
   long a,m,peren;  int i;

   peren = 0l;
   m = (MAX-1)/S;  /* maska */
   for (i=0; i<dl; i++) {
      a = gcoden(X);
      pcoden(X,(long)( ((a & m) * S) | peren) );
      peren = a >> (SMAX-j);
      X = X->prev;
   };
}

static oper(o,prn) int o,prn; {
   linkcb *p,*r,*f,*Xt,*Yt;
   long j,peren,knorm;
   int i,n;
   long a,a1,b,b1,c,x1,x2,y1,y2;

   if ( !dajarg() )  {
      refal.upshot = 2; return;  }
   switch (o) {
      case Osub:   /* izmenim znak i skladywaem  */
         if (Yzn == '-') Yzn = '+';
         else            Yzn = '-';
      case Oadd:
         if ( (Xdl==0) && (Ydl==0) ) {
REZ0:       if( prn == 1 ) return;  /* dlja n-operacij */
            x = refal.preva->next;
            x->tag = TAGN; pcoden(x, 0l);
            rftpl(refal.prevr, x->prev, x->next);
            return; }
         if ( Xdl==0 ) {  /*  rezult - wtoroe chislo  */
            Xn=Yn;  Xk=Yk;  Xzn=Yzn;  }
         else {
            if ( Ydl != 0 ) {  /* summiruem  */
               if (Xzn == Yzn) {  /*  skladywaem  */
                  if (Xdl < Ydl)  obmen();
                  /*  X  dlinnee  Y  (ili =)  */
                  Xn = Xn->prev;   /*  pripisywaem  0  */
                  Xn->tag = TAGN;   pcoden(Xn, 0l);
                  peren = 0L;
                  for (x=Xk,y=Yk; (x != Xn->prev); x = x->prev) {
                     if (y != Yn->prev) {
                        pcoden(x, gcoden(x) + gcoden(y) + peren);
                        y = y->prev;  }
                     else
                        pcoden(x, gcoden(x) + peren);
                     if (gcoden(x) >= MAX) {
                        pcoden(x, gcoden(x) - MAX);
                        peren = 1L;  }
                     else
                        peren = 0L;
                  }/*for*/
               }
               else {               /* wychitaem  */
                  if (xmy() == 2)  goto REZ0;
                  if ( xmy() ) obmen();  /*  menjaem x i y  */
                  Xn = Xn->prev;  /*  pripisywaem 0  */
                  Xn->tag = TAGN;  pcoden(Xn, 0l);
                  peren = 0L;
                  for (x=Xk, y=Yk; (x != Xn->prev); x = x->prev)  {
                     j = gcoden(x);
                     if (y !=Yn->prev) {
                        j -= (gcoden(y) + peren);
                        y = y->prev;  }
                     else  j -= peren;
                     if (j < 0L)  {
                        j += MAX;
                        peren = 1;  }
                     else peren = 0L;
                     pcoden(x, j);
                  }/*for*/
               }/*if*/
            }
         }
         break;
      case Omul:
         if ( (Xdl==0) || (Ydl==0) ) goto REZ0;
         if (lrqlk(Xdl+Ydl+1) == 0) {refal.upshot=3; return;}
         p = refal.preva; r = p->next;
         lins(p,Xdl+Ydl+1);   /*  1 zweno dlja znaka  */
         p = p->next;  r = r->prev;
         for (x=p; x != r->next; x = x->next) {
            pcoden(x, 0l); x->tag = TAGN;}  /*  zanulen rezultat  */
         if (Xdl < Ydl) obmen();
         /*  dobawim 0 k X dlja summir. s perenosom  */
         Xn = Xn->prev; Xn->tag = TAGN; pcoden(Xn, 0l);
         f=r;
         for (f=r,y=Yk; y != Yn->prev; y = y->prev, f = f->prev) {
            if ( gcoden(y) != 0l ) {  /* umn. na 1 cifru  */
               peren=0L;
               for (x=Xk,p=f; x != Xn->prev; x = x->prev, p = p->prev) {
                  a = gcoden(x);
                  b = gcoden(y);
                  ymn (&a,&b);    /* rez:a-st, b-ml  */
                  j = gcoden(p);
                  j += b + peren;
                  peren=0L;
                  if (j>= MAX) {j -= MAX; peren++;}
                  peren += a;
                  pcoden(p, j);
               }/*for*/
            }
         }
         if (Xzn != Yzn) Xzn='-'; else Xzn='+';
         Xn=p; Xk=r;
         break;
      case Odr :
         if( Ydl==0 ){
            refal.upshot = 2; return;  }
         if( Xdl==0 ){
            a=0L;  b=0L;  Xzn='+';  Yzn='+';  goto odnc; }
         if( xmy() == 2 ){  /*  rawny  */
            a=0L;  b=1L;  goto odnc; }
         if( xmy() ){  /*  delimoe < delitelja  */
            if( prn/2 == 1 ){  /* DIV, DIVN */
               a=0L;  b=0L;  Xzn='+';  Yzn='+';  goto odnc; }
            if( Xzn=='-' ){
               Xn = Xn->prev; Xn->tag = TAGO; pcoden(Xn, 0l);
                              Xn->info.infoc = '-';}
            Xn = Xn->prev;  Xk = Xk->next;
            Xn->tag = TAGLB;  Xk->tag = TAGRB;
            Xn->info.codep = Xk;  Xk->info.codep = Xn;
            if( prn==0 ){ Xn = Xn->prev; Xn->tag = TAGN; pcoden(Xn, 0l); }
            rftpl( refal.prevr, Xn->prev, Xk->next );
            return;
         }
         /*   delimoe > delitelja */
         if( Xdl == 1 ){ /*  oba po odnoj cifre  */
            a = (gcoden(Xn))%(gcoden(Yn));
            b = (gcoden(Xn))/(gcoden(Yn));
            goto odnc;  }
         /*  delenie mnogih  cifr  */
         if( lrqlk( (Xdl-Ydl)+2 ) == 0 ){  refal.upshot = 3; return; }
            /* t.k. k chastnomu dob. 0 i zweno na znak */
         p = refal.preva;  lins(p, (Xdl-Ydl)+2);
         p = p->next;  /*  dlja znaka  */
         r = p->next;  /*  dlja  perwoj  cifry  */
         nach = r  ;
         Xn = Xn->prev;  Xn->tag = TAGN; pcoden(Xn, 0l); Xdl++;
         for( i=0,x=Xn;  i<Ydl;  i++, x = x->next );
         y = Yn->prev; y->tag = TAGN; pcoden(y, 0l);
         if( Ydl != 0 ){ /* wozmovna normalizacija */
            b = gcoden(Yn);
            for( n=0, knorm=1L; b<(MAX/2); n++, knorm*=2, b*=2 );
            if (n != 0) { 
               norm(Xk,Xdl,knorm,n); norm(Yk,Ydl,knorm,n); }
         }
         do {
            a = gcoden(Xn), a1 = gcoden(Xn->next); 
            b = gcoden(Yn); 
            /*printf("\na=%lx_%lx b=%lx ",a,a1,b);*/
            if( (a==0l) && (a1<b) ) c=0l;
            else {
               if( (a==0l) && (a1>=b) ){ 
                  c=1l;  /*  t.k. b - normalizowano */ 
                  a=a1;
               }
               else {  /* delim a,a1 na b */
                  a =(a * 128) + (a1 >>17);
                  c =      (a/b) << 17;
                  a =((a%b)*128) + ((a1*1024) & 0x7F);
                  c = c + ((a/b)*1024); 
                  a =((a%b)*128) + ((a1*8) & 0x7F); 
                  c = c + ((a/b)*8); 
                  a =((a%b)*8) + (a1 & 7); 
                  c = c + a/b; 
               }
               if( Ydl > 1 ){
                  b1=gcoden(Yn->next);
                  x1=b1; x2=c;
                  ymn(&x1,&x2);
                  y1=a%b; y2=gcoden(Xn->next->next);
                  /*printf("\nBegin: c=%lx ",c);*/
                  /*printf(" x=%lx_%lx (b1*c)",x1,x2);
                  printf(" y=%lx_%lx (o..a2)",y1,y2);*/
                  i=0;
                  while( (x1>y1) || (x1==y1 && x2>y2) ){
                     c--; i=1;
                     x1=b1; x2=c; 
                     ymn(&x1,&x2);
                     y1 += b;
                     /*printf("\nc=%lx ",c);
                     printf(" x=%lx_%lx (b1*c)",x1,x2);
                     printf(" y=%lx_%lx (o..a2)",y1,y2);*/
                  }
                  if( i==1 ) c++; /* na wcjakij sluchaj */
               }
            }
            /* umnovenie  delitelja  na 'c' i wychit. iz X */
            if (c != 0L) {
               Yt = Yk;  Xt = x;  peren = 0L;
               for (; Yt != y->prev; Xt = Xt->prev, Yt = Yt->prev) {
                  b = gcoden(Yt);
                  a=c; ymn(&a, &b);
                  b += peren;
                  peren = (b >> SMAX);
                  b = b & (MAX-1);
                  j = gcoden(Xt);
                  if (j<b) { j+=MAX; peren +=1L; }
                  pcoden(Xt, j-b);
                  peren += a;
               }
               if (peren != 0L) {  /* cifra welika  */
                  /*long jj=0l; */     /* !!! wremenno !!! */
                  do { 
                     /*jj++;*/
                     c -= 1L; Xt = x; Yt = Yk; j = 0L;
                     for (; Yt != y->prev; Xt = Xt->prev, Yt = Yt->prev) {
                        a = gcoden(Xt) + gcoden(Yt) +j; j = 0L;
                        if (a >= MAX) { a -= MAX; j = 1L; }
                        pcoden(Xt,a);
                     }
                     peren -= j;
                  } while (peren != 0L);
                  /*printf("\nc veliko jj=%ld",jj);*/
               }
            }
            pcoden(r, c); r->tag = TAGN; r = r->next;
            x = x->next; Xn = Xn->next;
         } while( x != Xk->next );
         Xn = Xn->prev; r = r->prev;
         if( n != 0 ){ /* denormalizacija ostatka */
            peren=0L;
            for( x=Xn; x != Xk->next; x=x->next ){
               a = gcoden(x);
               b = (a >> n) | (peren << (SMAX-n));
               peren = a & (knorm-1);
               pcoden(x, b);
            }
         }
         /*for(i=0,x=Xn; x != Xk->next; x=x->next,i++)
            printf("\n ostat(%d)=%ld",i,gcoden(x));
         for(i=0,x=nach; x != r->next; x=x->next,i++)
            printf("\n chast(%d)=%ld",i,gcoden(x)); 
         */ 
         for( x=Xn; (x!=Xk->next) && (gcoden(x)==0l); x = x->next );
         x = x->prev;
         if( x != Xk ){
            if( Xzn != Yzn ){ x->tag = TAGO; pcoden(x, 0l);
                              x->info.infoc = '-'; }
            else x = x->next; }
         Xn = x;
         for( x=nach;  gcoden(x) == 0l;  x = x->next );
         if( Xzn == '-' ){ 
            x = x->prev; x->tag = TAGO; pcoden(x, 0l);
            x->info.infoc = '-';}
         if( prn%2 == 0 || Xn!=Xk || gcoden(Xn)!=0l) Xn = Xn->prev;
         Xn->tag = TAGLB; Xn->info.codep = Xk->next;
         Xk = Xk->next;
         Xk->tag = TAGRB; Xk->info.codep = Xn;
         if ( r->next != Xn ) rftpl(r, Xn->prev, Xk->next);
         if( prn/2 == 0 ) rftpl(refal.prevr, x->prev, Xk->next);
         else             rftpl(refal.prevr, x->prev, Xn);
         return;
   }  /* end case */
   /*  wozwratim X */
   /* podawim wed. nuli */
   for( x=Xn; (gcoden(x) == 0l); x = x->next );
   if( prn == 1 && x==Xk && gcoden(x)==0l) return;
   if( Xzn == '-' ){
      x = x->prev; x->tag = TAGO; pcoden(x, 0l);
      x->info.infoc = '-'; }
   /*  perenosim reultat */
   rftpl(refal.prevr, x->prev, Xk->next);
   return;
odnc: /* wywod rezultata delenija, kogda ostatok i chastnoe */
   /* rawno po odnoj makrocifre a - ost., b - chastnoe  */
   if( slins(refal.preva, 2) == 0) { refal.upshot = 3; return;}
   /* in bad case: /1/() - 3 zwena est uje + name */
   x = refal.preva;
   if (Xzn != Yzn) {
      x->tag = TAGO; pcoden(x, 0l);
               x->info.infoc = '-'; x = x->next;}
   if( b!=0l || (prn%2 == 0) ){ x->tag = TAGN; pcoden(x, b); x = x->next; }
   y = x->next;
   if (a != 0L) {
      if (Xzn != '+') {
         y->tag = TAGO; pcoden(y, 0l);
                 y->info.infoc = '-'; y = y->next;}
   }
   if( a!=0l || (prn%2 == 0) ){ y->tag = TAGN; pcoden(y, a); y = y->next; }
   x->tag = TAGLB; y->tag = TAGRB;
   x->info.codep = y; y->info.codep = x;
   if( prn/2 == 0 ) rftpl(refal.prevr, refal.preva->prev, y->next);
   else             rftpl(refal.prevr, refal.preva->prev, x);
   return;
}
      
static VOID nrel_() {
   char c;
   
   if ( !dajarg() )  {
      refal.upshot = 2; return;  }
   refal.preva->tag = TAGO;
   if ((Xdl==0)   && (Ydl==0))   c = '=';
   else {
      if ((Xzn==Yzn) && (xmy()==2)) c = '=';
      else {
         if ( ((Xzn=='-')&&(Yzn=='+')) ||
              ((Xzn=='-')&&(Yzn=='-')&& !xmy()) ||
              ((Xzn=='+')&&(Yzn=='+')&& xmy()) ) c = '<';
         else c = '>';
      }
   }
   pcoden(refal.preva, 0l);
   refal.preva->info.infoc = c;
   rftpl(refal.prevr, refal.preva->prev, refal.nexta);
   return;
}
static char nrel_0[] = {Z4'N','R','E','L','\004'};
G_L_B  char nrel = '\122';
static VOID (*nrel_1)() = nrel_;

