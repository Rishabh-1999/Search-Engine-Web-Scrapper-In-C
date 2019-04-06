#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#define BASE_URL "www.chitkara.edu.in"
#define urlLength 1000

struct LinkList                              // structure for linked list
{
	char url[250];
	int depth;
	int key;
	int flag;
	int count;
	struct LinkList *next;
}*Head;

struct LinkList *last;
struct hash
{
    struct LinkList *next;
    struct LinkList *last;
    int count;
} node[50];

int check_Dir(char *dir) {                                           //Check Dir and Check permission
  struct stat statbuf;                                              //Check Dir
  if ( stat(dir, &statbuf) == -1 )
  {
    fprintf(stderr, "Invalid directory\n");
    exit(1);
  }
  if ( !S_ISDIR(statbuf.st_mode) )                                   //Both check if there's a directory and if it's writable
  {
    fprintf(stderr, "Invalid directory entry. Your input isn't a directory\n");
    exit(1);
  }
  if ( (statbuf.st_mode & S_IWUSR) != S_IWUSR )                         //Check Permission
  {
    fprintf(stderr, "Invalid directory entry. It isn't writable\n");
    exit(1);
  }
  return 1;
}

char* changeurltochar(char *url) {                                      // fuction to put null or \0 in last anad return
    int i = 0;
    while(1) {
        if(url[i] == '\0')
            break;
        i++;
    }
    char *newurl = (char*)malloc(sizeof(char) * i+1);
    newurl = url;
    newurl[i] = '\0';
    return newurl;
}

int check_URL(char *url) {                                              //check URL
    char v[urlLength]="wget --spider ";
    strcat(v,url);
		if(!system(v))
		{                                                      // CONCAT v and given url
    if(strcmp(BASE_URL,changeurltochar(url)) && !system(v))             //check whether given url is same as base url
        return 0;
		}
		else                                                       //exit if bse url is incorrect
				return 0;
		return 1;
}

int checkDepth(char *d) {                                               // check given depth is wrong
        int DEPTH=0;                                                    //intialize varable DEPTH
        sscanf(d, "%d", &DEPTH);                                        // change char to int
        if(DEPTH>5 || DEPTH<1)                                          //check the depth of given argument that it is in range
            return 0;                                                   // exit if depth is out of range
        return 1;
}

void checkArguments(int argc,char* argv[]) {                             // function to check whether all arguments are correct or not!!
    	if(argc==4)
            if(checkDepth(argv[2]))
                if(check_URL(argv[1]))
                    if(check_Dir(argv[3]))
                    {
                        printf("All Arguments are correct\n");
                        return ;
                    }
                else
                    printf("Invalid URL\n");                                            //display seed_url is incorrect
            else
                printf("Wrong input of Depth");
        else
            printf("Arguments not recieved not properly");
        exit(0);
}

void get_page(char *url) {                                            //get data in file
    char urlbuffer[urlLength]="wget -O ./temp.txt ";
    strcat(urlbuffer,url);
    int n=system(urlbuffer);
}

void removeWhiteSpace(char* html) {                                  //Remove White Space
  char *buffer = (char*)malloc(strlen(html)+1), *p=(char*)malloc(sizeof(char)+1);
  memset(buffer,0,strlen(html)+1);
  for(int i=0;html[i];i++)
  {
    if(html[i]>32)
    {
      sprintf(p,"%c",html[i]);
      strcat(buffer,p);
    }
  }
  strcpy(html,buffer);
  free(buffer);
  free(p);
}

