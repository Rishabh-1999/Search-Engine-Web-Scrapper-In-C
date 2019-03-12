#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#define BASE_URL "www.chitkara.edu.in"

struct LinkList                              // structure for linked list
{
	char* url;
	int depth;
	struct LinkList *next;
	struct LinkList *back;
}*Head;

void check_Dir(char *dir)                           //Check Dir and Check permission
{
  struct stat statbuf;                              //Check Dir
  if ( stat(dir, &statbuf) == -1 )
  {
    fprintf(stderr, "Invalid directory\n");
    exit(1);
  }
  if ( !S_ISDIR(statbuf.st_mode) )                    //Both check if there's a directory and if it's writable
  {
    fprintf(stderr, "Invalid directory entry. Your input isn't a directory\n");
    exit(1);
  }
  if ( (statbuf.st_mode & S_IWUSR) != S_IWUSR )         //Check Permission
  {
    fprintf(stderr, "Invalid directory entry. It isn't writable\n");
    exit(1);
  }
}

void check_URL()                                    //check URL
{
    char v[]="wget --spider ";
    char b[]=BASE_URL;
    strcat(v,b);
    if(!system(v))
        printf("Valid URL\n");
    else
    {
        printf("Invalid URL\n");
        exit(0);
    }
}

void checkArguments(int argc,char *argv[])
{
    if(argc==4)                                                //check no of arguments
    {
        if(strcmp(BASE_URL,argv[2]))                          //check whether given url is same as base url
        {
            printf("SEED_URL is incorrect");                   //display seed_url is incorrect
            exit(0);                                           //exit if bse url is incorrect
        }
        int DEPTH=0;
        sscanf(argv[2], "%d", &DEPTH);
        if(DEPTH<5 && DEPTH<1)                                  //check the depth of given argument that it is in range
        {
            printf("Check your depth");                         //if incorrect show error_message
            exit(0);                                            // exit if depth is out of range
        }
        check_Dir(argv[3]);
        check_URL()  ;          // test directory
    }
    else
    printf("Arguments not recieved not properly");
}

void get_page(char *url)                            //get data in file
{
    char urlbuffer[300]="wget -O ./temp/1.txt ";
    strcat(urlbuffer,url);
    int n=system(urlbuffer);
}

void removeWhiteSpace(char* html)                //REmove White Space
{
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

int GetNextURL(char* html, char* urlofthispage, char* result, int pos)
{
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
          printf("%s\n",result);
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
            printf("%s\n",result);
            return (int)(p2 - html + 1);
        }
        if ((i <= 6)||(i > j)) {
          strcpy(result, urlofthispage);
          result[len] = '/';
          strncat(result, p1, p2 - p1);
            printf("%s\n",result);
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

int filesize()                                              //get filesize
{
    struct stat st;                                      //variable which will count length of file.
    stat("./temp/1.txt",&st);                             // temp.txt is the file where wget fetch the html
    int file_size=(int)st.st_size;
    return file_size;
}

char *readcontent()                           // get content from file
{
    int file_size=filesize();
    char *fcontent =(char*)malloc(sizeof(char)*file_size);
    FILE *fp= fopen("1.txt", "r+");
    char c;
    int i=0;
    while(1)
    {
        c=getc(fp);
        if(feof(fp))
            break;
        fcontent[i++]=c;
    }
        fclose(fp);
    return fcontent;
}

char transfer_File()                        // function to shift data from temp file to new actual file
{
	char File_Name[10],ch;
	static int file_no=1;
	sprintf(File_Name,"%d",file_no);
	strcat(File_Name, ".txt");
    FILE *oldFileName = fopen("temp.txt" , "r");
	FILE *newFileName = fopen(File_Name, "w");
	ch = getc(oldFileName);
	while(ch != EOF)
	{
	   putc(ch, newFileName);
	   ch = getc(oldFileName);
	}
	file_no++;
	printf("New File Created");
	fclose(oldFileName);
	fclose(newFileName);
}

void putDatainLinkList(char **links)                    // function to put data in linklist
{
	struct LinkList *obj, *Listptr;
	Head = (struct LinkList*)malloc(sizeof(struct LinkList));
	Listptr = Head;
	Listptr->url = links[0];
	Listptr->next = 0;
	for(int i=1;i<100;i++)
	{
	  obj = (struct LinkList*)malloc(sizeof(struct LinkList));
	  obj->url = links[i];
	  obj->next = 0;
	  Listptr->next = obj;
	  Listptr = Listptr->next;
	}
	while(Head->next != 0)  // printing all links!!
	{
 	   printf("%s\n", Head->url);
	   Head = Head->next;
	}
}

void fetch_Url(char *url)   // function will take url from nextGenurl function and put it in the array and check duplicay wheater url exits in array or not!!
{
    int File_Size =filesize();
    File_Size++;

	char *data = (char*)malloc(File_Size*sizeof(char));
	data = readcontent();

	char *result = (char*)malloc(File_Size*sizeof(char));
        int ans=0,flag=0,len=100,l=0;
        char **links;

    links=(char **)malloc(sizeof(char *)*101);
    for(int i=0;i<100;i++)
      links[i] = (char *)malloc(sizeof(char)*200);

	ans=GetNextURL(data,url,result ,0);
	strcpy(links[l++],result);

    while(l<100)
    {
      ans=GetNextURL(data,url,result ,ans);

      for(int j=0;j<l;j++)
      {
        if(strcmp(result,links[j])==0)
        {
          flag=1;
          break;
        }
      }
      if(flag==0){
       strcpy(links[l++],result);
      }
      else{
       ans=GetNextURL(data,url,result ,ans);
       flag=0;
      }
    }
	putDatainLinkList(links);
}

int main(int argc,char *argv[])
{
    checkArguments(argc, argv);
    get_page(argv[2]);
    fetch_Url(argv[1]);
    return 0;
}
