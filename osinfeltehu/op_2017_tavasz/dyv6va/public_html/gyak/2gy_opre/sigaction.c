#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

void handler(int signumber)
{
    printf("Signal with number %i has arrived\n", signumber);
}

int main()
{
	/** Structures of type struct sigaction are used in the sigaction function to 
	 * specify all the information about how to handle a particular signal. 
	 * This structure contains at least the following members:
	 *
	 *	- sa_handler : This is used in the same way as the action argument to the 
	 *				   signal function. The value can be SIG_DFL, SIG_IGN, or a 
	 *				   function pointer.
	 *
	 *		   		   # SIG_DFL : Specifies the default action for the particular 
	 *							   signal. The default actions for various kinds 
	 *							   of signals are stated in: 
	 *							   https://www.gnu.org/software/libc/manual/html_node/Standard-Signals.html#Standard-Signals
	 *		 		   # SIG_IGN : Specifies that the signal should be ignored.
	 *
	 *				   # function pointer : Signal handlers take one integer 
	 *										argument specifying the signal number, 
	 *										and have return type void.
	 *
	 *	- sa_mask : This specifies a set of signals to be blocked while the 
	 *				handler runs
	 *
	 *	- sa_flags : This specifies various flags which can affect the behavior 
	 *				 of the signal.
	 *
	 *	https://www.gnu.org/software/libc/manual/html_node/Advanced-Signal-Handling.html#Advanced-Signal-Handling
	 */
    struct sigaction sigact;
    sigact.sa_handler = handler;
	
	/** All of the signal blocking functions use a data structure called a signal 
	 * set to specify what signals are affected. Thus, every activity involves two 
	 * stages: creating the signal set, and then passing it as an argument to a 
	 * library function.
	 *
	 * There are two ways to initialize a signal set. You can initially specify it 
	 * to be empty with sigemptyset and then add specified signals individually. 
	 * Or you can specify it to be full with sigfillset and then delete specified 
	 * signals individually.
	 *
	 * You must always initialize the signal set with one of these two functions 
	 * before using it in any other way.
	 *
	 * This function initializes the signal set that was passed as a parameter 
	 * to exclude all of the defined signals. It always returns 0.
	 *
	 * Blocking a signal means telling the operating system to hold it and deliver 
	 * it later. Generally, a program does not block signals indefinitely—it might 
	 * as well ignore them by setting their actions to SIG_IGN. But it is useful 
	 * to block signals briefly, to prevent them from interrupting sensitive 
	 * operations. For instance:
	 *	
	 *	- You can use the sigprocmask function to block signals while you modify 
	 *	  global variables that are also modified by the handlers for these signals.
	 *	- You can set sa_mask in your sigaction call to block certain signals while 
	 *	  a particular signal handler runs. This way, the signal handler can run 
	 *	  without being interrupted itself by signals.
	 *
	 * https://www.gnu.org/software/libc/manual/html_node/Signal-Sets.html#index-sigemptyset
	 */
    sigemptyset(&sigact.sa_mask); //during execution of handler these signals will be blocked plus the signal
    //now only the arriving signal, SIGTERM will be blocked
    sigact.sa_flags = 0; //nothing special behaviour
	/** The sigaction function has the same basic effect as signal: to specify how 
	 * a signal should be handled by the process. However, sigaction offers more 
	 * control, at the expense of more complexity. In particular, sigaction allows 
	 * you to specify additional flags to control when the signal is generated and 
	 * how the handler is invoked.
	 *
	 * int sigaction(int signum, const struct sigaction *restrict action, 
	 *										struct sigaction *restrict old-action);
	 *
	 * The action argument is used to set up a new action for the signal signum, 
	 * while the old-action argument is used to return information about the action 
	 * previously associated with this signal. (In other words, old-action has the 
	 * same purpose as the signal function’s return value—you can check to see what 
	 * the old action in effect for the signal was, and restore it later if you want.)
	 *
	 * Either action or old-action can be a null pointer. If old-action is a null 
	 * pointer, this simply suppresses the return of information about the old action. 
	 * If action is a null pointer, the action associated with the signal signum is 
	 * unchanged; this allows you to inquire about how a signal is being handled without 
	 * changing that handling.
	 */
    sigaction(SIGTERM, &sigact, NULL);
    sigaction(SIGUSR1, &sigact, NULL);
    pid_t child = fork();
    if (child > 0)
    {
        sigset_t sigset;
		/** This function initializes the signal set set to include all of the 
		 * defined signals.
		 */
        sigfillset(&sigset);
		/** int sigdelset (sigset_t *set, int signum);
		 * This function removes the signal signum from the signal set set. All 
		 * sigdelset does is modify set; it does not block or unblock any signals. The
		 * return value and error conditions are the same as for sigaddset.
		 */
        sigdelset(&sigset, SIGTERM);
        sigsuspend(&sigset);
        // like pause() - except it waits only for signals not given in sigset
        //others will be blocked
        printf("The program comes back from suspending\n");
        int status;
        wait(&status);
        printf("Parent process ended\n");
    }
    else
    {
        printf("Waits 3 seconds, then send a SIGUSR %i signal (it is not waited for by suspend) - so the suspend continues waiting\n", SIGUSR1);
        sleep(3);
        kill(getppid(), SIGUSR1);
        printf("Waits 3 seconds, then send a SIGTERM %i signal (it is waited for by suspend)\n", SIGTERM);
        sleep(3);
        kill(getppid(), SIGTERM);
        printf("Child process ended\n");
    }
    return 0;
}