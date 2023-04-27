#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>

int checkFileType(char *path) {
    struct stat buf;

    if(lstat(path, &buf) == 0) {
        if(S_ISLNK(buf.st_mode) != 0) {
            return 0;
        }
    }
    if(stat(path, &buf) == 0) {
        if(S_ISREG(buf.st_mode) != 0) {
            return 1;
        }
        else if(S_ISDIR(buf.st_mode) != 0) {
            return 2;
        }
    }

    return -1;
}

int verifyInput(char *params, int type) {
    if(params[0] != '-') {
        printf("\nThe command must start with an '-'\n");
        return -1;
    }

    //printf("p: %s\n", params);
    int not = 0;

    //printf("size: %d\n", strlen(params));

    if(type == 0) {
        for(int i = 1; i < strlen(params)-1; i++) {
            //printf("- %c\n", params[i]);
            if(params[i] == 'n' || params[i] == 'd' || params[i] == 'h' || params[i] == 'm' || params[i] == 'a' || params[i] == 'l') {
                //
            }
            else {
                not = 1;
            }
        }

        if(not) 
            return 0;
        else
            return 1;
    }
    else if(type == 2) {
        for(int i = 1; i < strlen(params)-1; i++) {
            //printf("- %c\n", params[i]);
            if(params[i] == 'n' || params[i] == 'd' || params[i] == 'a' || params[i] == 'c') {
                //
            }
            else {
                not = 1;
            }
        }

        if(not) 
            return 0;
        else
            return 1;
    }
    else {
        for(int i = 1; i < strlen(params)-1; i++) {
            //printf("- %c\n", params[i]);
            if(params[i] == 'n' || params[i] == 'l' || params[i] == 'd' || params[i] == 't' || params[i] == 'a') {
                //
            }
            else {
                not = 1;
            }
        }

        if(not) 
            return 0;
        else
            return 1;
    }
}

void handleSym(char *path) {
    char params[10];

    int deleted = 0;

    struct stat buf;
    
    printf("---- MENU ----\n");
    printf("* n: name of symbolic link\n");
    printf("* l: delete symbolic link\n");
    printf("* d: size of symbolic link\n");
    printf("* t: size of target\n");
    printf("* a: access rights\n");
    printf("\n---- EXAMPLE INPUT ----\n");
    printf("-nl\n");
    printf("Used for displaying name of file and deleting the symbolic link.\n");

    printf("\n\nPlease enter your options\n\n");
    printf("STANDARD INPUT: ");
    fgets(params, 10, stdin);

    if(verifyInput(params, 1) != -1) {
        for(int i = 1; i < strlen(params)-1 && deleted == 0; i++) {
            if (params[i] == 'n') {
                printf("Name: %s\n", path);
                printf("\n");
            }
            if (params[i] == 'd') {
                if(lstat(path, &buf)) {
                    printf("error\n");
                }
                else {
                    printf("Size of symbolic link: %d bytes\n", (int)buf.st_size);
                    printf("\n");
                }
            }
            if (params[i] == 't') {
                if(stat(path, &buf)) {
                    printf("error\n");
                }
                else {
                    printf("Size of target file: %d bytes\n", (int)buf.st_size);
                    printf("\n");
                }
            }
            if (params[i] == 'l') {
                if(lstat(path, &buf)) {
                    printf("error\n");
                }
                else {
                    if(unlink(path) == 0) {
                        printf("Symlink deleted successfully.\n");
                        deleted = 1;
                    }
                    else {
                        printf("Error deleting the symlink.\n");
                    }
                    printf("\n");
                }
            }
            if (params[i] == 'a') {
                if(lstat(path, &buf)) {
                    printf("error\n");
                }
                else {
                    printf("Access rights: \n");

                    printf("User:\n");
                    buf.st_mode & S_IRUSR ? printf("   Read - yes\n") : printf("   Read - no\n");
                    buf.st_mode & S_IWUSR ? printf("   Write - yes\n") : printf("   Write - no\n");
                    buf.st_mode & S_IXUSR ? printf("   Execute - yes\n") : printf("   Execute - no\n");
                        
                    printf("Group:\n");
                    buf.st_mode & S_IRGRP ? printf("   Read - yes\n") : printf("   Read - no\n");
                    buf.st_mode & S_IWGRP ? printf("   Write - yes\n") : printf("   Write - no\n");
                    buf.st_mode & S_IXGRP ? printf("   Execute - yes\n") : printf("   Execute - no\n");

                    printf("Other:\n");
                    buf.st_mode & S_IROTH ? printf("   Read - yes\n") : printf("   Read - no\n");
                    buf.st_mode & S_IWOTH ? printf("   Write - yes\n") : printf("   Write - no\n");
                    buf.st_mode & S_IXOTH ? printf("   Execute - yes\n") : printf("   Execute - no\n");

                    printf("\n");
                }
            }
        }
    }
    else {
        printf("\n\nERROR. Input not right.\n");
    }

    printf("\n\n-----------------------------------------------\n\n");
}

