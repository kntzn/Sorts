#include <iostream>
#include <random>
#include <memory>
#include <bitset>
#include <cassert>
#include <thread>
#include <atomic>

//#define MULTITHREADING
#define MULTITHREADING_DEPTH 5

#define TIMER(id) std::atomic <long> id (clock ())
#define TIME(id) clock () - id

template<typename T>
void printArr (const T *arr, const size_t size)
    {
    for (size_t i = 0; i < size; i++)
        std::cout << arr[i] << " ";
    
    std::cout << std::endl;
    }

/*
void merge_sort (int* arr, int* buf, size_t start, size_t end, int dep = 0)
    {
    
    std::string tabs = "";
    for (int i = 0; i < dep; i++)
        tabs += '\t';
    
    std::cout << tabs <<"A:" << start << " " << end << "     ";
    for (int i = start; i <= end; i++)
        std::cout << arr [i] << " ";
    std::cout << std::endl;
    
    
    if (end - start == 0)
        return;
    
    int idx_mid = (end + start) / 2;
    
    merge_sort (arr, buf, start, idx_mid, dep + 1);
    merge_sort (arr, buf, idx_mid + 1, end, dep + 1);
    
    int iter_left = start, iter_right = idx_mid + 1, iter = start;
    
    // merge
    //std::cout << tabs << iter_left << " " << idx_mid << "  " << iter_right << " " << end << std::endl;
    while (iter_left < idx_mid + 1 && iter_right <= end)
        {
        //std::cout << tabs << "cmp arrl [" << iter_left  << "]=" << arr [iter_left]  <<
        //                        " arrr [" << iter_right << "]=" << arr [iter_right] << std::endl;
        
        if (arr [iter_right] < arr [iter_left])
            {
            //std::cout << tabs << "arr [" << iter_right << "]=" << arr [iter_right] << std::endl;
            buf [iter++] = arr [iter_right++];
            }
        else
            {
            //std::cout << tabs << "arr [" << iter_left << "]=" << arr [iter_left] << std::endl;
            buf [iter++] = arr [iter_left++];
            }
        }
        
    while (iter_left < idx_mid + 1)
        {
        //std::cout << tabs <<"arrl [" << iter_left << "]=" << arr [iter_left] << std::endl;
        buf[iter++] = arr[iter_left++];
        }
    while (iter_right <= end)
        {
        //std::cout << tabs << "arrr [" << iter_right << "]=" << arr [iter_right] << std::endl;
        buf[iter++] = arr[iter_right++];
        }
   
     
    //std::cout << tabs;
   
    for (int i = start; i <= end; i++)
        {
        //std::cout << buf [i] << " ";
        
        arr [i] = buf [i];
        }
    
    std::cout << std::endl;
    std::cout << std::endl;
    }


void merge_sort_idx (int* arr, int* buf, size_t start, size_t end)
    {
    if (end - start == 0)
        return;
    
    int idx_mid = (end + start) / 2;
    
    merge_sort_idx (arr, buf, start, idx_mid);
    merge_sort_idx (arr, buf, idx_mid + 1, end);
    
    int iter = start, iter_left = start, iter_right = idx_mid + 1;
    
    // merge
    while (iter_left < idx_mid + 1 && iter_right <= end)
        {
        if (arr [iter_right] < arr [iter_left])
            buf [iter++] = arr [iter_right++];
        else
            buf [iter++] = arr [iter_left++];
        }
    
    while (iter_left < idx_mid + 1)
        buf[iter++] = arr[iter_left++];
        
    while (iter_right <= end)
        buf[iter++] = arr[iter_right++];
    
    for (int i = start; i <= end; i++)
        arr [i] = buf [i];
    }*/

/*
 * C/C++ style merge sort function for numeric arrays
 *
 * T* arr - array to sort
 * T* buf - temp. buffer, at least size / 2 length
 * size_t size - array length
 * */
