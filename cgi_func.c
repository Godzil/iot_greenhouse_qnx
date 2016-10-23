#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN32
#include "ds.h"
#else
#include <ds.h>
#endif

#include "chtbl.h"
#include "hashpjw.h"

#include "cgi_theme_gest.h"

#define FALSE 0
#define TRUE 1
//#define DEBUG

extern char cgi_BaseName[100];

int cgi_started = FALSE;
int cgi_paramparsed = FALSE;

typedef struct cgiparam_
{
    char nom[25];
    char value[100];
} cgiparam;

CHTbl Parametres;


void GererParam(char *param);


int TestEgalite(const void *val1, const void *val2)
{
    cgiparam *val_1 = (cgiparam *)val1 , *val_2 = (cgiparam *)val2;
    int a = strcmp(val_1->nom,val_2->nom);

    if ( a == 0)
        a = 1;
    else 
        a = 0;

    return a;
}
int hashage(const void *cle)
{
    return hashpjw((void*) ( ((cgiparam*) cle)->nom));
}

void cgi_start()
{

    if ( cgi_started == FALSE )
        printf("Content-type: text/html\nCache-Control: no-cache\nPragma: no-cache\n\n");

    cgi_started = TRUE;

}

int cgi_printfile(char *filename)
{
    FILE *fp;
    char strline[255];
    if( (fp = fopen(filename,"rt") ) == NULL )
        return -1;

    do
    {
        fgets(strline,255,fp);
        printf("%s",strline);
    }
    while(!feof(fp));
    fclose(fp);
    return 0;
}

int cgi_parseparam()
{
#ifdef DEBUG
    char params[1001];
#else
    char *params;
#endif

    char temp[126];
    int OuQu_onest;

    int LenParams,index,LenParam,index2,index3;

    cgiparam *Param;

    if ( cgi_paramparsed == TRUE )
        return 1;

    chtbl_init(&Parametres,1000,hashage,TestEgalite,free);

#ifndef DEBUG
    params = getenv("QUERY_STRING");
#else
    strcpy(params,"erreur=consulter");
#endif

    if (params == NULL)
            return -1;
    

    LenParams = strlen(params);

    memset(temp,0,126);
    LenParam=0;

    for(index=0;index<(LenParams+1);index++)
    {
        if ( (params[index] == '&') || (params[index] == 0) )
        {
            if ( (Param = (cgiparam*) malloc(sizeof(cgiparam))) == NULL)
                return -1;
            memset(Param,0,sizeof(cgiparam));

            OuQu_onest = 0;
            index3=0;
            for(index2=0;index2<LenParam;index2++)
            {
                if(temp[index2] == '=')
                {
                    OuQu_onest = 1;
                    index3 = 0;
                }
                else
                {
                    if (OuQu_onest == 0)
                    {
                        Param->nom[index3++] = temp[index2];
                    }
                    else
                    {
                        Param->value[index3++] = temp[index2];
                    }
                }
            }
            chtbl_insert(&Parametres,(void*)Param);
            memset(temp,0,126);
            LenParam = 0;
        }
        else
        {
            temp[LenParam++] = params[index];
        }
    }
    cgi_paramparsed = TRUE;
    return 0;
}

char *cgi_getparam(char *ParamName)
{
    cgiparam Temp,*Param;

    char *Retour = NULL;

    if ( cgi_paramparsed == FALSE )
        return NULL;

    /* Initialisation de la structure */

    memset(&Temp,0,sizeof(cgiparam));
    
    /* On cree la structure qui va etre utilisé pour chercher dans la table */

    strcpy(Temp.nom,ParamName);

    /* On fait pointer Param vers la ou il faut */

    Param = &Temp;

    if ( chtbl_lookup(&Parametres,(void *)&Param) == 0)
        Retour = Param->value;

    return Retour;
}

int cgi_parsefile(char *filename)
{
    /* 
    Définition Attention ya une var :
    <!--#-->
    A mettre en début de ligne
    

    Df Var

    <!--TEST-->
    ou test est le nom de la var
    */
    FILE *fp;
    char strline[500];
    char strtest[61];

    int index,index2,LenLine;

    if( (fp = fopen(filename,"rt") ) == NULL )
        return -1;

    do
    {
        fgets(strline,500,fp);
        memset(strtest,0,61);
        memcpy(strtest,strline,8);
        if(strcmp(strtest,"<!--#-->") == 0)
        { /* On a une variable dans la ligne, on découpe... */
         LenLine = strlen(strline);
         for(index=8;index<LenLine;index++)
         {
            memset(strtest,0,9);
            memcpy(strtest,strline + index,4);
            if (strcmp(strtest,"<!--") == 0)
            {
                index = index + 4;
                index2 = 0;
                while(strline[ index + index2 ] != '-')
                    index2++;
                memcpy(strtest,strline + index,index2);
                GererParam(strtest);
                index = index + index2 + 2;
            }
            else
            {
                printf("%c",strline[index]);
            }
         }
        }
        else
        { /* Sinon on affiche normalement */
            printf("%s",strline);
        }
    }
    while(!feof(fp));
    fclose(fp);
    return 0;
}

void cgi_cleanup()
{
    if ( cgi_paramparsed == TRUE )
        chtbl_destroy(&Parametres);

    cgi_paramparsed = FALSE;
}


/*
Cette fonction a pour but de reagir en fonction des paramètres passé en Include dans le fichier qui est lu
( par ex pour <!--THEME--> le parametre est "THEME" )
*/
void GererParam(char *param)
{
    ds_t  ds_desc;
    char temp[512];

    if (strcmp(param,"THEME")==0)
    { /* On envois le theme courant*/
        printf("%s",get_act_theme());
    }
    else if(strcmp(param,"GET_THEME")==0)
    { /* On envois la liste de theme */
        cgi_parsefile("./theme/listtheme.file");
    }
    else if(strcmp(param,"CGI_BASENAME")==0)
    { /* On envois le nom de base du cgi */
        printf("%s",cgi_BaseName);
    }
    else
    {     /* Sinon on va chercher la valeur de la variable "qnx" grace a ds */
        ds_desc=ds_register();
        memset(temp,0,512);
        if ( ds_get(ds_desc,param,temp,511)    > 0)
        {
             printf("%s",temp);
        }
        ds_deregister(ds_desc);
    }
}
