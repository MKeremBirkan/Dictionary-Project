#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

struct line {
      char  word[10];
      char  descr[50];
      int   next_line;
      int   prev_line;
};

void initializeScreen() {
	printf("** Command mode **\n");
	printf("I for Insert\nD for Delete\nL for Displays\nS for Save\nX for Exit\n");
	printf("******************\n");
	
}
void INSERT(int* free,int* LC,int* start,struct line* textlines,char* word,char* desc){
	int i =	*start;
	int tempFree;
	int last = *start;	
	struct line temp;   		
	word[9] = '\0';
	desc[49]= '\0';
	strncpy(temp.word,word,10);
	strncpy(temp.descr,desc,50);		

	if(*LC==0){
		temp.prev_line = 999;
		temp.next_line = 1;
		textlines[0]=temp;
		*free+=1;
		*LC+=1;
	}
	else{
		while(textlines[i].next_line != -1){
			
			if(strcmp(textlines[i].word,word) == 0 ){//a=b
				printf("Entered word is exist!\n");
				break;							
			}
			else if (strcmp(textlines[i].word,word) < 0 ){//a < b
				
				if(strlen(textlines[textlines[i].next_line].word) > 0){ // bir sonraki bos degilse
					//printf("orta harf");
					if(strcmp(textlines[textlines[i].next_line].word,word) > 0){
						
						//ortaya ekleme
						temp.next_line = textlines[i].next_line;
						temp.prev_line = i;
						textlines[textlines[i].next_line].prev_line = *free;
						while(textlines[textlines[last].next_line].next_line != -1){
							last = textlines[last].next_line;
						}
						if(textlines[last].next_line == *free)
							textlines[last].next_line = *free+1;
						textlines[i].next_line = *free;
						tempFree = textlines[*free].prev_line;
						textlines[*free] = temp;
						*free =tempFree;
						*LC+=1;
						break;
					}
				}
				else {//sona ekleme
					temp.next_line = textlines[*free].prev_line;
					temp.prev_line = i;
					textlines[i].next_line = *free;
					tempFree = textlines[*free].prev_line;
					textlines[*free] = temp;
					*free =tempFree;
					*LC+=1;
					break;
				}	
			}	
			else{//b < a
				//en basa atılacaksa
				temp.prev_line = 999;
				temp.next_line = textlines[999].prev_line;
				textlines[textlines[999].prev_line].prev_line = *free;
				while(textlines[textlines[last].next_line].next_line != -1){
					last = textlines[last].next_line;
				}
				if(textlines[last].next_line == *free)
					textlines[last].next_line = *free+1;
				textlines[999].prev_line = *free;
				tempFree = textlines[*free].prev_line;
				textlines[*free] = temp;
				*free =tempFree;
				*LC+=1;
				break;
			}
			i=textlines[i].next_line;
		}	
	}
}
void DISPLAY(struct line* textlines,int start,int LC,char* word){
	int i;
	bool found = false;
	if(LC==0){
		printf("Dictionary is empty !\n");
		return;
	}
	else{
		
		while(textlines[start].next_line != -1){
			if(strcmp(textlines[start].word,word)==0){
				found = true;
				for (i=0 ; i<4 ; i++){
					if (strlen(textlines[start].word) > 0)
						printf("%-10s: %s\n",textlines[start].word,textlines[start].descr);
					if(textlines[start].next_line != -1)
						start = textlines[start].next_line;	
				}
				break;	
			}								
			start = textlines[start].next_line;
		}
		if(found == false)
			printf("Word does not exist !\n");
	}	
}
void DELETE(char* selected,struct line* textlines,int* LC,int* start,int* free){	
	int i = *start;
	bool found=false;	
	if(*LC==0)
		printf("Dictionary is empty!\n");
	else{
		while(textlines[i].next_line!=-1){
			if(!strcmp(textlines[i].word,selected)){
				//printf("Word is found!\n");
				//found=true;
				if(i==*start){//First element
					found=true;					
					*start = textlines[i].next_line;
					textlines[i].prev_line = *free;
					*free = i;
					*LC-=1;
					if(*LC==0)
						*start=0;
					printf("%s deleted!\n",selected);
					break;
				}else if(textlines[textlines[i].next_line].next_line ==-1){//Last Element
					found=true;
					textlines[i].next_line = -1;
					textlines[i].prev_line = *free;
					*free = i;
					*LC-=1;
					printf("%s deleted!\n",selected);
					break;
				}else{//Other elements
					found=true;
					textlines[textlines[i].prev_line].next_line = textlines[i].next_line;
					textlines[textlines[i].next_line].prev_line = textlines[i].prev_line;
					textlines[i].next_line = -1;
					textlines[i].prev_line = *free;
					*free = i;
					*LC-=1;
					printf("%s deleted!\n",selected);
					break;
				}
			}			
			i = textlines[i].next_line;
		}
		if(found==false)
			printf("Word does not exist !\n");
	}
	
}
void SAVE (struct line* textlines,int start,char* fileName){
	int i=start;		
	FILE *myFile;
	myFile = fopen(fileName, "wb");

	while(textlines[i].next_line!=-1){
      	fprintf(myFile,"%s: %s\n",textlines[i].word,textlines[i].descr);
		i = textlines[i].next_line;
	}
	fclose(myFile);
}

