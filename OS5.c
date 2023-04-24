#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#define MAX_OPTS 10

void display_menu(char type)
{
    switch (type)
    {
    case 'f': // Regular file
        printf("\nOptions:\n");
        printf("    -a : Display file contents\n");
        printf("    -l : Display file permissions and other information\n");
        break;
    case 'd': // Directory
        printf("\nOptions:\n");
        printf("    -a : Display directory contents\n");
        printf("    -l : Display directory permissions and other information\n");
        printf("    -c : Count the number of files in the directory\n");
        break;
    case 'l': // Symbolic link
        printf("\nOptions:\n");
        printf("    -t : Display the target of the symbolic link\n");
        break;
    default:
        printf("\nInvalid file type.\n");
    }
}

void display_file_info(char *path, char *opts)
{
    struct stat st;
    if (stat(path, &st) == -1)
    {
        perror("stat");
        return;
    }

    char type;
    if (S_ISREG(st.st_mode)) //regular file
        type = 'f';
    else if (S_ISDIR(st.st_mode))//directory
        type = 'd';
    else if (S_ISLNK(st.st_mode))//link
        type = 'l';
    else
        type = '-';

    printf("Name: %s\n", path);
    printf("Type: %c\n", type);

    display_menu(type);

    char opt_list[MAX_OPTS] = {0};
    int opt_count = 0;

    //extracts the options from the opts string and uses them to determine which information to display for the file.
    for (int i = 0; i < strlen(opts); i++)
    {
        if (opts[i] == '-')
        {
            i++;
            while (opts[i] != ' ' && opts[i] != '\0')
            {
                opt_list[opt_count++] = opts[i++];
            }
            break;
        }
    }

    for (int i = 0; i < opt_count; i++)
    {
        switch (opt_list[i])
        {
        case 'a': 
            if (type == 'f')//if its a file:
            {
                FILE *f = fopen(path, "r"); //opening the file
                if (f == NULL)
                {
                    perror("fopen");
                    return;
                }

                char c;
                printf("\nContents:\n");//printing the content of the file
                while ((c = fgetc(f)) != EOF)
                {
                    printf("%c", c);
                }
                fclose(f);
            }
            else if (type == 'd')//if its a directory
            {
                DIR *d = opendir(path);
                if (d == NULL)
                {
                    perror("opendir");
                    return;
                }

                struct dirent *entry;
                printf("\nContents:\n"); //reading the content:
                while ((entry = readdir(d)) != NULL)
                {
                    printf("%s\n", entry->d_name);
                }
                closedir(d);
            }
            break;
        case 'l':
            if (type == 'f' || type == 'd')
            {
                struct stat st;
                if (lstat(path, &st) == -1)
                {
                    perror("lstat");
                    return;
                }
                printf("\nPermissions:\n");
                printf((S_ISDIR(st.st_mode)) ? "d" : "-");
                printf((st.st_mode & S_IRUSR) ? "r" : "-");
                printf((st.st_mode & S_IWUSR) ? "w" : "-");
                printf((st.st_mode & S_IXUSR) ? "x" : "-");
                printf((st.st_mode & S_IRGRP) ? "r" : "-");
                printf((st.st_mode & S_IWGRP) ? "w" : "-");
                printf((st.st_mode & S_IXGRP) ? "x" : "-");
                printf((st.st_mode & S_IROTH) ? "r" : "-");
                printf((st.st_mode & S_IWOTH) ? "w" : "-");
                printf((st.st_mode & S_IXOTH) ? "x" : "-");
                printf("\n");
            }
            else if (type == 'l')
            {
                char target[256] = {0};
                ssize_t len = readlink(path, target, sizeof(target) - 1);
                if (len == -1)
                {
                    perror("readlink");
                    return;
                }
                printf("\nTarget: %s\n", target);
            }
            break;
        case 'c'://counts the number of files in the directory
            if (type == 'd')
            {
                DIR *d = opendir(path);
                if (d == NULL)
                {
                    perror("opendir");
                    return;
                }
                
                struct dirent *entry;
                int count = 0;
                while ((entry = readdir(d)) != NULL)
                {
                    count++;
                }
                closedir(d);
                printf("\nNumber of files in directory: %d\n", count);
            }
            break;
        default:
            printf("\nInvalid option: %c\n", opt_list[i]);
        }
    }
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Usage: %s <path1> [<path2> ...]\n", argv[0]);
        return 0;
    }
    for (int i = 1; i < argc; i++)
    {
        printf("\nFile %d:\n", i);
        display_file_info(argv[i], argv[i]);
    }

    return 0;
}
