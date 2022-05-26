#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <limits.h>
#include <time.h>
<<<<<<< HEAD
#include "heap.h"
=======
//#include "heap.h"

/// HEAP DECLARATION
// Structure for a single heap entry
typedef struct heap_entry {
    void* key;   // Key for this entry
    void* value; // Value for this entry
} heap_entry;


// Main struct for representing the heap
typedef struct heap {
    int (*compare_func)(void*, void*); // The key comparison function to use
    int active_entries;  // The number of entries in the heap
    int minimum_pages;   // The minimum number of pages to maintain, based on the initial cap.
    int allocated_pages; // The number of pages in memory that are allocated
    heap_entry* table; // Pointer to the table, which maps to the pages
} heap;

// Functions

/**
 * Creates a new heap
 * @param h Pointer to a heap structure that is initialized
 * @param initial_size What should the initial size of the heap be. If <= 0, then it will be set to the minimum
 * permissable size, of 1 page (512 entries on 32bit system with 4K pages).
 * @param comp_func A pointer to a function that can be used to compare the keys. If NULL, it will be set
 * to a function which treats keys as signed ints. This function must take two keys, given as pointers and return an int.
 * It should return -1 if key 1 is smaller, 0 if they are equal, and 1 if key 2 is smaller.
 */
void heap_create(heap* h, int initial_size, int (*comp_func)(void*,void*));

/**
 * Returns the size of the heap
 * @param h Pointer to a heap structure
 * @return The number of entries in the heap.
 */
int heap_size(heap* h);

/**
 * Inserts a new element into a heap.
 * @param h The heap to insert into
 * @param key The key of the new entry
 * @param value The value of the new entry
 */
void heap_insert(heap* h, void* key, void* value);

/**
 * Returns the element with the smallest key in the heap.
 * @param h Pointer to the heap structure
 * @param key A pointer to a pointer, to set to the minimum key
 * @param value Set to the value corresponding with the key
 * @return 1 if the minimum element exists and is set, 0 if there are no elements.
 */
int heap_min(heap* h, void** key, void** value);

/**
 * Deletes the element with the smallest key from the heap.
 * @param h Pointer to the heap structure
 * @param key A pointer to a pointer, to set to the minimum key
 * @param valu Set to the value corresponding with the key
 * @return 1if the minimum element exists and is deleted, 0 if there are no elements.
 */
int heap_delmin(heap* h, void** key, void** value);

/**
 * Calls a function for each entry in the heap.
 * @param h The heap to iterate over
 * @param func The function to call on each entry. Should take a void* key and value.
 */
void heap_foreach(heap* h, void (*func)(void*,void*));

/**
 * Destroys and cleans up a heap.
 * @param h The heap to destroy.
 */
void heap_destroy(heap* h);



///-------------

>>>>>>> f21d90c (make revendor)

int comp_func(register void*, register void*);
void delete_file(void*, void*);
void free_key_value(time_t*, char*);
void allocate_key_value(time_t**, time_t, char**, char*);
<<<<<<< HEAD

=======
int curate(char*, int);
>>>>>>> f21d90c (make revendor)
struct file {
    char *name;
    time_t modtime;
};



const char *file_path_delimiter = "/";
// const time_t MAX_MODTIME = ULONG_MAX;

int main (int argc, char *argv[]) {
<<<<<<< HEAD
    char *destination;
    char *file_full_path;
    char dir_path[512];
    int page_size;
=======
    char dir_path[512];
    int page_size;
    int interval_in_seconds;
    int result;
    time_t t;
    // Ensure correct argument count.
    if (argc != 4) {
        printf ("Usage: testprog <dirname> <pagesize> <interval in seconds>\n");
        return 1;
    }

    strcpy(dir_path, argv[1]);
    page_size = strtol(argv[2], NULL, 10);
    interval_in_seconds = strtol(argv[3], NULL, 10);

    while (1) {
       time(&t);
       printf("\nStarts to curate at: %s", ctime(&t));
       result = curate(dir_path, page_size);
       if (result) {
           return result;
       }
       time(&t);
       printf("Ends to curate at: %s", ctime(&t));
       sleep(interval_in_seconds);
    }

    return 0;
}

