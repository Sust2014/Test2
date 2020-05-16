/*
 * Auxiliary.h
 *
 *  Created on: 2020��1��3��
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
/*�жϸ�����жѹ���Ƿ��ܷ�����*/
bool UnloadValve ;
uint16_t UnloadCount ;

#endif /* AUXILIARY_H_ */
