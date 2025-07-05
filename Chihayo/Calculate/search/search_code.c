#include "__HARDWARE_CONFIG__.h"
#if __HARDWARE_CONFIG__SEARCH_CODE_ENABLE__	// begin of __HARDWARE_CONFIG__SEARCH_CODE_ENABLE__
//*******************************// include _h files    //************************************//
#include "search_code.h"
//*******************************// define parameters   //************************************//
//*******************************// parameters          //************************************//
//*******************************// define function     //************************************//

uint32_t binary_search(uint32_t *a,uint32_t array_size,uint32_t key){
    uint32_t middle,low = 0,high = array_size - 1;
    while(1){
        middle = (low + high) / 2;
        if (a[middle] == key) return middle;
        else if (a[middle] > key) high = middle - 1;
        else low = middle + 1;
        if (low > high) return -1;
    }
}
//
uint32_t linear_search(uint32_t *a,uint32_t array_size,uint32_t key){
    if (a[array_size - 1] == key) return array_size - 1;
    if (array_size == 1) return -1;
    linear_search(a,array_size - 1,key);
}

//*******************************// end_c               //************************************//
#endif	// end of __HARDWARE_CONFIG__SEARCH_CODE_ENABLE__