int curate(char *dir_path, int page_size) {
    char *destination;
    char *file_full_path;
>>>>>>> f21d90c (make revendor)
    //Variables to represent the youngest element in the heap
    time_t *youngest_modtime_ptr;
    time_t youngest_modtime;
    char *file_name;
    time_t *modtime;
    char *youngest_name;
    struct dirent *pDirent;
<<<<<<< HEAD
    struct stat fileStat;
    DIR *pDir;
    heap h;

    // Ensure correct argument count.
    if (argc != 3) {
        printf ("Usage: testprog <dirname> <pagesize>\n");
        return 1;
    }

    strcpy(dir_path, argv[1]);
    page_size = strtol(argv[2], NULL, 10);

    // Ensure we can open directory.
    pDir = opendir(dir_path);
    if (pDir == NULL) {
        printf ("Cannot open directory '%s'\n", argv[1]);
=======
    
    DIR *pDir;
    heap h;
    // Ensure we can open directory.
    pDir = opendir(dir_path);
    if (pDir == NULL) {
        printf ("Cannot open directory '%s'\n", dir_path);
>>>>>>> f21d90c (make revendor)
        return 1;
    }

    //Use dir_path to construct each file
<<<<<<< HEAD
    strcat(dir_path, file_path_delimiter);
    destination = dir_path + strlen(dir_path);
    file_full_path = dir_path;
=======
    file_full_path = (char*) malloc(1024*sizeof(char));
    strcpy(file_full_path, dir_path);
    strcat(file_full_path, file_path_delimiter);
    destination = file_full_path + strlen(file_full_path);
>>>>>>> f21d90c (make revendor)

    // Create heap
    heap_create(&h,page_size, comp_func);

    // Process each dir entry.
    while ((pDirent = readdir(pDir)) != NULL) {
<<<<<<< HEAD
=======
        struct stat fileStat;
        if (!strcmp(pDirent->d_name, "..") || !strcmp(pDirent->d_name, ".")){
            continue;
        }
        //puts("\n------------------------Start investigation of a file-----------------------------\n");
        //fileStat.st_mtime = 0;
        strcpy(destination, pDirent->d_name);
        if (lstat(file_full_path, &fileStat) < 0) {
            return 1;
        }
        //printf("investigating: %s with modtime G\n", pDirent->d_name, fileStat.st_mtime);
>>>>>>> f21d90c (make revendor)
        // The heap is full
        if (heap_min(&h, (void**)&youngest_modtime_ptr, (void**)&youngest_name) && (heap_size(&h) >= page_size)) {
            // Avoid unnecessary operation
            if (youngest_modtime <=  fileStat.st_mtime) {
<<<<<<< HEAD
=======
               // printf("file: %s with modtime %lu is youngest than %lu and will be skipped\n", pDirent->d_name, fileStat.st_mtime, youngest_modtime);
>>>>>>> f21d90c (make revendor)
                continue;
            }
            // The youngest element must be removed to store the older one
            heap_delmin(&h,  (void**)&youngest_modtime_ptr, (void**)&youngest_name);
<<<<<<< HEAD
            free_key_value(youngest_modtime_ptr, youngest_name);
        }
        
        strcpy(destination, pDirent->d_name);
        if (lstat(file_full_path, &fileStat) < 0) {
            return 1;
        }

        allocate_key_value(&modtime, fileStat.st_mtime, &file_name, file_full_path);
        
        heap_insert(&h, modtime, file_name);

        heap_min(&h, (void**)&youngest_modtime_ptr, (void**)&youngest_name);
        youngest_modtime = *youngest_modtime_ptr;
=======
            //puts("FILE DELETION\n");
            //printf("file: %s with modtime %lu will be removed because file %s with modtime %lu is older\n", youngest_name, *youngest_modtime_ptr, pDirent->d_name, fileStat.st_mtime);
            free_key_value(youngest_modtime_ptr, youngest_name);
        }

        
        allocate_key_value(&modtime, fileStat.st_mtime, &file_name, file_full_path);
        //puts("FILE INSERTION\n");
        //printf("full path: %s\n", file_name);
        //printf("relative path: %s\n", destination);
        //printf("modification time: %lu\n", *modtime);

        heap_insert(&h, modtime, file_name);

        heap_min(&h, (void**)&youngest_modtime_ptr, (void**)&youngest_name);
        //printf("New youngest file: %s with modtime %lu\n", youngest_name, *youngest_modtime_ptr);
        youngest_modtime = *youngest_modtime_ptr;
        //puts("\n------------------------End investigation of a file-----------------------------\n");
        fileStat.st_mtime = 0;
>>>>>>> f21d90c (make revendor)
    }

    // Close directory
    closedir (pDir);
<<<<<<< HEAD

    heap_foreach(&h, delete_file);
    return 0;
}