template<typename T>
void merge_sort_ptr_array (T *arr, T *buf, const size_t size, size_t depth = 0)
    {
    if (size <= 1)
        {
        return;
        }
    
    size_t idx_mid = size / 2;
    size_t iter_left = 0, iter_right = idx_mid, iter = 0;
    
    #ifdef MULTITHREADING
    if (depth < MULTITHREADING_DEPTH)
        {
        std::thread t_left  (merge_sort_ptr_array<T>, arr + 0,       buf + 0,       idx_mid - 0,       depth + 1);
        std::thread t_right (merge_sort_ptr_array<T>, arr + idx_mid, buf + idx_mid, size    - idx_mid, depth + 1);
    
        t_left.join ();
        t_right.join ();
        }
    else
        {
        merge_sort_ptr_array<T> (arr + 0,       buf, idx_mid - 0,    depth + 1);
        merge_sort_ptr_array<T> (arr + idx_mid, buf, size - idx_mid, depth + 1);
        }
        
    #else
        merge_sort_ptr_array<T> (arr + 0, buf, idx_mid - 0);
        merge_sort_ptr_array<T> (arr + idx_mid, buf, size - idx_mid);
    #endif
    // Merge process
    // common part
    while (iter_left < idx_mid && iter_right < size)
        {
        
        
        if (arr[iter_right] < arr[iter_left])
            {
            buf[iter++] = arr[iter_right++];
            }
        else
            {
            buf[iter++] = arr[iter_left++];
            }
        }
    
    // tails
    while (iter_left < idx_mid)
        buf[iter++] = arr[iter_left++];
    while (iter_right < size)
        buf[iter++] = arr[iter_right++];
    
    // Copies merged subarrays to main array
    std::copy (std::make_move_iterator (buf + 0),
               std::make_move_iterator (buf + size),
               arr);
    }
/*
 * C/C++ style merge sort function for any arrays
 *
 * T* arr - array to sort
 * T* buf - temp. buffer, at least size / 2 length
 * int (*cmp) - comparator function
 * size_t size - array length
 * */
template<typename T>
void merge_sort_ptr_array_cmp (T *arr, T *buf, int (*cmp) (T, T), const size_t size)
    {
    assert (arr && buf && cmp); // Bad alloc assertion
    
    if (size <= 1)
        {
        return;
        }
    
    size_t idx_mid = size / 2;
    size_t iter_left = 0, iter_right = idx_mid, iter = 0;
    
    std::thread t_left  (merge_sort_ptr_array_cmp<T>, arr + 0,       buf, cmp, idx_mid - 0);
    std::thread t_right (merge_sort_ptr_array_cmp<T>, arr + idx_mid, buf, cmp, size    - idx_mid);
    
    // Merge process
    // common part
    while (iter_left < idx_mid && iter_right < size)
        {
        if (cmp (arr[iter_right], arr[iter_left]) <= 0)
            {
            buf[iter++] = arr[iter_right++];
            }
        else
            {
            buf[iter++] = arr[iter_left++];
            }
        }
    
    // tails
    while (iter_left < idx_mid)
        buf[iter++] = arr[iter_left++];
    while (iter_right < size)
        buf[iter++] = arr[iter_right++];
    
    // Copies merged sub-arrays to main array
    std::copy (std::make_move_iterator (buf + 0),
               std::make_move_iterator (buf + size),
               arr);
    }

template<typename T>
void shell_sort (T *arr, const size_t size)
    {
    // dk = dk-1 / 2;  d0 = N/2
    for (size_t step = size / 2; step > 0; step /= 2)
        for (size_t i = step; i < size; i++)
            for (size_t j = i - step; j >= 0 && arr[j] > arr[j + step]; j--)
                std::swap (arr[j], arr[j + step]);
    }

void radixSortBase2 (int *arr, const size_t size, int dig = sizeof (int) * 8 - 1)
    {
    if (size <= 1 || dig < 0)
        {
        return;
        }
    
    size_t counter_l = 0, counter_r = size - 1;
    
    while (counter_l != counter_r)
        {
        while (!((arr[counter_l] >> dig) % 2) && counter_l != counter_r)
            counter_l++;
        while (((arr[counter_r] >> dig) % 2) && counter_l != counter_r)
            counter_r--;
        
        std::swap (arr[counter_l], arr[counter_r]);
        }
    
    int high_bit_start = 0;
    while ((arr[high_bit_start] >> dig) % 2 != 1 && high_bit_start < size)
        high_bit_start++;
    
    if (high_bit_start == 0 || high_bit_start == size)
        {
        radixSortBase2 (arr, size, dig - 1);
        }
    else
        {
        radixSortBase2 (arr + 0, high_bit_start - 0, dig - 1);
        radixSortBase2 (arr + high_bit_start, size - high_bit_start, dig - 1);
        }
    }
// template T RadixSort () // using key value for each object



/*
 * C/C++ style quick sort for numeric arrays
 *
 * T* arr - array to sort
 * size_t size - array length
 * */
