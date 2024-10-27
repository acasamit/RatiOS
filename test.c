void do_loop(char *str)
{
	int i = 0;

	while(str[i] != '\0')
	{
		do_thing(str[i]);
		i++;
	}
	return;
}

void do_loop(char *str)
{
	int i = -1;

	while(str[++i])
		do_thing(str[i]);
}


