/*
#
# melbo @ https://x-plane.org
#
# compile: cl xroofs.c /D "NODEBUG" /O2
*/

#define VERSION "0.1.4"

#ifdef _WIN32
 #include <windows.h>
 #include <process.h>
 #include "dirent.h"
#else
 #include <libgen.h>
 #include <unistd.h>
 #include <dirent.h>
#endif

#define _CRT_INTERNAL_NONSTDC_NAMES 1
#include <sys/stat.h>
#if !defined(S_ISREG) && defined(S_IFMT) && defined(S_IFREG)
 #define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)
#endif
#if !defined(S_ISDIR) && defined(S_IFMT) && defined(S_IFDIR)
 #define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define MAX_TXT 1024
#define MAX_WRD 1024

#define XSCENERYDIR     "./Custom Scenery"
#define XROOFSDIR       XSCENERYDIR"/Xroofs"
#define XROOFS          XROOFSDIR"/hard"
#define XTEXTURES       XROOFSDIR"/textures"
#define XTEXTURESROOF   XROOFSDIR"/textures_roof"
#define XFACADES        XROOFSDIR"/facades"
#define XLIB            XROOFSDIR"/library.txt"
#define DEFROOFS        "objects/facades"

char *XSCENES[100] = {
   "simHeaven_X-Europe-4-scenery",
   "simHeaven_X-America-4-scenery",
   "simHeaven_X-Europe-6-scenery",
   "simHeaven_X-America-6-scenery",
   "simHeaven_X-World_Europe-6-scenery",
   "simHeaven_X-World_America-6-scenery",
   NULL
};

char *FACADES[100] = {
   "industrial",
   "commerc",
   "bld-high",
   "hotel",
   "house_default_roof_flat",
   NULL
};

char *LIBRARY[200] = {
   "commercial.fac","commerce-01.fac",
   "commercial.fac","commerce-02.fac",
   "commercial.fac","commerce-03.fac",
   "commercial.fac","commerce-04.fac",
   "commercial.fac","commerce-05.fac",
   "commercial.fac","commerce-06.fac",
   "commercial.fac","commerce-07.fac",
   "commercial.fac","commerce-08.fac",
   "commercial-01.fac","commerce-03.fac",
   "commercial-02.fac","commerce-04.fac",
   "bld-high-res.fac","bld-high-res-01.fac",
   "bld-high-res.fac","bld-high-res-02.fac",
   "bld-high-res.fac","bld-high-res-03.fac",
   "bld-high-res.fac","bld-high-res-04.fac",
   "bld-high-res.fac","bld-high-res-05.fac",
   "bld-high-com.fac","bld-high-com-01.fac",
   "bld-high-com.fac","bld-high-com-02.fac",
   "bld-high-res-01.fac","bld-high-res-02.fac",
   "bld-high-res-02.fac","bld-high-res-05.fac",
   "bld-high-com-01.fac","bld-high-com-01.fac",
   "bld-high-com-02.fac","bld-high-com-03.fac",
   "industrial.fac","industrial-01.fac",
   "industrial.fac","industrial-02.fac",
   "industrial.fac","industrial-03.fac",
   "industrial.fac","industrial-04.fac",
   "industrial.fac","industrial-05.fac",
   "industrial.fac","industrial-06.fac",
   "industrial.fac","industrial-07.fac",
   "industrial.fac","industrial-08.fac",
   "industrial-01.fac","industrial-01.fac",
   "industrial-02.fac","industrial-04.fac",
   "hotel.fac","hotel-1.fac",
   "hotel.fac","hotel-2.fac",
   "hotel-01.fac","hotel-1.fac",
   "hotel-02.fac","hotel-2.fac",
   "residential.fac","house_default_roof_flat_gray_0.fac",
   "residential.fac","house_default_roof_flat_gray_1.fac",
   "residential.fac","house_default_roof_flat_gray_2.fac",
   "residential.fac","house_default_roof_flat_gray_3.fac",
   "residential.fac","house_default_roof_flat_gray_4.fac",
   "residential.fac","house_default_roof_flat_gray_5.fac",
   "residential.fac","house_default_roof_flat_green.fac",
   "building.fac","house_default_roof_flat_gray_0.fac",
   "building.fac","house_default_roof_flat_gray_1.fac",
   "building.fac","house_default_roof_flat_gray_2.fac",
   "building.fac","house_default_roof_flat_gray_3.fac",
   "building.fac","house_default_roof_flat_gray_4.fac",
   "building.fac","house_default_roof_flat_gray_5.fac",
   "residential-01.fac","house_default_roof_flat_gray_0.fac",
   "residential-02.fac","house_default_roof_flat_gray_1.fac",
   "residential-03.fac","house_default_roof_flat_gray_2.fac",
   "residential-04.fac","house_default_roof_flat_gray_3.fac",
   "building-01.fac","house_default_roof_flat_gray_4.fac",
   "building-02.fac","house_default_roof_flat_gray_5.fac",
   "building-03.fac","house_default_roof_flat_green.fac",
   NULL
};

