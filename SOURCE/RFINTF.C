#include "refal.def"
#include <stdio.h>
#define NULL 0L

extern REFAL refal;
extern linkcb *last_block;
extern int rf_init;

#ifdef PDP
void pcoden();
long gcoden();
#endif

rfabe (amsg) char *amsg;{
  printf ("\n *** refal-abend *** %s",amsg);
  exit(0); return; 
}

char convert(cm) char cm; {
 int j;
   j = (int) cm;
   if ((j>96) && (j<123)) return ( cm - '\40' );
   else return ( cm );  /* perewod  a..z w A..Z */
/*#ifdef IBM_PC
   if ((j>-97) && (j<-64))  return ( cm - '\40' );
 #else
   if ((j>-33) && (j<0))    return ( cm - '\40' );
 #endif*/
}

rfcanc (ast)
 st *ast;
 {
  linkcb *flhead1,*view1,*store1;

  if (rf_init) rfinit ();
  if (!lexist(ast))
         rfabe (" rfcanc: process doesn't exist");
  if (ast->state == 4)
        rfabe (" rfcanc: process is in job yet");
  ast->stprev->stnext = ast->stnext;
  ast->stnext->stprev = ast->stprev;
  flhead1 = refal.flhead->prev;
  view1 = ast->view->prev;
  store1 = ast->store->prev;
  flhead1->next = ast->view;
  ast->view->prev = flhead1;
  view1->next = ast->store;
  ast->store->prev = view1;
  store1->next = refal.flhead;
  refal.flhead->prev = store1; return; 
 }


/*    delete part of list and add it to free memory list */
rfdel (p,q)
 linkcb *p,*q;
 {
  linkcb *p1,*q1,*r;

  p1 = p->next;
  if (p1 == q) return;
  q1 = q->prev;
  r = refal.flhead->prev;
  p->next = q;
  q->prev = p;
  q1->next = refal.flhead;
  refal.flhead->prev = q1;
  r->next = p1;
  p1->prev = r; return; 
 }


#define ON_10000_STEPS
rfexec (func)
 char *func;
 {
  void rftermm();
  st s_st;
  if (rf_init == 1) rfinit();
  if (lincrm() ==0) goto LACK;
  if (lcre(&s_st) == 0) goto LACK;
  if (linskd(&s_st,func) == 0) goto LACK;
#ifdef ON_10000_STEPS
  s_st.stop = 1000000L;
#else
  s_st.stop = 0xFFFFFFFFL;
#endif
 AGAIN:
  rfrun (&s_st);
  if (s_st.state ==3)
   if (lincrm()) goto AGAIN;
  if (s_st.dot !=NULL) goto ABEND;
 DONE:
  printf ("\n concretization has been executed");
  goto EOJ;
 ABEND:
  switch (s_st.state) {
   case 1:
     printf ("\n stop on step number");
     break;
   case 2:
     printf ("\n recognition impossible");
     break;
   case 3:
     printf ("\n free memory exhausted");
  }
 EOJ:
  printf ("\n total steps number = %ld",s_st.step);
  if (s_st.view->next != s_st.view) {
    printf ("\n view field:");
    rfpexm (  "            ",s_st.view,s_st.view);
   };
  if (s_st.store->next !=s_st.store){
   printf ("\n burried:");
   rfpexm (  "         ",s_st.store,s_st.store);
  };
 rfcanc (&s_st);
 rftermm();
 return;
LACK:
 printf ("\n no ehough memory for initialization");
 rftermm();
 return;

}




/*    rfinit function      */
linkcb hd ;
REFAL refal;
rfinit ()
 { REFAL *p;
   linkcb *phd;
   rf_init = 0;
   p = &refal;
   p->crprev = (st *) &refal;  
   p->crnext = (st *) &refal; 
   p->upshot = 1;
   p->currst = NULL;
   p->svar =  NULL;
   p->dvar = NULL;
   p->flhead = &hd;
   phd = &hd;
   phd->prev = phd;
   phd->next = phd;
   phd->tag = 0;
   pcoden(phd,0L);
   p->nostm = 0;
   p->stmnmb = 0;
   p->tmmode = 0;
   p->tmintv = 0; return; 
 }


  
    /*  RFLIST -  procedure  */
