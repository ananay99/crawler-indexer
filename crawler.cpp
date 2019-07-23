#include<iostream>
#include<fstream>
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include"html.h"

using namespace std;

//linked list for storing all links
struct linkList{
 bool isvisited;  //keeping track of processed links
 int depth;
 char *link;
 struct linkList *next;
 struct linkList *hashNext;  //hash table next
};

//hash Table for fast finding of links
struct hashTable{
 struct linkList *hashNode; //starting node of hash
};

//keeping filetrack
int fileCount=0;

//function to be executed once in starting
void makeStartFile(char *link,char * file){
  fflush(stdout);
  //freopen(file,"w",stdout);
  ofstream fout(file);
  fout<<link<<" "<<0<<" "<<0<<"\n";
  fout.close();
}

//hash Function
int retHash(char *lnk){
 int hshValue=0;
 for(int x=0;lnk[x]!='\0';x++)
  hshValue+=lnk[x]*3;
 return(hshValue%101);
}

//searching if link already there if not adding it to llist and hash
struct linkList * fastSearchAndAdd(char *link,int depth,struct hashTable * hash,struct linkList * lastNode){

 int index=retHash(link);
 if(hash[index].hashNode==NULL){
   struct linkList *newnode=(struct linkList*)malloc(sizeof(struct linkList));
   newnode->link=(char*)malloc((strlen(link)+1)*sizeof(char));
   strcpy(newnode->link,link);
   newnode->next=NULL;
   newnode->hashNext=NULL;
   newnode->isvisited=0;
   newnode->depth=depth;
   lastNode->next=newnode;
   lastNode=newnode;
   hash[index].hashNode=newnode;
 }
 else{
  bool found=1;
  struct linkList *searchNode=hash[index].hashNode;
  while(searchNode->hashNext!=NULL){
    if(!strcmp(link,searchNode->link)){
      found=0;
      break;
     }
    searchNode=searchNode->hashNext;
  }
   if(!strcmp(link,searchNode->link)){
      found=0;
     }
  if(found){
   struct linkList *newnode=(struct linkList*)malloc(sizeof(struct linkList));
      newnode->link=(char*)malloc((strlen(link)+1)*sizeof(char));
   strcpy(newnode->link,link);
   newnode->next=NULL;
   newnode->hashNext=NULL;
   newnode->isvisited=0;
   newnode->depth=depth;
    lastNode->next=newnode;
   lastNode=newnode;
   searchNode->hashNext=newnode;
  }
 }
 return lastNode;
}

//Linked list for storing links and returns last node
struct linkList * makeList(struct linkList *head,char *file){
 //freopen(file,"r",stdin);
 ifstream fin(file);
 char link[1000];
 bool visitedFlag;
 int depth;
 while(fin>>link){
  fin>>visitedFlag;
  fin>>depth;
  head->next=(struct linkList*)malloc(sizeof(struct linkList));
  head=head->next;
  head->link=(char*)malloc((strlen(link)+1)*sizeof(char));
  strcpy(head->link,link);
  head->depth=depth;
  head->next=NULL;
  head->hashNext=NULL;
 }
 fin.close();
 return head;
}

//makes hashtable
void makeHash(struct hashTable *hash,struct linkList *head){
 int index;
 struct linkList *auxNode;
 while(head->next!=NULL){
  head=head->next;
  index=retHash(head->link);
  if(hash[index].hashNode==NULL){
  	hash[index].hashNode=head;
  }
  else{
  	auxNode=hash[index].hashNode;
  	  while(auxNode->hashNext!=NULL){
  	  auxNode=auxNode->hashNext;
  	  }
  	 auxNode->hashNext=head;
  }
 }
}

//loads file into memory
char * retStringOfFile(char *file){
 int strLen=0;
 //freopen(file,"r",stdin);
 ifstream fin(file);
 char * retString=(char *)malloc(1000000);
 char ch;
 while(fin>>ch){
  if(strLen==999999){
  	break;
  }
  retString[strLen++]=ch;
 }
 retString[strLen]='\0';
 fin.close();
 return(retString);
}

//Downloads File
void makeFile(char *lnk,char *file){
 //gets file from link
 char auxMemory[1000]="wget ";
 strcat(auxMemory,lnk);
 system(auxMemory);
 //renames file to given File name
 strcpy(auxMemory,"mv index.html ");
 strcat(auxMemory,file);
 system(auxMemory);
 system("sleep 1s");
}

struct linkList * checkFile(struct linkList *head,struct hashTable *hash,struct linkList *lastNode,char *file,int depth){
  int position=0;
  char retreivedLink[1000];
  while(true){
  position=GetNextURL(file,head->link,retreivedLink,position);
  if(position==-1)
  {
  break;
  }
  lastNode=fastSearchAndAdd(retreivedLink,head->depth+1,hash,lastNode);
  }
  return lastNode;
}

//depth function
void makeDepth(struct linkList *head,struct hashTable *hash,struct linkList *lastNode,int depth){

		while(head->next!=NULL&&head->depth<depth){
		  head=head->next;
		  if(!head->isvisited){
		    char fileName[10]="File";
		  	strcat(fileName,(to_string(fileCount++)).c_str());
		  	makeFile(head->link,fileName);
		  	char * strFile=retStringOfFile(fileName);
		  	lastNode=checkFile(head,hash,lastNode,strFile,head->depth);
		  	head->isvisited=1;
		  	free(strFile);
		  	system("sleep 3s");
		  }
		}
}

void saveLink(struct linkList * head,char * file){

 ofstream fout(file);
 head=head->next;
 struct linkList * head2=head;
 while(head->next!=NULL){
  fout<<head->link<<" "<<head->isvisited<<" "<<head->depth<<"\n";
  head2=head;
  head=head->next;
  free(head2->link);
  free(head2);
 }
   fout<<head->link<<" "<<head->isvisited<<" "<<head->depth<<"\n";
  head2=head;
  head=head->next;
  free(head2->link);
  free(head2);
  fout.close();
}

int main(int argc,char *argv[]){

 fflush(stdin);

 if(argc<4){
   cout<<"Less arguments than required\n";
   exit(0);
   }
	  
 char *lnk=argv[1];//baselink
 int depth=atoi(argv[2]);
 char *fileToSaveIn=argv[3];

 cout<<"Load From File(Y/N)"<<endl;
 char askFile;
 scanf("%c",&askFile);
 fflush(stdin);

 struct linkList head;
 struct linkList * lastNode;
 head.isvisited=1;
 head.next=NULL;
 
 struct hashTable hash[101];
 for(int x=0;x<101;x++)
   hash[x].hashNode=NULL;
	 
 if(askFile!='Y'){
   makeStartFile(lnk,fileToSaveIn);
   }
  lastNode=makeList(&head,fileToSaveIn);
  makeHash(hash,&head);
  makeDepth(&head,hash,lastNode,depth);
  saveLink(&head,fileToSaveIn);
 return 0;
}
