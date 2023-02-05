#include<stdio.h>
#include<ncurses.h>
//#include<process.h>
#include<stdlib.h>
#include<string.h>
//#include<dir.h>
#include <sys/types.h>
#include <dirent.h>
#include<errno.h>

#define FILE_SIZE 10000
#define LINE_SIZE 1000
#define NAME_SIZE 50      //Maximum Length of name of folder

// char* token = (char*)malloc(sizeof(char)*1000);

int a[1000] = {0};
int b[1000] = {0}; 
int word[1000] = {0};
char word1[100][100] = {'\0'};
int wordcounter1 = 0;
char word2[100][100] = {'\0'};
int wordcounter2 = 0;

void PtrToInt(FILE *f, int t){
    char c;
    if (t==-1){
        while((c = fgetc(f))!=EOF);
    }
    else{
        while(t!=0){
            c = fgetc(f);
            if (c == EOF){
                break;
            }
            t--;
        }
    }
}

int PosToPtr(char* address, int line, int pos){
    FILE *f;
    int t=0;
    char  c;
    f = fopen(address, "r");
    while(1){
        if((line == 1) && (pos == 0)){
            
            pos = -1;
            fclose(f);
            return t;
        }
        
        
        if ((line == 1) && (pos > 0)){
            pos--;
        }
        
        c = fgetc(f);
        
        
        if (c == '\n'){
            line--;
        }
        if (c == EOF){
            return -1;
        }
        t++;

    }
}

void CopyFile(char* address1, char* address2, int from, int to){
    FILE *F1;
    FILE *F2;
    // printf( "%s", address1);
    if ((F1 = fopen(address1, "r+")) == NULL){
        printf("kharabe!");
    }
    F2 = fopen(address2, "a+");
    char c;
    PtrToInt(F1, from);
    
    if (to == -1){
        while ((c = fgetc(F1))!=EOF){
            // exit(1);
            fputc((int)c, F2);
            // c = fgetc(F1);
            
        }
    }
    else {
        for(int i = from; i <= to; i++){
            c = fgetc(F1);
            fputc((int)c, F2);
        }

    }
    
    fclose(F1);
    fclose(F2);
}

int CreateFile(char* address){
    char* check;
    // printf("%s\n", address);
    check=(char*)malloc(1000*sizeof(char));
    *check = '.';
    *(check + 1) = '/';
    for(int i = 2; i < NAME_SIZE; i++){
        if (*(address+i)=='/'){
            *(check+i)='\0';
            // printf("%s 1\n", check);
            mkdir(check, 0777);
        }
        *(check+i) = *(address+i);
    }
    FILE *f;
    
    if ((f = fopen(check, "r")) != NULL){
        // printf("Error : Exist file with same name!\n") ;
        fclose(f);
        return 0;
    }
    // printf("%s1", check)
    // exit(1);
    // exit(1);
    f = fopen(check, "w");
    fclose(f);

    return 1;
    // MakeCopyForSave(address);
}

void ClearFile(char* address){
    FILE *f;
    f= fopen(address, "w");
    fclose(f);
}

void insertstr(char* address, char* str, int line, int pos){
    int count = PosToPtr(address, line, pos);
    if ((line!=1)||(pos != 0)){
        CopyFile(address, "komaki.txt", 0, count);
    }
    FILE *komaki;
    komaki = fopen("komaki.txt", "a+");
    fputs(str, komaki);
    fclose(komaki);
    CopyFile(address, "komaki.txt", count+1, -1);
    // MakeCopyForSave(address);
    remove(address);
    rename("komaki.txt", address);

}

void cat(char* address){
   FILE *fp ;
   char c ;
   fp = fopen ( address, "r" ) ; 
   while ( 1 )
   {
        c = fgetc ( fp ) ; // reading the file
        if ( c == EOF ){
            break ;
        }
        printf ( "%c", c ) ;
   }
   printf("\n");
   fclose ( fp ) ; // Closing the file
}

void Remove(char* address, int line, int pos, int size, char direction){
    
    int count1 = PosToPtr(address, line, pos);
    
    int count2;
    if(direction == 'f'){
        
        count2 = count1 + size + 1;
        // printf("%d %d\n", count2, count1);
        
        CopyFile(address, "komaki.txt", 0, count1);
        CopyFile(address, "komaki.txt", count2, -1);
        
        // SaveChangeFile(address);
        remove(address);
        rename("komaki.txt", address);
        // ClearFile(address);
        
        // CopyFile("komaki.txt", address, 0, -1);
        
        // remove("komaki.txt");
        
    }
    else if(direction == 'b'){
        count2 = count1 - size - 1;
        // printf("%d %d", count2, count1);
        CopyFile(address, "komaki.txt", 0, count2);
        CopyFile(address, "komaki.txt", count1, -1);
        // SaveChangeFile(address);
        remove(address);
        rename("komaki.txt",address);
    }
}

void Copy(char* address, int line, int pos, int size, char direction){
    int count1 = PosToPtr(address, line, pos);
    int count2;
    if (direction == 'f'){
        count2 = count1 + size;
        count1 += 1;
        printf("%d %d\n", count2, count1);
        ClearFile("Clipboard.txt");
        CopyFile(address, "Clipboard.txt", count1, count2);
    }
    if (direction == 'b'){
        count2 = count1 - size;
        count1 -= 1;
        ClearFile("Clipboard.txt");
        CopyFile(address, "Clipboard.txt", count2, count1);
    }
}

