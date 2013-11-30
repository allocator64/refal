#include <stdio.h>
#include "refal.def"

extern REFAL refal;
extern char GO();
int rf_init = 1;
linkcb *last_block = 0L;
int curr_size = 0;
char debug=0;
static st s_st;

void dbapp() {}

main () {
   rfinit();
   if (lincrm() ==0) goto LACK;
   if (lcre(&s_st) == 0) goto LACK;
   if (linskd(&s_st,&GO) == 0) goto LACK;
   s_st.stop = 0xFFFFFFFFL;
AGAIN:
   rfrun (&s_st);
   if (s_st.state ==3)
   if (lincrm()) goto AGAIN;
DONE:
ABEND:
EOJ:
LACK:
   return;
}
