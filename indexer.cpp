#include<iostream>
#include<fstream>
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<cstring>
#include<sstream>
using namespace std;
struct hsh
{
 char * word;
 char pgindex[1000]; 
 struct hsh *next;
};
//loads file into memory
char * retStringOfFile(char *file){
 int strLen=0;
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
int rethsh(char * str,int cn)
{
int hsh=0;
for(int x=0;x<cn;x++)
{
hsh+=str[x]*3;
}
return(hsh%101);
}
void addhsh(struct hsh *arr,char * st,int cn,int page)
{
int hshvalue=rethsh(st,cn);
bool chk=0;
struct hsh * ptr=(arr+hshvalue);
while(ptr->next!=NULL)
{

bool p=0;
int y=0;
for(int x=0;ptr->word[y]!='\0'&&x<cn;x++,y++)
{
if(ptr->word[y]!=st[x])
{
p=1;
break;
}
}
if(ptr->word[y]=='\0'&&y==cn)
{
	ptr->pgindex[page]=1;

		chk=1;
		break;
}
ptr=ptr->next;
}

if(chk==0)
{
bool p=0;
int y=0;
for(int x=0;ptr->word[y]!='\0'&&x<cn;x++,y++)
{
if(ptr->word[y]!=st[x])
{
p=1;
break;
}
}
if(ptr->word[y]=='\0'&&y==cn)
{
	ptr->pgindex[page]=1;

		chk=1;
}
}
if(chk==0)
{
ptr->next=(struct hsh*)malloc(sizeof(struct hsh));
ptr=ptr->next;
	ptr->next=NULL;
	ptr->word=(char *)malloc(cn+1);
	for(int x=0;x<1000;x++)
	{
	ptr->pgindex[x]=0;
	}
	for(int x=0;x<cn;x++)
	{
	ptr->word[x]=st[x];
	}
		ptr->word[cn]='\0';	
		ptr->pgindex[page]=1;
}
}
int clean(char *str,int cn)
{
int i=0;
for(int x=0;x<cn;x++)
{
if(str[x]>='a'&&str[x]<='z')
{
str[i++]=str[x];
}
else if(str[x]>='A'&&str[x]<='Z')
{
str[i++]='a'+str[x]-'A';
}
else if(str[x]=='<')
{
while(str[x]!='>'&&x<cn)
{
x++;
}
}
}
str[i]='\0';
return i;
}
//high complexity more accurate
/*void retstring(char * str,int cn,struct hsh *arr,int page)
{
 for(int x=0;x<cn;x++)
 {
 for(int y=x+1;y<cn;y++)
 addhsh(arr,str+x,y-x,page);
 }
}*/
//less complexity less acccurate
void retstring(char * str,int cn,struct hsh *arr,int page)
{
	cn=cn-4;
 for(int x=0;x<cn;x++)
 {
 	addhsh(arr,str+x,4,page);
 }
}
void retrievetag(char* strFile,char *tag,char* endingTag,struct hsh *arr,int page)
{
while(true)
{
strFile=strstr(strFile,tag);
if(strFile)
{
 strFile+=strlen(tag);
 char * endoftag=strstr(strFile,endingTag);

 if(endoftag)
 {
 char * str=(char *)malloc((endoftag-strFile+1)*sizeof(char));
 int cn=0;
 while(strFile!=endoftag)
 {	
 	str[cn++]=*strFile;
 	strFile++;
 }
 str[cn]='\0';
  cn=clean(str,cn);

 if(cn<500)
 {
 retstring(str,cn,arr,page);
 }
 free(str);
 strFile=endoftag;
 }
}
else
{
break;
}
}
}
void readfile(char * file,struct hsh *arr,int page)
{
ifstream fin(file);
if(fin)
{
fin.close();
char * strFile=retStringOfFile(file);
retrievetag(strFile,"<title>","</title>",arr,page);
retrievetag(strFile,"<h1>","</h1>",arr,page);
retrievetag(strFile,"<h2>","</h2>",arr,page);
retrievetag(strFile,"<h3>","</h3>",arr,page);
retrievetag(strFile,"<h4>","</h4>",arr,page);
retrievetag(strFile,"<h5>","</h5>",arr,page);
retrievetag(strFile,"<h6>","</h6>",arr,page);
retrievetag(strFile,"\"content=\"","\"",arr,page);
free(strFile);
}
}
void updatefilecn(char * input,int *filecn,struct hsh *arr)
{
int len=4;
int hashvalue=rethsh(input,4);
struct hsh * ptr=arr+hashvalue;
while(ptr->next!=NULL)
{
ptr=ptr->next;
bool p=0;
for(int x=0;x<4;x++)
{
if(input[x]!=ptr->word[x])
{
p=1;
break;
}
}
if(p==0)
{
for(int x=0;x<1000;x++)
{
filecn[x]+=ptr->pgindex[x];
}
break;
}
}
}
void retpages(char * input,int len,int *filecn,struct hsh *arr)
{
len-=4;
for(int x=0;x<len;x++)
{
updatefilecn(input+x,filecn,arr);
}
}
int main()
{
struct hsh arr[101];
for(int x=0;x<101;x++)
{
arr[x].word=(char *)malloc(sizeof(char));
arr[x].word[0]='\0';
arr[x].next=NULL;
}
 int filecount=0;
 char file[50];
 while(filecount<247)
 {
 strcpy(file,"File");
 strcat(file,to_string(filecount).c_str());
 readfile(file,arr,filecount);
 filecount++;
 }
cout<<"Enter Query"<<endl;
int filecn[1000];
for(int x=0;x<1000;x++)
{
filecn[x]=0;
}
char input[1000];
scanf("%[^\n]s",input);
int len=strlen(input);
clean(input,len);
retpages(input,len,filecn,arr);
int mx=0;
for(int x=0;x<1000;x++)
{
if(filecn[x]>mx)
{
mx=filecn[x];
}
}
if(mx==0)
{
cout<<"Sorry no relevant WebPage found!!!"<<endl;
}
for(int x=0;x<1000;x++)
{
if(mx==filecn[x])
{
strcpy(file,"gnome-open File");
 strcat(file,to_string(x).c_str());
 system(file);
}
}
return 0;
}