void Cut(char* address, int line, int pos, int size, char direction){
    Copy( address, line, pos, size, direction);
    Remove(address, line, pos, size, direction);
}

void Paste(char* address, int line, int pos){
    // printf("%s", address);
    int count = PosToPtr(address, line, pos);
    // exit(1);
    CopyFile(address, "komaki.txt", 0, count);
    CopyFile("Clipboard.txt", "komaki.txt", 0, -1);
    CopyFile(address, "komaki.txt", count+1, -1);
    // SaveChangeFile(address);
    remove(address);
    rename("komaki.txt", address);
}

void Grep(char* address, char* str , char x){
    char file[10000] = {0};
    int line = 1;
    int check;
    int t = 0;
    int s = 0;
    FILE *f;
    f =fopen(address, "r+");
    fread(file, 1000, 1, f);
    int count = strlen(file);
    // printf("%d", count);
    for (int i = 0; i < count; i++){
        // printf("salam\n");
        if (*(file+i) == (*str)){
            check = 1;
            for (int j = 1; j < strlen(str); j++){
                // printf("%c", *(file+i+j));
                if ((*(str+j) != *(file + i + j)) || (i + j == count)){
                    check = 0;
                    break;
                }
            }
            if (check == 1){
                t++;
                if ((x == 'x')&&(s == 0)){
                    // fclose(f);
                    printf("%s: ",address);
                    PrintFileLine(address, line);
                    s = 1;
                }
            }
        }
        if (*(file + i)=='\n'){
            s = 0;
            line++;
        }
    }
    fclose(f);
    if (x == 'C'){
        printf("%d\n",t);
    }
    else if (t==0) {
        printf("-1\n") ;
    }
    else if(x == 'I'){
        printf("%s\n", address);
    }
}   

void PrintFileLine(char* address, int line){
    int count = PosToPtr(address, line, 0);
    FILE *f;
    f = fopen(address, "r+");
    PtrToInt(f, count);
    char c = fgetc(f);
    while((c != '\n')&&(c!=EOF)){
        printf("%c", c);
        c = fgetc(f);
    }
    printf("\n");
    fclose(f);
}

void SaveChangeFile(char* address){
    char address2[NAME_SIZE] = "./root2";
    char address3[NAME_SIZE] = ".";
    
    strcat(address2, address);
    strcat(address3, address);
    ClearFile(address2);
    CopyFile(address3, address2, 0, -1);
    // exit(1);
}

void Undo(char* address){
    char address2[NAME_SIZE] = "./root2";
    char address3[NAME_SIZE] = ".";
    strcat(address2, address);
    strcat(address3, address);
    // printf("%s", address2);
    CopyFile(address3,"komaki.txt", 0, -1);
    ClearFile(address3);
    CopyFile(address2, address3, 0, -1);
    ClearFile(address2);
    CopyFile("komaki.txt", address2, 0, -1);
    remove("komaki.txt");
}

void Auto_Indent(char* address){
    FILE *f;
    FILE *k;
    char file[10000] = {0};
    f = fopen(address, "r+");
    k = fopen("komaki.txt", "a+");
    fread(file, 1000, 1, f);
    fclose(f);
    remove(address);
    int count = strlen(file);
    int tab = 0;
    // char c = fgetc(f);
    int t = 0;
    for(int i = 0; i < count; i++){
        if ((*(file+i) == ' ') || (*(file+i) == '\n')){
            int z = i;
            while((*(file+z) == ' ')|| (*(file+z) == '\n')){
                z++;
            }
            if (*(file + z) == '{'){
                if ((i >= 1) && (*(file+i-1) != '{') && (*(file+i-1) != '}'))
                fputc((int)(*(file + i)), k);    
                i = z;
            }
        }
        if(*(file+i)=='{'){
            if ((i >= 1) && (*(file+i-1) != '{') && (*(file+i-1) != '}')&& (*(file+i-1) != ' ')){
                fputc((int)(' '), k);
            }
            fputc((int)(*(file + i)), k);
            tab++;
            // CopyFile(address, "komaki.txt", 0, t);
            // printf("%d ", t);
            fputc(10, k);
            for (int j = 0; j < tab; j++){
                fputc(9, k);
            }
            // CopyFile(address, "komaki.txt", t+1, -1);
            // fclose(f);
            // remove(address);
            // rename("komaki.txt", address);
            // f = fopen(address, "r+");
            // fseek(f, 0, SEEK_SET);
            // PtrToInt(f, t);
        }
        else if(*(file+i) == '}'){
            tab--;
            fputc(10, k);
            for (int j = 0; j < tab; j++){
                fputc(9, k);
            }
            fputc((int)(*(file + i)), k);
            fputc(10, k);
        }

        else{
            fputc((int)(*(file + i)), k);
        }
        t++;
    }
    // printf("sasad");
    fclose(k);
    rename("komaki.txt", address);

}

