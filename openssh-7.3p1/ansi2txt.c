/*
 *     ansi2txt - A simple program that makes vt100/ansi terminal streams
 *                readable on text or html displays
 * 
 *     Copyright (C) 2007 Emmet Spier
 * 
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 * 
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 * 
 *     You should have received a copy of the GNU General Public License
 *     along with this program; if not, write to the Free Software
 *     Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DEFAULT_HEIGHT 60
#define DEFAULT_WIDTH  120

#define VT100_BUF 8
#define VT100_PARAMS 10
#define TAB 9

#define mode_bright       1
#define mode_dim          2
#define mode_underscore   8
#define mode_blink        16
#define mode_reverse      64
#define mode_hidden       128

// this extra one to support graphics characters (a bit)
#define mode_graphics     32

#define mode_wide         1
#define mode_tall_top     2
#define mode_tall_bot     4

#define m_black           0
#define m_red             1
#define m_green           2
#define m_yellow          3
#define m_blue            4
#define m_magenta         5
#define m_cyan            6
#define m_white           7

#define m_black_n_white  m_black+16*m_white
#define m_white_n_black  m_white+16*m_black

int html_refresh = 0;
int reverse_video = 0;

int cX = 0, cY = 0;
int max_cX = 0, max_cY = 0;
char *out = 0, *out_mode = 0, *out_col = 0, *out_size = 0;
int width = DEFAULT_WIDTH;
int height = DEFAULT_HEIGHT;

void clear_cells(int start, int len) {
	memset(out + start, 32, len);                       // ' '
	memset(out_mode + start, 0, len);                       // mode = 0
	memset(out_col + start, (m_white + m_black) >> 4, len); // fg white, bg black
}

void print_line(int line, FILE *dst);

int main(int argc, char **argv) {
	int loop, tmp;
	if (height < 1)
		height = DEFAULT_HEIGHT;
	if (width < 1)
		width = DEFAULT_WIDTH;

	out = (char*) malloc(width * height);
	out_mode = (char*) malloc(width * height);
	out_col = (char*) malloc(width * height);
	out_size = (char*) malloc(height);

	//cls
	clear_cells(0, width * height);  //out, out_mode, out_col, 0, width*height);
	memset(out_size, 0, height);

	if ((out == 0) || (out_mode == 0) || (out_col == 0) || (out_size == 0)) {
		printf("Memory allocation failure.\n");
		return 0;
	}

	char vt100[VT100_BUF];
	memset(vt100, 0, VT100_BUF);
	int vt100_ptr = 0;
	int vt100_params[VT100_PARAMS];
	int param_ptr = 0;

	char current_mode = 0;
	char current_col = m_white_n_black;
	int cX_save = 0, cY_save = 0;
	int q_mark = 0;
	char b = 0;
    int k_mark=0;
	FILE *f;
	FILE *dd;

	if (argc == 3) {
		f = fopen(argv[1], "r");
		if (f == NULL) {
			printf("File %s not found.\n", argv[1]);
			return 0;
		}

		dd = fopen(argv[2], "w+");
		if (dd == NULL) {
			printf("File %s not found.\n", argv[2]);
			return 0;
		}
	} else {
		printf("Must have two parameters a source file and an output file\n");
		return 0;
	}

	while (!feof(f)) {
		b = fgetc(f);
		if ((b > 31) && (b < 127)) {
			tmp = cX + cY * width;
			out[tmp] = b;
			out_mode[tmp] = current_mode;
			out_col[tmp] = current_col;
			cX++;
			printf("s1=%d  %c\n", b, b);
		}

		else {
			switch (b) {
			case 7:
				if (k_mark) {
					clear_cells(0, cX);
					cX = 0;
				}
				break;
			case 8:
				cX--;
				break;
			case 9:
				if (cX % TAB == 0) {
					cX = (cX / TAB) + TAB;
				} else {
					cX++;
				}
				break;
			case 10:
				cX = 0;
			case 11:
				cY++;
				break;
			case 12: // ^L form feed
				for (cY = 0; cY <= max_cY; cY++)
					print_line(cY, dd);
				clear_cells(0, width * height); //out, out_mode, out_col, 0, width*height);
				memset(out_size, 0, height);
				cX = 0;
				cY = 0;
				max_cX = 0;
				max_cY = 0;
				break;
			case 13:
				cX = 0;
				break;

			case 27: // ESC
				b = fgetc(f);
				printf("s3=%d %c\n", b, b);
				switch (b) { // ESC switch
				case '7':
					cX_save = cX;
					cY_save = cY;
					break; // cursor save
				case '8':
					cX = cX_save;
					cY = cY_save;
					break; // cursor restore

				case '#':
					b = fgetc(f);
					printf("s4=%d  %c\n", b, b);
					switch (b) {
					case '3':
						out_size[cY] = mode_tall_top;
						break; // Double Height top line
					case '4':
						out_size[cY] = mode_tall_bot;
						break; // Double Height bottom line
					case '5':
						out_size[cY] = 0;
						break; // Single width line
					case '6':
						out_size[cY] = mode_wide;
						break; // Double width line
					default:
						; // slurp
					}
					break;
				case 'P': // Device Control String, we slurp it up until ESC
						  //                    while (((b = fgetc(f)) != 27) && !feof(f)) ;
					b = fgetc(f);
					printf("s9=%d  %c\n", b, b);
					break;
				case '\\':
					printf("s13=%d   %c\n", b, b);
					break; // Termimation code for a Device Control String

				case '(': // Choose character set, we ignore
					b = fgetc(f);
					printf("s14=%d  %c\n", b, b);
					switch (b) {
					case '0':
						current_mode |= mode_graphics;
						break;
					default:
						current_mode &= (0xff - mode_graphics);
						break;
					}
					break;
				case ']':
					b = fgetc(f);
					printf("s22=%d  %c\n", b, b);
					switch (b) {
							case '0':
								b = fgetc(f);
								if (b ==';')
								k_mark=1;
								printf("s2233=%d  %c\n", b, b);
								break;
							default:
								break; // slurp
							}
					break;
				case '[':
					vt100_ptr = 0;
					for (loop = 0; loop < param_ptr; loop++)
						vt100_params[loop] = 0; // unset previous
					param_ptr = 0;
					b = ';';
					q_mark = 0;
					static int p_mark = 0;
					while (((b == ';') && (vt100_ptr < VT100_BUF)) && !feof(f)) {
						while (((b = fgetc(f)) <= '9') && !feof(f)) {

							vt100[vt100_ptr] = b;
							vt100_ptr++;
						}
						printf("s7=%d  %c\n", b, b);
						if (b == '?') {
							q_mark = 1;
							b = ';';
						} else {
							vt100[vt100_ptr] = 0;
							printf("s70= %d %c\n", b, b);
							vt100_params[param_ptr++] = atoi(vt100);
							vt100_ptr = 0;
						}
					}
					if (q_mark == 1) {
						printf("s10=%d %c\n", b, b);
						switch (b) { // q_mark switch
						case 8:
							cX--;
							break;
						case 9:
							cX = (cX / TAB) + TAB;
							break;
						case 10:
							cX = 0;
						case 11:
						case 12:
							cY++;
							break;
						case 13:
							cX = 0;
							break;
						default:
							;
						}
					} else {
						printf("s11=%d %c\n", b, b);
						switch (b) { // ESC action switch
						case 'H': // tab (row, col)
						case 'f':
							cY = (vt100_params[0] == 0) ?
									0 : vt100_params[0] - 1;
							cX = (vt100_params[1] == 0) ?
									0 : vt100_params[1] - 1;
							break;
						case 'A':
							cY -= (vt100_params[0] > 0) ? vt100_params[0] : 1;
							break; // cursor up
						case 'B':
							cY += (vt100_params[0] > 0) ? vt100_params[0] : 1;
							break; // cursor down
						case 'C':
							cX += (vt100_params[0] > 0) ? vt100_params[0] : 1;
							break; // cursor right
						case 'D':
							cX -= (vt100_params[0] > 0) ? vt100_params[0] : 1;
							break; // cursor left
						case 'P':
							p_mark +=1;
							printf("pp p_mark=%d, cX=%d, height=%d vt100_params[0]=%d \n", p_mark, cX, height, vt100_params[0]);
							switch (vt100_params[0]) {
							case 1:
								break;
							case 2:
								clear_cells(0, width * height);
								break;
							default:
								clear_cells(cX + cY * width,
										width * height - (cX + cY * width));
							}
							break; // cursor left
						case '@':
							printf(
									"pp cY=%d,  cX=%d, height=%d vt100_params[0]=%d b=%c\n",
									cY, cX, height, vt100_params[0], b);
							if (p_mark<=0) {
								cX -= (vt100_params[0] > 0) ?
										vt100_params[0] : 1;
								if (cX < 0)
									cX = 0;
								if ((cX - cY * width) > 0)
									clear_cells(0, cX - cY * width);
								else
									clear_cells(0, cX);
							}

							break;
						case 'd':
							cY = (vt100_params[0] == 0) ?
									0 : vt100_params[0] - 1;
							break; // vertical postion absolute
						case 'e':
							cY += vt100_params[0];
							break;                  // vertical postion relative
						case 's':
							cX_save = cX;
							cY_save = cY;
							break; // cursor save
						case 'u':
							cX = cX_save;
							cY = cY_save;
							break; // cursor restore
						case 'J': // erase screen (from cursor)
							switch (vt100_params[0]) {
							case 1:
								clear_cells(0, cX - cY * width);
								break;
							case 2:
								clear_cells(0, width * height);
								break;
							default:
								clear_cells(cX + cY * width,
										width * height - (cX + cY * width));
							}
							break;
						case 'K': // erase line (from cursor)
							switch (vt100_params[0]) {
							case 1:
								clear_cells(cY * width, cX);
								break;
							case 2:
								clear_cells(cY * width, width);
								break;
							default:
								clear_cells(cY * width + cX, width - cX);
							}
							break;
						case 'm': // color info
							for (loop = 0; loop < param_ptr; loop++) {
								if (vt100_params[loop] <= 8) {
									if (vt100_params[loop] == 0)
										current_mode &= mode_graphics; // reset all (graphics is not a mode)
									else
										current_mode |= 1
												<< (vt100_params[loop] - 1);
								}
								if ((vt100_params[loop] >= 30)
										&& (vt100_params[loop] <= 37))
									current_col = (vt100_params[loop] - 30)
											| (current_col & 0xf0);
								if ((vt100_params[loop] >= 40)
										&& (vt100_params[loop] <= 47))
									current_col = ((vt100_params[loop] - 40)
											* 16) | (current_col & 0xf);
							}
							break;
						case 'r': // DEC termial top and bottom margin scroll free areas
						case 'h': // Mode Set (4 = insert; 20 = auto linefeed)
						case 'l': // Mode Reset  (4 = insert; 20 = auto linefeed)
						case '?': // more stuff to ignore...
							break;
						default:
							;
						}
						break;
						default:
						ungetc(b, f);
					}
				}

				break;
			default:
				;
			}
		}
		if (cX < 0) {
			cX = width - 1;
			cY--;
		}
		if (cY < 0) {
			cY = 0;
		}
		if (cX >= width) {
			cX = 0;
			cY++;
		}

		if (cY >= height) { // height overflow so scroll buffer and print overflow
			tmp = cY - height + 1;
			if (tmp >= height)
				tmp = height - 1;
			for (loop = 0; loop < tmp; loop++)
				print_line(loop, dd);
			memmove(out, out + tmp * width, (height - tmp) * width);
			memmove(out_mode, out_mode + tmp * width, (height - tmp) * width);
			memmove(out_col, out_col + tmp * width, (height - tmp) * width);
			memmove(out_size, out_size + tmp, height - tmp);
			clear_cells((height - tmp) * width, tmp * width);

			cY = height - 1;
		}

		if (cX * ((out_size[cY] != 0) ? 2 : 1) > max_cX)
			max_cX = cX * ((out_size[cY] != 0) ? 2 : 1);
		if (cY > max_cY)
			max_cY = cY;
	}

	for (cY = 0; cY <= max_cY; cY++) {
		print_line(cY, dd);
	}
	fclose(f);
	fclose(dd);
	return 1;
}

void print_line(int line, FILE *dst) {
	int wide_set = 0, tall_set = 0;
	int cX, cY = line;
	int tmp = cY * width;

	if (out_size[cY] & mode_wide)
		wide_set = 1;
	else
		wide_set = 0;
	if (out_size[cY] & mode_tall_top)
		tall_set = 1;
	else
		tall_set = 0;

	for (cX = 0; cX <= max_cX; cX++) {
		tmp = cX + cY * width;

		if (!((wide_set || tall_set) && (cX > (max_cX / 2)))) {

			if (out_mode[tmp] & mode_hidden) {
				printf(" ");
				fprintf(dst, " ");
			} else {
				printf("%c", out[tmp]);
				fprintf(dst, "%c", out[tmp]);
			}
			if (wide_set && (!tall_set)) {
					printf(" ");
				fprintf(dst, " ");
			}
		}
	}
	printf("\n");
	fprintf(dst, "\n");
}
