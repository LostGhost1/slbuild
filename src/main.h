#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
struct target{
	char *name;
	char **deps;
	int ndeps;
	char **commands;
	int ncommands;
};
int parse(char *buildfname, char *targetarg, struct target **return_targets, int *return_ntargets);
/*
void dirpush(char *dirname);
char *dirpeek();
void dirpop();

void execute(struct command *comms);
char recursivedescent(char *targetname, char *parentname, int ntargets, struct target **targets, char *targetarg);
void blockadd(char *command, struct command **block);
void attachblock(struct target *target, struct command **block);
struct target *newtarget(char *name, int *ntarget, struct target ***targets);
void adddep(char *depname, char *targetname,int *ntarget, struct target ***targets);
void statepush(char new, unsigned long long *state);
char statepop(unsigned long long *state);
char statepeek(unsigned long long *state);
struct directory *dirs=NULL;
void printcommands(struct command *command);
void printdeps(struct target **deps, int ndeps);
void printtarget(struct target *target);
*/
