#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<string.h>
#include	<ctype.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<time.h>
#include	<math.h>
#include	<errno.h>
#include	<openssl/sha.h>
#include	<sys/time.h>
#include	<sys/socket.h>
#include	<arpa/inet.h>
#include	<netdb.h>

/* shslib.c */
double d_random ( void );
double d_random_range ( double low , double high );
int findmax ( double Array [], int Count );
short flip ( double probability );
int GetTokensD ( char xbuf [], char delim [], char *toks [], int maxtoks );
int GetTokensW ( char xbuf [], char *toks [], int maxtoks );
void MinMaxScale ( double Array [], int StartIndex , int Length );
long random_range ( long low , long high );
void seed_random_with_usec ( void );
int nsStrlen ( const char *cpOne );
char *nsStrchr ( const char *cpOne , int intChar );
