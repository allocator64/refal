/*------------------file----PRCOD.C-------------------*/
/*                                                    */
/*    print of the assembly language operators        */
/*                                                    */
/*          Last edition date : 07.07.86              */
/*----------------------------------------------------*/
struct linkti {
   short tag;
   union {
      char infoc;
      long coden;     
      char *codef;
   } info;
};  

/*   print operator code    ( for debugging )   */

prcode(k) char k; {
  switch ( k ) {
    case '\001' : { printf ("\n  sjump"); break; };
    case '\002' : {printf ("\n  fail"); break; };
    case '\003' : {printf ("\n  sb1b2"); break; };
    case '\004' : {printf ("\n  lsc"); break; };
    case '\005' : {printf ("\n  rsc"); break; };
    case '\006' : {printf ("\n  lsco"); break; };
    case '\007' : {printf ("\n  rsco"); break; };
    case '\010' : {printf ("\n  lsd"); break; };
    case '\011' : {printf ("\n  rsd"); break; };
    case '\012' : {printf ("\n  ltxt"); break; };
    case '\013' : {printf ("\n  rtxt"); break; };
    case '\014' : {printf ("\n  lb"); break; };
    case '\015' : {printf ("\n  lby"); break; };
    case '\016' : {printf ("\n  rb"); break; };
    case '\017' : {printf ("\n  rby"); break; };
    case '\020' : {printf ("\n  ls"); break; };
    case '\021' : {printf ("\n  rs"); break; };
    case '\022' : {printf ("\n  lw"); break; };
    case '\023' : {printf ("\n  rw"); break; };
    case '\024' : {printf ("\n  lbnil"); break; };
    case '\025' : {printf ("\n  rbnil"); break; };
    case '\026' : {printf ("\n  lbce"); break; }; 
    case '\027' : {printf ("\n  rbce"); break; };
    case '\030' : {printf ("\n  nil"); break; };
    case '\031' : {printf ("\n  ce"); break; };
    case '\032' : {printf ("\n  led"); break; };
    case '\033' : {printf ("\n  red"); break; };
    case '\034' : {printf ("\n  nnil"); break; };
    case '\035' : {printf ("\n  ple"); break; };
    case '\036' : {printf ("\n  plv"); break; };
    case '\037' : {printf ("\n  le"); break; };
    case '\040' : {printf ("\n  pre"); break; };
    case '\041' : {printf ("\n  prv"); break; };
    case '\042' : {printf ("\n  re"); break; };
    case '\043' : {printf ("\n  plesc"); break; };
    case '\044' : {printf ("\n  plvsc"); break; };
    case '\045' : {printf ("\n  lesc"); break; };
    case '\046' : {printf ("\n  presc"); break; };
    case '\047' : {printf ("\n  prvsc"); break; };
    case '\050' : {printf ("\n  resc"); break; };
    case '\051' : {printf ("\n  lesd"); break; };
    case '\052' : {printf ("\n  resd"); break; };
    case '\053' : {printf ("\n  pleb"); break; };
    case '\054' : {printf ("\n  plvb"); break; };
    case '\055' : {printf ("\n  leb"); break; };
    case '\056' : {printf ("\n  preb"); break; };
    case '\057' : {printf ("\n  prvb"); break; };
    case '\060' : {printf ("\n  reb"); break; };
    case '\061' : {printf ("\n  eoe"); break; };
    case '\062' : {printf ("\n  eoei"); break; };
    case '\063' : {printf ("\n  lsrch"); break; };
    case '\064' : {printf ("\n  rsrch"); break; };
    case '\065' : printf ("\n  wspc"); break;
    case '\066' : printf ("\n  espc"); break;
    case '\067' : printf ("\n  plespc"); break;
    case '\070' : printf ("\n  lespc"); break;
    case '\071' : printf ("\n  prespc"); break;
    case '\072' : printf ("\n  respc"); break;
    case '\073' : printf ("\n  lmax"); break;
    case '\074' : printf ("\n  rmax"); break;
    case '\075' : printf ("\n  eor"); break;
    case '\076' : printf ("\n  ns"); break;
    case '\077' : printf ("\n  nso"); break;
    case '\100' : printf ("\n  text"); break;
    case '\101' : printf ("\n  bl"); break;
    case '\102' : printf ("\n  br"); break;
    case '\103' : printf ("\n  blr"); break;
    case '\104' : printf ("\n  bract"); break;
    case '\105' : printf ("\n  act"); break;
    case '\106' : printf ("\n  muls"); break;
    case '\107' : printf ("\n  mule"); break;
    case '\110' : printf ("\n  tpl"); break;
    case '\111' : printf ("\n  tplm"); break;
    case '\112' : printf ("\n  tple"); break;
    case '\113' : printf ("\n  tplv"); break;
    case '\114' : printf ("\n  tpls"); break;
    case '\115' : printf ("\n  eos"); break;
    case '\116' : printf ("\n  swap"); break;
    case '\117' : printf ("\n  blf"); break; 
    case '\120' : printf ("\n  eossn"); break;
    case '\121' : printf ("\n  setnos"); break;
    default : printf ("\n  unknown operator ");
   }
   return;  
}

/*     print struct "linkti"  (for debugging)          */

prstruct(code) struct linkti *code; {
  printf ("\n tag= %d", code->tag );
  switch ( code->tag ) {
    case 0 : { printf ("\n  sign= %c", code->info.infoc ); break; };
    case 2 : { printf ("\n  addr= %lx", code->info.codef ); break; };
    case 4 : { printf ("\n  numb= %d", code->info.coden ); break; };
    default : printf ("\n unknown tag ");
  } 
  return;  
}
/*---------------- end of file PRCOD.C ------------------*/
