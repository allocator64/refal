#include "refal.def"
#include <stdio.h>
extern REFAL refal;

static br_(refpt)
 REFAL *refpt;
 {
  st *ast;
  linkcb *p,*pr,*pl;
  ast = refpt->currst;
  p = refpt->preva;
  while ((p->tag != TAGO) || (p->info.infoc != '=')) {
   p = p->next;
   if (p == refpt->nexta) {refpt->upshot = 2; return;}; /* FAIL */
  }
  if (!lins(ast->store,2)) { refpt->upshot = 3; return;}; /* LACK */
  pl = ast->store->next;
  pr = pl->next;
  pl->info.codep = pr; pl->tag = TAGLB;
  pr->info.codep = pl; pr->tag = TAGRB;
  rftpl(pl,refpt->preva,refpt->nexta);  return; 
 }
static char br_0[] = {Z2'B','R','\002'};
G_L_B char br = '\122';
static (*br_1)() = br_;


static dg_(refpt)
 REFAL *refpt;
 {
  linkcb *lldupl();
  linkcb *pl,*pr,*q;
  st *ast;

  ast = refpt->currst;
  pr = ast->store;
 DG1:
  pl = pr->next;
  if (pl == ast->store) return;
  if (pl->tag != TAGLB) {refpt->upshot = 2; return;}; /* FAIL */
  pr = pl->info.codep;
  if ((q=lldupl(refpt->preva,refpt->nexta,pl)) == 0) goto DG1;
  if ((q->tag != TAGO) || (q->info.infoc != '=')) goto DG1;
  rftpl(refpt->prevr,q,pr);
  pl = pl->prev; pr = pr->next;
  rfdel(pl,pr);  return;
 }
static char dg_0[] = {Z2'D','G','\002'};
G_L_B char dg = '\122';
static (*dg_1)() = dg_;



static dgall_(refpt)
 REFAL *refpt;
 {
  st *ast;

  ast = refpt->currst;
  if(refpt->preva->next != refpt->nexta) refpt->upshot = 2; /* FAIL */
  else
    rftpl(refpt->prevr,ast->store,ast->store);  return; 
 }
static char dgal_0[] = {Z5'D','G','A','L','L','\005'};
G_L_B char dgall = '\122';
static (*dgal_1)() = dgall_;


static gtr_(refpt)
 REFAL *refpt;
 {
  linkcb *p,*r;
  int emp;

  emp = 1;
  if(!enter(refpt,emp,&p,&r)) {refpt->upshot = 2; return;}; /* FAIL */
#ifdef PRINT_F
 rfpexm("gtr:",p,p);
#endif
  rftpl(refpt->prevr,p,p); return;
 }
static char gtr_0[] = { Z3'G','T','R','\003'};
G_L_B char gtr ='\122';
static (*gtr_1)() = gtr_;


static rdr_(refpt)
 REFAL *refpt;
 {
  linkcb *p,*r;
  int emp;

  emp = 1;
  if(!enter(refpt,emp,&p,&r)) {refpt->upshot = 2; return;}; /* FAIL */
  if(!lcopy(refpt->prevr,p,p)) {refpt->upshot = 3; return;}; /* LACK */
 return;   }
static char rdr_0[] = {Z3'R','D','R','\003'};
G_L_B char rdr = '\122';
static (*rdr_1)() = rdr_;


static ptr_(refpt)
 REFAL *refpt;
 {
  linkcb *p,*r,*q;
  int emp;

  emp = 0;
  if(!enter(refpt,emp,&p,&r)) {refpt->upshot = 2; return;}; /* FAIL */
  q = p->prev;
  rftpl(q,r,refpt->nexta);
#ifdef PRINT_F
 rfpexm("ptr:",p,p);
#endif
 return;  }
static char ptr_0[] = {Z3'P','T','R','\003'};
G_L_B char ptr = '\122';    
static (*ptr_1)() = ptr_;


static wtr_(refpt)
 REFAL *refpt;
 {
  linkcb *p,*r;
  int emp;

  emp = 0;
  if(!enter(refpt,emp,&p,&r)) {refpt->upshot = 2; return;}; /* FAIL */
  rfdel(p,p);
  rftpl(p,r,refpt->nexta);
#ifdef PRINT_F
 rfpex("wtr:",p,p);
#endif
 return; }
static char wtr_0[] = {Z3'W','T','R','\003'};
G_L_B char wtr = '\122';
static (*wtr_1)() = wtr_;