int LenghOfLine(char* address, int line){
    int count = PosToPtr(address, line, 0);
    FILE *f;
    f = fopen(address, "r");
    PtrToInt(f, count);
    char  c = fgetc(f);
    int t = 0;
    while((c != '\n') && (c!=EOF)){
        c = fgetc(f);
        t++;
        
    }
    fclose(f);
    return (t-1);
}

int NumberOfLine(char* address){
    FILE *f;
    int line = 1;
    f = fopen(address, "r");
    char c;
    while((c = fgetc(f))!= EOF){
        if (c == '\n'){
            line++;
        }
    }
    fclose(f);
    return line;
}

void Compare(char* address1, char* address2){
    FILE *f1;
    FILE *f2;
    int check;
    int count1, count2;
    f1 = fopen(address1, "r");
    f2 = fopen(address2, "r");
    int lline1, lline2;
    char c1, c2;
    int tline1 = NumberOfLine(address1);
    int tline2 = NumberOfLine(address2);
    // printf("%d %d", tline1, tline2);
    if (tline1 < tline2){
        for (int i = 1; i <= tline1; i++){
            check = 1;
            lline1 = LenghOfLine(address1, i);
            lline2 = LenghOfLine(address2, i);
            // printf("%d %d\n", lline1, lline2);
            if (lline1 != lline2){
                printf("==========#%d==========\n", i);
                PrintFileLine(address1, i);
                PrintFileLine(address2, i);
                count1 = PosToPtr(address1, i+1, 0);
                fseek(f1, 0, SEEK_SET);
                PtrToInt(f1, count1);
                count2 = PosToPtr(address2, i+1, 0);
                fseek(f2, 0, SEEK_SET);
                PtrToInt(f2, count2);
            }
            else{
                for (int j = 0; j < lline1+1; j++){
                    c1 = fgetc(f1);
                    c2 = fgetc(f2);
                    // printf(" %d %c %c\n", j, c1, c2);
                    if (c1 != c2){                        
                        check = 0;
                        break;
                    }
                }
                if (check==0){
                    printf("==========#%d==========\n", i);
                    PrintFileLine(address1, i);
                    PrintFileLine(address2, i);
                    count1 = PosToPtr(address1, i+1, 0);
                    fseek(f1, 0, SEEK_SET);
                    PtrToInt(f1, count1);
                    count2 = PosToPtr(address2, i+1, 0);
                    fseek(f1, 0, SEEK_SET);
                    PtrToInt(f2, count2);                    
                }
            }
        }
        printf("==========#%d-#%d==========\n", tline1+1,  tline2);
        for(int i = (tline1+1); i <= tline2; i++){
            PrintFileLine(address2, i);
        }
    }
    else if (tline1 > tline2){
        for (int i = 1; i <= tline2; i++){
            check = 1;
            lline1 = LenghOfLine(address1, i);
            lline2 = LenghOfLine(address2, i);
            if (lline1 != lline2){
                printf("==========#%d==========\n", i);
                PrintFileLine(address1, i);
                PrintFileLine(address2, i);
                count1 = PosToPtr(address1, i+1, 0);
                fseek(f1, 0, SEEK_SET);
                PtrToInt(f1, count1);
                count2 = PosToPtr(address2, i+1, 0);
                fseek(f2, 0, SEEK_SET);
                PtrToInt(f2, count2);
            }
            else{
                for (int j = 0; j < lline2+1; j++){
                    c1 = fgetc(f1);
                    c2 = fgetc(f2);
                    // printf("%d %c %c \n", j, c1, c2);
                    if (c1 != c2){
                        
                        check = 0;
                        break;
                    }
                }
                if (check==0){
                    printf("==========#%d==========\n", i);
                    PrintFileLine(address1, i);
                    PrintFileLine(address2, i);
                    count1 = PosToPtr(address1, i+1, 0);
                    fseek(f1, 0, SEEK_SET);
                    PtrToInt(f1, count1);
                    count2 = PosToPtr(address2, i+1, 0);
                    fseek(f2, 0, SEEK_SET);
                    PtrToInt(f2, count2);                    
                }
            }
        }
        printf(">>>>>>>>>>#%d-#%d>>>>>>>>>>\n", tline2+1,  tline1);
        for(int i = (tline2+1); i <= tline1; i++){
            PrintFileLine(address1, i);
        }
    }
    else{
        for (int i = 1; i <= tline2; i++){
            check = 1;
            lline1 = LenghOfLine(address1, i);
            lline2 = LenghOfLine(address2, i);
            if (lline1 != lline2){
                printf("==========#%d==========\n", i);
                PrintFileLine(address1, i);
                PrintFileLine(address2, i);
                count1 = PosToPtr(address1, i+1, 0);
                fseek(f1, 0, SEEK_SET);
                PtrToInt(f1, count1);
                count2 = PosToPtr(address2, i+1, 0);
                fseek(f2, 0, SEEK_SET);
                PtrToInt(f2, count2);
            }
            else{
                for (int j = 0; j < lline2+1; j++){
                    c1 = fgetc(f1);
                    c2 = fgetc(f2);
                    if (c1 != c2){
                        check = 0;
                        break;
                    }
                }
                if (check==0){
                    printf("==========#%d==========\n", i);
                    PrintFileLine(address1, i);
                    PrintFileLine(address2, i);
                    count1 = PosToPtr(address1, i+1, 0);
                    fseek(f1, 0, SEEK_SET);
                    PtrToInt(f1, count1);
                    count2 = PosToPtr(address2, i+1, 0);
                    fseek(f2, 0, SEEK_SET);
                    PtrToInt(f2, count2);                    
                }
            }
        }
    }
    fclose(f1);
    fclose(f2);
}

