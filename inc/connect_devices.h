/*
 * connect_devices.h
 *
 *  Created on: Jun 18, 2020
 *      Author: inv
 */

#ifndef CONNECT_DEVICES_H_
#define CONNECT_DEVICES_H_

#include "motorica.h"

/*
 * Push connect devices menu to naviframe stack
 */
void
push_connect_devices(appdata_s* ad);

/*
 * Pop connect devices menu from naviframe stack
 */
void
pop_connect_devices(appdata_s* ad);

#endif /* CONNECT_DEVICES_H_ */
