// vim:fdm=marker
// Név: Németh Tamás
// Neptun-kód: HADD7B

/* Macros {{{ */
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define LEN(MSG) (sizeof(MSG))

#define SEPARATOR "_____________________________"
#define TITLE  "King of Strands nyilvántartás"
#define WELCOME   "\n" TITLE "\n" SEPARATOR "\n\n"
#define BOUND 512
#define FNEXISTS "A rendezvény nem létezik.\n"
#define QNEXISTS "A vendég nem található.\n"
#define FCRTD "A megadott rendezvény még nem létezik, így létre lett hozva.\n"
/* }}} */

/* Structures {{{*/
struct menu_item
{
	void (*funcptr)(char*, char*, char*);
	char* name;
	int msg_count;
	char* msgs[3];	
};
/* }}}*/

/* Helper functions {{{*/
char** split_str(char* str, size_t str_len, char delimiter)
{
	int d_arr[str_len];
	// array containing offset of delimiters
	d_arr[0] = -1;
	// d_size is incremented when a delimiter is found
	int d_size = 1;
	for(int i=0; i<=str_len; ++i)
	{
		if(str[i] == delimiter)
		{
			d_arr[d_size] = i;
			++d_size;
		}
		else if(str[i] == '\0')
		{
			d_arr[d_size] = str_len;
			++d_size;
			break;
		}
	}
	
	char** tokens;
	tokens = mmap(NULL, sizeof(char)*(d_size+1),
		PROT_READ|PROT_WRITE,
		MAP_PRIVATE|MAP_ANONYMOUS,
		-1, 0);
	for(int i=0; i<d_size-1; ++i)
	{
		tokens[i] = mmap(NULL, sizeof(tokens[0])*(d_arr[i+1]),
			PROT_READ|PROT_WRITE,
			MAP_PRIVATE|MAP_ANONYMOUS,
			-1, 0);
		memcpy(tokens[i], &str[d_arr[i]+1], d_arr[i+1] - d_arr[i] - 1);
		tokens[i][d_arr[i+1]] = '\0';
	}
	// terminator token is a string comprising a terminator character
	tokens[d_size-1] = mmap(NULL, sizeof(tokens[0]),
			PROT_READ|PROT_WRITE,
			MAP_PRIVATE|MAP_ANONYMOUS,
			-1, 0);
	tokens[d_size-1] = "\0";

	return tokens;
}

int equals(char* strl, char* strr)
{
	return (*strl) && (*strr) ? 
		*strl == *strr ? equals(++strl, ++strr) : 0 
		: (*strl) || (*strr) ? 0 : 1;
}

size_t str_len(char* str)
{ 
	return (*str) ? str_len(++str) + 1 : 0;
}

char* concat(char* strl, char* strr)
{
	size_t endl = 0;
	size_t endr = 0;
	for(; strl[endl] != '\0'; ++endl) { }
	for(; strr[endr] != '\0'; ++endr) { }

	char* retval = mmap(NULL, sizeof(char)*(endl+endr),
		PROT_READ|PROT_WRITE,
		MAP_PRIVATE|MAP_ANONYMOUS,
		-1, 0);

	for(int i=0; i<endl; ++i)
	{
		retval[i] = strl[i];
	}
	for(int i=0; i<endr; ++i)
	{
		retval[i+endl] = strr[i];
	}

	return retval;
}

char* itoa(int num)
{
	if(num == 0) return "0";

	int length = (int)((floor(log10(num))+1)*sizeof(char));

	char* retval = mmap(NULL, sizeof(char)*(length + 1),
		PROT_READ|PROT_WRITE,
		MAP_PRIVATE|MAP_ANONYMOUS,
		-1, 0);

	for(int i=length; i>0; --i)
	{
		int remainder = (num % (int)pow(10, length - i + 1)) / (int)pow(10, length - i);
		num -= remainder;
		retval[i-1] = remainder + '0';
	}

	return retval;
}

void rm_newline(char* str)
{
	return (*str) && *str == '\n' ? *str = '\0' : rm_newline(++str);
}

int inc_guest_count(char* str, int quantity)
{
	//guestdir/.eventname
	char* prefix = "events/.";
	char* loc = concat(prefix, str);
	int fd = open(loc, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);		
	
	char buf[BOUND] = {'\0'};
	read(fd, buf, BOUND);
	int count = atoi(buf) + quantity;
	memset(buf, 0, BOUND);
	close(fd);

	fd = open(loc, O_RDWR|O_TRUNC, S_IRWXU|S_IRWXG);
	write(fd, itoa(count), str_len(itoa(count)));
	close(fd);
	
	return count;
}

/* }}}*/