void Tree(char *address, const int root, int depth){
    if (depth<-1){
        printf("invalid ...");
        return;
    }
    char path[NAME_SIZE];
    struct dirent *dp;
    DIR *dir = opendir(address);

    if (!dir)
        return;
    if (depth == 0){
        return;
    }

    while ((dp = readdir(dir)) != NULL){
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0){
            for (int i=0; i<root; i++) {
                if (i%2 == 0 || i == 0){
                    printf("%c", 179);
                }
                else{
                    printf(" ");
                }
            }

            printf("%c%c%s\n", 195, 196, dp->d_name);

            strcpy(path, address);
            strcat(path, "/");
            strcat(path, dp->d_name);
            //depth--;
            if (depth==-1){
                Tree(path, root + 2, depth);
            }
            else{
                Tree(path, root + 2, depth - 1);
            }
        }
    }

    closedir(dir);
}

int LetterByWord(char* str, int counter){
    int word = 1;
    for (int i = 0; i <= counter ; i++){
        if (((*(str + i)==' ') && (*(str + i + 1) !=' '))||(*(str + i)==' ') && (*(str + i + 1) !='\n')||((*(str + i)=='\n') && (*(str + i + 1) !=' '))||((*(str + i)=='\n') && (*(str + i + 1) !='\n'))){
            word++;
        }
    }
    return word;
}

void SearchStr(char* address, char* str){
    int t = 1;
    char file[10000] = {0};
    int s;
    char str1[10000]; 
    int size = strlen(str);
    // char file[10000];
    // printf("%s",  str);
    int count = 0;
    if (*str == '*'){
        s = 1;
        for(int i = 1; i < size; i++){
            *(str1 + i - 1) = *(str + i);
        }
    }
    else if ((*(str+ size -1) == '*')&&(*(str+ size -2) != '\\')){
        s = 2;
        // printf("%s",  str);
        for(int i = 0; i < size - 1; i++){
            // printf("%c", *(str+i-1));
            *(str1 + i) = *(str + i );
        }
    }
    else{
        s = 0;
        strcpy(str1, str);
    }
    // printf("%d %s", s, str1);
    FILE *f;
    f = fopen(address,  "r");
    fread(file, 10000, 1, f);

    int k = strlen(file);
    for (int i = 0; i < k; i++){
        if (*(file+i)==*str1){
            int check = 1;
            for (int j = 1; j < strlen(str1); j++){
                if ((*(str1 + j) != *(file + i + j))|| (i+j == k)){
                    check = 0;
                    break;
                }
            }
            // printf("%d", check);
            if (check == 1){
                // printf("%d",s);
                if (s == 0){
                    a[t] = i;
                    word[t] = LetterByWord(file, i);
                    b[t] = i + strlen(str1) -1;
                    t++;
                }
                else if (s == 1){
                    // printf("sas");
                    int z = i;
                    while((*(file + z) != ' ')&&(z != 0)&&(*(file+z) != '\n')){
                        z--;
                    }
                    a[t] = z;
                    word[t] = LetterByWord(file, z);
                    b[t] = i + strlen(str1) - 1;
                    t++;
                }
                else{
                    int z = i + strlen(str1) - 1 ;
                    while(((*(file + z)) != ' ')&&(*(file + z) != '\n')&&(*(file + z) != EOF)){
                        // printf("%c %d\n", *(file+z), z);
                        z++;
                    }
                    a[t] = i;
                    word[t] = LetterByWord(file, i);
                    b[t] = z;
                    t++;
                }
            }
        }
    }
    a[0] = t - 1; 
    b[0] = t - 1;
    word[0] = t-1;
    fclose(f);
}

int MoghWildWord(char* pattern, char* word){
    int size1 = strlen(pattern);
    int size2 = strlen(word);
    int check ;
    int k;
    if (*pattern == '*'){
        for(int j = 0; j < size2; j++){
            k = j;
            if (*(pattern+1)==*(word+k)){
                check = 1;
                k++;
                for (int i = 2 ; i <= (size1 -1); i++){
                    if (*(pattern + i) != *(word+k)){
                        check = 0;
                        break;
                    }
                }
            }
            if (check == 1){
                return 1;
            }
        }
        return 0;
    }
    else  if (*(pattern + size1 - 1) == '*'){
        for(int j = 0; j < size2; j++){
            k = j;
            if (*(pattern+1)==*(word+k)){
                check = 1;
                k++;
                for (int i = 2 ; i <= (size1 -1); i++){
                    if (*(pattern + i) != *(word+k)){
                        check = 0;
                        break;
                    }
                }
            }
            if (check == 1){
                return 1;
            }
        }
        return 0;
    }
    else{
        if (strcmp(pattern, word) != 0){
            return 0;
        }
        return 1;
    }

}

int NumOfWord(char* str){
    int t = 1;
    for (int i = 0; i < strlen(str); i++){
        if (((*(str+i))==' ')||((*(str+i))=='\n')){
            if ((((*(str+i+1))!=' ')||((*(str+i))=='\n'))){
                t++;
            }
        }
    }
    return t;
}