rflist(par,n)
 linkcb *par;
 int n;
 {
  linkcb *p, *q;
  int k;
  if (rf_init != 0) rfinit();
  q = par;
  p = refal.flhead->prev;
  for (k = 1; k<=n; k++)
   {   p->next = q;
       q->prev = p;
       q->tag = 0;
       q->info.codep = NULL;
       p = q;
       q++;
   }
  p->next = refal.flhead;
  refal.flhead->prev = p; return; 
 }


rfpex(pt,pr,pn) char *pt; linkcb *pr,*pn; {
  char *f;
  unsigned char c;
  int l,k;
  linkcb *pr1;

  printf ("\n%s",pt);
  while (pr != pn->prev) {
     pr1 = pr;
     pr = pr->next;
     if (pr1 != pr->prev) rfabe("rfpex: list structure violation");
     if (pr->tag == TAGO) putchar (pr->info.infoc);
     else if (pr->tag == TAGK) putchar ('k');
     else if (pr->tag == TAGD) putchar ('.');
     else if (pr->tag == TAGLB) putchar('(');
     else if (pr->tag == TAGRB) putchar(')');
     else if (pr->tag == TAGN) printf ("'%ld'",gcoden(pr));
     else if (pr->tag == TAGF) {
        putchar ('\'');
        f = pr->info.codef - 1;
        c = *f;
        l = (int)c;
        f = f - l;
        for (k=1; k<=l; k++,f++) putchar (convert(*f));
        putchar ('\'');
     }
     else if (pr->tag == TAGR) printf ("'%%%lx'",pr->info.codep); 
     else if ((pr->tag & 0001) != 0)  rfabe("rfpex: unknown bracket type ");
     else printf ("'%x,%lx'",pr->tag,pr->info.codep);
  }  return; 
}

     #define CHECK_PREV
rfpexm(pt,pr,pn) char *pt; linkcb *pr,*pn; {
  char *f;
  unsigned char c;
  int l,k,fr;
  linkcb *pr1;

  fr = 0;
  printf ("\n%s",pt);
  while (pr != pn->prev) {
     pr1 = pr;
     pr = pr->next;
#ifdef CHECK_PREV
     if(pr->prev != pr1) rfabe("rfpexm: list structure violation");
#endif
     if (pr->tag == TAGO) {
        if (fr == 0) {
           fr = 1;
           putchar ('\'');
        };
        putchar (pr->info.infoc);
     }
     else {
        if (fr == 1) {
           fr = 0;
           putchar ('\'');
        };
        if (pr->tag == TAGK) putchar ('k');
        else if (pr->tag == TAGD) putchar ('.');
        else if (pr->tag == TAGLB) putchar('(');
        else if (pr->tag == TAGRB) putchar(')');
        else if (pr->tag == TAGN) printf ("/%ld/",gcoden(pr));
        else if (pr->tag == TAGF) {
           putchar ('/');
           f = pr->info.codef - 1;
           c = *f;
           l = (int)c;
           f -= l;
           for (k=1; k<=l; k++,f++) putchar (convert(*f));
           putchar ('/');
        }
        else if (pr->tag == TAGR) printf ("/%%%lx/",pr->info.codep); 
        else if ((pr->tag & 0001 ) != 0) rfabe("rfpexm: unknown bracket type ");
        else printf ("/<%x>,%lx/",pr->tag,pr->info.codep);
     }  
  }
  if (fr == 1) putchar('\''); 
  return; 
}

void rftermm () {
  linkcb *new_block;

  while (last_block != NULL) {
   new_block = last_block;
   last_block = new_block->prev;
/*   printf("\n  do free rftermm=%lx",new_block);*/
   free(new_block);
  }
/*  return;*/
}


