
/*****************************************************************************
 * Copyright (C) Mayuri Jangam  mayurijangam4@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/


					/*==============================calender================================*/


#include<ncurses.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define TRUE 1
#define MAXCH 51
#define CH '_'
#define LEAP_YEAR      (year % 4 == 0 && year % 100!= 0 || year % 400 == 0)
#define WIDTH 80
#define HEIGHT 37
#define MAXNO 91
int startx = 0;
int starty = 0;
char *days[] = { 
			"SUN",
			"MON",
			"TUE",
			"WED",
			"THU",
			"FRI",
			"SAT",
		  };
int n_choices = sizeof(days) / sizeof(char *);
int monthday[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
char *monthname[] = {
			"January",
          		 "February",
		         "March",
		         "April",
	       	         "May",
		         "June",
		         "July",
                         "August",
		         "September",
		         "October",
		         "November",
		         "December",
		
		    };
char *monthname1[] = {
			"JAN",
			"FEB",
			"MAR", 
			"APR",
			"MAY",
			"JUN",
			"JUL",
			"AUG",
			"SEP",
                        "OCT",
			"NOV",
			"DEC",
		
			};
int fm( int date, int month, int year);
void printdate(WINDOW *menu_win,int month,int);
void savefile(WINDOW *menu_win, int M,int y, int z);
int getkey(WINDOW *menu_win);
int firstday(int date, int month, int year);	
int main(){
	WINDOW *menu_win, *menu_win1, *menu_win3, *menu_win4, *menu_win5;
	int highlight = 1, choice = 0, row, col, z, keycode, flag = 1, x, y, i, c, month, year;
	getmaxyx(stdscr,row,col);
	char mesg[]="*******************CALENDER********************";
	initscr();
	start_color();
	clear();
	noecho();
	cbreak();
	startx =  15;
	starty = 5;
	attron(A_BOLD);
	mvprintw(3, 29, "%s", mesg);
	attroff(A_BOLD);	
	init_color(0, 1000, 0, 0);
	init_pair(2, COLOR_BLACK, COLOR_WHITE);
	init_pair(1, COLOR_BLACK,COLOR_YELLOW);
	init_pair(3, COLOR_RED, COLOR_WHITE);
	init_pair(4, COLOR_WHITE, COLOR_BLACK);
	init_pair(5,COLOR_BLACK,COLOR_WHITE);
	init_pair(6, COLOR_BLACK,COLOR_WHITE);
	init_pair(7, COLOR_BLACK,COLOR_CYAN);
	init_pair(8, COLOR_RED,COLOR_WHITE);
label1:	menu_win = newwin(HEIGHT, WIDTH, starty, startx);
	keypad(menu_win, TRUE);
	refresh();
	while( flag == 1){
		wclear(menu_win);
		wattron(menu_win,COLOR_PAIR(5));
		mvwprintw(menu_win,1,1,"enter mm-yyyy (e.g 10-2016)");
		wscanw(menu_win ,"%d-%d",&month,&year);
		if(month > 12 || month < 1 || year < 0 ){
			mvwprintw(menu_win, 9,29, " Enter VALID I/P!!!!!");
			wgetch(menu_win);
			wrefresh(menu_win);
			flag = 1;
		}
		else
			flag = 0;
	}	
	wattroff(menu_win, COLOR_PAIR(5));
	while (1) {
		wclear(menu_win);
		wbkgd(menu_win, COLOR_PAIR(1));
		flag  = 1;
		z = firstday(1,  month, year);
		if(z == 0)
			z = 7;
		wattron(menu_win, A_BOLD);
		wattron(menu_win, COLOR_PAIR(4));
		mvwprintw(menu_win, 2, 32, "%s-%d", monthname[month - 1], year);
		wattroff(menu_win, COLOR_PAIR(4));
		wattroff(menu_win, A_BOLD);
		monthday[1] = LEAP_YEAR ? 29 : 28;					
		printdate(menu_win, month ,z);
		keycode = getkey(menu_win);						
		if ( keycode == 261) {
			year++;
		}
		if( keycode == 260 ) {
			year--;
		}
		if( keycode == 259) {
			month++;
			if( month > 12) {
				month = 1;
				year++;
			}
		}
		if (keycode == 258) {
			month--;
			if( month < 1){
				month = 12;
				year--;
			}
		}

		/***************** saving calender (key P) ****************/

		if (keycode == 112) {
			savefile(menu_win5, month, year, z);
		}

		/*****************new month new yr (key I ) ****************/

		if (keycode == 105) {
			goto label1;
		}

		/******************create event ( key E ) *****************/

		if (keycode == 101) { 
			menu_win4 = newwin(HEIGHT, WIDTH, starty, startx);	
			wclear(menu_win4);
			wrefresh(menu_win4);
			FILE *fp;
			char mesg[127] = "Event created successfully!!!! ";
			char str[128], buffer[12], ch;
			keypad(menu_win, TRUE);
			char  filename[12];
			strcpy(filename, monthname[month - 1]);
			snprintf(buffer, sizeof(str), "%d", year);
			strcat(filename, buffer);
			strcat(filename, ".txt");
			fp = fopen(filename, "w");
			if(fp == NULL){
				wprintw(menu_win4,"cannot create event\n");
				wgetch(menu_win4);
				goto label1;
			}
			while ((ch = wgetch(menu_win4)) != 10 ) {
				wprintw(menu_win4,"%c",ch);
				fprintf(fp,"%c",ch);
			}
			fclose(fp);
			wrefresh(menu_win);
			wrefresh(menu_win);
			if( ch == 10) {
				wattron(menu_win4, COLOR_PAIR(5));
				mvwprintw(menu_win4, 10, 10, " %s", mesg);
				wattroff(menu_win4, COLOR_PAIR(5));
				wrefresh(menu_win4);
				wgetch(menu_win4);
				goto label1;
			}
		}

		/**********************shows events (key X) *************/

		if(keycode == 120){ 
			char ch;
			FILE *fp12;
			menu_win3 = newwin(HEIGHT, WIDTH, starty, startx);
			keypad(menu_win3, TRUE);	
			wclear(menu_win3);
			wrefresh(menu_win3);
			wattron(menu_win3, A_BOLD);
			mvwprintw(menu_win3, 2, 32, "%s-%d\n\n\t Events :\n\t", monthname[month - 1], year);
			wattroff(menu_win3,A_BOLD);
			char str12[128], buffer12[12], filename12[12]; 
			strcpy(filename12, monthname[month - 1]);
			snprintf(buffer12, sizeof(str12), "%d", year);
			strcat(filename12, buffer12);
			strcat(filename12,".txt");
			fp12 = fopen(filename12, "r");
			if (fp12 == NULL) {
				wclear(menu_win3);
				wattron(menu_win3,A_BOLD);
				wattron(menu_win3, COLOR_PAIR(5));
				mvwprintw(menu_win3, 5, 32, "No Events Available!!!!!!");
				wattroff(menu_win3, COLOR_PAIR(5));
				wrefresh(menu_win3);
				wattroff(menu_win3, A_BOLD);
				wgetch(menu_win3);
				goto label1;
			}
			while(!feof(fp12)){
				fscanf(fp12, "%c", &ch);
				wprintw(menu_win3, "%c", ch);
			}
			wrefresh(menu_win3);
			wgetch(menu_win3);
			delwin(menu_win3);
		}
	
		/******************datewise calender ********************************/

		if(keycode == 100){                               
			int h = z -1, x, y;
			char ch;
			menu_win1 = newwin(HEIGHT, WIDTH, starty, startx);
			keypad(menu_win, TRUE);	
			wclear(menu_win1);
			wbkgd(menu_win, COLOR_PAIR(1));
			wattron(menu_win1, A_BOLD);
			wrefresh(menu_win);
			box(menu_win1, 0, 0);
			wattron(menu_win1, COLOR_PAIR(5));
			mvwprintw(menu_win1, 2, 32, "%s-%d", monthname[month - 1], year);
			x = 3;
			y = 3;
			for(i = 1; i <= monthday[month -1]; i++){
				mvwprintw(menu_win1, x, y, "%d %s",i,days[h]);
				x = x+2;
				if(x == 33){
					y = y + 20;
					x = 5;
				}
				h++;
				if(h == 7)
					h = 0;
			}
			wattroff(menu_win1, COLOR_PAIR(5));
			wattroff(menu_win1, A_BOLD);
			wrefresh(menu_win);
			wgetch(menu_win1);
			delwin(menu_win1);
		}  
		
	}
	clrtoeol();
	refresh();
	endwin();
	return 0;
}
	/***********************************printing dates **************************************/

void printdate(WINDOW *menu_win,int month ,int j){
	int x ,y, i, k = 0, z;
	x = 5;
	y = 10;
	box(menu_win, 0, 0);
	wattron(menu_win,A_BOLD);
	wattron(menu_win, COLOR_PAIR(3));
	mvwprintw(menu_win,  x, y, "%s", days[0]);
	wattroff(menu_win, COLOR_PAIR(3));
	y = y + 9;
	wattron(menu_win, COLOR_PAIR(2));
	for(i = 1; i < n_choices;  ++i){
		mvwprintw(menu_win,  x, y, "%s", days[i]);
		y = y + 9 ;
	}
	wattron(menu_win, COLOR_PAIR(8));
	mvwprintw(menu_win, 37 - 8, 1, "(*)");
	wattroff(menu_win, COLOR_PAIR(8));
	wattron(menu_win, COLOR_PAIR(7));
	mvwprintw(menu_win, 37 - 8, 5, "press E to create event");
	wattroff(menu_win, COLOR_PAIR(7));
	wattron(menu_win, COLOR_PAIR(8));
	mvwprintw(menu_win, 37 - 7, 1, "(*)");
	wattroff(menu_win, COLOR_PAIR(8));
	wattron(menu_win, COLOR_PAIR(7));
	mvwprintw(menu_win, 37 - 7, 5, "use LEFT RIGHT UP DOWN arrow");
	wattroff(menu_win, COLOR_PAIR(7));
	wattron(menu_win, COLOR_PAIR(8));
	mvwprintw(menu_win, 37 - 6, 1, "(*)");
	wattroff(menu_win, COLOR_PAIR(8));
	wattron(menu_win, COLOR_PAIR(7));
	mvwprintw(menu_win, 37 - 6, 5, "press P to save calender");
	wattroff(menu_win, COLOR_PAIR(7));
	wattron(menu_win, COLOR_PAIR(8));
	mvwprintw(menu_win, 37 - 5, 1, "(*)");
	wattroff(menu_win, COLOR_PAIR(8));
	wattron(menu_win, COLOR_PAIR(7));
	mvwprintw(menu_win, 37 - 5, 5, "press D for datewise calender");
	wattron(menu_win, COLOR_PAIR(8));
	mvwprintw(menu_win, 37 - 4, 1, "(*)");
	wattroff(menu_win, COLOR_PAIR(8));
	wattron(menu_win, COLOR_PAIR(7));
	mvwprintw(menu_win, 37 - 4, 5, "use X for events !");
	wattroff(menu_win, COLOR_PAIR(7));
	wattroff(menu_win, COLOR_PAIR(7));
	wattron(menu_win, COLOR_PAIR(8));
	mvwprintw(menu_win,37 - 3, 1, "(*)");
	wattroff(menu_win, COLOR_PAIR(8));
	wattron(menu_win, COLOR_PAIR(7));
	mvwprintw(menu_win,37 - 3, 5, "press I for new month and new year");
	wattroff(menu_win, COLOR_PAIR(7));
	wattron(menu_win, COLOR_PAIR(8));
	mvwprintw(menu_win, 37 - 2, 1, "(*)");
	wattroff(menu_win,COLOR_PAIR(8));
	wattron(menu_win, COLOR_PAIR(7));
	mvwprintw(menu_win,37 - 2, 5, "press ESC to exit");
	wattroff(menu_win, COLOR_PAIR(7));
	x = 7;
	y = 11;
	wattron(menu_win, A_BOLD);
	while( j != 1) {
		mvwprintw(menu_win, 7, y, "-");
		j-- ;
		y = y + 9;
	}
	x = 7;
	for ( i = 1; i <= monthday[month -1]; i++) {
	 	if (y == 74) {
			y = 11;
			x = x +2;
			mvwprintw(menu_win, x, y, "%2d", i);
		}
		else {	
			mvwprintw(menu_win, x, y, "%2d", i);

		}
		y = y + 9; 
	}
	wattroff(menu_win, A_BOLD);
	wrefresh(menu_win);
}
/* ============= ==================to save calender file=============== ======*/

void savefile(WINDOW *menu_win5, int M, int year, int z) {
	int i , j, k;
	char filename[12];
	char str[128];
	char buffer[12];
	char mesg[] = "Error : can't save !! ";
	char mesg1[] = "calendar saved successfully!!";
	FILE *fp;
	strcpy(filename , monthname1[M - 1]);
	snprintf(buffer, sizeof(str), "%d", year);
	strcat(filename , buffer);
	strcat(filename,".txt");
	menu_win5 = newwin(HEIGHT, WIDTH, starty, startx);
	keypad(menu_win5, TRUE);	
	wclear(menu_win5);
	wattron(menu_win5, A_BOLD);
	wrefresh(menu_win5);
	box(menu_win5, 0, 0);
	fp = fopen(filename,"w");
	if(fp == NULL){
		wclear(menu_win5);
		wprintw(menu_win5, "%s", mesg);
		exit(1);
		return;
	}
		fprintf(fp, "\n\n\t\t");
		fprintf(fp, "%s-%d\n", monthname[M - 1], year);


	/*============= checking leap year or not==========*/

		monthday[1] = LEAP_YEAR ?29:28;

	/*====================output=======================*/

	fprintf(fp, "\n\t");
	for( i = 0; i <= MAXCH ; i++){
		fprintf( fp,"%c",CH);
		fprintf(fp,  "%c",CH );
	}
	fprintf(fp, "\n");
		fprintf(fp, "\t");
		fprintf(fp, "SUN");
		fprintf(fp, "	MON	TUE	WED	THU	FRI	SAT");
		fprintf(fp, "\n\t");
	for( i = 0; i <= MAXCH ; i++){
		fprintf( fp,"%c", CH);
		fprintf(fp,  "%c", CH );
	}
	fprintf(fp, "\n");

	/*================ setting position===================*/

	j = z;
	while( z != 1){
		fprintf(fp, "\t-");
		z-- ;
	}
	 k = 0;

	/*==================printing dates===================*/
		
	for( i = 1; i <= monthday[M -1]; i++) {
		k = j % 7 + 1;
		j++;
		if(k == 2) {
							
			fprintf(fp, "\n\t");
			fprintf(fp, "%2d",i);
		}
		else {
			fprintf(fp, "	%2d",i);
		
		} 
	}
	fprintf(fp, "\n\t");
	for( i = 0; i <= MAXCH; i++) {
		fprintf(fp, "%c", CH);
		fprintf(fp,  "%c", CH);
	}
	fprintf(fp, "\n\n\t");
	for( i = 1; i <= MAXNO; i++){
		fprintf(fp,  "_");
	}
	fclose(fp);
	mvwprintw(menu_win5, 10, 10, " %s", mesg1);
	wrefresh(menu_win5);
	wgetch(menu_win5);
	delwin(menu_win5);		
	return;
}
/*****************************getting keycode*************************/

int getkey(WINDOW *menu_win){
	int key;
	keypad(stdscr, TRUE);
	key = wgetch(menu_win);
	noecho();
	if(key == 27){
		endwin();
		exit(0);
		return 27; 	
	}
	if(key == 105)
		return 105;
	if(key == 72)
		return 72;
	if(key == 261)
		return 261; 	
	if(key == 259)
		return 259; 	
	if(key == 260)
		return 260;  	
	if( key == 258)
		return 258; 
	if(key == 101)
		return 101;
	if(key == 112)
		return 112;
	if(key == 120)
		return 120;
	if(key == 100)
		return 100;
	else 
 		return 0;
	
}

int fm (int date, int month, int year) {
	int leap, fmonth;
	if(year % 4 == 0 && year % 100!=0 || year % 400 == 0)
		leap = 1;
	else
		leap = 0;
	fmonth = 3 + ( 2 - leap ) * ((month + 2) / (2 * month)) + ( 5 * month + month / 9) / 2;
	fmonth = fmonth % 7;
	return fmonth;
}

/* ===================first day of month=====================*/

int firstday(int date, int month, int year){							
	int firstday, yy, century;
	yy = year % 100;
	century = year / 100 ;
	firstday = 1.25 * yy + fm(date , month, year) + date - 2 * (century % 4);
	firstday = firstday % 7;
	return firstday;
}

