/*
 * Internal temperature sensor
 *
 *  Created on: 14 mars 2016
 *      Author: faitaoudia
 */

#ifndef INTERN_TEMP_SENSOR_INTERN_TEMP_SENSOR_H_
#define INTERN_TEMP_SENSOR_INTERN_TEMP_SENSOR_H_

#include <stdint.h>


/*
 * Initialize the internal temperature sensor
 */
void temp_sensor_init(void);

/*
 * Get a reading of the internal temperature
 */
int temp_sensor_get_reading(void);


#endif /* INTERN_TEMP_SENSOR_INTERN_TEMP_SENSOR_H_ */
