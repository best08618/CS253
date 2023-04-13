#define MAXFILE 500000
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <ctype.h>
typedef struct
{
char* key;
int value;
} MAP;
void stringRemoveNonAlphaNum(char *str)
{
    unsigned long i = 0;
    unsigned long j = 0;
    char c;

    while ((c = str[i++]) != '\0')
    {
        if (isalnum(c))
        {
            str[j++] = c;
        }
      else{
        str[j++] = ' ';
      }
    }
    str[j] = '\0';
}

char** read_stopwords()
{
  char ** stopwords= malloc(500 * sizeof(char*));;
  char * buffer = NULL;
  size_t len = 0;
  ssize_t read;
  FILE * f = fopen ("../stop_words.txt", "rb");
  while ((read = getline(&buffer, &len, f)) != -1) {
  int i=0;
  char * token;
  token = strtok(buffer, ",");
  /* walk through other tokens */
   while( token != NULL ) {
      stopwords[i]=malloc(sizeof(char) * 10);
      strcpy(stopwords[i++], token);
      token = strtok(NULL, ",");
   }}
  return stopwords;
}
char* toLower(char* s) {
  for(char *p=s; *p; p++) *p=tolower(*p);
  return s;
}

char** read_file(char* file_path){
  FILE * f = fopen (file_path, "rb");
  char * line =  NULL;
  size_t len = 0;
  ssize_t read;
  char** contents = malloc(MAXFILE * sizeof(char*));
  int i =0;
  while ((read = getline(&line, &len, f)) != -1) {
    stringRemoveNonAlphaNum(line); 
    if (line!=NULL){
      char* token = strtok(line, " ");
      while (token != NULL){
        if(sizeof(token) >2){
          contents[i]=malloc(sizeof(char) * 1000);
          strcpy(contents[i++], toLower(token));
        }
        token = strtok(NULL, " ");
      }
    }
  }
  return contents;
}

char** remove_stopwords(char **contents){
  char ** stopwords = read_stopwords();
  int stop = 0;
  int i=0;
  int sw= 0;
  while(contents[i]){
    sw = 0;
    int j=0;
    while(stopwords[j]){
      if ((strcmp(contents[i],stopwords[j++])==0)){
        sw = 1; 
        break;
      }
    }
    if (sw==1){
      contents[i] = " ";
    }
    i++;
  }
  return contents;
}

MAP* count_fq(char** contents){
  MAP* fq = malloc(15000 * sizeof(MAP)); 
  int m=0;
  for(int i=0; i < MAXFILE;i++){
    if (!contents[i]) break;
    if (strlen(contents[i]) <2) continue;
    if (strcmp(contents[i]," ")!=0){
      m=0;
      while(fq[m].key){
        if (strcmp(fq[m].key,contents[i]) == 0 ){
          fq[m].value ++;
          for (int cmp=m-1;cmp>=0;cmp--){
            if (fq[m].value > fq[cmp].value){
              MAP imm;
              imm.value = fq[cmp].value;
              imm.key = fq[cmp].key;
              fq[cmp].key = fq[m].key;
              fq[cmp].value = fq[m].value;
              fq[m].key = imm.key;
              fq[m].value = imm.value;
              m = cmp;
            }
            else{
              break;
            }
          }
          break;
        }
        m++;
      }
      if (!fq[m].key){
        fq[m].key = contents[i];
        fq[m].value = 1;
      }
    }
  }
  return fq;
}

void top_print(MAP* fq){
  for(int i=0;i<25;i++){
    if (!fq[i].key) break;
    else{
      printf("%s - %d\n",fq[i].key,fq[i].value);
      fflush(stdout);
    }
  }
}


int main(int argc, char *argv[]){
  top_print(count_fq(remove_stopwords(read_file(argv[1]))));
}