rftpl(r,p,q)
 linkcb *p, *r, *q;
{
 linkcb *r1, *q1, *p1;
 
 p1 = p->next;
 if ( p1 == q ) return;
 r1 = r->next;
 q1 = q->prev;
 p->next = q;
 q->prev = p;
 q1->next = r1;
 r1->prev = q1;
 r->next = p1;
 p1->prev = r;  return; 
}

/*  copy expression and add it to nessecary place  */
lcopy (r,p,q)
 linkcb *r,*p,*q;
 {
 linkcb *r1,*f,*f0,*f1,*lastb = NULL;

 f = refal.flhead;
 f0 =  p->next;
 while (f0 != q) {
  f = f->next;
  if (f == refal.flhead) return(0);
  switch (f0->tag) {

   case TAGLB:
    f->info.codep = lastb;
    lastb = f;
    break;

   case TAGRB:
    f->info.codep = lastb;
    f->tag = TAGRB;
    f1 = lastb->info.codep;
    lastb->info.codep = f;
    lastb->tag = TAGLB;
    lastb = f1;
    break;

   default:
    f->tag = f0->tag;
    pcoden(f,gcoden(f0));
  }
  f0 = f0->next;
 }
 if (refal.flhead == f) return(1);
 f0 = refal.flhead->next;
 f1 = f->next;
 refal.flhead->next = f1;
 f1->prev = refal.flhead;
 r1 = r->next;
 f->next = r1;
 r1->prev = f;
 r->next = f0;
 f0->prev = r;
 return(1);
}



lexist (ast)
 st *ast;
 {
 REFAL *p;

  p = &refal;
  do {
   p =(REFAL *) (p->crnext); 
   if (p == (REFAL *) ast) return (1);
  } while (p != &refal);
  return(0);
 }
 
 
lcre (ast)
 st *ast;
 {

  st *q;
  linkcb *flhead1;
 if (rf_init ==1) rfinit();
  if (lexist(ast) == 1)
        rfabe ("rfabe: process already exists");
  ast->view = refal.flhead->next;
  if (ast->view == refal.flhead) return(0);
  ast->store = ast->view->next;
  if (ast->store == refal.flhead) return (0);
  flhead1 = ast->store->next;
  refal.flhead->next = flhead1;
  flhead1->prev = refal.flhead;
  (ast->view->next) = (ast->view);
  (ast->view->prev) = (ast->view);
  (ast->store->next) =(ast->store);
  (ast->store->prev) = (ast->store);
  q = refal.crprev;
  ast->stnext = (st *) &refal;  
  refal.crprev = ast;
  q->stnext = ast;
  ast->stprev = q;
  ast->state = 1;
  ast->dot = NULL;
  ast->step = 0L;
  ast->stop = -1L;
  return (1);
 }



