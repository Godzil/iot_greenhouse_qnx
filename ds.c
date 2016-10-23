#ifdef WIN32

#include <stdio.h>
#include <string.h>
#include "ds.h"

unsigned long int suivant = 1;


int rand()
{
    suivant = suivant * 1103515245 + 12345;
    return (unsigned int)(suivant/65536) % 32768;
}

int ds_register()
{
    return rand();
}
int ds_get(ds_t ds,char *arg,char *val,int valsize)
{
    FILE *fp;
    char file[512];
    strcpy(file,"c:/ds/");
    strcat(file,arg);
    fp = fopen(file,"rb");
    if ( fp != NULL )
    {
        fread(val,valsize,1,fp);
        fclose(fp);
    }
    else
    {
        strcpy(val,"NOT_EXIST");
    }
    return 1;
}
void ds_deregister(ds_t t)
{
        
}


int ds_set(ds_t dsdes,const char* variable_name,const char* variable_data,size_t data_len )
{
    FILE *fp;
    char file[512];
    strcpy(file,"c:/ds/");
    strcat(file,variable_name);
    fp = fopen(file,"wb");
    if ( fp != NULL )
    {
        fwrite(variable_data,data_len,1,fp);
        fclose(fp);    
    }
    return 0;
}

#endif