/*------------------------------------------------------------------------------------------------------------------------------
//                                                    _   _ ___ _     ___  
//                                                   | | | |_ _| |   / _ \
//                                                   | |_| || || |  | | | |
//                                                   |  _  || || |__| |_| |
//                                                   |_| |_|___|_____\___/ 
//                                                    Hardware in the loop
//
//
// File:  utilities.c
//
// Author: Silvano Catinella <catinella@yahoo.com>
//
// Description:
//		It is a collection of utilities used by other files
//		
// License:  LGPL ver 3.0
//
// 		This script is a wfree software; you can redistribute it and/or modify it under the terms	of the GNU
// 		Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License,
// 		or (at your option) any later version. 
//
//		For further details please read the full LGPL text file [https://www.gnu.org/licenses/lgpl-3.0.txt]
// 		You should have received a copy of the GNU General Public License along with this file; if not, write to the 
//
//			Free Software Foundation, Inc.,
//			59 Temple Place, Suite 330,
//			Boston, MA  02111-1307  USA
//
//
------------------------------------------------------------------------------------------------------------------------------*/
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
