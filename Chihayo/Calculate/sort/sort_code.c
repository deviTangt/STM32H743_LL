#include "__HARDWARE_CONFIG__.h"
#if __HARDWARE_CONFIG__SORT_CODE_ENABLE__	// begin of __HARDWARE_CONFIG__SORT_CODE_ENABLE__
//*******************************// include _h files    //************************************//
#include "sort_code.h"
//*******************************// define parameters   //************************************//

#define ALEN 600

//*******************************// parameters          //************************************//
//*******************************// define function     //************************************//

//
void show_array(uint32_t *a,uint32_t array_size){
    for (uint32_t i = 0;i < array_size;i += 1) {
        //printf("%8d",a[i]);
        //if (i % 20 == 0 && i != 0) printf("\n\t");
    }
    //puts("");
}
//
void swap(uint32_t *a,uint32_t i,uint32_t j){
    uint32_t temp = a[j];
    a[j] = a[i];
    a[i] = temp;
}
//
void reset_array(uint32_t *a,uint32_t array_size,uint32_t range,uint32_t offset){
    for (uint32_t i = 0;i < array_size;i += 1) a[i] = rand()%range + 1 + offset;
    //puts("Random reset the array:");
    show_array(a,array_size);
}
//
void bubble_sort(uint32_t *a,uint32_t array_size){
    //puts("Sort the array: bubble sort");
    for (uint32_t i = 0;i < array_size;i += 1){
        for (uint32_t j = 0;j < array_size - 1 - i;j += 1) if (a[j] > a[j + 1]) swap(a,j,j + 1);
        show_array(a,array_size - i);
    }
    show_array(a,array_size);
    //puts("");
}
//
void cocktail_sort(uint32_t *a,uint32_t array_size){
    //puts("Sort the array: cocktail sort");
    for (uint32_t i = 0,j = array_size - 1;i < j;i += 1,j -= 1){
        for (uint32_t p = i;p < j;p += 1) if (a[p] > a[p + 1]) swap(a,p,p + 1);
        for (uint32_t q = j;q > i;q -= 1) if (a[q] < a[q - 1]) swap(a,q,q - 1);
        for (uint32_t m = 0;m < i;m += 1) printf("%8s","");
        show_array(&a[i],j - i + 1);
    }
    show_array(a,array_size);
    //puts("");
}
//
void select_sort(uint32_t *a,uint32_t array_size){
    //puts("Sort the array: select sort");
    for (uint32_t i = 0,j = array_size - 1;i < j;i += 1,j -= 1){
        uint32_t min_seq = i,max_seq = j;
        for (uint32_t p = i;p <= j;p += 1){
            if (a[p] < a[min_seq]) min_seq = p;
            if (a[p] > a[max_seq]) max_seq = p;
        }
        if (min_seq != i) {if (!(min_seq == j && max_seq == i)) swap(a,i,min_seq);}
        if (max_seq != j) swap(a,j,max_seq); 
        //for (uint32_t h = 0;h < i;h += 1) printf("%8s","");
        show_array(&a[i],array_size - 2*i);
    }
    show_array(a,array_size);
}
//
void select_sort_recursion(uint32_t *a,uint32_t array_size){
    uint32_t min_seq = 0,max_seq = array_size - 1;
    for (uint32_t p = 0;p <= array_size - 1;p += 1){
        if (a[p] < a[min_seq]) min_seq = p;
        if (a[p] > a[max_seq]) max_seq = p;
    }
    if (min_seq != 0) {if (!(min_seq == array_size - 1 && max_seq == 0)) swap(a,0,min_seq);} 
    if (max_seq != array_size - 1) swap(a,array_size - 1,max_seq); 
    //for (uint32_t i = 0;i < (ALEN - array_size) / 2;i += 1) printf("%8s","");
    show_array(a,array_size);
    if (1 <= array_size - 2) select_sort_recursion(&a[1],array_size - 2);
}
//
void insert_sort(uint32_t *a,uint32_t array_size){
    //puts("Sort the array: insert sort");
    for (uint32_t i = 1;i < array_size;i += 1){
        uint32_t temp = a[i];
        uint32_t cur_index = i;
        while (cur_index > 0 && a[cur_index - 1] > temp) a[cur_index] = a[cur_index - 1],cur_index -= 1;
        if (cur_index != i) a[cur_index] = temp;
        show_array(a,i + 1);
    }
    puts("");
}
//
void gnome_sort(uint32_t *a,uint32_t array_size){
    //puts("Sort the array: gnome sort");
    uint32_t cur_index = 0;
    while (cur_index < array_size){
        if (a[cur_index] <= a[cur_index + 1]) cur_index += 1;
        else swap(a,cur_index,cur_index + 1),cur_index -= 1;
    }
    show_array(a,array_size);
    //puts("");
}
//
void merge_subArray(uint32_t *a,uint32_t left,uint32_t middle1,uint32_t middle2,uint32_t right){
    uint32_t temp_array[ALEN];
    uint32_t left_index = left , combined_index = left , right_index = middle2;
    while (left_index <= middle1 && right_index <= right){
        if (a[left_index] <= a[right_index]) temp_array[combined_index ++] = a[left_index ++];
        else temp_array[combined_index ++] = a[right_index ++];
    }
    while (left_index <= middle1) temp_array[combined_index ++] = a[left_index ++];
    while (right_index <= right) temp_array[combined_index ++] = a[right_index ++];
    //for (uint32_t i = left;i <= right;i += 1) a[i] = temp_array[i];
    memcpy(&a[left],&temp_array[left],sizeof(a[0])*(right - left + 1));
}
//
void merge_sort_subArray(uint32_t *a,uint32_t low,uint32_t high){
    if (high > low) {
        uint32_t middle1 = (high + low) / 2 , middle2 = middle1 + 1;
        merge_sort_subArray(a,low,middle1);
        merge_sort_subArray(a,middle2,high);
        merge_subArray(a,low,middle1,middle2,high);
        for (uint32_t i = 0;i < low;i += 1) printf("%8s","");
        show_array(&a[low],high - low + 1);
    }
}
//
void merge_sort(uint32_t *a,uint32_t array_size){
    //puts("Sort the array: merge sort");
    merge_sort_subArray(a,0,array_size - 1);
    //puts("");
}
//
static uint32_t buckets[10][ALEN] __attribute__ ((section ("DMA_ADC1_RX_BUF"),  zero_init));
static uint32_t status;
static uint32_t i, j, n, m, p;
void bucket_sort(uint32_t *a,uint32_t array_size){  //LSD base sort
    //puts("Sort the array: bucket sort");
    for (p = 1;;p *= 10){
        for (i = 0;i < 10;i ++)
            for (j = 0;j < array_size;j ++){
                buckets[i][j] = 0;
            }
        status = 0;
        for (i = 0;i < array_size;i += 1) {
            if (a[i] / p) status = 1;
            buckets[a[i] / p % 10][i] = a[i];
        }
        if (status == 0) break;
        n = 0;
        for (j = 0;j < 10;j += 1){
            for (m = 0;m < array_size;m += 1){
                if (buckets[j][m] != 0) a[n ++] = buckets[j][m];
                if (n == ALEN) break;
            }
            if (n == ALEN) break;
        }
        //show_array(a,array_size);
    }
    //puts("");
}
//
void quick_sort_subArray(uint32_t *a,uint32_t low,uint32_t high){
    if (low < high){   //If an array has only one element,there's no need to sort it.
        uint32_t cur_index = low;
        while(1){
            uint32_t status = 0;
            for (uint32_t i = high;i > cur_index;i -= 1) if (a[i] < a[cur_index]) {swap(a,cur_index,i); cur_index = i; status = 1;}
            for (uint32_t j = low;j < cur_index;j += 1) if (a[j] > a[cur_index]) {swap(a,cur_index,j); cur_index = j; status = 1;}
            if (status == 0) break;  //If all the elements left to current element are all less than or equal to it and right greater
                                     //than or eauql to,then stop loop.
        }
        if (cur_index != low) quick_sort_subArray(a,low,cur_index - 1);
        if (cur_index != high) quick_sort_subArray(a,cur_index + 1,high);
    }
    for (uint32_t i = 0;i < low;i += 1) printf("%8s","");
    show_array(&a[low],high - low + 1);
}
//
void quick_sort(uint32_t *a,uint32_t array_size){
    //puts("Sort the array: quick sort");
    quick_sort_subArray(a,0,array_size - 1);
    puts("");
}
//
//插入排序改进算法
void insert_sort_for_quickupgrade(uint32_t *a,uint32_t low,uint32_t high){
    uint32_t index = low;
    while (index < high){
        if (a[index] <= a[index + 1]) index += 1;
        else swap(a,index,index + 1),index -= 1;
    }
}
//
void mid_swap(uint32_t *a,uint32_t low,uint32_t high){
    uint32_t mid = (low + high)/2;
    
    if (a[low] > a[high]) swap(a,low,high);
    if (a[mid] > a[high]) swap(a,mid,high);
    if (a[low] > a[mid]) swap(a,low,mid);
}
//
void quick_sort_upgrade(uint32_t *a,uint32_t low,uint32_t high){
    if (low >= high) return;
    if (high - low <= 10){
        insert_sort_for_quickupgrade(a,low,high);
        return;
    }

    mid_swap(a,low,high);
    uint32_t i = low,j = high;
    uint32_t left = low,right = high;
    uint32_t left_len = 0,right_len = 0;
    uint32_t key = a[low];
    while(i < j){
        while (i < j && key <= a[j]) {
            if (a[j] == key) swap(a,j,right),right -= 1,right_len += 1;
            j -= 1;
        }
        a[i] = a[j];
        while (i < j && key >= a[i]) {
            if (a[i] == key) swap(a,i,left),left += 1,left_len += 1;
            i += 1;
        }
        a[j] = a[i];
    }
    a[i] = key;

    uint32_t p = low,q = i - 1;
    while (p < q && a[q] != key){
        swap(a,p,q);
        p += 1,q -= 1;
    }
    p = i + 1,q = high;
    while (p < q && a[p] != key){
        swap(a,p,q);
        p += 1,q -= 1;
    }

    quick_sort_upgrade(a,low,i - 1 - left_len);
    quick_sort_upgrade(a,i + 1 + right_len,high);
}
//

//*******************************// end_c               //************************************//
#endif	// end of __HARDWARE_CONFIG__SORT_CODE_ENABLE__


