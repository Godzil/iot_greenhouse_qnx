/*
 ---- Modification du profil ----
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#ifdef WIN32
#include "ds.h"
#else
#include <ds.h>
#endif

#include "cgi_func.h"
#include "cgi_theme_gest.h"
#include "test_serre.h"

char cgi_BaseName[256];

int isnumber(char *value)
{
    char Car;
    Car = value[0];
    value++;
    while(Car != 0) /* Tant qu'on est pas a la fin de la chaine */
    {
        if ( isdigit(Car) == 0)
        {
            printf("\n<div align='center'><h3><font color=\"#FF0000\">La valeur entrée n'est pas un nombre !</font></h3></div>\n");
            return 1;
        }
        Car = value[0];
        value++;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    char *argument,*valeur,val[60];
    ds_t ds_desc;
    
    ds_desc = ds_register();

    cgi_start();
    cgi_parseparam();

    cgitheme_init();

    strncpy(cgi_BaseName,argv[0], 256);


    if ( (cgi_getparam("restart")) != NULL )
    {
        //system("ls");
        cgi_printfile("./files/restart.file");
    }
    else if (serre_test_start()==-1)
    {
        cgi_parsefile("./files/error-noserver.file");
    }
    else
    {
        cgi_parsefile("./files/base-start.file");

        argument = cgi_getparam("type");
        valeur = cgi_getparam("valeur");
        if(argument == NULL)
        {
            cgi_parsefile("./files/modprofil.file");
        }
        else if(strcmp(argument,"hygrometrie")==0)
        {
            if ( (valeur != NULL) && (isnumber(valeur) == 0) )
            {
                cgi_parsefile("./files/fin-modseuil.file");
                strcpy(val,"SEUIL_HYGR");
                ds_set(ds_desc,val,valeur,strlen(valeur));
            }
            else
            {
                cgi_parsefile("./files/modprofil.file");
            }
        }
        else if(strcmp(argument,"temperature")==0)
        {
            if ( (valeur != NULL) && (isnumber(valeur) == 0) )
            {
                cgi_parsefile("./files/fin-modseuil.file");
                strcpy(val,"SEUIL_TEMP");
                ds_set(ds_desc,val,valeur,strlen(valeur));
            }
            else
            {
                cgi_parsefile("./files/modprofil.file");
            }
            
        }
        else //if(strcmp(argument,"mod")==0)
        {
            cgi_parsefile("./files/modprofil.file");
        }

        cgi_parsefile("./files/base-end.file");
    }

    cgitheme_close();

    cgi_cleanup();
    return 0;
}
