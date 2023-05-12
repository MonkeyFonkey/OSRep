#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>

#define MAX_FILES 100 // Adjust the maximum number of files as needed

pid_t child_pids[MAX_FILES] = {-1};
int num_processes = 0;




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


void runScriptFileFor_c_file(const char* filename) {
    char command[100];
    sprintf(command, "./script.sh %s 2>&1", filename);

    FILE* pipe = popen(command, "r");
    if (pipe == NULL) {
        perror("Error executing script.sh");
        return;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        printf("%s", buffer);
    }

    int status = pclose(pipe);
    if (status == -1) {
        perror("Error closing pipe");
    } else {
        if (WIFEXITED(status)) {
           // printf("Script.sh exited with status %d\n", WEXITSTATUS(status));
        } else {
            printf("Script.sh exited abnormally\n");
        }
    }
}

int computeScore(int numErrors, int numWarnings) {
    if (numErrors == 0 && numWarnings == 0) {
        return 10;
    } else if (numErrors >= 1) {
        return 1;
    } else if (numWarnings > 10) {
        return 2;
    } else {
        return 2 + 8 * (10 - numWarnings) / 10;
    }
}

int countLinesInFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    int lineCount = 0;
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        lineCount++;
    }

    fclose(file);
    return lineCount;
}

int checkFileExtension(const char* filename, const char* extension) {
    const char* fileExt = strrchr(filename, '.');
    if (fileExt != NULL) {
        if (strcmp(fileExt + 1, extension) == 0) {
            return 1;
        }
    }
    return 0;
}

void handleRegular(char *filename) {
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
    printf("Displays name of the file and the number of hard links\n");

    printf("\n\nPlease enter your options\n\n");
    printf(">> ");
    fgets(params, 10, stdin);
    
    if(verifyInput(params, 0) != -1) {
        for(int i = 1; i < strlen(params)-1; i++) {
            if (params[i] == 'n') {
                printf("Name: %s\n", filename);
                printf("\n");
            }
            if (params[i] == 'd') {
                if(stat(filename, &buf)) {
                    printf("error\n");
                }
                else {
                    printf("Size: %d bytes\n", (int)buf.st_size);
                    printf("\n");
                }
            }
            if (params[i] == 'h') {
                if(stat(filename, &buf)) {
                    printf("error\n");
                }
                else {
                    printf("Hard links count: %d\n", (int)buf.st_nlink);
                    printf("\n");
                }
            }
            if (params[i] == 'm') {
                if(stat(filename, &buf)) {
                    printf("error\n");
                }
                else {
                    printf("Time of last modification: %ld\n", buf.st_mtime);
                    printf("\n");
                }
            }
            if (params[i] == 'a') {
                if(stat(filename, &buf)) {
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

                if(stat(filename, &buf)) {
                    printf("error\n");
                }
                else {
                    if(symlink(filename, link) == 0) {
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
                char *dir_name = NULL;
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

void handle_arg(char* arg) {
    pid_t pid = fork();
    if (pid == 0) {
        // Determine the type of the argument
        DIR* dir = opendir(arg);
        if (dir != NULL) {
            // Argument is a directory
            closedir(dir);
            create_text_file(arg);
            exit(0);
        }
        else {
            // Argument is a file
            closedir(dir);
            char* ext = strrchr(arg, '.');
            if (ext != NULL && strcmp(ext, ".c") == 0) {
                // Argument is a .c file
                compile_file(arg);
            }
            exit(0);
        }
    }
    else if (pid < 0) {
        perror("Error forking child process");
    }
}



int main(int argc, char **argv) {


    if(argc > 1) {
         for (int i = 1; i < argc; i++) {
             handle_arg(argv[i]);
        }

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
                
                char *filename = argv[i];

                // Parent process
                int status1, status2;
                pid_t pid1 = -1, pid2 = -1;

                // Fork the first child process (PID1)
                pid1 = fork();

                if (pid1 == -1) {
                    perror("Error forking child process (PID1)");
                    exit(EXIT_FAILURE);
                } else if (pid1 == 0) {
                    // Child process (PID1)
                    // Check if it is a .c file
                    if (checkFileExtension(filename, "c")) {
                        runScriptFileFor_c_file(filename);
                    }
                    exit(EXIT_SUCCESS);
                } else {
                    // Fork the second child process (PID2)
                    pid2 = fork();

                    if (pid2 == -1) {
                        perror("Error forking child process (PID2)");
                        exit(EXIT_FAILURE);
                    } else if (pid2 == 0) {
                        // Child process (PID2)
                        // Count the number of lines in the file
                        int lineCount = countLinesInFile(filename);
                        if(lineCount == -1){
                            printf("Error counting lines in file\n");
                        } else {
                            printf("The file %s has %d lines\n", filename, lineCount);
                        }
                        exit(EXIT_SUCCESS);
                    } else {
                        // Wait for both child processes
                        if (waitpid(pid1, &status1, 0) == -1) {
                            perror("Error waiting for child process (PID1)");
                            exit(EXIT_FAILURE);
                        }

                        if (waitpid(pid2, &status2, 0) == -1) {
                            perror("Error waiting for child process (PID2)");
                            exit(EXIT_FAILURE);
                        }

                        // Process the exit status of PID1
                        if (WIFEXITED(status1)) {
                            printf("The process with PID1 %d has ended with the exit code %d\n\n", pid1, WEXITSTATUS(status1));

                            // Check if it is a .c file
                            if (checkFileExtension(filename, "c")) {
                                // Write the score to grades.txt
                                FILE* gradesFile = fopen("grades.txt", "a");
                                if (gradesFile == NULL) {
                                    perror("Error opening grades.txt");
                                    exit(EXIT_FAILURE);
                                }

                                fprintf(gradesFile, "%s: %d\n", filename, WEXITSTATUS(status1));
                                fclose(gradesFile);
                            }
                        } else {
                            printf("Child process with PID1 %d exited abnormally\n\n", pid1);
                        }

                        // Process the exit status of PID2
                        if (WIFEXITED(status2)) {
                            printf("The process with PID2 %d has ended with the exit code %d\n\n", pid2, WEXITSTATUS(status2));
                        } else {
                            printf("Child process with PID2 %d exited abnormally\n\n", pid2);
                        }
                    }
                }

                        
            }else if(type == 2) {
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