int main(int argc, char *argv[]){
	int LC=0,free=0,i=0;//line(word) counter - free list index
	char currentMode;
	FILE *file;
	char filename[20];	
	char word[10];
	char desc[50];
	char getline[61];
	struct line textlines[1000];
	struct line *start;
	start = &textlines[999];	

	for (i=0; i< 1000; i++)
	{
		textlines[i].prev_line = i+1;
		textlines[i].next_line = -1;
	}
	start->prev_line = 0;
	start->next_line = 1;
	
	if(argc==1){
		file = fopen( "newDICT.txt", "ab+" );
		printf ("New dictionary created newDICT.txt!\n");
		strcpy(filename,"newDICT.txt");
	}
	else if(argc==2){
		file = fopen( argv[1], "r" );
		strcpy(filename,argv[1]);
		printf ("Installing Dictionary...\n");
		while(fgets(getline,61,file)!=NULL)
   		{
   			strncpy(word,strtok(getline," :\n"),10);
   			strncpy(desc,strtok(NULL," :\n"),50);
   			if(strlen(word)>0 && strlen(desc) > 0)	
				INSERT(&free,&LC,&start->prev_line,textlines,word,desc);
   				
		}
		printf ("*******DONE*******\n");
		fclose(file);
	}
	else{
		printf ("Error : Too much arguments entered !\n");
		return 0;
	}			
	while (1) {
		
		initializeScreen();
		printf("Enter :");	
		scanf(" %c",&currentMode);
		
		switch(currentMode) {
			
			case 'I': // Insert Mode
				printf("Enter like  'word:description'  =  ");
				scanf("%s",getline);
				strncpy(word,strtok(getline," :\n"),10);
   				strncpy(desc,strtok(NULL," :\n"),50);
				INSERT(&free,&LC,&start->prev_line,textlines,word,desc);
				break;
			case 'D': // Delete Command
				printf("Enter a word for deletion : ");
				scanf("%s",word);
				DELETE(word,textlines,&LC,&start->prev_line,&free);
				break;
			case 'L': // Display Mode
				printf("Enter word:");
				scanf("%s",word);
				DISPLAY(textlines,start->prev_line,LC,word);
				break;
			case 'S': // Save Commmand
				SAVE(textlines,start->prev_line,filename);
				break;
			case 'X': // Exit Commmand
				return 0;
				break;
			default :
				printf("--Invalid entered!--\n");
				
		}

	}
	return 0;
}

