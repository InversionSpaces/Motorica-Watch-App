/*
 * bt.h
 *
 *  Created on: May 26, 2020
 *      Author: inv
 */

#ifndef BT_H_
#define BT_H_

#include "motorica.h"

void bt_init(appdata_s *ad);

void bt_deinit(void);

bool bt_is_on(void);

int bt_onoff_operation(void);

int bt_discover(void);


#endif /* BT_H_ */
