#include "main.h"
#include "debug.c"
char *argv1;
int resolve(char *targetname, char *parentname, struct target *targets, int ntargets){
	char s[128];
	getcwd(s,128);
	int i=0;
	int needsupdate=0;
	int result=0;
	struct stat statsdep, statstarget;
	int flag=0;
	for(i=0; i<ntargets; i++){
		if(strcmp(targetname,targets[i].name)==0){flag=1; break;}
	}
	if(!flag) return 0;
	if( access( targetname, F_OK ) == -1 ){needsupdate=1;}
	for(int j=0;j<targets[i].ndeps;j++){
		if(!stat(targets[i].deps[j], &statsdep)){
			if(!stat(targetname,&statstarget)){
					if(difftime(statsdep.st_mtime,statstarget.st_mtime)>0){
						needsupdate=1;
						break;
					}
			}
		};
	}
	for(int j=0;j<targets[i].ndeps;j++)
		if(resolve(targets[i].deps[j],targetname,targets,ntargets)){needsupdate=1;}
	if(needsupdate)
	for(int j=0;j<targets[i].ncommands;j++){
		printf("%s",targets[i].commands[j]);
		chdir(targets[i].location);
		system(targets[i].commands[j]);
	}
	return (needsupdate);
}
int parse(char *buildfname, char *targetname, struct target **targets_return, int *ntargets_return){
	FILE *f=fopen(buildfname,"r");
	if(f==NULL){printf("Couldn't open %s\n",buildfname); return 1;}
	char buffer[512];
	char cwd[512];
	getcwd(cwd,512);
	strcat(cwd,"/");
	char *last_token;
	char *last_token_t;
	char *ptr;
	struct target *current_target;
	struct target *targets=*targets_return;
	int ntargets=*ntargets_return;	
	char keepbuffer=0;
	int current_target_n;
	while(1){
		if(!keepbuffer)
			if(fgets(buffer, 512, f) == NULL)break;
		keepbuffer=0;
		if(buffer[0]=='\n')continue;
		if(buffer[0]=='#')continue;
		buffer[strlen(buffer)-1]='\0';
	        last_token_t=strtok_r(buffer," ",&ptr);
	        last_token=strdup(last_token_t);
	        ntargets+=1;
	        targets=realloc(targets,ntargets*sizeof(struct target));
	        current_target_n=ntargets-1;
		current_target=&targets[current_target_n];
		memset(current_target,0,sizeof(struct target));
	        last_token[strlen(last_token)-1]='\0';
	        current_target->name=malloc(1024);
	        current_target->name[0]='\0';
	        current_target->location=malloc(1024);
	        current_target->location[0]='\0';
		strcat(current_target->location,cwd);
	        strcat(current_target->name,cwd);
	        strcat(current_target->name,last_token);
	        while(1){
	        	last_token_t=strtok_r(NULL," ",&ptr);
			if(last_token_t==NULL)break;
	        	last_token=strdup(last_token_t);
			struct stat token_stat;
			stat(last_token, &token_stat);
			if(access( last_token, F_OK )!=-1)
			if(!S_ISREG(token_stat.st_mode)){
				chdir(last_token);
				char t[512];
				getcwd(t,512*sizeof(char));
				current_target->ndeps+=1;
				current_target->deps=realloc(current_target->deps,(current_target->ndeps)*sizeof(char*));
				current_target->deps[current_target->ndeps-1]=malloc(1024);
				current_target->deps[current_target->ndeps-1][0]='\0';
				strcat(t,"/");
				strcat(current_target->deps[current_target->ndeps-1],t);
				strcat(current_target->deps[current_target->ndeps-1],targetname);
				strcat(t,"Buildfile");
				parse(t,targetname,&targets,&ntargets);
				current_target=&targets[current_target_n];
				chdir(cwd);
				continue;
			}
			current_target->ndeps+=1;
			current_target->deps=realloc(current_target->deps,(current_target->ndeps)*sizeof(char*));
			current_target->deps[current_target->ndeps-1]=malloc(1024);
			current_target->deps[current_target->ndeps-1][0]='\0';
			strcat(current_target->deps[current_target->ndeps-1],cwd);
			strcat(current_target->deps[current_target->ndeps-1],last_token);
		}
		keepbuffer=1;
		while(1){
			if(fgets(buffer, 512, f) == NULL){keepbuffer=0; break;}
			if(buffer[0]=='#')continue;
			if(buffer[0]!='\t')break;
			current_target->ncommands+=1;
			current_target->commands= realloc(current_target->commands,(current_target->ncommands)*sizeof(char*));
			current_target->commands[current_target->ncommands-1]=strdup(buffer);
		}
	}
	*targets_return=targets;
	*ntargets_return=ntargets;
	return 2;
}
int main(int argc, char **argv){
	if(argc!=2){printf("Bad usage.\n"); return 1;}
	argv1=argv[1];
	printf("Reaching target %s...\n",argv[1]);
	struct target *targets=NULL;
	int ntargets=0;
	parse("./Buildfile",argv[1],&targets,&ntargets);
	char s[1024];
	s[0]='\0';
	char cwd[512];
	getcwd(cwd,512);
	strcat(cwd,"/");
	strcat(s,cwd);
	strcat(s,argv[1]);
	resolve(s,NULL,targets,ntargets);
	printf("Target %s reached successfully.\n",argv[1]); 
	return 0;
}