char XSCENE[MAX_TXT] = "";
char XTEST[MAX_TXT] = "";
char *words[MAX_WRD];
int hasXE = 0;

/*-----------------------------------------------------------------*/

int shift(char *s) {           /* shift string to the right */
   int i = strlen(s);
   while (i >= 0) {
      s[i+1] = s[i];
      i--;
   }
   s[i] = ' ';
   return(0);
}

/*-----------------------------------------------------------------*/

int strip(char *s) {           /* remove trailing whitespace */
   int i = strlen(s);
   while (i >= 0 && s[i] <= ' ') {
      s[i--] = '\0';
   }
   return(0);
}

/*-----------------------------------------------------------------*/

void join(char *s, char *w[], int n) {
   int i = 0;

   s[0]='\0';
   while (i<n) {
      strcat(s,w[i++]);  
      strcat(s," ");  
   }
}

/*-----------------------------------------------------------------*/

int split(char *s) {
   int w = 0;
   int i = 0;
 
   char tmp[MAX_TXT];
   char *k;

   k = tmp;

   strcpy(tmp,s);

   while(tmp[i] > '\0' && w < MAX_WRD ) {
      if (tmp[i] <= ' ' ) {
         tmp[i++] = '\0';

         if ( words[w] == NULL ) words[w] = (char*) malloc(MAX_TXT);
         strcpy(words[w++], k);
         while(tmp[i] > '\0' && tmp[i] <= ' ' )
            i++;
         k = &tmp[i];
      } else {
         i++;
      }
   }
   if ( words[w] == NULL ) words[w] = (char*) malloc(MAX_TXT);
   strcpy(words[w++], k);
   return(w);
}

/*-----------------------------------------------------------------*/

int isFile(char *s) {
   struct stat sb;

   if (stat(s, &sb) == 0 && S_ISREG(sb.st_mode)) {
      return(1); 
   } else {
      return(0); 
   }
}

/*-----------------------------------------------------------------*/

int isDir(char *s) {
   struct stat sb;

   if (stat(s, &sb) == 0 && S_ISDIR(sb.st_mode)) {
      return(1); 
   } else {
      return(0); 
   }
}

/*-----------------------------------------------------------------*/

int genLibrary() {
   FILE *fp;
   int i;
   char tmp[MAX_TXT];

   if ( (fp = fopen(XLIB,"w")) != NULL ) {
      fputs("A\n800\nLIBRARY\n\n# generated by Xroofs "VERSION"\n\n",fp);
      for ( i=0 ; LIBRARY[i] != NULL ; i=i+2 ) {
         sprintf(tmp,"EXPORT_EXCLUDE  simheaven/facades/%s   hard/%s\n",LIBRARY[i],LIBRARY[i+1]);
         fputs(tmp,fp);
      }
      return(0);
   } else {
      return(-1);
   }
}

/*-----------------------------------------------------------------*/

int genFacFile(char *s) {

   FILE *in,*out;
   char infile[MAX_TXT];
   char outfile[MAX_TXT];
   char buf[MAX_TXT];
  
   int n = 0;
   
   sprintf(infile,"%s/%s/facades/%s",XSCENERYDIR,XSCENE,s);
   sprintf(outfile,"%s/%s",XROOFS,s);
   if ( (in = fopen(infile,"r")) ) {
      if ( (out = fopen(outfile,"w")) ) {
         while ( fgets(buf, MAX_TXT, in) != NULL ) {
            strip(buf);
            if ( buf[0] == '#' ) {
               //printf("comment\n");
            } else {               
               if ( strstr(buf,"LOD 0") ) {
                  sprintf(buf,"LOD 0 9000\nHARD_ROOF concrete\n");
               } else {
                  if ( strstr(buf,"TEXTURE") ) {
                     if ( ! strstr(buf,"../textures") ) {
                        n = split(buf);
                        if( strstr(buf,"TEXTURE_NORMAL") ) {
                           sprintf(buf,"TEXTURE_NORMAL 1.0 ../facades/%s",words[2]);
                        } else {
                           if ( strstr(buf,"TEXTURE_LIT") ) {
                              sprintf(buf,"TEXTURE_LIT ../facades/%s",words[1]);
                           } else {
                              sprintf(buf,"TEXTURE ../facades/%s",words[1]);
                           }
                        }
                     }
                  }
               }
            }
            fputs(buf, out);
            fputs("\n", out);
         }
         fclose(out);
      }
      fclose(in);
   } else {
      printf("cannot open %s\n",infile);
   }
  
   return(0);
}

