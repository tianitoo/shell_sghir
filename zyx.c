#include <libc.h>

int main(int ac, char **av, char **envp)
{
   int i = 0;
   char *env_var = envp[i];
   while(env_var != NULL)
   {
		printf("%s\n",env_var);
		env_var = envp[i];
		i++;
   }
}