int GetNextURL(char* html, char* urlofthispage, char* result, int pos) {
  char c;
  int len, i, j;
  char* p1;
  char* p2;

  if(pos == 0) {
    removeWhiteSpace(html);
  }
  while (0 != (c = html[pos]))
  {
    if ((c=='<') && ((html[pos+1] == 'a') || (html[pos+1] == 'A'))) {
      break;
    }
    pos++;
  }
  if (c)
  {
    p1 = strchr(&(html[pos+1]), '=');
    if ((!p1) || (*(p1-1) == 'e') || ((p1 - html - pos) > 10))
    {
      return GetNextURL(html,urlofthispage,result,pos+1);
    }
    if (*(p1+1) == '\"' || *(p1+1) == '\'')
      p1++;
    p1++;
    p2 = strpbrk(p1, "\'\">");
    if (!p2)
    {
      return GetNextURL(html,urlofthispage,result,pos+1);
    }
    if (*p1 == '#')
    {
      return GetNextURL(html,urlofthispage,result,pos+1);
    }
    if (!strncmp(p1, "mailto:",7))
      return GetNextURL(html, urlofthispage, result, pos+1);
    if (!strncmp(p1, "http", 4) || !strncmp(p1, "HTTP", 4))
    {
      strncpy(result, p1, (p2-p1));
      return  (int)(p2 - html + 1);
    } else {
      if (p1[0] == '.') {
        return GetNextURL(html,urlofthispage,result,pos+1);
      }
      if (p1[0] == '/') {
        for (i = 7; i < strlen(urlofthispage); i++)
          if (urlofthispage[i] == '/')
            break;
        strcpy(result, urlofthispage);
        result[i] = 0;
        strncat(result, p1, (p2 - p1));
        return (int)(p2 - html + 1);
      } else {
        len = strlen(urlofthispage);
        for (i = (len - 1); i >= 0; i--)
          if (urlofthispage[i] == '/')
            break;
        for (j = (len - 1); j >= 0; j--)
          if (urlofthispage[j] == '.')
              break;
        if (i == (len -1)) {
            strcpy(result, urlofthispage);
            result[i + 1] = 0;
            strncat(result, p1, p2 - p1);
            return (int)(p2 - html + 1);
        }
        if ((i <= 6)||(i > j)) {
          strcpy(result, urlofthispage);
          result[len] = '/';
          strncat(result, p1, p2 - p1);
          return (int)(p2 - html + 1);
        }
        strcpy(result, urlofthispage);
        result[i + 1] = 0;
        strncat(result, p1, p2 - p1);
        return (int)(p2 - html + 1);
      }
    }
  }
  return -1;
}

int filesize() {                                                          //get filesize
    struct stat st;
    char str[]="./temp.txt";
    stat(str,&st);                                                     // temp.txt is the file where wget fetch the html
    int file_size=(int)st.st_size;
    return file_size;
}

char *readcontent() {                                                     // get content from file
    char *fcontent =(char*)malloc(sizeof(char)*filesize());
    char str[]="./temp.txt";
    FILE *fp= fopen(str, "r+");
    char c;
    int i=0;
		c=getc(fp);
    while(c!=EOF)
    {
			fcontent[i++]=c;
      c=getc(fp);
    }
		fcontent[i]='\0';
    fclose(fp);
    return fcontent;
}

void transfer_File(char *f)                        // function to shift data from temp file to new actual file
{
	char File_Name[10]="./temp/",ch;
	char in[10];
	static int file_no=1;
	sprintf(in,"%d",file_no);
	strcat(File_Name,in);
	strcat(File_Name, ".txt");
        FILE *oldFileName = fopen("temp.txt" , "r");
	FILE *newFileName = fopen(File_Name, "w");
	/*
	while(*f!='\0')
	{
        putc(*f, newFileName);
        f++;
	}*/
	//putc('\n', newFileName);
	ch = getc(oldFileName);
	while(ch != EOF)
	{
	   putc(ch, newFileName);
	   ch = getc(oldFileName);
	}
	printf("File Transfered to %d.txt\n",file_no);
	file_no++;
	fclose(oldFileName);
	//fclose(newFileName);
}

int findkey(char *str)
{
	int i=0,key=0;
	while(str[i]!='\0')
	{
	  key+=(int)str[i++];
	}
	while(key>100)
	{
	  key/=10;
	}
	return key;
}

