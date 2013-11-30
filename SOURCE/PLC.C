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
  for ( ; j>0; --j) *(t++) = *(s++); 
 return; /* eg */}
copystr(s1,s2,j)/* copystring from s2 to s1 having length j */
 char *s1;
 char *s2;
 int  j;
 {
 int i;
  for (i = 0 ; i<j; i++)
   *(s1+i) = *(s2+i);  return; /* eg */
 }
cmpstr(n,s1,s2) /* comparison two string . if s1<s2 then return < 0 */
              /* if s1 = s2 return 0. if s1>s2 then return > 0    */
 char *s1;
 char *s2;
 int n;
 {
  int i;
/*printf("\ncmpstr: 1=");for(i=0;i<n;i++)printf("%c",*(s1+i));
printf(" 2=");for(i=0;i<n;i++)printf("%c",*(s2+i));*/
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
  for ( ; (*s1 = *s2) != '\0'; s1++, s2++);  return;  /* eg */
 }
   
/*---------------  end  --  of  --  file  --  PLC.C  -------------*/