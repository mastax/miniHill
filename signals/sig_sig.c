#include "../mini_shell.h"

int get_sigint(int n)
{
	static int v = 0;
	
	if (n != -500)
		v = n;
	return (v);
}
int get_sigquit(int n)
{
	static int v = 0;
	
	if (n != -500)
		v = n;
	return (v);
}
int get_pid(int n)
{
	static int v = 0;
	
	if (n != -500)
		v = n;
	return (v);
}
int get_exit_status(int n)
{
	static int v = 0;
	
	if (n != -500)
		v = n;
	return (v);
}
int get_in_heredoc(int n)
{
	static int v = 0;
	
	if (n != -500)
		v = n;
	return (v);
}