#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

struct word
{
    char filename[100];
    int count;
    struct word *next;
};

struct wordNode
{
    char word[25];
    int count;
    struct wordNode *next;
    struct word *start;
};

struct hashtable
{
    struct wordNode *start;
}hash[123];

struct word *createWordStruct(char *filename){
  struct word *temp = (struct word*)malloc(sizeof(struct word));
  strcpy(temp->filename,filename);
  temp->count = 1;
  temp->next = NULL;
  return temp;
}

struct wordNode *createWordNodeStruct(char *word){
  struct wordNode *temp = (struct wordNode*)malloc(sizeof(struct wordNode));
  strcpy(temp->word,word);
  temp->count=1;
  temp->next = NULL;
  return temp;
}

int filesize(char dir[100]) {                                             //get filesize
    struct stat st;                            //variable which will count length of file.
    char str[20];
    strcpy(str,dir);
    stat(str,&st);                             // temp.txt is the file where wget fetch the html
    int file_size=(int)st.st_size;
    printf("%d\n",file_size );
    return file_size;
}

int findkey(char *str) {
	int i=0,key=0;
	while(str[i]!='\0') {
	  key+=(int)str[i++];
	}
	while(key>100) {
	  key/=10;
  }
	return key;
}
char fcontent[20000000];
char *readcontent(char *dir) {                                                     // get content from file
  printf("%s\n","ok" );

    printf("%s\n","okll" );
    FILE *fp= fopen(dir, "r+");
    char c;
    int i=0;
		c=getc(fp);
    while(c!=EOF)
    {
			fcontent[i++]=c;
        printf("%c",c );
      c=getc(fp);
    }
		fcontent[i]='\0';
    fclose(fp);
    return fcontent;
}

void addWord(char *arr, char* file){
  struct wordNode *ptr = hash[arr[0]].start;
  if(ptr==NULL){
    ptr = createWordNodeStruct(arr);
    ptr->start=createWordStruct(file);
    hash[arr[0]].start = ptr;
  }else{
    while(1){
      if(!strcmp(ptr->word,arr)){
        (ptr->count)++;
        struct word *p = ptr->start;
        while(1){
          if(!strcmp(file,p->filename)){
            p->count++;
            return;
          }
          if(p->next==NULL){
            break;
          }
          p=p->next;
        }
        p->next = createWordStruct(file);
        return;
      }
      if(ptr->next==NULL){
        break;
      }
      ptr=ptr->next;
    }
    struct wordNode *p1 = createWordNodeStruct(arr);
    p1->start = createWordStruct(file);
    ptr->next = p1;
  }
}

void word_count(char *str,char *filename) //function take whole string as argument n convert it in words
{
  int i=0,flag=0,j=0;
  char arr[50];
  while(str[i]!='\0'){
    if(str[i]==' '){
      if(flag==1){
        arr[j]='\0';
        addWord(arr,filename);	//function add the word to the linklist
        j=0;
        flag=0;
      }
    }
    else{
      arr[j++]=str[i];
      if(flag==0){
        flag=1;
      }
    }
    i++;
 }
}

