/*
 * bt.h
 *
 *  Created on: May 26, 2020
 *      Author: inv
 */

#ifndef BT_H_
#define BT_H_

#include <bluetooth.h>

#include "motorica.h"

void bt_init(void);

void bt_deinit(void);

bool bt_is_on(void);

int bt_onoff_operation(void);

int bt_discover(bt_adapter_device_discovery_state_changed_cb discovery_cb, void *cb_data);


#endif /* BT_H_ */
