#include "refal.def"
#include <stdio.h>
struct lw_ {
   int i1;
   char c2;
   char c3;
};
typedef struct lw_ lw;
static union {
   lw p1;
   long p2;
} pole;  
void pcoden(x,l) linkcb *x; long l; {
   pole.p2 = l;
   x->info.codep = NULL;
   x->info.coden = pole.p1.i1;
   x->zzp = pole.p1.c2;
   return;
}
long gcoden(x) linkcb *x; {
   pole.p1.i1 = x->info.coden;
   pole.p1.c2 = x->zzp;
   pole.p1.c3 = 0;
   return (pole.p2);
}