=======
    free(file_full_path);
    heap_foreach(&h, delete_file);
    printf("%d files deleted\n", heap_size(&h));
    return 0;
}


>>>>>>> f21d90c (make revendor)
inline int comp_func(register void* key1, register void* key2) {
    register time_t file1_mod_time = *((time_t*)key1);
    register time_t file2_mod_time = *((time_t*)key2);

    if (file1_mod_time > file2_mod_time) {
        return -1;
    }else if (file1_mod_time == file2_mod_time) {
        return 0;
    }else {
        return 1;
    }      
}

inline void delete_file(void *key, void *value) {
<<<<<<< HEAD
    printf("Real min: %s\n", (char*)value);
=======
    //printf("DELETE FILE: %s with modtime %lu\n", (char*)value, *((time_t*)key));
>>>>>>> f21d90c (make revendor)
}

inline void free_key_value(time_t *modtime, char * name) {
    free(modtime);
    free(name);
}

inline void allocate_key_value(time_t **modtime, time_t mtime, char **name, char *file_full_path) {
<<<<<<< HEAD
        *name = (char*) malloc(strlen(file_full_path)*sizeof(char));
=======
        *name = (char*) malloc((strlen(file_full_path)+1)*sizeof(char));
>>>>>>> f21d90c (make revendor)
        if (*name == NULL) {
            exit(2);
        }
        strcpy(*name, file_full_path);

        *modtime = (time_t*) malloc(sizeof(time_t));
        if (modtime == NULL) {
            exit(2);
        }
        **modtime = mtime;
}
<<<<<<< HEAD
=======




/**
 * This file defines the methods declared in heap.h
 * These are used to create and manipulate a heap
 * data structure.
 */

#include <unistd.h>
#include <sys/mman.h>
#include <assert.h>
#include <strings.h>
#include "heap.h"

// Helpful Macros
#define LEFT_CHILD(i)   ((i<<1)+1)
#define RIGHT_CHILD(i)  ((i<<1)+2)
#define PARENT_ENTRY(i) ((i-1)>>1)
#define SWAP_ENTRIES(parent,child)  { \
                                      void* temp = parent->key; \
                                      parent->key = child->key;          \
                                      child->key = temp;                 \
                                      temp = parent->value;              \
                                      parent->value = child->value;      \
                                      child->value = temp;               \
                                    }

#define GET_ENTRY(index,table) ((heap_entry*)(table+index))


////HEAP definition

/**
 * Stores the number of heap_entry structures
 * we can fit into a single page of memory.
 *
 * This is determined by the page size, so we
 * need to determine this at run time.
 */
static int ENTRIES_PER_PAGE = 0;

/**
 * Stores the number of bytes in a single
 * page of memory.
 */
static int PAGE_SIZE = 0;

// Helper function to map a number of pages into memory
// Returns NULL on error, otherwise returns a pointer to the
// first page.
static void* map_in_pages(int page_count) {
    // Check everything
    assert(page_count > 0);

    // Call mmmap to get the pages
    void* addr = mmap(NULL, page_count*PAGE_SIZE, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);

    if (addr == MAP_FAILED)
        return NULL;
    else {
        // Clear the memory
        bzero(addr,page_count*PAGE_SIZE);
        
        // Return the address 
        return addr;
    }
}