void WordOfStr1(char* str, int n){
    int i = 0;
    int j = 0;
    while((n!=1) && (*(str+i)!='\0')){
        if (((*(str+i))==' ')||((*(str+i))=='\n')){
            if ((((*(str+i+1))!=' ')||((*(str+i))=='\n'))){
                n--;
            }
        }
        i++;
    }
    while((*(str + i + j)!=' ') && (*(str + i + j)!='\0') && (*(str + i + j)!='\n')){
        word1[wordcounter1][j] = *(str + i + j);
        j++;
    }
    wordcounter1++;
    
}

void WordOfStr2(char* str, int n){
    int i = 0;
    int j = 0;
    while((n!=1) && (*(str+i)!='\0')){
        if (((*(str+i))==' ')||((*(str+i))=='\n')){
            if ((((*(str+i+1))!=' ')||((*(str+i))=='\n'))){
                n--;
            }
        }
        i++;
    }
    while((*(str + i + j)!=' ') && (*(str + i + j)!='\0') && (*(str + i + j)!='\n')){
        
        word2[wordcounter2][j] = *(str + i + j);
        j++;
    }
   wordcounter2++;
}

void SearchPattern(char* address, char* str){
    // int j = 1;
    char x[1000];
    int count = 0;
    FILE *f;
    f = fopen(address,  "r");
    int t = NumOfWord(str);
    // printf("%d", t);
    fread(x, 1000, 1, f);
    int k = NumOfWord(x);
    for (int i = 0; i < t; i++){
        WordOfStr1(str, i+1);
        // printf("%s\n", word1[i]);
    }
    // printf("file world.......................................\n");
    for (int i = 0; i < k; i++){
        WordOfStr2(x, i+1);
        // printf("%s\n", word2[i]);
    }
    int check;
    int  i = 1;
    while(i != k){
        if (MoghWildWord(word1[0], word2[i-1])==1){
            int save = i;
            i++;
            int jk = i;
            check = 1;
            for (int j = 1; j < t; j++){
                if ((MoghWildWord(word1[j], word2[jk - 1])==0) || jk==k){
                    check = 0;
                    break;
                }  
                jk++;          
            }
            if (check == 1){
                printf("%d", save);
                check = 0;
            }
        }
        else{
            i++;
        }

    }
    fclose(f);
}

void Replace_All(char* address, char* str1, char* str2){
    SearchStr(address, str1);
    int t = 0;
    for (int i = 1; i <= a[0]; i++){
        CopyFile(address, "komaki.txt", t, a[i] - 1);

        FILE *k;
        k = fopen("komaki.txt", "a+");
        fputs(str2, k);
        fclose(k);

        // CopyFile(address, "komaki.txt", b[i] + 1, a[i+1] - 1);
        t = b[i] + 1;
    }
    CopyFile(address, "komaki.txt", b[a[0]] + 1, -1);
    remove(address);
    rename("komaki.txt", address);
    }


void Replace_At(char* address, char* str1, char* str2, int at){
    SearchStr(address, str1);
    if(a[0] < at){
        printf("out of bound");
        return;
    }
    printf("%s", str2);
    CopyFile(address, "komaki.txt", 0, a[at] - 1);
    FILE *k;
    k = fopen("komaki.txt", "a+");
    // fputc(10, k);
    fputs(str2, k);
    fclose(k);
    // fprintf(k, "%s", str2);
    CopyFile(address, "komaki.txt", b[at] + 1, -1);
    remove(address);
    rename("komaki.txt", address);
}
// ...........................................................................................
// ........................................input..............................................
// ...........................................................................................

int check_creatfile(char* input, char* address, char* address2){
    char other[10000];
    char command[10000];
    char file[10000];
    sscanf(input, "%s %[^\n]*c" ,command , other);
    if (strcmp(command, "creatfile")==0){
        sscanf(other, "%s %[^\n]*c", file, address);
        // printf("%s\n", file);
        if (strcmp(file, "--file")!=0){
            printf("invalid input\n");
            return 0;
        }
        else{
            // printf("%s", address);
            char address1[1000];
            if (*address == '"'){
                if (*(address + strlen(address) - 1) == '"'){
                    for(int i = 1; i < strlen(address)-1; i++){
                        address1[i - 1] = address[i];
                    }
                }
                else{
                    printf("invalid input\n");
                    return 0;
                }
                strcat(address2, address1);
            }
            else{
                strcat(address2, address);
            }
            // printf("%s", address2);
             return CreateFile(address2);
            
        }
    }
    // char other2[10000];
}

void print_invalid_input(){
    printf("invalid input\n");
}

