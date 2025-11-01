#include <utilities.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

char *strupr(const char *s) {
	static char p[TD_STRINGMAXSIZE];
	uint16_t idx = 0;
	memset(p, '\0', (strlen(s)+1) * sizeof(char));

	while (s[idx] != '\0' && idx < (TD_STRINGMAXSIZE-2)) {
		p[idx] = toupper(s[idx]);
		idx++;
	}
	p[idx] = '\0';

	return (char*)p;
}
