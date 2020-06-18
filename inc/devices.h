/*
 * devices.h
 *
 *  Created on: Jun 18, 2020
 *      Author: inv
 */

#ifndef DEVICES_H_
#define DEVICES_H_

#include "motorica.h"

/*
 * Push devices menu to naviframe stack
 */
void
push_devices(appdata_s* ad);

/*
 * Pop devices menu from naviframe stack
 */
void
pop_devices(appdata_s* ad);

#endif /* DEVICES_H_ */
