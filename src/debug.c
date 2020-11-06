void printtargets(struct target *targets, int ntargets){
	for(int i=0;i<ntargets;i++){
		printf("target %s\n",targets[i].name);
		for(int j=0;j<targets[i].ndeps;j++){
			printf("\t%s;\n",targets[i].deps[j]);
		}
		for(int j=0;j<targets[i].ncommands;j++){
			printf("\t%s;\n",targets[i].commands[j]);
		}
	}
}