/*
void findWord(char *str,char *dir) {
char *string;
int i=0,endIndex=0,startIndex=0;
int flag=0,no;
       while(str[i] != '\0')
	{
        if(str[i] == ' ')
		{
            if(flag == 1)
			{
                endIndex = i;
                no=endIndex-startIndex;
                string=(char*)malloc(sizeof(char)*(no));
                memcpy(string, &str[startIndex], no);
                *(string+no)='\0';
                printf("%s\n",string);
                addWord(string,dir);
                addWord(string,dir);
                free(string);
            }
            flag = 0;
        }
		else
		{
            if(flag == 0)
                startIndex = i;
            flag = 1;
        }
        i++;
    }
    if(str[i] == '\0')
	{
                endIndex=i;
                no=endIndex-startIndex;
                string=(char*)malloc(sizeof(char)*(no));
                memcpy(string, &str[startIndex], no);
                *(string+no)='\0';
                printf("%s\n",string);
                free(string);
    }
    printf("--------------------------------------------\n");
}
*/
void getheadingtag(char *str,char *dir) {
    printf("-----------------------------headingtag---------------------------\n" );
    char *hStart,*hEnd,*subStr;
    int no;
    for(int i=0;i<strlen(str);i++)
    {
        if(str[i]=='<' && str[i+1]=='h' && (str[i+2]=='1' || str[i+2]=='2' || str[i+2]=='3' || str[i+2]=='4' || str[i+2]=='5' || str[i+2]=='6'))
        {
          while(str[i]!='>')
          {
            i++;
          }
        hStart=&str[i];
        hStart++;
        hEnd=strpbrk(hStart, "<");
        no=(hEnd-hStart);
        i=i+no;
        subStr=(char *)malloc(sizeof(char)*no);
        memcpy(subStr,hStart,no);
        *(subStr+no)='\0';
        printf("%s\n",subStr );
        addWord(subStr,dir);
        }
    }
}
/*
char* getFileName(char *dir)
{
    char *fcontent =(char*)malloc(sizeof(char)*50);
    FILE *fp= fopen(dir, "r+");
    int i=0;
    char c;
    c=getc(fp);
    while(c!='\n')
    {
        fcontent[i++]=c;
        c=getc(fp);
    }
    fcontent[i]='\0';
    fclose(fp);
    return fcontent;
}
*/
void getTitle(char *str,char *dir)
{
    printf("-----------------------------title---------------------------\n" );
    char *data;
    char *titleStart,*titleEnd;
    for(int i=0;i<strlen(str);i++)
    {
        if(str[i]=='<' && str[i+1]=='t')
        {
        while(str[i]!='>')
        {
            i++;
        }
        titleStart=&str[i];
        titleStart++;
        titleEnd=strpbrk(titleStart, "<");
        int j=0;
        data=(char*)malloc(sizeof(char)*(titleEnd-titleStart+1));

            for( ;titleStart!=titleEnd;titleStart++)
            {
                data[j++]=*titleStart;
            }
            data[j++]='\0';
            //printf("%s\n",data);
            addWord(data,dir);

            free(data);
        }
    }
}


void getMeta(char *str,char *dir)
{
    printf("-----------------------------meta---------------------------\n" );
    char *titleStart,*titleEnd,data[100];
    char content[]="content=";
    for(int i=0;i<strlen(str);i++)
    {
        if(str[i]=='<' && str[i+1]=='m' && str[i+2]=='e')
        {
            for(int j=i;j<strlen(str);j++)
            {
                if(str[j]=='c')
                {
                    int k=0;
                    while(str[j]==content[k])
                    {
                        j++;
                        k++;
                    }
                    if(content[k]=='\0')
                    {
                        titleStart=&str[j];
                        titleStart++;
                        break;
                    }
                }
            }
            titleEnd=strpbrk(titleStart, "\"");
            int l=0;
                for( ;titleStart!=titleEnd;titleStart++)
                {
                  data[l++]=*titleStart;
                }
                data[l++]='\0';
                addWord(data,dir);
                //printf("\n")
            }
    }
}

void print()
{
  for(int i=65;i<123;i++){
    struct wordNode *ptr = hash[i].start;
    if(ptr!=0){
      while(ptr->next!=NULL){
        struct word *p = ptr->start;
        while(p!=NULL){
          printf("%s %s %d\n",ptr->word,p->filename,p->count);
          p=p->next;
        }
        ptr=ptr->next;
      }
    }
  }
}

void saveData()
{
  FILE *f = fopen("word_with_filename.txt","a");
  for(int i=65;i<123;i++){
    struct wordNode *ptr = hash[i].start;
    if(ptr!=0){
      while(ptr->next!=NULL){
        struct word *p = ptr->start;
        while(p!=NULL){
          fprintf(f,"%s %s %d\n",ptr->word,p->filename,p->count);
          p=p->next;
        }
        ptr=ptr->next;
      }
    }
  }
  fclose(f);
}


int main(int argc,char *argv[])
{
  char dir[100],c[10];
  char *fileContent;
  for(int i=1;i<3;i++)
  {
    strcpy(dir,"./temp/");
    sprintf(c,"%d",i);
    strcat(dir,c);
    strcat(dir,".txt");
    printf("%s\n",dir);
    fileContent=readcontent(dir);
      printf("%s\n","ok" );
    getTitle(fileContent,dir);
    getheadingtag(fileContent,dir);
    printf("%s\n","ok" );
    print();
    saveData();
    free(fileContent);
    //getMeta(fileContent);
  }
    return 0;
}
