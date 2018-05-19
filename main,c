#include <stdio.h>
#include <stdlib.h>
#include "BigInteger.h"

int main(void) {
	bint a = string_to_bint("-1920141206309129219096030192491204920630102316363533"),
	b = string_to_bint("12301290296039019501290312903291052910549210421");
	bint c = bint_mod(a, b);

	print_bint(a);
	printf(" %% ");
	print_bint(b);
	printf(" = ");
	print_bint(c);

	free_bint(a);
	free_bint(b);
	free_bint(c);
	return 0;
}
