// Helper functions

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
