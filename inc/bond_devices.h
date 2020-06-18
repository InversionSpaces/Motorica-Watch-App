/*
 * devices.h
 *
 *  Created on: May 26, 2020
 *      Author: inv
 */

#ifndef BOND_DEVICES_H_
#define BOND_DEVICES_H_

#include "motorica.h"

/*
 * Push bond devices menu to naviframe stack
 */
void
push_bond_devices(appdata_s* ad);

/*
 * Pop bond devices menu from naviframe stack
 */
void
pop_bond_devices(appdata_s* ad);

#endif /* BOND_DEVICES_H_ */
