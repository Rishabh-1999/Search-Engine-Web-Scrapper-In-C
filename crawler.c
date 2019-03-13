#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#define BASE_URL "www.chitkara.edu.in"
#define urlLength 200

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

char* changeurltochar(char *url) {                                // fuction to put null or \0 in last anad return
    int i = 0;
    while(1) {
        if(url[i++] == '\0')
            break;
        i++;
    }
    char *newurl = (char*)malloc(sizeof(char) * i+1);
    newurl = url;
    newurl[i] = '\0';
    return newurl;
}

void check_URL(char *url)                                        //check URL
{
    char v[urlLength]="wget --spider ";
    strcat(v,url);                                                 // CONCAT v and given url
    if(strcmp(BASE_URL,changeurltochar(url)) && !system(v))      //check whether given url is same as base url
    {
        printf("Invalid URL\n");                                //display seed_url is incorrect
        exit(0);                                                //exit if bse url is incorrect
    }
}

char checkDepth(char *d)                                       // check given depth is wrong
{
        int DEPTH=0;                                            //intialize varable DEPTH
        sscanf(d, "%d", &DEPTH);                                // change char to int
        if(DEPTH>5 || DEPTH<1)                                  //check the depth of given argument that it is in range
        {
            printf("Check your depth");                         //if incorrect show error_message
            exit(0);                                            // exit if depth is out of range
        }
}

void checkArguments(int argc,char *argv[])
{
    if(argc==4)                                                //check no of arguments
    {
        check_URL(argv[1])  ;                                   // check url
        checkDepth(argv[2]);                                    //check DEPTH
        check_Dir(argv[3]);                                     //check Directory
    }
    else
    {
        printf("Arguments not recieved not properly %d",argc);
        exit(0);
    }
}

void get_page(char *url)                                             //get data in file
{
    char urlbuffer[urlLength]="wget -O ./temp/1.txt ";
    strcat(urlbuffer,url);
    int n=system(urlbuffer);
}

void removeWhiteSpace(char* html)                                   //Remove White Space
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
	Listptr->next = NULL;
	for(int i=1;i<100;i++)
	{
	  obj = (struct LinkList*)malloc(sizeof(struct LinkList));
	  obj->url = links[i];
	  obj->next = NULL;
	  Listptr->next = obj;
	  Listptr = Listptr->next;
	}
	while(Head->next != 0)                               // printing all links!!
	{
 	   printf("%s\n", Head->url);
	   Head = Head->next;
	}
}

void fetch_links(char *url)   // function will take url from nextGenurl function and put it in the array and check duplicay wheater url exits in array or not!!
{
    int file_Size =filesize();
    file_Size++;

	char *filecontent = (char*)malloc(file_Size*sizeof(char));              // create pointer where all file content can store
	filecontent = readcontent();

	char *result = (char*)malloc(file_Size*sizeof(char));                   // create pointer where link will come from
    int lastpos=0,flag=0,count=0;

    char **linkarray;                                                       // create 2d array to store links

    linkarray=(char **)malloc(sizeof(char *)*101);                          // giving space for rows

    for(int i=0;i<100;i++)
      linkarray[i] = (char *)malloc(sizeof(char)*200);                      // each rows in allocated 200 char space

	lastpos=GetNextURL(filecontent,url,result ,0);
	strcpy(linkarray[count++],result);

    while(count<100)
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
      {
        strcpy(linkarray[count++],result);
      }
      else
      {
       lastpos=GetNextURL(filecontent,url,result ,lastpos);
       flag=0;
      }
    }
	putDatainLinkList(linkarray);
}

int main(int argc,char *argv[])
{
    checkArguments(argc, argv);
    get_page(argv[1]);
    fetch_links(argv[1]);
    return 0;
}