/*-----------------------------------------------------------------*/

int genFacFiles() {
   DIR *d;
   struct dirent *dir;

   char tmp[MAX_TXT];
   int match,i;

   sprintf(tmp,"%s/%s/facades",XSCENERYDIR,XSCENE);
   //printf("opening dir %s\n",tmp);

   d = opendir(tmp);
   if (d) {
      while ((dir = readdir(d)) != NULL) {
         if( strstr(dir->d_name,".fac") ) {

            // printf("testing file %s \n", dir->d_name);
            match = 0; 

            // see if FAC file needs to be changed
            for ( i=0 ; FACADES[i] != NULL ; i++ ) {
               if ( strstr(dir->d_name,FACADES[i]) ) {
                  match = 1;
                  break;
               }
            }

            if ( match ) {
               /* fac file */
               printf("adding %s \n", dir->d_name);
               genFacFile(dir->d_name);
            }
         }
      }
      closedir(d);
      return(0);
   }
   return(1);
}

/*-----------------------------------------------------------------*/

int main(int argc, char **argv) {

   char tmp[MAX_TXT];
   int i;

   printf("Xroofs - %s\n",VERSION);

   // identify installed SimHeaven scenery packs
   for ( i=0 ; XSCENES[i] != NULL ; i++ ) {
      sprintf(XTEST,"%s/%s",XSCENERYDIR,XSCENES[i]);
      if ( isDir(XTEST) ) {
         strcpy(XSCENE,XSCENES[i]);
      }
   }

   // do we have a SimHeaven package installed ?
   if ( strlen(XSCENE) > 0 ) {
      hasXE = 1;
      strcat(XSCENE,"/objects");
   } else {
      printf("info: no supported SimHeaven scenery found\n");
      exit(-1);
   }
 
#ifndef _WIN32
   strcpy(tmp,dirname(argv[0]));
   printf("changing to %s\n",tmp);
   chdir(tmp);
#endif

   // create "Xroofs" folder
   if ( ! isDir(XROOFSDIR) ) {
      if ( mkdir(XROOFSDIR,0755) ) {
         printf("ERROR: cannot create %s\n",XROOFSDIR);
         return(-1);
      } else {
         printf("%s created\n",XROOFSDIR);
      }
   }

   // create "hard" folder
   if ( ! isDir(XROOFS) ) {
      mkdir(XROOFS,0755);
   } else {
      printf("%s already exists\n",XROOFS);
   }

   // create "textures" link
   if ( ! isFile(XTEXTURES"/house1.dds") ) {
#ifdef _WIN32
      sprintf(tmp,"mklink /j \"%s\" \"%s/%s/textures\"",XTEXTURES,XSCENERYDIR,XSCENE);
      // printf("%s\n",tmp);
      system(tmp);
#else
      sprintf(tmp,"../%s/textures",XSCENE);
      symlink(tmp,XTEXTURES);
#endif
   } else {
      printf("%s already exists\n",XTEXTURES);
   }

   // create "textures_roof" link
   if ( ! isFile(XTEXTURESROOF"/roof.dds") ) {
#ifdef _WIN32
      sprintf(tmp,"mklink /j \"%s\" \"%s/%s/textures_roof\"",XTEXTURESROOF,XSCENERYDIR,XSCENE);
      // printf("%s\n",tmp);
      system(tmp);
#else
      sprintf(tmp,"../%s/textures_roof",XSCENE);
      symlink(tmp,XTEXTURESROOF);
#endif
   } else {
      printf("%s already exists\n",XTEXTURESROOF);
   }

   // create "facades" link
   if ( ! isFile(XFACADES"/ikea.fac") ) {
#ifdef _WIN32
      sprintf(tmp,"mklink /j \"%s\" \"%s/%s/facades\"",XFACADES,XSCENERYDIR,XSCENE);
      //printf("%s\n",tmp); 
      system(tmp);
#else
      sprintf(tmp,"../%s/facades",XSCENE);
      symlink(tmp,XFACADES);
#endif
   } else {
      printf("%s already exists\n",XFACADES);
   }

   genFacFiles();

   genLibrary();

}


/*
    example library.txt

A
800
LIBRARY

EXPORT_EXCLUDE  simheaven/facades/commercial.fac        hard/commerce-01.fac
EXPORT_EXCLUDE  simheaven/facades/commercial.fac        hard/commerce-02.fac


*/
