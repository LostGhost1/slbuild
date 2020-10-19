#include "main.h"
int resolve(char *targetname, char *parentname, struct target *targets, int ntargets){
	int i=0;
	int needsupdate=0;
	int result=0;
	struct stat statsparent, statstarget;
	int flag=0;
	for(i=0; i<ntargets; i++){
		if(strcmp(targetname,targets[i].name)==0){flag=1; break;}
	}
	if( access( targetname, F_OK ) == -1 ){needsupdate=1;}
	if(parentname!=NULL)	
	if(!stat(parentname, &statsparent)){
		if(!stat(targetname,&statstarget)){
				if(difftime(statsparent.st_mtime,statstarget.st_mtime)<0){
					result=1;
				}
		}
	};
	if(!flag){
		return (needsupdate || result);
	}
	for(int j=0;j<targets[i].ndeps;j++){
		if(resolve(targets[i].deps[j],targetname,targets,ntargets)){needsupdate=1;}
	}
	if(needsupdate)
	for(int j=0;j<targets[i].ncommands;j++){
		printf("%s",targets[i].commands[j]);
		system(targets[i].commands[j]);
	}
	return (needsupdate || result);
}
int parse(char *buildfname, char *targetname, struct target **return_targets, int *return_ntargets){
	FILE *f=fopen(buildfname,"r");
	if(f==NULL){printf("Couldn't open %s\n",buildfname); return 1;}
	char buffer[512];
	char *last_token;
	struct target *targets=NULL;
	int ntargets=0;
	char keepbuffer=0;
	while(1){
		if(!keepbuffer)
			if(fgets(buffer, 512, f) == NULL)break;
		keepbuffer=0;
		if(buffer[0]=='\n')continue;
		if(buffer[0]=='#')continue;
		buffer[strlen(buffer)-1]='\0';
	        last_token = strtok( buffer, " " );
	        ntargets+=1;
	        targets=realloc(targets,ntargets*sizeof(struct target));
		struct target *current_target=&targets[ntargets-1];
		memset(current_target,0,sizeof(current_target));
	        last_token[strlen(last_token)-1]='\0';
	        current_target->name=strdup(last_token);
	        while(1){
	        	last_token = strtok( NULL, " " );
			if(last_token==NULL)break;
			struct stat token_stat;
			stat(last_token, &token_stat);
			if(access( last_token, F_OK )!=-1)
			if(!S_ISREG(token_stat.st_mode)){
				char cwd[512];
				getcwd(cwd,512*sizeof(char));
				chdir(last_token);
				struct target *_targets=NULL;
				int _ntargets=0;
				parse("./Buildfile",strdup(targetname),&_targets,&_ntargets);
				resolve(targetname,NULL,_targets,_ntargets);
				chdir(cwd);
				continue;
			}
			current_target->ndeps+=1;
			current_target->deps=NULL;
			current_target->deps=realloc(current_target->deps,(current_target->ndeps)*sizeof(char*));
			current_target->deps[current_target->ndeps-1]=strdup(last_token);
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
	*return_targets=targets;
	*return_ntargets=ntargets;
	return 2;
}
int main(int argc, char **argv){
	if(argc!=2){printf("Bad usage.\n"); return 1;}
	printf("Reaching target %s...\n",argv[1]);
	struct target *targets;
	int ntargets;
	int result=parse("./Buildfile",argv[1],&targets,&ntargets);
	resolve(argv[1],NULL,targets,ntargets);
	if(result==1){printf("Failed reaching the target %s: an error occured.\n",argv[1]); return 1;};
	if(result==0){printf("Nothing to do for target %s.\n",argv[1]); return 0;};
	if(result==2){printf("Target %s reached successfully.\n",argv[1]); return 2;};
}