/*  #define PR_COUNT  */
lgcl()
 {
  st *p;
  int was_coll;
  linkcb *pzero;
  linkcb *q,*r,*flhead1,*p1;
  linkcb hdvar, *hd;
#ifdef PR_COUNT
 int count;
 count = 0;
#endif

  hd = &hdvar;
  if(refal.dvar == NULL) return(0);
/* mark boxes achieved from view field & burriage */
  was_coll = 0;
  pzero = NULL;
  p = (st *) refal.crnext; 
#ifdef PR_COUNT
  printf("\n ------ before while (into lgcl) ----");
#endif
  while (p != (st *) &refal) {   
#ifdef PR_COUNT
     rfpexm("lgcl: ",p->view,p->view);
#endif
     mark(p->view);
#ifdef PR_COUNT
  printf("\n ------ between mark (into lgcl) ----");
#endif
     mark(p->store);
     p = p->stnext;
  }
#ifdef PR_COUNT
  printf("\n ------ after while (into lgcl) ----");
#endif
/* mark boxes achieved from static boxes    */
  if (refal.svar != NULL) {
     r = refal.svar;
     do {
         mark(r);
         r = r->info.codep;
        } while(r != pzero);
                              /*   remove garbage   */
      hd->info.codep = refal.dvar;
      p1 = hd;
      q = refal.dvar;
      do {
          if(q->tag != 0) {   /* box isn't remaved   */
            q->tag = 0;
            p1 = q;
          }
          else {                        /* remove box     */
            was_coll = 1;
            p1->info.codep = q->info.codep;
            p1->tag = q->tag;
            r = q->prev;
            flhead1 = refal.flhead->next;
            r->next = flhead1;
            flhead1->prev = r;
            refal.flhead->next = q;
            q->prev = refal.flhead;
#ifdef PR_COUNT
  count++;
#endif
            }
          q = p1->info.codep;
        } while (q != pzero);
   if (hd->info.codep == pzero)
      refal.dvar = NULL;
   else
       refal.dvar = hd->info.codep;
    /*   return(was_coll);*/    
#ifdef PR_COUNT
 printf ("\n %d boxes removed ", count);
#endif
   }  return (was_coll);  
 }



static mark(root)
 linkcb *root;
 {
  linkcb *h,*p,*q,*r;

      h = p = root;
MRK:
      if(p->next == h) goto UP;
#ifdef PR_COUNT
  printf("\n        ------ MRK: (into lgcl) ----");
#endif
      p = p->next;
      if (p->tag != TAGR) goto MRK;
#ifdef PR_COUNT
  printf("\n                    ------ p->tag == TAGR ----");
#endif
      q = p->info.codep;
      if (q->tag != 0) goto MRK;
#ifdef PR_COUNT
  printf("\n                    ------ q->tag == 0 ----");
#endif
#ifdef PR_COUNT
     rfpexm("MARK: ",q,q);
#endif
      q->tag = 0xFFFF;
      p->info.codep = h;
      q->prev = p;
      h = p = q;
      goto MRK;
UP:   if (h == root) return;
#ifdef PR_COUNT
  printf("\n        ------ UP: (into lgcl) ----");
#endif
      q = h->prev;
      h->prev = p;
      r = h;
      h = q->info.codep;
      q->info.codep = r;
      q->tag = TAGR;
      p = q;
      goto MRK;
 }

/*#define PR_SIZE*/
/*#define PR_ALL*/
extern int curr_size;
linkcb *calloc();

lincrm() {
 linkcb *first_free, *p;
 linkcb *new_block; 
 int was_coll, n ;
   if (last_block != NULL) {
      first_free = refal.flhead->next;
      was_coll = lgcl();
      if (was_coll == 1) {
         p = refal.flhead->next;
         n = 0;
         while (( p != first_free) && ( n != 120)) {
            n++;
            p = p->next;
         }
         if (n == 120) return (TRUE);
      }
   }
   new_block = calloc(1001,sizeof(linkcb));  /* kras 06.12.88 */
#ifdef mdebug
printf("\n lincrm: n=%d after new_block=%lx",n,new_block);
#endif
   if (new_block == NULL) return (FALSE);
   new_block->prev = last_block;
   last_block = new_block;
   curr_size = curr_size + 1000;   /* kras 06.12.88 */
#ifdef PR_SIZE
printf ("\n current size %d items ", curr_size);
#endif
   rflist (new_block + 1, 1000);   /* kras 06.12.88 */
#ifdef PR_ALL
printf("\n lincrm: return current size = %d",curr_size);
#endif
   return (TRUE);
}

linskd (ast,f)  st *ast;  char *f; {
 linkcb *p,*q,*r;
   if (lexist(ast) == 0)
       rfabe ("linskd: process doesn't exist still");
   if (ast->dot!=NULL)
       rfabe ("linskd: there are 'k'-signes in view field");
   if (lrqlk(3) == 0) return (0);
   lins (ast->view,3);
   p= ast->view->next;
   r = p->next;
   q = ast->view->prev;
   p->tag = TAGK;
   q->tag = TAGD;
   q->info.codep = p;
   r->tag = TAGF;
   r->info.codep = (linkcb *) f;  
   ast->dot = q;
   return (1);
}

