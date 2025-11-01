#include <utilities.h>
#include <stdint.h>
#include <ctype.h>

char *strupr(const char *s) {
	char p[TD_STRINGMAXSIZE];
	uint16_t idx = 0;
	while (p[idx] != '\0') {
		p[idx] = toupper((unsigned char)*s);
		idx++;
	}
	p[idx] = '\0';

	return (char*)p;
}
