#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#define BASE_URL "www.chitkara.edu.in"
#define DEPTH 5

//Structure of Link Node
struct Link
{
        int Link_key;
        int Link_flag;
        char *Link_url;
        struct Link *Link_next;
        int Link_depth;
}*head=NULL,*last=NULL;

// Structure of Link Hash Node
struct Hash
{
        struct Link* first;
        struct Link* last;
        int count;
}hash[50];

void put_links_in_list(char **result,int l,int depth);

//Function to remove or trim whiespace or space from Link
void removeWhiteSpace(char* html) {
  int i;
  char *buffer = malloc(strlen(html)+1), *p=malloc (sizeof(char)+1);
  memset(buffer,0,strlen(html)+1);
  for (i=0;html[i];i++) {
    if(html[i]>32)
    {
      sprintf(p,"%c",html[i]);
      strcat(buffer,p);
    }
  }
  strcpy(html,buffer);
  free(buffer); free(p);
}

//Function to Get Link from Array containing data of webpage
int GetNextURL(char* html, char* urlofthispage, char* result, int pos) {
  char c, *p1,* p2;
  int len, i, j;

  if(pos == 0) {
    removeWhiteSpace(html);
  }
  while (0 != (c = html[pos])) {
    if ((c=='<') &&
        ((html[pos+1] == 'a') || (html[pos+1] == 'A')))
                break;
    pos++;
  }
  if (c) {
    p1 = strchr(&(html[pos+1]), '=');
    if ((!p1) || (*(p1-1) == 'e') || ((p1 - html - pos) > 10))
      return GetNextURL(html,urlofthispage,result,pos+1);
    if (*(p1+1) == '\"' || *(p1+1) == '\'')
      p1++;
    p1++;
    p2 = strpbrk(p1, "\'\">");
    if (!p2)
      return GetNextURL(html,urlofthispage,result,pos+1);
    if (*p1 == '#')
      return GetNextURL(html,urlofthispage,result,pos+1);
    if (!strncmp(p1, "mailto:",7))
      return GetNextURL(html, urlofthispage, result, pos+1);
    if (!strncmp(p1, "http", 4) || !strncmp(p1, "HTTP", 4))
    {
      strncpy(result, p1, (p2-p1));
      return  (int)(p2 - html + 1);
    }
    else
    {
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

//Function to check if link is alreday present or not
int check_Already_Present(char *tmp,char **result,int l) {
        for(int i=0;i<l;i++)
                if(strcmp(result[i],tmp)==0)
                        return 1;
        return 0;
}

//Function that find URL and put in array of char
void get_urls(char *html,int file_size,char *urlofthispage,int depth) {
        int i,pos=0;
        char *result[11],*tmp;
        result[0]=urlofthispage;
        for(i=1;i<11 && pos<file_size;)
        {
                tmp=(char*)calloc(1,125);
                pos=GetNextURL(html , urlofthispage , tmp , pos);
                if(!(check_Already_Present(tmp,result,i)))
                {
                        result[i]=tmp;
                        i++;
                }
                else
                free(tmp);
        }
        put_links_in_list(result,i,depth);
}

//Function to copy content of file to Array
void copy_content(char *open,char *url,int depth) {
        struct stat st;
        stat(open,&st);
        int i=0,file_size=st.st_size;
        char *content=(char*)calloc(1,file_size),ch;
        FILE *temp=fopen(open,"r");
        while((ch=fgetc(temp))!=EOF)
                content[i++]=ch;
        content[i]='\0';
        get_urls(content,file_size,url,depth);
        free(content);
}

//Function to intialize Struct
void initialise_Struct() {
        for(int i=0;i<50;i++)
        {
                hash[i].first=NULL;
                hash[i].last=NULL;
                hash[i].count=0;
        }
}

//Function to generate key by modulus 50
int generate_Key(char *url) {
        int i=0,l=strlen(url),key=0;
        while(url[i]!='\0')
            key+=(int)(url[i++]);
        return key%50;
}

//Function to print data got from the linklist
void print_list(struct Link *head) {
    struct Link *temp=head;
    while(temp!=NULL)
    {
        printf("key = %d Link = %s depth = %d flag= %d\n",temp->Link_key,temp->Link_url,temp->Link_depth,temp->Link_flag);
        temp=temp->Link_next;
    }
}

//Function to save URL in File
void save_links_in_file(struct Link* head) {
        FILE *ptr=fopen("./links.txt","w");
        struct Link *temp=head;
        while(temp!=NULL) {
                fprintf(ptr,"%s ",temp->Link_url);
                fprintf(ptr,"%d ",temp->Link_key);
                fprintf(ptr,"%d ",temp->Link_depth);
                fprintf(ptr,"%d\n",temp->Link_flag);
                temp=temp->Link_next;
        }
}

//Function to retrive links from File stored
void retrieve_links_From_File() {
        initialise_Struct();  // Inirialise Struct
        head=NULL;
        FILE *ptr=fopen("./links.txt","r");
        char ch,c[150]={0},url[125]={0},*urlss=NULL;
        int i=0,j=0,k=0;
        struct Link *trav;
        while((ch=fgetc(ptr))!=EOF)
        {
                if(ch=='\n')
                {
                        c[i]='\0';
                        i=j=k=0;
                        struct Link *temp=(struct Link*)calloc(1,sizeof(struct Link));
                        while(c[k]!=' ')
                        {
                                url[j]=c[k];
                                j++;
                                k++;
                        }
                        url[j]='\0';
                        urlss=(char*)calloc(1,100);
                        strcpy(urlss,url);
                        temp->Link_key=generate_Key(url);
                        int key=temp->Link_key;
                        temp->Link_url=urlss;
                        k++;
                        j=0;
                        while(c[k]!=' ')
                                k++;
                        k++;
                        while(c[k]!=' ')
                        {
                                url[j]=c[k];
                                j++;
                                k++;
                        }
                        url[j]='\0';
                        temp->Link_depth=atoi(url);
                        j=0;
                        k++;
                        while(c[k]!='\0')
                        {
                                url[j]=c[k];
                                j++;
                                k++;
                        }
                        url[j]='\0';
                        temp->Link_flag=atoi(url);
                        if(head==NULL)
                        {       temp->Link_next=NULL;
                                head=temp;
                                last=temp;
                                hash[key].first=temp;
                                hash[key].last=temp;
                                hash[key].count=1;
                                continue;
                        }
                        else
                        {
                                if(hash[key].first==NULL)
                                {
                                        last->Link_next=temp;
                                        temp->Link_next=NULL;
                                        last=temp;
                                        hash[key].first=temp;
                                        hash[key].last=temp;
                                        hash[key].count=1;
                                }
                                else
                                {
                                        trav=hash[key].first;
                                        if((strcmp(hash[key].first->Link_url,temp->Link_url)==0)||(strcmp(hash[key].last->Link_url,temp->Link_url)==0))
                                        {
                                                free(temp);
                                                free(urlss);
                                                continue;
                                        }
                                int f=0;
                                while(trav->Link_next!=NULL&&trav->Link_next->Link_key==key)
                                {
                                        if(strcmp(trav->Link_url,temp->Link_url)==0)
                                        {
                                                free(temp);
                                                f=1;
                                                break;
                                        }
                                        trav=trav->Link_next;
                                }
                                if(f)
                                continue;
                                if(trav->Link_next==NULL)
                                last=temp;
                                temp->Link_next=trav->Link_next;
                                trav->Link_next=temp;
                                hash[key].last=temp;
                                hash[key].count++;
                              }
                        }
                        c[0]='\0';
                }
                else
                        c[i++]=ch;
        }
        print_list(head);
}

//Function to put links in LinkList
void put_links_in_list(char **result,int l,int depth) {
        struct Link *temp,*trav;
        for(int i=0;i<l;i++)
        {
                temp=(struct Link*)calloc(1,sizeof(struct Link));
                temp->Link_key=generate_Key(result[i]);
                int key=temp->Link_key;
                temp->Link_url=result[i];
                temp->Link_depth=depth+1;
                if(head==NULL)
                {       temp->Link_next=NULL;
                        temp->Link_flag=1;
                        temp->Link_depth=1;
                        head=temp;
                        last=temp;
                        hash[key].first=temp;
                        hash[key].last=temp;
                        hash[key].count=1;
                        continue;
                }
                else
                {
                        if(hash[key].first==NULL)
                        {
                               last->Link_next=temp;
                               temp->Link_next=NULL;
                               temp->Link_flag=0;
                               last=temp;
                               hash[key].first=temp;
                               hash[key].last=temp;
                               hash[key].count=1;
                        }
                        else
                        {
                            trav=hash[key].first;
                            if((strcmp(hash[key].first->Link_url,temp->Link_url)==0)||(strcmp(hash[key].last->Link_url,temp->Link_url)==0))
                                {
                                    free(temp);
                                    continue;
                                }
                            int f=0;
                            while(trav->Link_next!=NULL&&trav->Link_next->Link_key==key)
                            {
                                if(strcmp(trav->Link_url,temp->Link_url)==0)
                                {
                                    free(temp);
                                    f=1;
                                    break;
                                }
                                trav=trav->Link_next;
                            }
                            if(f)
                            continue;
                            if(trav->Link_next==NULL)
                            last=temp;
                            temp->Link_flag=0;
                            temp->Link_next=trav->Link_next;
                            trav->Link_next=temp;
                            hash[key].last=temp;
                            hash[key].count++;
                        }
                }
        }
        save_links_in_file(head);
}

//Function that put url in linklist
char* get_URL_from_linklist(struct Link *head,int depth) {
        struct Link *temp=head;
        while(temp!=NULL) {
                if(temp->Link_flag==0 && temp->Link_depth==depth)
                {
                        temp->Link_flag=1;
                        return temp->Link_url;
                }
                temp=temp->Link_next;
        }
        return NULL;
}

//Function that transfer data from temp file to orginal file
void transfer_File(char *url,int depth) {
        static int fileNo=1;
        char file_name[20]={0},ch[3],ch1;
        sprintf(ch, "%d" , fileNo);
        strcat(file_name,"./temp/");
        strcat(file_name,ch);
        strcat(file_name,".txt");
        FILE *oldFile=fopen("./temp.txt","r");
        FILE *newFile=fopen(file_name,"w");
        printf("\n----%s----\n",file_name);
        fprintf(newFile,"%s\n",url);
        fprintf(newFile,"%d\n",depth);

        while((ch1=fgetc(oldFile))!=EOF)
                putc(ch1,newFile);

        copy_content(file_name,url,depth);
        fclose(oldFile);
        fileNo++;
}

//Function to get WebPage or download webpage
void getpage(char *url,char *dir,int depth) {
          char urlbuffer[200]={0};
	        strcat(urlbuffer, "wget -O ./temp.txt ");
	        strcat(urlbuffer, url);
	        system(urlbuffer);
	        transfer_File(url,depth);
}

//Function to check Depth
int check_URL(char argv[]) {
	int i=0;
	char seed_url[strlen(argv)+5];
	while(argv[i]!='/'&&argv[i]!='\0')
	{
		seed_url[i]=argv[i];
		i++;
	}
	seed_url[i]='\0';
	if(strcmp(seed_url,BASE_URL)==0)
		return 1;
 	else
		return 0;
}

//Function to depth
int checkDepth(char *d) {
        int de=0;
        sscanf(d, "%d", &de);
        if(de>5 || de<1)
            return 0;
        return 1;
}

//Function to check Dir
int check_Dir(char *dir) {
  struct stat statbuf;
  if ( stat(dir, &statbuf) == -1 )
  {
    fprintf(stderr, "Invalid directory\n");
    exit(1);
  }
  if ( !S_ISDIR(statbuf.st_mode) )
  {
    fprintf(stderr, "Invalid directory entry. Your input isn't a directory\n");
    exit(1);
  }
  if ( (statbuf.st_mode & S_IWUSR) != S_IWUSR )
  {
    fprintf(stderr, "Invalid directory entry. It isn't writable\n");
    exit(1);
  }
  return 1;
}

//Function to check arguments are correct or Nost
void checkArguments(int argc,char* argv[]) {
            if(checkDepth(argv[2]))
                if(check_URL(argv[1]))
                    if(check_Dir(argv[3]))
                    {
                        printf("All Arguments are correct\n");
                        return ;
                    }
                else
                    printf("Invalid URL\n");
            else
                printf("Wrong input of Depth");
        else
            printf("Arguments not recieved not properly");
        exit(0);
}

//Main Function
int main(int argc,char *argv[])
{
        char c,*url=NULL;
        int depth=0;
        checkArguments(argc,argv);  //Function to check Arguments
        initialise_Struct();        //intialize struct
        printf("Capture New Data(n or N) or Retrive Saved Data(p || P) \n");
        scanf("%c",&c);
        if(c=='p' || c=='P')
              retrieve_links_From_File();   //Function to retrieve data from file
        else if(c=='n' || c=='N')
	        getpage(argv[1],argv[3],1);      //Function to get Webpage
        else
        {
          printf("Data entered wrong");
          exit(0);
        }
        sscanf(argv[2], "%d", &depth);    //Convert arguments into int
        while(depth<5)
        {
                while((url=get_URL_from_linklist(head,depth))!=NULL)
                        getpage(url,argv[3],depth+1);   //Function to get Webpage
                depth++;
        }
        print_list(head);     //Function to print Key,Link,Depth ,flag
	      return 0;
}