void putDatainHash(char **result,int depth)
{
struct	LinkList *temp=NULL,*p=NULL,*trav;
int index,i,f;
        for(i=0;i<10;i++)
        {
                temp=(struct LinkList*)malloc(sizeof(struct LinkList));
                temp->key=findkey(result[i]);
                temp->next=NULL;
                index=temp->key;
                strcpy(temp->url,result[i]);
                temp->depth=depth+1;
                if(Head==NULL)
                {
                        temp->next=NULL;
                        temp->flag=1;
                        temp->depth=depth;
                        Head=temp;
                        last=temp;
                        node[index].next=temp;
                        node[index].last=temp;
                        node[index].count=1;
                }
                else
                {
                        if(node[index].next==NULL)
                        {
                               last->next=temp;
                               temp->next=NULL;
                               temp->flag=0;
                               last=temp;
                               node[index].next=temp;
                               node[index].last=temp;
                               node[index].count=1;
                        }
                        else
                        {
                            trav=node[index].next;
                            if((strcmp(node[index].next->url,temp->url)==0)||(strcmp(node[index].last->url,temp->url)==0))
                                {
                                    free(temp);
                                    continue;
                                }
                            f=0;
                            while(trav->next!=NULL && trav->key==index)
                            {
                                if(strcmp(trav->url,temp->url)==0)
                                {
                                    free(temp);
                                    f=1;
                                    break;
                                }
                                trav=trav->next;
                            }
                            if(f)
                            continue;
                            if(trav->next==NULL)
                            last=temp;
                            temp->flag=0;
                            temp->next=trav->next;
                            trav->next=temp;
                            node[index].last=temp;
                            node[index].count++;
                        }
                }
        }
        struct LinkList *head=Head;
        while(head!=NULL)
        {
            printf("---linklist---%s\n",head->url);
            head=head->next;
        }
    }

void saveFile()
{
    FILE *fp= fopen("links.txt", "wb+");
    struct hash *hash=node;
    int i=0;
    while(i++<50){

		}
    fclose(fp);
}

/*
void loadFile()
{
    FILE *fp= fopen("links.txt", "r+");
    struct LinkList *head=Head;
    int i=0,n;
    char str[100];
    head=(struct LinkList*)malloc(sizeof(struct LinkList));
    n=fscanf(fp,"%s[^\n]", str);
    strcpy(head->url,str);
    head->next=NULL;
    while(i<100)
    {
        head->next=(struct LinkList*)malloc(sizeof(struct LinkList));
        head=head->next;
        n=fscanf(fp,"%s[^\n]", str);
        strcpy(head->url,str);
        head->next=NULL;
        i++;
    }
    head=Head;
    for(int i=0;i<100;i++)
    {
        printf("-newLinkList--%s\n",head->url);
        head=head->next;
    }
    fclose(fp);
}*/

char** fetch_links(char *url)   // function will take url from nextGenurl function and put it in the array and check duplicay wheater url exits in array or not!!
{
    int file_Size =filesize();
    file_Size++;

	char *filecontent = (char*)malloc(file_Size*sizeof(char));              // create pointer where all file content can store
	filecontent = readcontent();

	char *result = (char*)malloc(file_Size*sizeof(char));                   // create pointer where link will come from
    int lastpos=0,flag=0,count=0;

    char **linkarray;                                                       // create 2d array to store links
    linkarray=(char **)malloc(sizeof(char *)*100);                          // giving space for rows

    for(int i=0;i<100;i++)
      linkarray[i] = (char *)malloc(sizeof(char)*200);                      // each rows in allocated 200 char space

	lastpos=GetNextURL(filecontent,url,result ,0);
	strcpy(linkarray[count++],result);

    while(count<10)
    {
      lastpos=GetNextURL(filecontent,url,result ,lastpos);
      for(int j=0;j<count;j++)
      {
        if(strcmp(result,linkarray[j])==0)
        {
          flag=1;
          break;
        }
      }
      if(flag==0)
        strcpy(linkarray[count++],result);
      else
      {
       lastpos=GetNextURL(filecontent,url,result ,lastpos);
       flag=0;
      }
    }
	return(linkarray);
}

char* get_next_url(struct LinkList *head,int depth)
{
        struct LinkList *temp=head;
        while(temp!=NULL)
        {
                if(temp->flag==0)
                {
                        temp->flag=1;
                        return temp->url;
                }
                temp=temp->next;
        }
        return NULL;
}

int main(int argc,char *argv[])
{
    char **ptr;
    checkArguments(argc, argv);
    get_page(argv[1]);
    ptr=fetch_links(argv[1]);
    putDatainHash(ptr,1);
    transfer_File(argv[1]);

    int depth=2;
    struct LinkList *head=Head,*newlink;
    while(depth<50)
    {
        free(ptr);
        if(head->flag==0)
        {
            printf("----------------------Link is %s\n",head->url);
            get_page(head->url);
            ptr=fetch_links(head->url);
            putDatainHash(ptr,depth+1);
            transfer_File(head->url);
        }
        head=head->next;
        depth++;
    }
    return 0;
}
