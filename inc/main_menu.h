/*
 * main_menu.h
 *
 *  Created on: May 22, 2020
 *      Author: inv
 */

#ifndef MAIN_MENU_H_
#define MAIN_MENU_H_

#include "motorica.h"

/*
 * Push main menu to naviframe stack
 */
void
push_menu(appdata_s* ad);

/*
 * Pop main menu from naviframe stack
 */
void
pop_menu(appdata_s* ad);

#endif /* MAIN_MENU_H_ */