/* Members of the menu {{{*/
void add_query(char* query)
{
	char** strs = split_str(query, str_len(query), ',');
	char* fname = strs[2];
	if(fname[0] == ' ') ++fname;

	query = concat(concat(strs[0], ","), strs[1]);

	if(access(fname, R_OK|W_OK) < 0)
	{
		write(2, FCRTD, LEN(FCRTD));
	}

	int fd = open(fname, O_RDWR|O_APPEND|O_CREAT, S_IRWXU|S_IRWXG);
	time_t ct = time(NULL);

	if(write(fd, concat(concat(query, ", "), ctime(&ct)), str_len(query)
		+ str_len(ctime(&ct)) + 2))
	{
		int query_id = inc_guest_count(fname, 1);
		close(fd);
		write(1, concat(itoa(query_id), ". jelentkező!\n"), BOUND);
	}
}

void list_queries(char* fname)
{
	if(access(fname, R_OK) < 0)
	{
		write(1, FNEXISTS, LEN(FNEXISTS));
		return;
	}

	int fd = open(fname, O_RDONLY);
	off_t FSIZE = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);

	char buf[FSIZE];
	read(fd, buf, FSIZE);
	write(1, buf, FSIZE);
	close(fd);
}

void modify_query(char* fname, char* query, char* newstr)
{
	if(access(fname, R_OK|W_OK) < 0)
	{
		write(2, FNEXISTS, LEN(FNEXISTS));
		return;
	}

	int fd = open(fname, O_RDONLY);
	
	off_t FSIZE = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);
	char fcontent[FSIZE];
	read(fd, fcontent, FSIZE);
	close(fd);

	char** lines = split_str(fcontent, FSIZE, '\n');
	char** offset = &(*lines);
	int ls = FSIZE;
	char* target = split_str(query, ls, ',')[0];
	char* line = split_str(*lines, ls, ',')[0];
	for(; line[0] != '\0' && !equals(line, target); line = split_str(*(++lines), ls, ',')[0]) { }
	if(equals(line, target))
	{
		*lines = newstr;

		fd = open(fname, O_RDWR|O_TRUNC, S_IRWXU|S_IRWXG);
		time_t ct = time(NULL);
		for(; *offset[0] != '\0'; ++offset)
		{
			write(fd, concat(concat(*offset, ", "), ctime(&ct)),
			str_len(*offset) + str_len(ctime(&ct)) + 2); //size of row
		}
		close(fd);
	}
	else
	{
		write(2, QNEXISTS, LEN(QNEXISTS));
	}
}

void remove_query(char* fname, char* query)
{
	modify_query(fname, query, "");
	inc_guest_count(fname, -1);
}

void create_file(char* fname)
{
	int fd = open(fname, O_RDWR|O_CREAT, S_IRWXU|S_IRWXG);
	close(fd);
}

void trunc_file(char* fname)
{
	int fd = open(fname, O_RDWR|O_TRUNC, S_IRWXU|S_IRWXG);
	close(fd);
	char* prefix = "events/.";
	char* loc = concat(prefix, fname);
	unlink(loc);
}

void __exit(char* anws)
{
	if(equals("Y", anws) || equals("y", anws)) _exit(0);
}
/* }}}*/

/* Dispatcher {{{*/
struct menu_item dispatcher[] =
{
	{
		(void*)add_query,
		"Vendég felvétele",
		1,
		{
			"Vendég adatai:"
		}
	},
	{
		(void*)list_queries,
		"Vendégek listázása",
		1,
		{
			"Rendezvény neve:"
		}
	},
	{
		(void*)modify_query,
		"Vendég adatainak módosítása",
		3,
		{
			"Rendezvény neve:",
			"Vendég neve:",
			"Új bejegyzés:"
		}
	},
	{
		(void*)remove_query,
		"Vendég törlése",
		2,
		{
			"Rendezvény neve:",
			"Vendég neve:"
		}
	},
	{
		(void*)create_file,
		"Rendezvény létrehozása",
		1,
		{
			"Rendezvény neve:"
		}
	},
	{
		(void*)trunc_file,
		"Rendezvény összes vendégének törlése",
		1,
		{
			"Rendezvény neve:"
		}
	},
	{
		(void*)__exit,
		"Kilépés",
		1,
		{ 
			"Biztosan kilép?(Y/y)"
		}
	}
};
/* }}}*/

/* Menu {{{ */
extern const int mlen = sizeof(dispatcher)/sizeof(struct menu_item);

void print_menu()
{
	write(1, WELCOME, LEN(WELCOME));

	for(int i=0; i<mlen; ++i)
	{
		write(1, concat(itoa(i), ": "), 3);
		write(1, dispatcher[i].name, str_len(dispatcher[i].name));
		write(1, "\n", 2);
	}
}
/* }}} */

