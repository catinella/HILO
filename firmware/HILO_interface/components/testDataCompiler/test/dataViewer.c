/*------------------------------------------------------------------------------------------------------------------------------
//                                                    _   _ ___ _     ___  
//                                                   | | | |_ _| |   / _ \
//                                                   | |_| || || |  | | | |
//                                                   |  _  || || |__| |_| |
//                                                   |_| |_|___|_____\___/ 
//                                                    Hardware in the loop
//
//
// File:   dataViewer.c
//
// Author: Silvano Catinella <catinella@yahoo.com>
//
// Description:
//		It is a useful tool to check for the resulted data by the mocked functions. This tool realizes a virtual digital
//		states alalizer, mainly. Its TUI uses NCURSES library, so check for this dependence before to compile it.
//
// License:  LGPL ver 3.0
//
// 		This script is a wfree software; you can redistribute it and/or modify it under the terms	of the GNU
// 		Lesser General Public License as published by the Free Software Foundation; either version 3.0 of the License,
// 		or (at your option) any later version. 
//
//		For further details please read the full LGPL text file  "Linuxwoodo/trunk/templates/lgpl-3.0.txt".
// 		You should have received a copy of the GNU General Public License along with this file; 
// 		if not, write to the 
//
//			Free Software Foundation, Inc.,
//			59 Temple Place, Suite 330,
//			Boston, MA  02111-1307  USA
//
//
------------------------------------------------------------------------------------------------------------------------------*/
#include <stdint.h>
#include <debugTools.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <wError.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#define SWIN_SIZE 20
#define GRAYCOLOR 10

typedef uint16_t TDdata_t;

wError dataDrawing (WINDOW *win, FILE *fh, uint32_t offset) {
	wError err = WERROR_SUCCESS;

	// Labels printing
	for (uint8_t b = 0; b < (8 * sizeof (TDdata_t)); b++)
		mvwprintw (win, (b + 2), 2, "bit%02d: ", b);

	// Loocking for the data-block
	if (fseek (fh, (offset * sizeof (TDdata_t)), SEEK_SET) < 0) {
		// ERROR!
		err = WERROR_ERRUTEST_IOERROR;

	} else {
		TDdata_t bitconf = 0;
		size_t   psize;
		uint16_t spliter_counter = offset;
		uint8_t  spliter_size = 0;

		// Data printing
		for (uint8_t col = 0; (col + spliter_size) < (COLS - 20); col++) {
			if ((psize = fread (&bitconf, 1, sizeof (bitconf), fh)) && psize == sizeof (bitconf)) {
				
				mvwprintw(win, ((8 * sizeof (TDdata_t)) + 2), (col + 8 + spliter_size), "    ");
				if ((spliter_counter % 10) == 0) {
					char tmp[8];

					wattron(win, COLOR_PAIR(GRAYCOLOR));
					for (uint8_t y = 0; y < (8 * sizeof (TDdata_t)); y++) 
						mvwprintw(win, (y + 2), (col + 8 + spliter_size), "|");

					sprintf(tmp, "%d", (int)trunc(spliter_counter/10));
					mvwprintw(win, ((8 * sizeof (TDdata_t)) + 2), (col + 8 + spliter_size), tmp);

					spliter_size++;
					wattroff(win, COLOR_PAIR(GRAYCOLOR));
				}
				
				for (uint8_t y = 0; y < (8 * sizeof (TDdata_t)); y++) {
					if ((bitconf & 1 << y) > 0)
						mvwprintw(win, (y + 2), (col + 8 + spliter_size), "#");

					else
						mvwprintw(win, (y + 2), (col + 8 + spliter_size), "_");
				}
				spliter_counter++;
				
			} else {
				fprintf (stderr, "ERROR! reading operation failed: %s\n", strerror (errno));
				err = WERROR_ERRUTEST_IOERROR;
				break;
			}
		}
	}

	return (err);
}

//------------------------------------------------------------------------------------------------------------------------------
//                                                      M A I N
//------------------------------------------------------------------------------------------------------------------------------
int main (int argc, char *argv[]) {
	wError err = WERROR_SUCCESS;
	FILE   *fh = NULL;
	
	if (argv[1] == NULL || strlen(argv[1]) == 0) {
		// ERROR!
		fprintf(stderr, "ERROR! Use %s <binary-data file>\n", argv[0]);
		err = WERROR_ERROR_ILLEGALARG;
		
	} else if ((fh = fopen(argv[1], "r")) == NULL) {
		// ERROR!
		fprintf(stderr, "ERROR! I cannot open the \"%s\" file\n", argv[1]);
		err = WERROR_ERROR_ILLEGALARG;
	
	} else {
		WINDOW *win = NULL;
		int    opt;

		initscr();
		cbreak();
		noecho();
		curs_set(0);

		{
			// Actual size
			int rows, cols; getmaxyx(stdscr, rows, cols);
			
			// Window size
			int h = SWIN_SIZE, w = cols - 2;
			if (w < 4)  w = cols;            // fallback
			if (h > rows - 1) h = rows - 1;  // Saving one line for the status banner
			if (h < 3)        h = rows;      // Fix
			
			// Windows position
			int y = (rows - h) / 2;
			int x = (cols - w) / 2;
			if (y < 0) y = 0;
			if (x < 0) x = 0;

			win = newwin(h, w, y, x);
		}

		if (win == NULL) {
			// ERROR
			err = WERROR_ERROR_INTERNALS;
			fprintf(stderr, "ERROR! I cannot create new windows\n");

		} else {
			uint32_t pos = 0;
			int      gray = (COLORS >= 16) ? 8 : COLOR_WHITE;   // fallback for 8-colors palette
			
			keypad(win, TRUE);
			nodelay(win, FALSE);
			set_escdelay(300);
			wtimeout(win, -1);
			
			mvprintw(LINES-2, COLS/2-8, "Press: [q] to quit or LEFT/RIGHT arrows to scroll the data");
			refresh();

			start_color();
			use_default_colors();  // It allows transparent background (bgcolor=-1)
		
			if (has_colors() && COLORS >= 16) {
				start_color();
				init_pair(GRAYCOLOR, gray, -1); // 8 = gray
			}
		
			box(win, 0, 0);
			mvwprintw(win, 0, 4, "[ Logic analyzer ]");

			while (opt != 'q' && WERROR_ISERROR(err) == false) {
				if ((err = dataDrawing(win, fh, pos)) && WERROR_ISERROR(err) == false) {
					wrefresh(win);
					opt = wgetch(win);
				}
				if      (opt == KEY_LEFT && pos > 0) pos--;
				else if (opt == KEY_RIGHT)           pos++;
			}
		
			delwin(win);
		}
		endwin();
		fclose(fh);
	}

	
	return(err);
}
