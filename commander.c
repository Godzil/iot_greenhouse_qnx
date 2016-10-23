/*
---- Commander un effecteur ----
*/
#include <stdio.h>
#include <string.h>

#ifdef WIN32

/*
Includes spécifiques a windows
*/
#include <windows.h>
#include <winsock.h>


#else

/*
QNX Specific
*/

#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
typedef int SOCKET;
typedef struct sockaddr SOCKADDR;
typedef struct sockaddr_in SOCKADDR_IN;
#define closesocket(x) close(x);
#define INVALID_SOCKET -1

#endif


#include "cgi_func.h"
#include "cgi_theme_gest.h"
#include "test_serre.h"

char cgi_BaseName[100];


int main(int argc, char *argv[])
{

#ifdef WIN32
/* Spécifique Windows 32 */
    struct WSAData pWSA;
#endif

    SOCKET ConnectSocket;
    SOCKADDR_IN SockOPT;

    char *argument;

    cgi_start();
    cgi_parseparam();

    cgitheme_init();

    strcpy(cgi_BaseName,argv[0]);


    if ( (cgi_getparam("restart")) != NULL )
    {
        cgi_printfile("./files/restart.file");
    }
    else if (serre_test_start()==-1)
    {
        cgi_parsefile("./files/error-noserver.file");
    }
    else
    {
        cgi_parsefile("./files/base-start.file");

        argument = cgi_getparam("effecteur");
        if(argument != NULL)
        {
            printf("<br><br>");
            printf("<img src='/image/info.gif'>Connection au programme principal<br>\n");
            printf("<img src='/image/info.gif'>Mise en oeuvre de la liaison...<br>\n");
            
#ifdef WIN32
            if ( WSAStartup(MAKEWORD(2,0),&pWSA) == 0 )
            {
                printf("<img src='/image/ok.gif'>WSAStartup<br>\n");
#endif

                if ( (ConnectSocket = socket(AF_INET,SOCK_STREAM,0)) != INVALID_SOCKET)
                {
                    printf("<img src='/image/ok.gif'>Creation de la socket<br>\n");
                    SockOPT.sin_family = AF_INET ; 
                    SockOPT.sin_port = htons (60042);
                    SockOPT.sin_addr.s_addr = 0x7F000001; /* 127.0.0.1: on accepte que des connection locales */
                    if ( connect(ConnectSocket,(SOCKADDR *)&SockOPT,sizeof(SOCKADDR_IN)) != INVALID_SOCKET)
                    {
                        printf("<img src='/image/ok.gif'>Connection<br>\n");
                        printf("<img src='/image/info.gif'>Envoi des donnees<br>\n");
                        send(ConnectSocket,argument,strlen(argument),0);
                        printf("<img src='/image/ok.gif'>Envoi effectue<br>\n");
                        shutdown(ConnectSocket,2);
                        printf("<img src='/image/ok.gif'>Deconnection<br>\n");
                        closesocket(ConnectSocket);
#ifdef WIN32
                        WSACleanup();
#endif
                        printf("<img src='/image/info.gif'>Tout c'est bien passe !<br>\n");
                        cgi_parsefile("./files/fin-commander.file");
                    }
                    else
                    {
                        printf("<img src='/image/stop.gif'>Connection<br>\n");
                    }
                }
                else
                {
                    printf("<img src='/image/stop.gif'>Creation de la socket<br>\n");
                }
#ifdef WIN32
            }
            else
            {
            printf("<img src='/image/stop.gif'>WSAStartup<br>\n");
            }
#endif
        }
        printf("<div align='center'><font color=#FF0000><h2>Si se message apparait, c'est qu'un erreur c'est produite lors de la demande de changement d'etat manuel d'un des effecteurs.</h2></font></div>");
        cgi_parsefile("./files/base-end.file");
    }

    cgitheme_close();

    cgi_cleanup();
    return 0;
}
