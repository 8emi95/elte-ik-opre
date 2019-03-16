#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h> //fork-hoz
#include <sys/stat.h>
#include <unistd.h> //fork
#include <string.h> //strlen
#include <fcntl.h> //lock

int main()
{
    int f;
    char text1[] = " Parent Parent Parent Parent Parent Parent Parent Parent Parent Parent ";
    int length1 = strlen(text1);
    char text2[] = " Child Child Child Child Child Child Child Child Child Child Child Child ";
    int length2 = strlen(text2);
    int i;
	/** flock, or file lock is a type of a lock. Locks are associated with processes. A 
	 *	process can only have one kind of lock set for each byte of a given file. When any 
	 *	file descriptor for that file is closed by the process, all of the locks that process 
	 *	holds on that file are released, even if the locks were made using other descriptors
	 *	that remain open. Likewise, locks are released when a process exits, and are not 
	 *	inherited by child processes created using fork.
	 *
	 *	When making a lock, use a struct flock to specify what kind of lock and where. This 
	 *	data type and the associated macros for the fcntl function are declared in the header 
	 *	file fcntl.h.
	 *
	 *	https://www.gnu.org/software/libc/manual/html_node/File-Locks.html
	 */
    struct flock lock_data;

	/** This corresponds to the whence argument to fseek or lseek, and specifies what the offset
	 *  is relative to. Its value can be one of SEEK_SET, SEEK_CUR, or SEEK_END.
	 *
	 *	- SEEK_SET : start of file, 
	 *	- SEEK_END : end of file,
	 *	- SEEK_CUR : actual position
	 */
    lock_data.l_whence = SEEK_SET;
	/** This specifies the offset of the start of the region to which the lock applies, and is 
	 *  given in bytes relative to the point specified by the l_whence member.
	 */
    lock_data.l_start = 0;
	/** This specifies the length of the region to be locked. A value of 0 is treated specially;
	 *	it means the region extends to the end of the file.
	 */
    lock_data.l_len = 0; 
    int rc = 0; //result of lock
    f = open("data.txt", O_RDWR | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);
    pid_t child = fork();
    if (child < 0) //error
    {
        perror("Error"); exit(1);
    }
    if (child > 0) // Parent process
    {
        int j;
        for (j = 0; j < 25; j++) //to avoid too big data files
        {
            // printf("szulo %i ",getpid());
            lock_data.l_pid = getpid(); //which process locks
			/** for what it is locked 
			 *	- F_WRLCK - write,
			 *	- FRDLCK - read,
			 *	- F_UNLCK - unlock
			 */
            lock_data.l_type = F_WRLCK; 
			/** https://www.gnu.org/software/libc/manual/html_node/Control-Operations.html
			 *  The fcntl function performs the operation specified by command on the file 
			 *	descriptor filedes. Some commands require additional arguments to be supplied. 
			 *	These additional arguments and the return value and error conditions are 
			 *	given in the detailed descriptions of the individual commands. Examples:
			 *
			 *	- F_SETLK :
			 *
			 *		This macro is used as the command argument to fcntl, to specify that it 
			 *		should set or clear a lock. This command requires a third argument of type 
			 *		struct flock * to be passed to fcntl.
			 *
			 *		If the process already has a lock on any part of the region, the old lock 
			 *		on that part is replaced with the new lock. You can remove a lock by 
			 *		specifying a lock type of F_UNLCK.
			 *
			 *	- F_SETLKW :
			 *
			 *		This macro is used as the command argument to fcntl, to specify that it	
			 *		should set or clear a lock. It is just like the F_SETLK command, but 
			 *		causes the process to block (or wait) until the request can be specified.
			 *
			 *		This command requires a third argument of type struct flock *, as for the 
			 *		F_SETLK command.
			 *
			 *	- Full description: 
			 *	
			 *		https://www.gnu.org/software/libc/manual/html_node/File-Locks.html#File-Locks
			 */
            rc = fcntl(f, F_SETLKW, &lock_data); 
			printf("%d has locked the file!\n", getpid()); // Note: This is thread-safe, but
            struct flock l2;							   // the printing sequence between 
            if (rc != -1)								   // threads is undefined
            {
                for (i = 0; i < length1; i++)
                {
                    write(f, &text1[i], 1);
                    usleep(20); //waits 20 milisec to let slow down writing
                }
                write(f, "\n", 1);
                lock_data.l_type = F_UNLCK;
                fcntl(f, F_SETLKW, &lock_data); //unlock file
            }
        }
        int status;
        waitpid(child, &status, 0); //wait for the end of child process
    }
    else //child == 0, child process
    {
        int j;
        for (j = 0; j < 25; j++) //to avoid too big data file
        {
            lock_data.l_pid = getpid();
            lock_data.l_type = F_WRLCK;
            rc = fcntl(f, F_SETLKW, &lock_data); //lock file
			printf("%d has locked the file!\n", getpid());
            if (rc != (-1))
            {
                for (i = 0; i < length2; i++)
                {
                    write(f, &text2[i], 1);
                    usleep(20);
                }
                write(f, "\n", 1);
                lock_data.l_type = F_UNLCK;
                fcntl(f, F_SETLKW, &lock_data); //unlock file
            }
        }
    }
    close(f);
    return 0;
}
