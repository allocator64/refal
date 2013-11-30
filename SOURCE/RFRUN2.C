/*         #define PR_CMPR     */
#include "refal.def"
/*-------------------------------------*/
/*            file rfrun2.c            */
/*     last edition date: 11.07.86     */
/*-------------------------------------*/
#define NMBL sizeof(char)

struct spcs{
 int  spls;
 char *svpc;
           };
spc(pspcsp,vpc,b)
/* specifier interpreter */
struct spcs *pspcsp;
char *vpc;
linkcb *b;
 {
  int spcwrk;/* work variable */
  int spcpls;/* positiveness feature of specifier element */
  struct spcs *spcsp;/* spcs-pointer */
  char *spcvpc;/* virtual specifier counter */
  char spcopc;/*specifier code */
 spcsp = pspcsp;
 move(LBLL,vpc+1,&spcvpc);/* spcvpc = L */
 spcpls = TRUE;
 goto SPCNXT;
/* return from specifier element if "YES" */
SPCRET:
 if (spcsp == pspcsp) return(spcpls);
 spcsp--;
 spcwrk = spcpls;
 spcpls = spcsp->spls; /*getss (spcpls,spcvpc);*/
 spcvpc = spcsp->svpc;
 if (spcwrk == TRUE) goto SPCRET;
 spcvpc = spcvpc + LBLL;
/* return from specifier element if "NO" */
SPCNXT:
 spcopc = *spcvpc;
 spcvpc = spcvpc + NMBL;
/* switch   */
/* SPCOP    */
 switch(spcopc){
  case 0000: goto SPCCLL;
  case 0001: goto SPCW;
  case 0002: goto SPCNG;
  case 0003: goto SPCNGW;
  case 0004: goto SPCSC;
  case 0005: goto SPCS;
  case 0006: goto SPCB;
  case 0007: goto SPCF;
  case 0010: goto SPCN;
  case 0011: goto SPCR;
  case 0012: goto SPCO;
  case 0013: goto SPCD;
  case 0014: goto SPCL;
               };
                                                     /* SPCCLL(L); */
SPCCLL:
 spcsp->spls = spcpls;
 spcsp->svpc = spcvpc;
 move(LBLL,spcvpc,&spcvpc);
 spcsp++;
 spcpls = TRUE;
 goto SPCNXT;
SPCW:
 goto SPCRET;
SPCNG:
 spcpls = not(spcpls);
 goto SPCNXT;
SPCNGW:
 spcpls = not(spcpls);
 goto SPCRET;
SPCSC:
 if (cmpr(SMBL,spcvpc,&(b->tag)) == 1) goto SPCRET;
 spcvpc = spcvpc + SMBL;
 goto SPCNXT;
SPCS:
 if ((b->tag & 0001) == 0) goto SPCRET;
 goto SPCNXT;
SPCB:
 if ((b->tag & 0001) != 0) goto SPCRET;
 goto SPCNXT;
SPCF:
 if (b->tag == TAGF) goto SPCRET;
 goto SPCNXT;
SPCN:
 if (b->tag == TAGN) goto SPCRET;
 goto SPCNXT;
SPCR:
 if (b->tag == TAGR) goto SPCRET;
 goto SPCNXT;
SPCO:
 if (b->tag == TAGO) goto SPCRET;
 goto SPCNXT;
SPCD:
 if (b->tag != TAGO) goto SPCNXT;
 if (digit(b->info.infoc) == 1) goto SPCRET;
 goto SPCNXT;
SPCL:
 if (b->tag != TAGO) goto SPCNXT;
 if (letter(b->info.infoc) == 1) goto SPCRET;
 goto SPCNXT;
} /*             end      spc          */

letter(s) unsigned char s; {
   if( (s>64  && s<91)  || /* A..Z     */
       (s>96  && s<123) || /* a..z     */
       (s>127 && s<176) || /* €..Ÿ ..¯ */
       (s>223 && s<240) )  /* à..ï     */   return(1); 
   return(0);
}

digit(s) char s; {
   if( s>47 && s<58 )  return(1);
   return(0);
}

not(spcpls)
int spcpls;
{
if (spcpls == TRUE) return(FALSE);
return(TRUE);
}

link(x,y)
linkcb *x,*y;
{
x->next = y;
y->prev = x;  return; 
}

putjs(jsp,ab1,ab2,anel,avpc)
struct jsw{
 linkcb *jsb1;
 linkcb *jsb2;
 int jsnel;
 char *jsvpc;
 } *jsp;
linkcb **ab1;
linkcb **ab2;
int *anel;
char **avpc;
{
jsp->jsb1 = *ab1;
jsp->jsb2 = *ab2;
jsp->jsnel = *anel;
jsp->jsvpc= *avpc; return; 
}

getjs(jsp,ab1,ab2,anel,avpc)
struct _jsw{
 linkcb *jsb1;
 linkcb *jsb2;
 int jsnel;
 char *jsvpc;
} *jsp;
linkcb **ab1;
linkcb **ab2;
int *anel;
char **avpc;
{
*ab1 = jsp->jsb1;
*ab2 = jsp->jsb2;
*anel =jsp->jsnel;
*avpc= jsp->jsvpc;  return; 
}

putts(tsp,ax,ay,az)
struct ts{
 linkcb *ts0,*ts1,*ts2;
} *tsp;
linkcb **ax;
linkcb **ay;
linkcb **az;
{
tsp->ts0 = *ax;
tsp->ts1 = *ay;
tsp->ts2 = *az; return; 
}

getts(tsp,ax,ay,az)
struct _ts{
 linkcb *ts0,*ts1,*ts2;
} *tsp;
linkcb **ax,**ay,**az;
{
*ax = tsp->ts0;
*ay = tsp->ts1;
*az = tsp->ts2; return; 
}

move(n,pf,pt)
int n;
char *pf,*pt;
{
 int i;
 for (i = 1;i <= n;i++){
  *pt = *pf; pt++; pf++;
 }; return; 
}

cmpr(n,p1,p2)
int n;
char *p1,*p2;
{
int i;
for (i = 1;i <= n;i++){
   if (*p1 != *p2) return(0);
   p1++; p2++;
};
return(1);
}

/*:::::::::::::      end of RFRUN2.C         :::::::::::::::::::::::::*/