template <typename T>
void qsort (T* arr, size_t size, size_t left = 0, size_t right = -1)
    {
    if (right == -1)
        right = size;
    
    size_t CurL = left;
    size_t CurR = right;
    T CurMid = arr [(left + right) / 2];
    
    while (CurL <= CurR)
        {
        while (arr [CurL] < CurMid) CurL++;
        while (arr [CurR] > CurMid) CurR--;
        
        if (CurL <= CurR)
            {
            if (arr [CurL] > arr [CurR])
                {
                T temp_left = arr [CurL];
                arr [CurL]  = arr [CurR];
                arr [CurR]  = temp_left;
                }
            
            CurL++;
            CurR--;
            }
        
        if (CurL > CurR) break;
        }
    
    if (CurL < right)
        qsort (arr, size, CurL, right);
    if (left < CurR)
        qsort (arr, size, left, CurR);
    
    }
    
    
/*
 * Linear search algorithm
 *
 * T* arr      - prt to array
 * size_t size - size of the array
 * T key       - key value to search
 *
 * returns element index if key value
 * was found or -1 if arr does not
 * contain such key
 *
 * requires array to be sorted
 */
template <typename T>
long int lsearch (T* arr, size_t size, T key)
    {
    for (size_t i = 0; i < size; i++)
        if (arr [i] == key)
            return i;
    
    return -1;
    }
/*
 * Binary search algorithm
 *
 * T* arr      - prt to array
 * size_t size - size of the array
 * T key       - key value to search
 *
 * returns element index if key value
 * was found or -1 if arr does not
 * contain such key
 *
 * requires array to be sorted
 */
template <typename T>
long int bsearch (T* arr,  T key, size_t size, size_t start = 0)
    {
    size_t idx_mid = (size + start) / 2;
    
    if (size - start > 1)
        {
        if (arr[idx_mid] == key)
            {
        
            return idx_mid;
            }
        else if (arr[idx_mid] < key)
            {
            return bsearch (arr, key, size, idx_mid);
            }
        else
            {
            return bsearch (arr, key, idx_mid, 0);
            }
        }
    else if (arr [idx_mid] == key)
        return idx_mid;
    else
        return -1;
        
    }

/*
* Interpole search algorithm
*
* T* arr      - prt to array
* size_t size - size of the array
* T key       - key value to search
*
* returns element index if key value
* was found or -1 if arr does not
* contain such key
*/
template <typename T>
long int isearch (T* arr,  T key, size_t size, size_t start = 0)
    {
    size_t idx_approximation =
            start + static_cast <size_t>
                (static_cast <double> ((key - arr [start]) * (size-1 - start)) /
                 static_cast <double>  (arr [size-1] - arr [start]));

    if (arr [idx_approximation] == key)
        return idx_approximation;
    else if (arr [idx_approximation] < key)
        return isearch (arr, key, size, idx_approximation + 1);
    else
        return isearch (arr, key, idx_approximation - 1, 0);
    
    }
    
/*
 * C/C++ style numeric array orderliness check
 *
 * T* arr - array to sort
 * size_t size - array length
 * */
template<typename T>
size_t sorted (const T *arr, const size_t size)
    {
    for (size_t i = 0; i < size - 1; i++)
        if (arr[i] > arr[i + 1])
            {
            return i;
            }
    
    return -1;
    }

/*
 * C/C++ style numeric array orderliness check
 *
 * T* arr      - array to sort
 * size_t size - array length
 * int (*cmp)  - comparator function
 * */
template<typename T>
size_t sorted_cmp (const T *arr, const size_t size, int (*cmp) (T, T))
    {
    for (size_t i = 0; i < size - 1; i++)
        if (cmp (arr[i], arr[i + 1]) > 0)
            return i;
            
    
    return -1;
    }
    
// CONFIG
#define SIZE 100000000
//#define PRNT_ARR

typedef int benchType;

int cmp (const int left, const int right)
    { return left - right; }

int main ()
    {
    auto buf = (benchType *) calloc (SIZE, sizeof (benchType));
    auto arr = new benchType[SIZE];
    
    for (int i = 0; i < SIZE; i++)
        arr[i] = random () % 10;
    
    #ifdef PRNT_ARR
    printArr (arr, SIZE);
    #endif
    
    //TIMER (timer_0);
    std::atomic <time_t> timer_0 (clock ());
    
    merge_sort_ptr_array (arr, buf, SIZE);
    
    std::cout << "Time: " << clock () - timer_0 << " us" << std::endl;
    
    //std::cout << isearch (arr, 600, SIZE) << std::endl;
    
    #ifdef PRNT_ARR
    printArr (arr, SIZE);
    #endif
    
    
    
    // Is sorted
    int isSorted = sorted_cmp (arr, SIZE, cmp);
    std::cout << std::endl << isSorted << std::endl;
    if (isSorted != -1)
        {
        for (int i = isSorted - 10; i < isSorted + 10; i++)
            std::cout << arr[i] << std::endl;
        }
    
    return 0;
    }