void input_to_str(char* input, char* str){
    int j = 0;
    int s = 0;
    for (int i = 0; i < strlen(input); i++){
        if  (*(input + i) == '\\'){
            if ((*(input + i + 1) == 'n') && (s == 0)){
                // printf("salam\n");
                *(str + j) = '\n';
                j++;
                i++;
            }
            else if((*(input + i + 1)== 'n') && (s != 0)){
                *(str + j) = 'n';
                i++;
                j++;
            }
            else if(*(input + i + 1)== '\\'){
                s = 1;
                *(str + j) = '\\';
                j++;
            }
        }
        else{
            s = 0;
            *(str + j) = *(input + i);
            j++;
        }
    }
}
// int check_insertstr(char* input){
//     if(strcmp(input,"insertstr")==0){
//         sscanf(other,"%s %[^\n]%*c",useless,address);
//         int line , pos;
//         char esc;
//         char str [100];
//         char rest[100];
//         if(address[0]=='"'){
//             sscanf(address+1,"%[^\"] %*c %s %s %s %[^\n]",address+1,useless,str,other,rest);
//             *(address)='"';
//             strcat(address,"\"");
//             }
//         else if(address[0]=='/')
//             sscanf(address,"%s %*c %s %s %s  %[^\n]",address,useless,str,other,rest);
//         line = rest[0] -'0';
//         pos = rest[2]-'0';
//         //printf("%d%d\n",line,pos);
//         //printf("%s\n%s\n%d\n%d\n",address,str,line ,pos);
//         insertstr(address,str,line ,pos);
    
