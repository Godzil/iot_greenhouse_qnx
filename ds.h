#ifdef WIN32
/*
Ds.h
*/
#ifndef BIDON_DS
#define BIDON_DS

typedef int ds_t;

int ds_register();
int ds_get(ds_t ds,char *,char *,int);
int ds_set(ds_t dsdes,const char* variable_name,const char* variable_data,size_t data_len );
void ds_deregister(ds_t);

#endif

#endif