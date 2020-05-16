/*
 * Auxiliary.h
 *
 *  Created on: 2020年1月3日
 *      Author: Cenx
 */

#ifndef AUXILIARY_H_
#define AUXILIARY_H_

#include "derivative.h"
#include "utility_function.h"

typedef struct{
	uint16_t Ulsec;
	uint8_t Ulmin;
	uint8_t Ulhour;
}stUl;
stUl Unload;
/*判断干燥器卸压阀是否密封正常*/
bool UnloadValve ;
uint16_t UnloadCount ;

#endif /* AUXILIARY_H_ */