void handleRegular(char *path) {
    char params[10];

    struct stat buf;
    
    printf("---- MENU ----\n");
    printf("* n: name of file\n");
    printf("* d: size of file\n");
    printf("* h: count of hard links\n");
    printf("* m: last time of modifications\n");
    printf("* a: access rights\n");
    printf("* l: create symbolic link *waits for input*\n");
    printf("\n---- EXAMPLE INPUT ----\n");
    printf("-nh\n");
    printf("Used for displaying name of file and the count of hard links\n");

    printf("\n\nPlease enter your options\n\n");
    printf("STANDARD INPUT: ");
    fgets(params, 10, stdin);

    if(verifyInput(params, 0) != -1) {
        for(int i = 1; i < strlen(params)-1; i++) {
            if (params[i] == 'n') {
                printf("Name: %s\n", path);
                printf("\n");
            }
            if (params[i] == 'd') {
                if(stat(path, &buf)) {
                    printf("error\n");
                }
                else {
                    printf("Size: %d bytes\n", (int)buf.st_size);
                    printf("\n");
                }
            }
            if (params[i] == 'h') {
                if(stat(path, &buf)) {
                    printf("error\n");
                }
                else {
                    printf("Hard links count: %d\n", (int)buf.st_nlink);
                    printf("\n");
                }
            }
            if (params[i] == 'm') {
                if(stat(path, &buf)) {
                    printf("error\n");
                }
                else {
                    printf("Time of last modification: %ld\n", buf.st_mtime);
                    printf("\n");
                }
            }
            if (params[i] == 'a') {
                if(stat(path, &buf)) {
                    printf("error\n");
                }
                else {
                    printf("Access rights: \n");

                    printf("User:\n");
                    buf.st_mode & S_IRUSR ? printf("   Read - yes\n") : printf("   Read - no\n");
                    buf.st_mode & S_IWUSR ? printf("   Write - yes\n") : printf("   Write - no\n");
                    buf.st_mode & S_IXUSR ? printf("   Execute - yes\n") : printf("   Execute - no\n");
                        
                    printf("Group:\n");
                    buf.st_mode & S_IRGRP ? printf("   Read - yes\n") : printf("   Read - no\n");
                    buf.st_mode & S_IWGRP ? printf("   Write - yes\n") : printf("   Write - no\n");
                    buf.st_mode & S_IXGRP ? printf("   Execute - yes\n") : printf("   Execute - no\n");

                    printf("Other:\n");
                    buf.st_mode & S_IROTH ? printf("   Read - yes\n") : printf("   Read - no\n");
                    buf.st_mode & S_IWOTH ? printf("   Write - yes\n") : printf("   Write - no\n");
                    buf.st_mode & S_IXOTH ? printf("   Execute - yes\n") : printf("   Execute - no\n");

                    printf("\n");
                }
            }

            if (params[i] == 'l') {
                char link[15];

                printf("Enter name of symlink: ");
                fgets(link, 15, stdin);

                //remove the null character at the end if present
                char *newline = strchr(link, '\n');
                if (newline != NULL) {
                    *newline = '\0';
                }

                if(stat(path, &buf)) {
                    printf("error\n");
                }
                else {
                    if(symlink(path, link) == 0) {
                        printf("Success! You created a symbolic link: %s\n", link);
                        printf("Here's how it looks:\n");
                        char command[256];
                        sprintf(command, "ls -l %s", link);
                        system(command);
                    }
                    else {
                        printf("It didn't work.");
                    }
                    printf("\n");
                }
            }
        }
    }
    else {
        printf("\n\nERROR. Input not right.\n");
    }

    printf("\n\n-----------------------------------------------\n\n");
}

