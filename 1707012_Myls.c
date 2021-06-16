#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include<time.h>
#include<string.h>



void output_type(mode_t m)
{
    if(S_ISDIR(m))
        printf("%s","d");
    else if(S_ISREG(m))
        printf("%s","-");
    else if(S_ISLNK(m))
        printf("%s","l");
    else
        printf("%s","s");

    //printf("\t");

}

void output_permission(mode_t m)
{
    if(S_IRUSR & m)
        printf("%s","r");
    else
        printf("%s","-");
    if(S_IWUSR & m)
        printf("%s","w");
    else
        printf("%s","-");
    if(S_IXUSR & m)
        printf("%s","x");
    else
        printf("%s","-");

    if(S_IRGRP & m)
        printf("%s","r");
    else
        printf("%s","-");
    if(S_IWGRP & m)
        printf("%s","w");
    else
        printf("%s","-");
    if(S_IXGRP & m)
        printf("%s","x");
    else
        printf("%s","-");

    if(S_IROTH & m)
        printf("%s","r");
    else
        printf("%s","-");
    if(S_IWOTH & m)
        printf("%s","w");
    else
        printf("%s","-");
    if(S_IXOTH & m)
        printf("%s","x");
    else
        printf("%s","-");

    printf(" ");

}

void output_month(int t)
{
    if(t==1)
        printf("%3s ","Jan");
    else if(t==2)
        printf("%3s ","Feb");
    else if(t==3)
        printf("%3s ","Mar");
    else if(t==4)
        printf("%3s ","Apr");
    else if(t==5)
        printf("%3s ","May");
    else if(t==6)
        printf("%3s ","Jun");
    else if(t==7)
        printf("%3s ","Jul");
    else if(t==8)
        printf("%3s ","Aug");
    else if(t==9)
        printf("%3s ","Sep");
    else if(t==10)
        printf("%3s ","Oct");
    else if(t==11)
        printf("%3s ","Nov");
    else if(t==12)
        printf("%3s ","Dec");


}


void _ls(const char *dir,int op_a,int op_l,int op_t)
{

    int n, i=0,p;
      int total=0;
    struct dirent **list;
    char **d;
    struct stat fs;

    n = scandir(".", &list, 0, alphasort);
    if(n<0)
    {
        perror("scandir");
        return;
    }


    d = malloc(n* sizeof(char*));


    while(n--)
    {



        d[i] = malloc((n) * sizeof(char*));

        strcpy(d[i], list[n]->d_name);




            stat(d[i],&fs);
            if(d[i][0] != '.')
            total += fs.st_blocks;

           i=i+1;
    }
    p=i;

    if(op_t)
    {
        time_t m,n;
        for (int i = 0; i < p-1; i++)

            for (int j = 0; j < p-i-1; j++)
            {
                stat(d[j],&fs);
                m=fs.st_mtime;
                stat(d[j+1],&fs);
                n=fs.st_mtime;
                if (m<n)
                {
                    char *a;
                    a=d[j];
                    d[j]=d[j+1];
                    d[j+1]=a;
                }
            }
        for (int i = 0; i < p; i++)
        {
            if(d[i][0]=='.')
                continue;
            printf("%s\n",d[i]);
        }
    }
    else if(op_l)
    printf("total %d\n",total/2);



    //While the next entry is not readable we will print directory files
    while (i--&&!op_t)
    {

        //If hidden files are found we continue
        if (!op_a && d[i][0] == '.')
            continue;

        //stat(d[i],&fs);
        if(op_l)
        {


            stat(d[i],&fs);

            output_type(fs.st_mode);
            output_permission(fs.st_mode);
            printf("%ld ",fs.st_nlink);

            struct passwd *pw = getpwuid(fs.st_uid);
            struct group  *gr = getgrgid(fs.st_gid);
            printf("%s  ",pw->pw_name);
            printf("%s  ",gr->gr_name);

            printf("%-6ld  ",fs.st_size);

            //char date[10];
            //strftime(date, 10, "%d-%m-%y", localtime(&(fs.st_ctime)));
            //printf("%s  ",date);


            struct tm *time;
            time=localtime(&fs.st_mtime);
            output_month(time->tm_mon);


            printf("%-5d %.02d:%.02d   ",time->tm_mday,time->tm_hour,time->tm_min);
            //printf("%-20s\n", d[i]);
        }

        printf("%-20s\n", d[i]);



    }

}
int main(int argc, const char *argv[])
{
    if (argc == 1)
    {
        //printf("%d\n",argc);
        _ls(".",0,0,0);
    }
    else if (argc == 2)
    {
        //printf("%d\n",argc);
        if (argv[1][0] == '-')
        {
            //Checking if option is passed
            //Options supporting: a, l
            int op_a = 0, op_l = 0,op_t=0;
            char *p = (char*)(argv[1] + 1);
            while(*p)
            {
                if(*p == 'a') op_a = 1;
                else if(*p == 'l') op_l = 1;
                else if(*p == 't') op_t = 1;
                else
                {
                    perror("Option not available");
                    exit(EXIT_FAILURE);
                }
                p++;
            }
            _ls(".",op_a,op_l,op_t);
        }
    }
    //echo "export PATH=$PATH:$HOME/tools/web/visual-studio-code" >> ~/.bashrc
    return 0;
}

