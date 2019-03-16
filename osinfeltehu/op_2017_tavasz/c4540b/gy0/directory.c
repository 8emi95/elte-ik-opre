//print out the files from a directory
//sign each of the directories

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <dirent.h> //opendir, readdir
#include <sys/types.h> //opendir
#include <sys/stat.h> //stat
#include <unistd.h>   //stat
/*
struct dirent {
    ino_t          d_ino;       // inode number - a fájl indexe, sorszáma(linux)
    off_t      d_off;       // offset to the next dirent - a következő dirent helye
    unsigned short d_reclen;    // length of this record - ennek a rekordnak a hossza
    unsigned char  d_type;      // type of file; not supported - a fájl típusa
                                   by all file system types 
    char           d_name[256]; // filename - a fájl neve (max 256 hosszú egy fájl)
};

struct stat {
    dev_t     st_dev;     // ID of device containing file - fájl azonosítója
    ino_t     st_ino;     // inode number - a fájl indexe, sorszáma (linux)
    mode_t    st_mode;    // protection - a fájl védettsége
    nlink_t   st_nlink;   // number of hard links  
    uid_t     st_uid;     // user ID of owner 
    gid_t     st_gid;     // group ID of owner 
    dev_t     st_rdev;    // device ID (if special file) 
    off_t     st_size;    // total size, in bytes - teljes méret
    blksize_t st_blksize; // blocksize for file system I/O 
    blkcnt_t  st_blocks;  // number of 512B blocks allocated - leallokált tárterület
    time_t    st_atime;   // time of last access - utolsó belépés
    time_t    st_mtime;   // time of last modification - utolsó módosítás
    time_t    st_ctime;   // time of last status change - utolsó státusz változás
};
*/
int main(){
 DIR* d; 
 struct dirent * dp; //pointer to dirent struct
 struct stat st;
 d=opendir("."); //parameter the directory name (with path) we want to open - amelyik mappa fájlait akarjuk kiírni
 if (d<0){perror("Some problem in opening");exit(1);} // ha nem sikerül hibaüzenet
 while (dp=readdir(d)){    //amíg van fájl
   stat(dp->d_name,&st);  //fstat(file descriptor, stat structure) // a fájl neve, és típusa
   if (S_ISDIR(st.st_mode)) {       //a fájl védettsége
     printf("d:");
   }
   else {
     if (st.st_mode & S_IRUSR) printf("r "); //to read out the permissions 
     else printf("- "); 
   }
   printf("%s\n",dp->d_name); //végül kiírjuk a fájl nevét
 }
 closedir(d); //close directory after opendir - bezárjuk a könyvtárt a megnyitás után
 return 0;
}

//Kiírjá a fájlokat egy adott könyvtárból
//Ugyanúgy a stat structot használja