void handleDirectory(char *path) {
    char params[10];

    struct stat buf;
    
    printf("---- MENU ----\n");
    printf("* n: name of directory\n");
    printf("* d: size of directory\n");
    printf("* a: access rights\n");
    printf("* c: number of files with the .c extension\n");
    printf("\n---- EXAMPLE INPUT ----\n");
    printf("-nd\n");
    printf("Used for displaying the name of directory and the size of the directory\n");

    printf("\n\nPlease enter your options\n\n");
    printf("STANDARD INPUT: ");
    fgets(params, 10, stdin);

    if(verifyInput(params, 2) != -1) {
        for(int i = 1; i < strlen(params)-1; i++) {
            if (params[i] == 'n') {
                printf("Name: %s\n", path);
                printf("\n");
            }
            if (params[i] == 'd') {
                if(stat(path, &buf)) {
                    printf("error\n");
                }
                else {
                    printf("Size: %d bytes\n", (int)buf.st_size);
                    printf("\n");
                }
            }
            if (params[i] == 'a') {
                if(stat(path, &buf)) {
                    printf("error\n");
                }
                else {
                    printf("Access rights: \n");

                    printf("User:\n");
                    buf.st_mode & S_IRUSR ? printf("   Read - yes\n") : printf("   Read - no\n");
                    buf.st_mode & S_IWUSR ? printf("   Write - yes\n") : printf("   Write - no\n");
                    buf.st_mode & S_IXUSR ? printf("   Execute - yes\n") : printf("   Execute - no\n");
                        
                    printf("Group:\n");
                    buf.st_mode & S_IRGRP ? printf("   Read - yes\n") : printf("   Read - no\n");
                    buf.st_mode & S_IWGRP ? printf("   Write - yes\n") : printf("   Write - no\n");
                    buf.st_mode & S_IXGRP ? printf("   Execute - yes\n") : printf("   Execute - no\n");

                    printf("Other:\n");
                    buf.st_mode & S_IROTH ? printf("   Read - yes\n") : printf("   Read - no\n");
                    buf.st_mode & S_IWOTH ? printf("   Write - yes\n") : printf("   Write - no\n");
                    buf.st_mode & S_IXOTH ? printf("   Execute - yes\n") : printf("   Execute - no\n");

                    printf("\n");
                }
            }

            if (params[i] == 'c') {
                
                DIR *dir;
                struct dirent *ent;
                int count = 0;
                char *extension = ".c";
                char *dir_name;
                strcpy(dir_name, path);
                struct stat buf;
                char absolute_path[1024];

                //get absolute path
                 if (stat(path, &buf) == 0) {
                    if (S_ISDIR(buf.st_mode)) {
                        printf("%s is a directory\n", path);
                    } else {
                        printf("%s is a file\n", path);
                    }
                    if (realpath(path, absolute_path) == NULL) {
                        perror("realpath");
                    }
                    printf("Absolute path: %s\n", absolute_path);
                } else {
                    perror("stat");
                }

                if ((dir = opendir(dir_name)) != NULL) {
                    while ((ent = readdir(dir)) != NULL) {
                        if (ent->d_type == DT_REG) {
                            char *file_ext = strrchr(ent->d_name, '.');
                            if (file_ext != NULL && strcmp(file_ext, extension) == 0) {
                                count++;
                            }
                        }
                    }
                    closedir(dir);
                    printf("Number of .c files in %s: %d\n", dir_name, count);
                } else {
                    perror("Could not open directory");
                }

            }
        }
    }
    else {
        printf("\n\nERROR. Input not right.\n");
    }

    printf("\n\n-----------------------------------------------\n\n");
}

/*.......................................

    Part 2 of the project
*/
void compile_file(char* filename) {
    pid_t pid = fork();
    if (pid == 0) {
        char* cmd = (char*) malloc(strlen(filename) + 10);
        sprintf(cmd, "gcc -o %s.out %s", filename, filename);
        system(cmd);
        free(cmd);
        char* err_cmd = (char*) malloc(strlen(filename) + 20);
        sprintf(err_cmd, "gcc -o %s.out %s 2>&1 >/dev/null | grep error | wc -l", filename, filename);
        char* warn_cmd = (char*) malloc(strlen(filename) + 20);
        sprintf(warn_cmd, "gcc -o %s.out %s 2>&1 >/dev/null | grep warning | wc -l", filename, filename);
        int errors = system(err_cmd);
        int warnings = system(warn_cmd);
        printf("%s: %d errors, %d warnings\n", filename, errors, warnings);
        free(err_cmd);
        free(warn_cmd);
        exit(0);
    }
    else if (pid < 0) {
        perror("Error forking child process");
    }
}

void create_text_file(char* dirname) {
    pid_t pid = fork();
    if (pid == 0) {
        char* filename = (char*) malloc(strlen(dirname) + 10);
        sprintf(filename, "%s_file.txt", dirname);
        FILE* fp = fopen(filename, "w");
        if (fp != NULL) {
            fprintf(fp, "This is a text file created by the program.\n");
            fclose(fp);
        }
        free(filename);
        exit(0);
    }
    else if (pid < 0) {
        perror("Error forking child process");
    }
}



int main(int argc, char **argv) {

    if(argc > 1) {
        for(int i = 1; i < argc; i++) {
            int type = checkFileType(argv[i]);

            if(type == 0) {
                printf("%s : SYMBOLIC LINK\n", argv[i]);
                printf("\n\n==========================\n\n");
                handleSym(argv[i]);
            }
            else if(type == 1) {
                printf("%s : REGULAR FILE\n", argv[i]);
                printf("\n\n==========================\n\n");
                handleRegular(argv[i]);
            }
            else if(type == 2) {
                printf("%s : DIRECTORY\n", argv[i]);
                printf("\n\n==========================\n\n");
                handleDirectory(argv[i]);
            }
            else {
                printf("You didn't enter a regular file/directory/symbolic link");
            }
        }
    }
    return 0;
}