// Helper function to map a number of pages out of memory
static void map_out_pages(void* addr, int page_count) {
    // Check everything
    assert(addr != NULL);
    assert(page_count > 0);

    // Call munmap to get rid of the pages
    int result = munmap(addr, page_count*PAGE_SIZE);

    // The result should be 0
    assert(result == 0);
}


// This is a comparison function that treats keys as signed ints
int compare_int_keys(register void* key1, register void* key2) {
    // Cast them as int* and read them in
    register int key1_v = *((int*)key1);
    register int key2_v = *((int*)key2);

    // Perform the comparison
    if (key1_v < key2_v)
        return -1;
    else if (key1_v == key2_v)
        return 0;
    else
        return 1;
}


// Creates a new heap
void heap_create(heap* h, int initial_size, int (*comp_func)(void*,void*)) {
    // Check if we need to setup our globals
    if (PAGE_SIZE == 0) {
        // Get the page size
        PAGE_SIZE = getpagesize();

        // Calculate the max entries
        ENTRIES_PER_PAGE = PAGE_SIZE / sizeof(heap_entry);
    }
    
    // Check that initial size is greater than 0, else set it to ENTRIES_PER_PAGE
    if (initial_size <= 0)
        initial_size = ENTRIES_PER_PAGE;

    // If the comp_func is null, treat the keys as signed ints
    if (comp_func == NULL)
        comp_func = compare_int_keys;


    // Store the compare function
    h->compare_func = comp_func;

    // Set active entries to 0
    h->active_entries = 0;

    // Determine how many pages of entries we need
    h->allocated_pages = initial_size / ENTRIES_PER_PAGE + ((initial_size % ENTRIES_PER_PAGE > 0) ? 1 : 0);
    h->minimum_pages = h->allocated_pages;

    // Allocate the table
    h->table = (void*)map_in_pages(h->allocated_pages);
}


// Cleanup a heap
void heap_destroy(heap* h) {
    // Check that h is not null
    assert(h != NULL);

    // Map out the table
    map_out_pages(h->table, h->allocated_pages);

    // Clear everything
    h->active_entries = 0;
    h->allocated_pages = 0;
    h->table = NULL;
}


// Gets the size of the heap
inline int heap_size(heap* h) {
    // Return the active entries
    return h->active_entries;
}


// Gets the minimum element
int heap_min(heap* h, void** key, void** value) {
    // Check the number of elements, abort if 0
    if (h->active_entries == 0)
        return 0;

    // Get the 0th element
    heap_entry* root = GET_ENTRY(0, h->table);

    // Set the key and value
    *key = root->key;
    *value = root->value;

    // Success
    return 1;
}


// Insert a new element
void heap_insert(heap *h, void* key, void* value) {
    // Check if this heap is not destoyed
    assert(h->table != NULL);

    // Check if we have room
    int max_entries = h->allocated_pages * ENTRIES_PER_PAGE;
    if (h->active_entries + 1 > max_entries) {
        // Get the new number of entries we need
        int new_size = h->allocated_pages * 2;

        // Map in a new table
        heap_entry* new_table = map_in_pages(new_size);

        // Copy the old entries, copy the entire pages
        memcpy(new_table, h->table, h->allocated_pages*PAGE_SIZE);
        
        // Cleanup the old table
        map_out_pages(h->table, h->allocated_pages);

        // Switch to the new table
        h->table = new_table;
        h->allocated_pages = new_size;
    }
    
    // Store the comparison function
    int (*cmp_func)(void*,void*) = h->compare_func;

    // Store the table address
    heap_entry* table = h->table;

    // Get the current index
    int current_index = h->active_entries;
    heap_entry* current = GET_ENTRY(current_index, table);

    // Loop variables
    int parent_index;
    heap_entry *parent;

    // While we can, keep swapping with our parent
    while (current_index > 0) {
        // Get the parent index
        parent_index = PARENT_ENTRY(current_index);

        // Get the parent entry
        parent = GET_ENTRY(parent_index, table);
       
        // Compare the keys, and swap if we need to 
        if (cmp_func(key, parent->key) < 0) {
            // Move the parent down
            current->key = parent->key;
            current->value = parent->value;

            // Move our reference
            current_index = parent_index;
            current = parent;

        // We are done swapping
        }   else
            break;
    }

    // Insert at the current index
    current->key = key;
    current->value = value; 

    // Increase the number of active entries
    h->active_entries++;
}


