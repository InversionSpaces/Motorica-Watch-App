/*
 * bt.h
 *
 *  Created on: May 26, 2020
 *      Author: inv
 */

#ifndef BT_H_
#define BT_H_

#include "motorica.h"

/*
 * Initialize bluetooth
 */
void bt_init(appdata_s *ad);

/*
 * Deinitialize bluetooth
 */
void bt_deinit(void);

/*
 * Check if adapter is enabled
 */
bool bt_is_on(void);

/*
 * Pop window to ask to enable bluetooth (not working)
 */
int bt_onoff_operation(void);

/*
 * Start devices discovery
 */
int bt_discover(void);


#endif /* BT_H_ */
