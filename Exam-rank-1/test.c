#include <stdio.h>

int ft_printf(char const *fmt, ...);
int main()
{

	ft_printf("%d\n", (int)-2147483649);
	ft_printf("%x\n", 0);
}