static swr_(refpt)
 REFAL *refpt;
 {
  linkcb *p,*r;
  int emp;

  emp = 0;
  if(!enter(refpt,emp,&p,&r)) {refpt->upshot = 2; return;}; /* FAIL */
  rftpl(refpt->prevr,p,p);
  rftpl(p,r,refpt->nexta); return; 
 }
static char swr_0[] = {Z3'S','W','R','\003'};
G_L_B char swr = '\122';
static (*swr_1)() = swr_;


#define N_SWAP 0116
static enter(refpt,emp,pp,rp)
 REFAL *refpt;
 int emp;
 linkcb **pp,**rp;
 {
  linkcb *p,*r;
  char *q;

  r = refpt->preva->next;
  if(r == refpt->nexta) return(0);
  if(emp && (r->next != refpt->nexta)) return(0);
  if(r->tag == TAGR) p = r->info.codep;
  else if (r->tag == TAGF) {
        q = r->info.codef;
        if( *q != N_SWAP ) return(0);
        q++;
        p = (linkcb *) q;  /*eg*/
        if(p->prev == NULL) {   
         p->prev = p->next = p;
         p->info.codep = refpt->svar;
         p->tag = 0;
         refpt->svar = p;
        }
       }
       else return (0);
  *pp = p;
  *rp = r;
  return(1);
 }


 static rp_(refpt)
 REFAL *refpt;
 {
  st *ast;
  linkcb *lldupl();    
  linkcb *p,*pr,*pl,*q;

  ast = refpt->currst;
  p = refpt->preva;
  while ((p->tag != TAGO) || (p->info.infoc != '=')) {
   p = p->next;
   if (p == refpt->nexta) goto FAIL;
  };
  pr = ast->store;
 RP1:
  pl = pr->next;
  if (pl == ast->store) {
   if (!lins(ast->store,2)) {refpt->upshot = 3; return;}; /* LACK */
   pl = ast->store->next;
   pr = pl->next;
   pl->info.codep = pr; pl->tag = TAGLB;
   pr->info.codep = pl; pr->tag = TAGRB;
   rftpl(pl,refpt->preva,refpt->nexta);
  }
  else {
   if(pl->tag != TAGLB) goto FAIL;
   pr = pl->info.codep;
   if((q = lldupl(refpt->preva,p,pl)) == 0) goto RP1;
   if((q->tag != TAGO) || (q->info.infoc != '=')) goto RP1;
   rfdel(q,pr);
   rftpl(q,p,refpt->nexta);
  }
  return;
 FAIL:
  refpt->upshot = 2; return; 
 }
static char rp_0[] = {Z2'R','P','\002'};
G_L_B char rp = '\122';
static (*rp_1)() = rp_;


static cp_(refpt)
 REFAL *refpt;
 {
  st *ast;
  linkcb *pl,*pr,*q;
  linkcb *lldupl();

  ast = refpt->currst;
  pr = ast->store;
 CP1:
  pl = pr->next;
  if(pl == ast->store) return;
  if(pl->tag != TAGLB) {refpt->upshot = 2; return;}; /* FAIL */
  pr = pl->info.codep;
  if((q = lldupl(refpt->preva,refpt->nexta,pl)) == 0) goto CP1;
  if((q->tag != TAGO) || (q->info.infoc != '=')) goto CP1;
  if(!lcopy(refpt->prevr,q,pr)) refpt->upshot = 3;   /* LACK */
 return;  }
static char cp_0[] = {Z2'C','P','\002'};
G_L_B char cp = '\122';
static (*cp_1)() = cp_;


static new_(refpt)
 REFAL *refpt;
 {
  linkcb *p,*r;

  if(!lins(refpt->prevr,1)) {refpt->upshot = 3; return;}; /* LACK */
  r = refpt->prevr->next;
  r->info.codep = refpt->preva; r->tag = TAGR;
#ifdef PRINT_NEW
 rfpexm("new before:",refpt->preva,refpt->nexta);
#endif
  p = refpt->nexta->prev;
  p->next = refpt->preva;
  refpt->preva->prev = p;
  refpt->nextr->next = refpt->nexta;
  refpt->nexta->prev = refpt->nextr;
#ifdef PRINT_NEW
 rfpexm("new after:",refpt->nextr->prev,refpt->nexta->next);
#endif
  refpt->preva->info.codep = refpt->dvar;
  refpt->preva->tag = 0;
  refpt->dvar = refpt->preva;
#ifdef PRINT_F
rfpexm(" new:",refpt->preva,refpt->preva);
#endif
 return;  }
static char new_0[] = {Z3'N','E','W','\003'};
G_L_B char new = '\122';
static (*new_1)() = new_;


