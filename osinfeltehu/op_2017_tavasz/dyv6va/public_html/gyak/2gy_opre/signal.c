#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

//https://www.gnu.org/software/libc/manual/html_node/Function-Index.html

void handler(int signumber)
{
    printf("Signal with number %i has arrived\n", signumber);
}

int main()
{
	/** The signal function establishes its first argument as the 
	 * action for its second argument.
	 *
	 * The first argument, signum, identifies the signal whose 
	 * behavior you want to control, and should be a signal number.
	 * The proper way to specify a signal number is with one of 
	 * the symbolic signal names. See:
	 * https://www.gnu.org/software/libc/manual/html_node/Standard-Signals.html#Standard-Signals
	 *
	 * The second argument specifies the action to use for
	 * the signal (first argument). This can be one of the following:
	 *
	 *  # SIG_DFL : Specifies the default action for the particular 
	 *			   signal. The default actions for various kinds 
	 *			   of signals are stated in: 
	 *			   https://www.gnu.org/software/libc/manual/html_node/Standard-Signals.html#Standard-Signals
	 *  # SIG_IGN : Specifies that the signal should be ignored.
	 *
	 * https://www.gnu.org/software/libc/manual/html_node/Basic-Signal-Handling.html#index-signal-2
	 */
    signal(SIGTERM, handler);

    pid_t child = fork();
    if (child > 0)
    {
		/** The pause function suspends program execution until a 
		 * signal arrives whose action is either to execute a 
		 * handler function, or to terminate the process.
		 */
        pause();
        printf("Signal arrived\n", SIGTERM);
        int status;
        wait(&status);
        printf("Parent process ended\n");
    }
    else
    {
        printf("Waits 3 seconds, then send a SIGTERM %i signal\n", SIGTERM);
        sleep(3);
		/** int kill(pid_t pid, int signum);
		 *
		 * The kill function sends the signal signum to the process 
		 * or process group specified by pid. Besides the signals  //process group is defined later
		 * listed in Standard Signals, signum can also have a value 
		 * of zero to check the validity of the pid.
		 *
		 * If pid == -1: If the process is privileged, send the 
		 * signal to all processes except for some special system 
		 * processes. Otherwise, send the signal to all processes 
		 * with the same effective user ID.
		 *
		 * The kill function can be used to send a signal to another 
		 * process. In spite of its name, it can be used for a lot 
		 * of things other than causing a process to terminate. Some 
		 * examples of situations where you might want to send signals 
		 * between processes are:
		 *
		 *	- A parent process starts a child to perform a task 
		 *	  (perhaps having the child running an infinite loop)and 
		 *	  then terminates the child when the task is no longer 
		 *	  needed.
		 *
		 *	- A process executes as part of a group, and needs to 
		 *	  terminate or notify the other processes in the group when 
		 *	  an error or other event occurs.
		 *
		 *	- Two processes need to synchronize while working together.
		 * https://www.gnu.org/software/libc/manual/html_node/Signaling-Another-Process.html#index-kill
		 */
        kill(getppid(), SIGTERM);
		sleep(1);
        printf("Child process ended\n");
    }
    return 0;
}
/** Every process belongs to a process group. When a process is created, 
 * it becomes a member of the same process group and session as its 
 * parent process. You can put it in another process group using the 
 * setpgid function, provided the process group belongs to the same 
 * session.
 *
 * (my comments) This is why the parent process sees the sent signal from
 * the child process, even though the child process sent the signal to
 * its own pid.
 *
 * https://ftp.gnu.org/old-gnu/Manuals/glibc-2.2.3/html_chapter/libc_27.html#SEC573
 */