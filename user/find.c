#include "kernel/types.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
#include "user/user.h"

void find(char* path, const char* filename)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    // open the path
    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    // get file metadata. fd: source file. st: store the file metadata
    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot fstat %s\n", path);
        close(fd);
        return;
    }

    // the first argument must be a directory
    if(st.type != T_DIR){
        fprintf(2, "usage: find <DIRECTORY> <filename>\n");
        return;
    }

    // Limited size
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
        fprintf(2, "find: path too long\n");
        return;
    }

    strcpy(buf, path);
    p = buf + strlen(buf);
    // Note: The p is assigned first and then add up one. point to the address after '/'
    *p++ = '/';  

    while(read(fd, &de, sizeof de) == sizeof de){
        if(de.inum == 0) continue;  // 0用作标记值以指示null或无inode。
        memmove(p, de.name, DIRSIZ);  // the file name is moved to 'p'
        p[DIRSIZ] = '\0';  // end of string

        if(stat(buf, &st) < 0) {  // buf to st
            fprintf(2, "find: cannot stat %s\n", buf);
            continue;
        }

        // if the file is a directory, and not the previous directory or current directory
        if(st.type == T_DIR && strcmp(p, ".") != 0 && strcmp(p, "..") != 0){
            find(buf, filename);
        } else if(strcmp(filename, p) == 0)  // gotcha
            printf("%s\n", buf);
    }
    close(fd);
}

int main(int argc, char** argv){
    if(argc != 3){
        fprintf(2, "usage: find <directory> <filename>\n");
        exit(1);        
    }
    find(argv[1], argv[2]);
    exit(0);
}