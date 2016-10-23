/*
---- A propos ----
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cgi_func.h"
#include "cgi_theme_gest.h"
#include "test_serre.h"

char cgi_BaseName[100];

int main(int argc, char *argv[])
{
	cgi_start();
	cgi_parseparam();

	cgitheme_init();

	strcpy(cgi_BaseName,argv[0]);


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
		cgi_parsefile("./files/about.file");
		cgi_parsefile("./files/base-end.file");
	}

	cgitheme_close();

	cgi_cleanup();
	return 0;
}
