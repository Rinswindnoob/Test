#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>
int main(void)
{

const char *url="https://api.stackexchange.com/2.2/search?order=desc&sort=activity&intitle=beautiful&site=stackoverflow";
static const char *headerfilename = "head.out";
  FILE *headerfile;
  static const char *bodyfilename = "body.out";
  FILE *bodyfile;

CURLcode res;
curl_global_init(CURL_GLOBAL_DEFAULT);

CURL *curl=curl_easy_init();
if(curl)
	{
	//Send the request to the url
	curl_easy_setopt(curl,CURLOPT_URL,url);
	curl_easy_setopt(curl,CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl,CURLOPT_SSL_VERIFYHOST, 0L);
	//These come in a compressed form, unpack them
	curl_easy_setopt(curl,CURLOPT_ENCODING,"gzip,deflate");
//open files for writing
headerfile = fopen(headerfilename,"w");
  if (headerfile == NULL) {
    curl_easy_cleanup(curl);
    return -1;
  }
  bodyfile = fopen(bodyfilename,"w+");
  if (bodyfile == NULL) {
    curl_easy_cleanup(curl);
    return -1;
  }
 
  /* put headers in the file descriptor, we do not need, but let it be  */ 
	curl_easy_setopt(curl,  CURLOPT_WRITEHEADER, headerfile);
	//put the rest in the file descriptor
 	curl_easy_setopt(curl,	CURLOPT_WRITEDATA,bodyfile);

	res=curl_easy_perform(curl);
	if(res != CURLE_OK)
	fprintf (stderr, "curl_easy_perform() failed: %s\n",
		curl_easy_strerror(res));
	
	
	}
	curl_easy_cleanup(curl);
	//Close file
	fclose(bodyfile);
	fclose(headerfile);
char str[65000];
char *istr;
char *last;
char sep[10]="},{";

FILE *fpt;
if ((fpt = fopen("body.out", "r"))==NULL) {
printf("Cannot open file.\n");
exit(1);
}


FILE *fps;
if ((fps = fopen("body1.out", "w"))==NULL) {
printf("Cannot open file.\n");
exit(1);
}
	//Since the data was written in one line, we split all the data into several lines.
	fgets(str,65000,fpt);
	istr=strtok_r(str,sep,&last);
	//We write the received lines to a file
	while(istr!=NULL)
	{
	fputs(istr,fps);
	fputs("\n",fps);
	istr=strtok_r(NULL,sep,&last);
	}
	//Close file
	fclose(fps);
	fclose(fpt);


/*Next, the data in the file will be processed to the type and content we need*/
	char *estr;
	char tmp[20]="\"display_name\"";
	char tmp1[20]="\"link\"";
	char tmp2[20]="\"is_answered\"";
	char tmp3[20]="\"title\"";
	char stmp[1000],stmp1[1000],stmp2[1000],stmp3[1000];
	int i=0;
	char *stp;
	char stf[65000];
	
FILE *finalfp;
if ((finalfp = fopen("body1.out", "r"))==NULL) {
printf("Cannot open file.\n");
exit(1);
}

FILE *final;
if ((final = fopen("final.out", "w"))==NULL) {
printf("Cannot open file.\n");
exit(1);
}

	fputs("Title\tAutor\tAnswered?\ttLink",final);
	fputs("\n",final);
//start an endless loop to process the data line by line
	 while (1)
   {
      // Reading one line from a file
      estr = fgets (stf,65000,finalfp);
	stp=strtok_r(stf,"\n",&last);
	
      //Check for end of file or read error
      if (estr == NULL)
      {
         // We check what exactly happened: the file ran out
         // or is it a read error
         if ( feof (finalfp) != 0)
         {  
            //If the file is over, display the completion message
            //reading and exit the endless loop
            printf ("\nЧтение файла закончено\n");
            break;
         }
         else
         {
            //If an error occurred while reading, display the message
            //an error and exit the infinite loop
            printf ("\nОшибка чтения из файла\n");
            break;
         }
	}
	//If the file did not end and there was no read error
	//compare 2 strings and if they match, write the string
	if(strncmp(tmp,estr,10)==0)
		{strcpy(stmp,&stp[15]);i++;}
	if(strncmp(tmp1,estr,3)==0)
		{strcpy(stmp1,&stp[7]);i++;}
	if(strncmp(tmp2,estr,10)==0)
		{strcpy(stmp2,&stp[14]);i++;}
	if(strncmp(tmp3,estr,5)==0)
		{strcpy(stmp3,&stp[8]);i++;}

	//We write the received lines to the file in a certain order
	if(i==5)
	{
	fputs(stmp3,final);
	fputs("\t",final);
	fputs(stmp,final);
	fputs("\t",final);
	fputs(stmp2,final);
	fputs("\t",final);
	fputs(stmp1,final);
	fputs("\n",final);
	i=0;
	}
	
   }
	fclose(final);
	fclose(finalfp);
curl_global_cleanup();//clear Curl
return 0;
}
