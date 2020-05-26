/*
 * bt.h
 *
 *  Created on: May 26, 2020
 *      Author: inv
 */

#ifndef BT_H_
#define BT_H_

#include <bluetooth.h>
#include <app_control.h>
#include <dlog.h>

#include "motorica.h"

void bt_init(void);

Eina_Bool bt_is_on(void);

int bt_onoff_operation(void);


#endif /* BT_H_ */
