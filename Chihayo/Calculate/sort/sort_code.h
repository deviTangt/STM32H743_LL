#include "__HARDWARE_CONFIG__.h"
#if __HARDWARE_CONFIG__SORT_CODE_ENABLE__	// begin of __HARDWARE_CONFIG__SORT_CODE_ENABLE__
#ifndef __SORT_CODE_H__	// begin of __SORT_CODE_H__
#define __SORT_CODE_H__
//*******************************// include extern .h files //************************************//
#include "main.h"
#include<stdio.h>
#include <stdlib.h>
#include <string.h>
//*******************************// define statement        //************************************//
//*******************************// extern parameters       //************************************//
//*******************************// define structure unity  //************************************//
//*******************************// define parameters       //************************************//
//*******************************// extern function         //************************************//

//
void show_array(uint32_t *a,uint32_t array_size);

void swap(uint32_t *a,uint32_t i,uint32_t j);

void reset_array(uint32_t *a,uint32_t array_size,uint32_t range,uint32_t offset);

void bubble_sort(uint32_t *a,uint32_t array_size);

void cocktail_sort(uint32_t *a,uint32_t array_size);

void select_sort(uint32_t *a,uint32_t array_size);

void insert_sort(uint32_t *a,uint32_t array_size);

void gnome_sort(uint32_t *a,uint32_t array_size);

void merge_subArray(uint32_t *a,uint32_t left,uint32_t middle1,uint32_t middle2,uint32_t right);

void merge_sort_subArray(uint32_t *a,uint32_t low,uint32_t high);

void merge_sort(uint32_t *a,uint32_t array_size);

void bucket_sort(uint32_t *a,uint32_t array_size);

void insert_sort_for_quickupgrade(uint32_t *a,uint32_t low,uint32_t high);

void mid_swap(uint32_t *a,uint32_t low,uint32_t high);

void quick_sort_upgrade(uint32_t *a,uint32_t low,uint32_t high);

void quick_sort_subArray(uint32_t *a,uint32_t low,uint32_t high);
//

//*******************************// end_h                   //************************************//
#endif	// end of __SORT_CODE_H__
#endif	// end of __HARDWARE_CONFIG__SORT_CODE_ENABLE__