// Deletes the minimum entry in the heap
int heap_delmin(heap* h, void** key, void** value) {
    // Check there is a minimum
    if (h->active_entries == 0)
        return 0;

    // Load in the map table
    heap_entry* table = h->table;

    // Get the root element
    int current_index = 0;
    heap_entry* current = GET_ENTRY(current_index, table);

    // Store the outputs
    *key = current->key;
    *value = current->value;

    // Reduce the number of active entries
    h->active_entries--;

    // Get the active entries
    int entries = h->active_entries;
   
    // If there are any other nodes, we may need to move them up
    if (h->active_entries > 0) {
        // Move the last element to the root
        heap_entry* last = GET_ENTRY(entries,table);
        current->key = last->key;
        current->value = last->value;

        // Loop variables
        heap_entry* left_child;
        heap_entry* right_child;

        // Load the comparison function
        int (*cmp_func)(void*,void*) = h->compare_func;

        // Store the left index
        int left_child_index;

        while (left_child_index = LEFT_CHILD(current_index), left_child_index < entries) {
            // Load the left child
            left_child = GET_ENTRY(left_child_index, table);

            // We have a left + right child
            if (left_child_index+1 < entries) {
                // Load the right child
                right_child = GET_ENTRY((left_child_index+1), table);

                // Find the smaller child
                if (cmp_func(left_child->key, right_child->key) <= 0) {

                    // Swap with the left if it is smaller
                    if (cmp_func(current->key, left_child->key) == 1) {
                        SWAP_ENTRIES(current,left_child);
                        current_index = left_child_index;
                        current = left_child;

                    // Otherwise, the current is smaller
                    } else
                        break;

                // Right child is smaller
                } else {

                    // Swap with the right if it is smaller
                    if (cmp_func(current->key, right_child->key) == 1) {
                        SWAP_ENTRIES(current,right_child);
                        current_index = left_child_index+1;
                        current = right_child;

                    // Current is smaller
                    } else
                        break;

                }


            // We only have a left child, only do something if the left is smaller
            } else if (cmp_func(current->key, left_child->key) == 1) {
                SWAP_ENTRIES(current,left_child);
                current_index = left_child_index;
                current = left_child;

            // Done otherwise
            }  else
                break;

        }
    } 

    // Check if we should release a page of memory
    int used_pages = entries / ENTRIES_PER_PAGE + ((entries % ENTRIES_PER_PAGE > 0) ? 1 : 0);

    // Allow one empty page, but not two
    if (h->allocated_pages / 2 > used_pages + 1 && h->allocated_pages / 2 >= h->minimum_pages) {
        // Get the new number of entries we need
        int new_size = h->allocated_pages / 2;

        // Map in a new table
        heap_entry* new_table = map_in_pages(new_size);

        // Copy the old entries, copy the entire pages
        memcpy(new_table, h->table, used_pages*PAGE_SIZE);
        
        // Cleanup the old table
        map_out_pages(h->table, h->allocated_pages);

        // Switch to the new table
        h->table = new_table;
        h->allocated_pages = new_size;
    }

    // Success
    return 1;
}


// Allows a user to iterate over all entries, e.g. to free() the memory
void heap_foreach(heap* h, void (*func)(void*,void*)) {
    // Store the current index and max index
    int index = 0;
    int entries = h->active_entries;

    heap_entry* entry;
    heap_entry* table = h->table;

    for (;index<entries;index++) {
        // Get the entry
        entry = GET_ENTRY(index,table);

        // Call the user function
        func(entry->key, entry->value);
    }
}

>>>>>>> f21d90c (make revendor)
