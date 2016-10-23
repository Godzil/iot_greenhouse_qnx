#include "cgi_func.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define set_act_theme(val) theme = val

char theme_base[] = "basebleu";

char themestart[100];
char themeend[100];

char *theme;

char *get_act_theme()
{
	return theme;
}

void cgitheme_init()
{

	char *temp;	

	cgi_start();
	cgi_parseparam();

	temp = cgi_getparam("theme");
	if (temp == NULL)
		temp = theme_base;

#ifdef _WIN32
	sprintf(themestart,".\\theme\\%s\\start.theme",temp);
	sprintf(themeend,".\\theme\\%s\\end.theme",temp);
#else /* On est pas sous windows, c qu'on est sous un UNIX */
	sprintf(themestart,"./theme/%s/start.theme",temp);
	sprintf(themeend,"./theme/%s/end.theme",temp);
#endif
	
	if ( cgi_printfile(themestart) == -1 )
	{
	temp = theme_base;
#ifdef _WIN32
	sprintf(themestart,".\\theme\\%s\\start.theme",temp);
	sprintf(themeend,".\\theme\\%s\\end.theme",temp);
#else /* On est pas sous windows, c qu'on est sous un UNIX */
	sprintf(themestart,"./theme/%s/start.theme",temp);
	sprintf(themeend,"./theme/%s/end.theme",temp);
#endif

	cgi_printfile(themestart);
	}
	
	set_act_theme(temp);
	
}

void cgitheme_close()
{
	cgi_printfile(themeend);
}