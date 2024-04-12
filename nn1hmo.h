

#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>
#include	<unistd.h>		// for sleep()
#include	<sys/time.h>
#include	<signal.h>

#include	"shslib.h"

#ifdef MAIN
#define		TYPE		/* */
#else
#define		TYPE		extern
#endif

// #define		MAXINPUTS				128
#define		MAXINPUTS				784
#define		MAXOUTPUTS				10
#define		MAXROWS					100000
#define		MAXLAYERS				10

#define		DEFAULT_EPOCHS			2500
#define		DEFAULT_MODEL_FILE		"data.model"	
#define		CONSECUTIVE_ACCURACY	5
#define		DEFAULT_LEARNING_RATE	0.1
#define		DEFAULT_TARGET_ACCURACY	0.99

#define		MODE_TRAIN		1
#define		MODE_ALGO		2
#define		MODE_TEST		3
#define		MODE_PREDICT	4
#define		MODE_LEARN		5
#define		MODE_DUMP_FMT	6
#define		MODE_DUMP_RAW	7
#define		MODE_FOO		8
TYPE	int		RunMode;

#define		SEED_NONE		1
#define		SEED_USEC		2
#define		SEED_SAME		3

TYPE	int		Debug;
TYPE	int		WindowWidth;
TYPE	char	DataFilename[1024];
TYPE	double	TargetAccuracy;
TYPE	int		EpochCount;
TYPE	double	LearningRate;
TYPE	char	ModelFilename[1024];
TYPE	long	InputCount;
TYPE	long	OutputCount;
TYPE	int		HiddenLayers;
TYPE	long	HiddenNeurons[MAXLAYERS];
TYPE	double	SplitTrain;
TYPE	double	SplitTest;
TYPE	int		Normalize;
TYPE	int		HasID;
TYPE	char	SplitFile;
TYPE	long	StartTime, EndTime, TotalTime, Minutes, Seconds;

#define	INDEX(a,c2,b)	a*c2+b


TYPE	double	*InputArray;
TYPE	double	*WeightsIH;
TYPE	double	*HiddenBias;
TYPE	double	*HiddenArray;
TYPE	double	*OutputBias;
TYPE	double	*WeightsHO;
TYPE	double	*OutputArray;


/*----------------------------
:.,$d
:r ! mkproto -p *.c
----------------------------*/

/* algo.c */
void select_parents ( int *Parent1 , int *Parent2 );
void algo ( void );

/* genparms.c */
void genparms ( char *ParmFile );

/* getargs.c */
void getargs ( int argc , char *argv []);

/* getparms.c */
void getparms ( char *ParmFile );

/* init.c */
void init ( int SeedMode );

/* learn.c */
void learn ( int SeedMode , int ProgressMode );

/* miscfuncs.c */
double sigmoid ( double x );
double dSigmoid ( double x );
void foo ( void );
double CurrentSecondOfDay ( void );

/* modelfuncs.c */
void savemodel ( void );
void loadmodel ( void );
void WriteModelAndExit ( int Signal );
void dump ( void );

/* nn1hmo.c */
int main ( int argc , char *argv []);

/* predict.c */
void predict ( void );

/* shslib.c */
double d_random ( void );
double d_random_range ( double low , double high );
int findmax ( double Array [], int Count );
short flip ( double probability );
int GetTokensD ( char xbuf [], char delim [], char *toks [], int maxtoks );
int GetTokensW ( char xbuf [], char *toks [], int maxtoks );
void MinMaxScale ( double Array [], int StartIndex , int Length );
long random_range ( long low , long high );
void shs_seed_random ( void );
int nsStrlen ( const char *cpOne );
char *nsStrchr ( const char *cpOne , int intChar );

/* SignalFuncs.c */
void StartSignals ( void );

/* test.c */
void test ( void );

/* train.c */
double train ( int SeedMode , int ProgressMode );
