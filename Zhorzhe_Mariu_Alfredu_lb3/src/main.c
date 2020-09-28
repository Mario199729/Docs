#include<stdio.h>
#include<string.h>
#include<dirent.h>
#include<sys/types.h>


int add(char* path, char *simb){
    int res = 0, lenpath;
    DIR *dir = opendir(path);
    struct dirent *de;
	FILE *file;
    int value;

    if(simb == "mul")
        res = 1;

    while((de = readdir(dir)) != NULL){
        lenpath = strlen(path);

        if(strcmp(de->d_name, "add") == 0){
            strcat(path, "/");
            strcat(path, de->d_name);
            if(simb == "add")
                res += add(path, "add");
            else if(simb == "mul")
                res *= add(path, "add");
            path[lenpath] = '\0';
        }

        if(strcmp(de->d_name, "mul") == 0){
            strcat(path, "/");
            strcat(path, de->d_name);
            if(simb == "add")
                res += add(path, "mul");
            else if(simb == "mul")
                res *= add(path, "mul");
            path[lenpath] = '\0';
        }

        if(de->d_name[strlen(de->d_name) - 3] == 't' && de->d_name[strlen(de->d_name) - 2] == 'x' && de->d_name[strlen(de->d_name) - 1] == 't'){
            strcat(path, "/");
            strcat(path, de->d_name);
            file = fopen(path, "r");
            if(simb == "add"){
                while(fscanf(file, "%d", &value) != EOF)
                    res +=value;
            }else if(simb == "mul"){
                while(fscanf(file, "%d", &value) != EOF)
                    res *=value;
            }
            path[lenpath] = '\0';
        }
    }

    return res;
}

int get_all_files( DIR * dir, char * path){
    struct dirent *de;
    int r = 0;
    while((de = readdir(dir)) !=NULL)
    {
        if(strcmp(de->d_name, "add") == 0){
            strcat(path, "/");
            strcat(path, de->d_name);
            r = add(path, "add");
        }

        if(strcmp(de->d_name, "mul") == 0){
            strcat(path, "/");
            strcat(path, de->d_name);
            r = add(path, "mul");
        }
    }
    
    return r;
}

int main(){

    char caminho[100]  = "root";
    FILE *result = fopen("result.txt", "w");
	DIR *dir;
    int res;

	if((dir = opendir(caminho)))
        res = get_all_files(dir, caminho);
    
    fprintf(result, "%d", res);
    printf("%d\n", res);
    closedir(dir);    
    fclose(result);

	return 0;
}