lins (p,l) linkcb *p; int l; {
  int n;
  linkcb *p1,*q,*q1,*r;

  if (l<1) return (1);
  q1 = refal.flhead;
  for (n=1; n<=l; n++) {
    q1 = q1->next;
    if (q1 == refal.flhead) return (0);
    q1->tag = TAGO;
    q1->info.codep = NULL;
  }
  r = q1->next;
  q = refal.flhead->next;
  refal.flhead->next = r;
  r->prev = refal.flhead;
  p1 = p->next;
  q1->next = p1;
  p1->prev = q1;
  p->next = q;
  q->prev = p;
  return (1);
}

slins (p,k) linkcb *p; int k; {
M: if (lrqlk(k)==0)  {
      if(lincrm()==0) {
         refal.upshot=3; return(0); }
      else goto M; }
   lins(p,k);
   return(1);
}

linkcb *lldupl(p,q,u) linkcb *p,*q,*u; {
 linkcb *x,*y;
   x = p->next;
   y = u->next;
   while ( x != q) {
      if (x->tag != y->tag) return(0);
      if (x->info.codef != y->info.codef)
         if ((x->tag != TAGLB) && (x->tag != TAGRB)) return(0);
      x = x->next;
      y = y->next;
   }
   return (y);
}

/*  check a number of items in free items list */
lrqlk (l) int l; {
 linkcb *p;
 int n;
   p = refal.flhead;
   for (n = 1; n<=l; n++) {
      p = p->next;
      if ( p == refal.flhead) return (0);
   }
   return (1);
}
/*--------------------  file  --  PLC.C --------------------*/
/*                                                          */
/*     standard PL/I-functions implemented on C-language    */
/*                                                          */
/*                Last edition date : 10.02.86              */
/*                                                          */
/*----------------------------------------------------------*/
length(s)/* length of varying string s */
char *s;
 {
  int n;
  for (n = 0; *s != '\0'; s++)
   n++;
  return(n);
 }
index(s,l,t,m)/* first left insertion t into  s */
              /* if not: -1                     */
 char *s;
 char *t;
 int l,m;
 {
  int i;
  int j;
  int k;
  for (i = 0; (i < l) && (*(s+i) != '\0') ; i++)
    for ( j = i, k = 0 ; (j < l) && (*(s+j) != '\0') ; j++ )
      {
       if (*(s+j) != *(t+k)) break;
       k++;
       if ( k >= m ) return(i);
      };
  return(-1);
 }
substr(s,i,j,t) /* t - substring of s,begining on i symbol with length j*/
 char *s;
 char *t;
 int  i;
 int  j;
 {
  --i;
  for ( ; i>0; --i) s++;
  for ( ; j>0; --j) *(t++) = *(s++); return; 
 }
copystr(s1,s2,j)/* copystring from s2 to s1 having length j */
 char *s1;
 char *s2;
 int  j;
 {
 int i;
  for (i = 0 ; i<j; i++)
   *(s1+i) = *(s2+i);  return; 
 }
cmpstr(n,s1,s2) /* comparison two string . if s1<s2 then return < 0 */
              /* if s1 = s2 return 0. if s1>s2 then return > 0    */
 char *s1;
 char *s2;
 int n;
 {
  int i;
  for (i=0; i<n; i++, s1++, s2++)
   if (*s1 != *s2)
    return(*s1 - *s2);
  return(0);
 }
concat(s1,s2)   /* concatenation of s1 and s2. It puts result into s1 */
 char *s1;
 char *s2;
 {
  for ( ; *s1 != '\0'; s1++);
  for ( ; (*s1 = *s2) != '\0'; s1++, s2++); return; 
 }