//    }
// }
int main() {  
    // insertstr("root/test.txt", " moraghebe \nkhodetun\\n bashid! ", 3, 10);
    // cat("root/me/you/v2.txt");
    // CopyFile("test.txt","root/me/you/v1.txt", 1, -1);
    // printf("%d", PosToPtr("test.txt", 2, 6));
    // Remove("root/f1.txt", 3, 3, 5, 'f');
    // Copy("test.txt", 2, 10, 30, 'b');
    // Cut("test.txt", 2, 10, 30, 'f');
    // Paste("root/f1.txt", 3, 3);
    // Grep("root/tof.txt","me",'C');
    // SearchStr("root/test.txt", "*alam");
    // printf("%d\n", a[0]);
    // for (int i = 1; i <= a[0]; i++){
    //     printf("%d %d\n", a[i], b[i]);
    //     int k = LetterByWord(file, a[i]);
    //     printf("%d ", k);
    // }
    // printf("%d", a);
    // PrintFileLine("test.txt", 7);
    // MakeCopyForSave("root/me/you/v1.txt");
    // CreateFile("./root/f1.txt");
    // MakeCopyForSave("root/v1.txt");
    // Undo("/root/f1.txt");
    // Auto_Indent("root/v1.txt");
    // int l = NumberOfLine("root/test.txt");
    // int l = LenghOfLine("root/test.txt", 4);
    // printf("%d", l);
    // Compare("root/f1.txt","root/s.txt");
    // Tree("C:/proj", 0, -2);
    // ................................................
    // int* a = (int*)malloc(sizeof(int)*NAME_SIZE);
    // a = SearchStr("root/test.txt", "sal");
    // for (int i = 1; i <= a[0]; i++){
    //     printf("%d ", a[i]);
    // } 
    // .................................................
    // int k = MoghWildWord("abcd", "abcd");
    // printf("%d", k);
    // .................................................
    // printf("%d", NumOfWord("salam man MDF hastam !"));
    // .................................................
    // char s[10000];
    // WordOfStr("salam man MDF hastam.", 2);
    // WordOfStr("salam man MDF hastam.", 4);
    // printf("%s", word);
    // .................................................
    // printf("salamj");
    // SearchPattern("root/test.txt", "b* dustan*");
    // printf("sadds");
    // .................................................
    // Replace_All("root/test.txt", "pp*", "dadash");
    //.................................................. 
    // char str[1000];
    // char input[1000];
    // gets(input);
    // input_to_str(input, str);
    // printf("%s", str);
    // .................................................
    while(1){
        char command[20];
        int line , pos, size, count;
        int invalid;
        char c;
        char str[100];
        char str2[100];
        char str3[100];
        char file_useless[10];
        char str_useless[10];
        char other[200];
        char other2[200];
        char option1[200];
        char option2[200];
        char address[1000] = {'\0'};
        char address1[100] = {'\0'};
        char address2[100] = ".";
        char input[100] = {'\0'};

        gets(input);

        if (strcmp(input, "exit")==0){
            break;
        }

        sscanf(input, "%s %[^\n]%*c" ,command , other);

        if (strcmp(command, "createfile")==0){
            sscanf(other, "%s %[^\n]%*c", file_useless, address);
            // printf("salam");
            if (strcmp(file_useless, "--file")!=0){
                printf("invalid input\n");
            }
            else{
                if (*address == '"'){
                    if (*(address + strlen(address) - 1) == '"'){
                        for(int i = 1; i < strlen(address)-1; i++){
                            address1[i - 1] = address[i];
                        }
                    }
                    else{
                        printf("invalid input\n");
                    }
                    strcat(address2, address1);
                }
                else{
                    strcat(address2, address);
                }                
            }
            if (CreateFile(address2)==0){
                printf("Error : Exist file with same name!\n");
            }       
            else{
                printf("File Created\n");
            }
        }
    
        if (strcmp(command, "insertstr")==0){
            invalid = sscanf(other,"--file %[^\n]*c",address);
            // printf("%d", invalid);
            if (invalid!=1){
                print_invalid_input();
            }
            // printf("%s %s\n", address ,str);
            else if (*address == '"'){
                invalid = sscanf(address+1, "%[^\"]%*c --str %[^\n]*c", address1, str);
                // sscanf(address+1, "%[^\"]*c --str %[^\n]*c", address, str);
                // printf("%s %s %s\n", address1 ,str_useless, other);
                // printf("%s %s %s %s %s", address1 ,str_useless, str, pos_useless, pos);
                // printf("invalid = %d", invalid);
                if (invalid!=2){
                    print_invalid_input();
                }               
                else if (*str =='"'){
                    invalid = sscanf(str+1, "%[^\"]%*c --pos %d:%d", str2, &line, &pos);
                    if (invalid!=3){
                        print_invalid_input();
                    }                     
                    else{
                        strcat(address2, address1);
                        SaveChangeFile(address1);
                        input_to_str(str2, str3);
                        insertstr(address2, str3, line, pos);
                        
                    }
                }
                else{
                    invalid = sscanf(str, "%s --pos %d:%d", str2, &line, &pos);
                    // printf("%s %d %d %d\n", str2, line, pos, invalid);
                    if (invalid!=3){
                        print_invalid_input();
                    } 
                    
                    else{
                        strcat(address2, address1);
                        SaveChangeFile(address1);
                        input_to_str(str2, str3);
                        insertstr(address2, str3, line, pos);
                        
                    }
                }
            }
            else{
                invalid = sscanf(address, "%s --str %[^\n]%*c", address1, str);
                if (invalid!=2){
                    print_invalid_input();
                }               
                else if (*str =='"'){
                    invalid = sscanf(str+1, "%[^\"]%*c --pos %d:%d", str2, &line, &pos);
                    if (invalid!=3){
                        print_invalid_input();
                    }                     
                    else{
                        strcat(address2, address1);
                        SaveChangeFile(address1);
                        input_to_str(str2, str3);
                        insertstr(address2, str3, line, pos);
                        
                    }
                }
                else{
                    invalid = sscanf(str, "%s --pos %d:%d", str2, &line, &pos);
                    // printf("%s %d %d %d\n", str2, line, pos, invalid);
                    if (invalid!=3){
                        print_invalid_input();
                    } 
                    
                    else{
                        strcat(address2, address1);
                        SaveChangeFile(address1);
                        input_to_str(str2, str3);
                        insertstr(address2, str3, line, pos);
                        
                    }
                }
            }
        }

        if (strcmp(command, "cat")==0){
            invalid = sscanf(other, "--file %[^\n]*c", address);
            if (invalid != 1){
                print_invalid_input();
            }
            else if(*address=='"'){
                for (int i = 1; i < strlen(address)-1; i++){
                    *(address1+i-1) = *(address+i); 
                }
                strcat(address2,address1);
                cat(address2);
            }
            else{
                strcat(address2,address);
                cat(address2);
            }
        }
    
        if (strcmp(command, "removestr")==0){
            invalid = sscanf(other, "--pos %d:%d -size %d -%c --file %[^\n]*c", &line, &pos, &size, &c, address);
            if (invalid != 5){
                print_invalid_input();
            }
            else if(*address=='"'){
                for (int i = 1; i < strlen(address)-1; i++){
                    *(address1+i-1) = *(address+i); 
                }
                strcat(address2,address1);
                SaveChangeFile(address1);
                Remove(address2, line, pos, size, c);
                
            }
            else{
                strcat(address2,address);
                
                // printf("%s %d %d %d %c\n", address2, line, pos, size, c);
                // exit(1);
                SaveChangeFile(address);
                
                Remove(address2, line, pos, size, c);
                
            }
        }
    
        if (strcmp(command, "copy")==0){
            invalid = sscanf(other, "--pos %d:%d -size %d -%c --file %[^\n]*c", &line, &pos, &size, &c, address);
            if (invalid != 5){
                print_invalid_input();
            }
            
            else if(*address=='"'){
                for (int i = 1; i < strlen(address)-1; i++){
                    *(address1+i-1) = *(address+i); 
                }
                strcat(address2,address1);
                Copy(address2, line, pos, size, c);
            }
            else{
                strcat(address2,address);
                // printf("salam2\n");
                Copy(address2, line, pos, size, c);
            }
        }
    
        if (strcmp(command, "cut")==0){
            invalid = sscanf(other, "--pos %d:%d -size %d -%c --file %[^\n]*c", &line, &pos, &size, &c, address);
            if (invalid != 5){
                print_invalid_input();
            }
            
            else if(*address=='"'){
                for (int i = 1; i < strlen(address)-1; i++){
                    *(address1+i-1) = *(address+i); 
                }
                strcat(address2,address1);
                SaveChangeFile(address1);
                Cut(address2, line, pos, size, c);
                
            }
            else{
                strcat(address2,address);
                // printf("salam2\n");
                SaveChangeFile(address);
                Cut(address2, line, pos, size, c);
                
            }
        }
    
        if (strcmp(command, "paste")==0){
            invalid = sscanf(other, "--pos %d:%d --file %[^\n]*c", &line, &pos, address);
            if (invalid != 3){
                print_invalid_input();
            }
            
            else if(*address=='"'){
                for (int i = 1; i < strlen(address)-1; i++){
                    *(address1+i-1) = *(address+i); 
                }
                strcat(address2,address1);
                SaveChangeFile(address1);
                Paste(address2, line, pos);
                
            }
            else{
                strcat(address2,address);
                // printf("salam2\n");
                SaveChangeFile(address);
                Paste(address2, line, pos);
                
            }
        }
    
        if (strcmp(command, "find")==0){
            invalid = sscanf(other, "%s %s %s %s %s %[^\n]%*c", str_useless, str, file_useless, address, option1 ,other2);
            if (invalid == 4){
                strcat(address2, address);
                SearchStr(address, str);
                if(a[0]>0){
                    printf("%d", a[1]);
                }
                else{
                    printf("-1");
                }
            }
            else if(invalid = 6){
                if (option1 == "-at"){
                    if(sscanf(other2, "%d %s", &count, option2) == 2){
                        if (option2 == "-byword"){
                            printf("salam.");
                        }
                        else{
                            print_invalid_input();
                        }
                    }
                    else{

                    }
                }
            }
        }
    
        // if (strcmp(command, "replace")==0){
            
        // }
    
        if (strcmp(command, "grep")==0){
            invalid = sscanf(other, "-%c --str %[^\n] %*c", &c, str);
            // printf("%s", str);
            // printf("salam1");
            if (invalid == 2){
                // printf("salam");
                if (*str == '"'){
                    invalid  = sscanf(str+1, "%[^\"] %*c --files %[^\n] %*c", str2, address);
                    // printf("%s %s\n", str2, address);
                    if (invalid != 2){
                        // printf("%d\n", invalid);
                        print_invalid_input();
                    }
                    else{
                        while(sscanf(address, "%s %[^\n]*c", address1, address)==2){
                            char address3[100] = ".";
                            strcat(address3, address1);
                            // printf("%s %s\n", address3, str2);
                            Grep(address3, str2, 'x');
                        }
                        char address3[100] = ".";
                        strcat(address3, address1);
                        // printf("%s %s\n", address3, str2);
                        Grep(address3, str2, 'x');
                    }
                    
                    
                    
                }
                else{
                    invalid  = sscanf(str, "%s --files %[^\n]*c", str2, address1);
                    printf("%s %s %d\n", str2, address1, invalid);
                    if (invalid != 2){
                        printf("%d ajab\n", invalid);
                        print_invalid_input();
                    }
                    else{
                        while(sscanf(address, "%s %[^\n]*c", address1, address)==2){
                            char address3[100] = ".";
                            strcat(address3, address1);
                            // printf("%s\n", address3);
                            Grep(address3, str2, c);
                        }
                        char address3[1000] = ".";
                        strcat(address3, address1);
                        // printf("%s\n", address3);
                        Grep(address3, str2, c);
                    }                   
                }
            }
        }   
    
        if (strcmp(command, "undo")==0){
            invalid = sscanf(other, "--file %[^\n]*c", address);
            if (invalid != 1){
                print_invalid_input();
            }
            else if(*address=='"'){
                for (int i = 1; i < strlen(address)-1; i++){
                    *(address1+i-1) = *(address+i); 
                }
                strcat(address2,address1);
                // SaveChangeFile(address1);
                Undo(address1);
                
            }
            else{
                strcat(address2,address);
                // SaveChangeFile(address);
                Undo(address);
                
            }
        }
    
        if (strcmp(command, "auto-indent")==0){
            invalid = sscanf(other, "--file %[^\n]*c", address);
            if (invalid != 1){
                print_invalid_input();
            }
            else if(*address=='"'){
                for (int i = 1; i < strlen(address)-1; i++){
                    *(address1+i-1) = *(address+i); 
                }
                strcat(address2,address1);
                SaveChangeFile(address1);
                Auto_Indent(address2);
            }
            else{
                strcat(address2,address);
                SaveChangeFile(address);
                Auto_Indent(address2);
            }
        }
    
        if (strcmp(command, "compare")==0){
            char add[1000] = {'\0'};
            char add2[1000] = ".";
            if (*other == '"'){
                invalid = sscanf(other + 1, "%[^\"]*c %[^\n]*c", add, address);
                if (invalid != 2){
                    print_invalid_input();
                }
                else if(*address=='"'){
                    for (int i = 1; i < strlen(address)-1; i++){
                        *(address1+i-1) = *(address+i); 
                    }
                    strcat(add2, add);
                    strcat(address2,address1);
                    Compare(add2, address2);
                }
                else{
                    strcat(add2, add);
                    strcat(address2,address);
                    Compare(add2, address2);
                }
            }
            else {
                invalid = sscanf(other, "%s %[^\n]*c", add, address);
                if (invalid != 2){
                    print_invalid_input();
                }
                else if(*address=='"'){
                    for (int i = 1; i < strlen(address)-1; i++){
                        *(address1+i-1) = *(address+i); 
                    }
                    strcat(add2, add);
                    strcat(address2,address1);
                    Compare(add2, address2);
                }
                else{
                    strcat(add2, add);
                    strcat(address2,address);
                    Compare(add2, address2);
                }
            }
        }
    
        if (strcmp(command, "tree")==0){
            int depth;
            invalid = sscanf(other, "%d", &depth);
            if  (invalid != 1){
                print_invalid_input();
            }
            else{
                Tree("./root", 0, depth);
            }
        }
    }    

    return 0;
}
