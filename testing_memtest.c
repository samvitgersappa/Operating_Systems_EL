#include<stdio.h>
#include<sys/resource.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/time.h>
#include<math.h>
#define MAX_DATA_ENTRIES 100
void rec(int n){
	int a = 1;
	
	int b = 9;
	if(n == 0) return;
	else rec(n-1);
}

struct memory_data{
long pages_replaced;
long context_switches;
long runtime;
long prog_size;
long resident_memory;
long shared_memory;
long program_memory;
long text_memory;
long lib_pages;
long data_pages;
long dirty_pages;
int id;
};

struct node{
    struct node *next;
    // struct node *prev;
};

struct pool{
    struct node* free1;
    struct node* occuppied;
    struct node *initial;
    int size_global;
};

// int size_global , divisions_global;

struct pool *POOL_GLOBAL;



void insert_free(struct pool **POOL, struct node *temp){
    // handle empty head
    struct node *head;
    
    // free2 = malloc(sizeof(struct node));
    head = (*POOL)->free1;
    struct node *p;
    p = head;
    if(head == NULL){
        // printf("possibly causing error\n");
        temp->next = temp;
        head = temp;
        (*POOL)->free1 = head;
        return;

    }
    while (p->next != head){
        p = p->next;
    }
    
    p->next = temp;
    temp->next = head;
    (*POOL)->free1 = head;
    // display(*head);
    // printf("next of last: %ld \n" , (temp->next) - free1);


}

void insert_occuppied(struct pool **POOL, struct node *temp){
    // handle empty head
    struct node *head;
    
    // free2 = malloc(sizeof(struct node));
    head = (*POOL)->occuppied;
    struct node *p;
    p = head;
    if(head == NULL){
        // printf("possibly causing error\n");
        temp->next = temp;
        head = temp;
        (*POOL)->occuppied = head;
       
        return;

    }
    while (p->next != head){
        p = p->next;
    }
    
    p->next = temp;
    temp->next = head;
    (*POOL)->occuppied = head;
    
    // display(*head);
    // printf("next of last: %ld \n" , (temp->next) - free1);


}

struct node* delete_occuppied(struct pool **POOL , struct node *temp){
    // there was a problem in this function where if head was the element removed, head must be updated
    // printf("hi\n");
    struct node *head;
    head = (*POOL)->occuppied;
    
    // printf("head: %p\n" , head);
    /*ERROR IS BEING CAUSED BECAUSE HEAD OF OCCUPPIED DOES NOT HAVE A NEXT 
    PROBABLY BEING CAUSED COZ (*POOL)->OCCUPPIED ISNT BEING REASSIGNED HEAD IN THE OLD FUNCTION*/

    
    struct node *p , *q;
    q = head;
    p = q->next;
  
    if(p == q){
        
        // printf("one\n");
        head = NULL;
        (*POOL)->occuppied= head;
        return temp;

    }


    // if(p == temp){
    //     // printf("two\n");
    //     q->next = p->next;
    //     display(occuppied);
    //     return p;
    // }
    // q = p;
    // printf("pointer test: %ld\n" , p - free1);
    // p = p->next;
    // printf("pointer test: %ld\n" , p - free1);

    while(p!=head){
        // printf("fault here\n");
        if(p == temp){
            q->next = p->next;
            

            // display(occuppied);
            return p;
        }

        q = p;
        p = p->next;
        // printf("pointer test: %ld\n" ,q - free1);
        // printf("pointer test: %ld\n" , p - free1);
        // printf("two\n");
    }

    // here handle deletion of head itself
    if(p == head){
        // printf("fault here\n");
        q->next = p->next;
        head = p->next;
        (*POOL)->occuppied = head;
        // printf("DEBUG: %ld\n" , occuppied - free1);
        // printf("DEBUG: %ld\n" , occuppied->next - free1);
        // printf("DEBUG: %ld\n" , occuppied->next->next - free1);
        return p;
    }

    printf("NO MATCH\n");
    return NULL;

}

// void delete_free(struct node **head){
//     struct node *p;
//     p = *head;
//     *head = p->next;
//     free(p);
//     return;
// }

// change everything to take parameter as pool

struct node* delete_free(struct pool **POOL){
    struct node *head;
    head = (*POOL)->free1;
    struct node *p, *q;
    p = head;
    q = (head)->next;
    if(p == p->next){
        // printf("hi\n");
        head = NULL;
        return p;
    }
    (head)->next = q->next;
    
    // printf("temp: %p\n" , q);
    return q;
}

void display(struct node *head){
    struct node *p;
    p = head;
    if(p == NULL){
        printf("NO elements\n");
        return;
    }
    if(p->next == p){
        printf("%p-%ld ",p, p - (struct node*)POOL_GLOBAL);
        return;
    }
    while(p->next != head){
        printf("%p-%ld ",p, p - (struct node*)POOL_GLOBAL);
        // printf("%p ", p);
        p = p->next;
    }
    printf("%p-%ld ",p, p - (struct node*)POOL_GLOBAL);
    // printf("%p ", p);
}



struct pool* create_pool(int size , int divisions){
   
    // printf("hi1");
    // printf("hi");
    struct node *pool;
    // struct node *free1;
    struct pool *POOL , *tempo;
    POOL = (struct pool*)malloc(size*divisions + divisions*sizeof(struct node*) + sizeof(struct pool));
    // printf("size of pointer: %ld\n" , sizeof(struct node*));
    // printf("total size allocated: %ld\n" , size*divisions + divisions*sizeof(struct node*) + sizeof(struct pool));
    // printf("\npool: %p " , POOL);
    POOL->initial = POOL;
    POOL->size_global = size;
    tempo = (struct node*)POOL;
    tempo = tempo + sizeof(struct pool);
    POOL->free1 = tempo;
    POOL_GLOBAL = tempo;

    
    (POOL->free1)->next = POOL->free1;
    POOL->occuppied = NULL;
   // printf("\nfree: %p \n" , POOL->free1);
    // printf("hi2");
    struct node *temp;
    temp = POOL->free1;
    
    insert_free(&POOL , temp->next);
    // free1->next = free1;
    for(int i = 1 ; i < divisions ; i++){
        
        temp = temp + size +sizeof(struct node*);
        insert_free(&POOL, temp);
      //  printf("\ndiff: %ld\n" , temp - POOL->free1);
        

     }
    // free1->next = pool;
    return POOL;
}

void *allocate(struct pool **POOL){
    int available = pool_available(*POOL);
    if(available == 0){
    	printf("POOL IS OCCUPIED!!");
    	return NULL;
    } 
    struct node *p = delete_free(POOL);
   
    // display(free1);
    p = p + sizeof(struct node*);
    insert_occuppied(POOL, (struct node*)(p - sizeof(struct node*)));
    //printf("to be inserted to occu: %ld\n" , p - sizeof(struct node*) - (POOL->free1));
  //  printf("displaying occuppied for testing: \n");
    //display((*POOL)->occuppied);
  //  printf("ALLOCATED: %ld\n" , p - sizeof(struct node*) - ((*POOL)->free1));
    void *mem;
    mem = (void*)p;
 //   printf("ALLOCATED: %p\n" , mem);
   
    return mem;    
}

void deallocate(struct pool **POOL , void *ptr){
    struct node *temp;
    
    // printf("DEBUG: %p\n" , (*POOL)->occuppied);
    // printf("DEBUG: %p\n" , (*POOL)->occuppied->next);
    // printf("DEBUG: %p\n" , (*POOL)->occuppied->next->next);
    
    /* FOR SOME REASON FREE LINKED LIST IS WORKING PERFECTLY 
    BUT OCCUPPIED LINKED LIST IS GIVING SOME ERROR COZ IT ISNT ALLOCATING PROPERLY
    CHECK ALLOCATION OF OCCUPPIED LINKED LIST */

  //  printf("to be deleted: %ld\n" , ((((struct node*)ptr-((*POOL)->free1))/((*POOL)->size_global + sizeof(struct node*)))*((*POOL)->size_global + sizeof(struct node))));
    temp = (struct node*)((*POOL)->free1) + ((((struct node*)ptr-((*POOL)->free1))/((*POOL)->size_global + sizeof(struct node*)))*((*POOL)->size_global + sizeof(struct node)));
 //   printf("ptr to be deleted %p\n" , temp);
    //status(*POOL);
    delete_occuppied(POOL , temp);
    // printf("DEBUG: %ld\n" , occuppied - free1);
    // printf("DEBUG: %ld\n" , occuppied->next - free1);
    // printf("DEBUG: %ld\n" , occuppied->next->next - free1);
    
    insert_free(POOL , temp);
}

void freeall(struct pool **POOL){
    free(*POOL);
    return;
}

void status(struct pool *POOL){ 
    printf("\n");
    printf("FREE: \n");
    display((POOL->free1));
    printf("\n");
    printf("OCCUPPIED: \n");
    display((POOL->occuppied));
    printf("\n");
}

int pool_available(struct pool *POOL){
    if(POOL->free1 == NULL){
        return 0;
    }
}

void clear_pool(struct pool *POOL){
    if(POOL->occuppied == NULL){
    	return;
    }
    struct node * temp;
    struct node * ptr;
   
    temp = (POOL->occuppied) -> next;
    while(temp != NULL){
    	ptr = delete_occuppied(&(POOL->occuppied), temp);
    	insert_free(&POOL, ptr);
    }
    return;
}


// memory pool ends



long mem_usage(){
struct rusage usage;
getrusage(RUSAGE_SELF , &usage);
return usage.ru_maxrss;
}

void save_memory_data(){
// add int id parameter
	FILE *file = fopen("output.dat", "ab");

    if (file == NULL) {
        fprintf(stderr, "Error opening file for writing.\n");
        return;
    }

    // Create a sample structure
    struct memory_data data;
    // set all data fields here
    struct rusage usage;
    getrusage(RUSAGE_SELF , &usage);
    data.pages_replaced = usage.ru_minflt;
    data.context_switches = usage.ru_nvcsw;
    
    const char* statm_path = "/proc/self/statm";
 

  FILE *f = fopen(statm_path,"r");
  fscanf(f , "%ld" , &data.prog_size);
  
  
  fscanf(f , "%ld" , &data.resident_memory);
 
  
  fscanf(f , "%ld" , &data.shared_memory);
  
  
  
  data.program_memory = data.resident_memory - data.shared_memory;
  
  fscanf(f , "%ld" , &data.text_memory);
  
  
  fscanf(f , "%ld" , &data.lib_pages);

  
  fscanf(f , "%ld" , &data.data_pages);


  fscanf(f , "%ld" , &data.dirty_pages);
  //data.id = id;
   
  fclose(f);
    
    

    // Write the structure to the file
    size_t num_elements_written = fwrite(&data, sizeof(struct memory_data), 1, file);

    if (num_elements_written != 1) {
        fprintf(stderr, "Error writing structure to file.\n");
        fclose(file);
        return;
    }

    // Close the file
    fclose(file);
}

struct memory_data retrieve_data(){
// add int id parameter
	struct memory_data data;
	
     FILE *file = fopen("output.dat", "rb");
     if (file == NULL) {
        fprintf(stderr, "Error opening file for reading.\n");
        return data;
    }
    
    size_t num_elements_read = fread(&data, sizeof(struct memory_data), 1, file);
    if (num_elements_read != 1) {
        fprintf(stderr, "Error reading structure from file.\n");
        fclose(file);
        return data;
    }
    fclose(file);

    
    printf("Read structure from file:\n");
    printf("Pages Replaced: %ld\nContext Switches: %ld\nResident Memory Pages: %ld\nShared Memory Pages:%ld\nProgram Memory Pages: %ld\nText Memory Pages: %ld\nLib Pages: %ld\nData Pages : %ld\n", data.pages_replaced, data.context_switches, data.resident_memory,data.shared_memory,data.program_memory,data.text_memory,data.lib_pages,data.data_pages);

    return data;
}

long *compare(){
    struct memory_data data_old;
    long out[8];
	
     FILE *file = fopen("output.dat", "rb");
     if (file == NULL) {
        fprintf(stderr, "Error opening file for reading.\n");
        // return data_old;
    }

    size_t num_elements_read = fread(&data_old, sizeof(struct memory_data), 1, file);
    if (num_elements_read != 1) {
        fprintf(stderr, "Error reading structure from file.\n");
        fclose(file);
        // return data_old;
    }
    fclose(file);

    struct memory_data data;

    // set all data fields here
    struct rusage usage;
    getrusage(RUSAGE_SELF , &usage);
    data.pages_replaced = usage.ru_minflt;
    data.context_switches = usage.ru_nvcsw;
    
    const char* statm_path = "/proc/self/statm";
 

  FILE *f = fopen(statm_path,"r");
  fscanf(f , "%ld" , &data.prog_size);
  fscanf(f , "%ld" , &data.resident_memory);
  fscanf(f , "%ld" , &data.shared_memory);
  data.program_memory = data.resident_memory - data.shared_memory;
  fscanf(f , "%ld" , &data.text_memory);
  fscanf(f , "%ld" , &data.lib_pages);
  fscanf(f , "%ld" , &data.data_pages);
  fscanf(f , "%ld" , &data.dirty_pages);
  //data.id = id;
  fclose(f);

    printf("\n\nComparing...\n");

    printf("Pages Replaced: %ld\nContext Switches: %ld\nResident Memory Pages: %ld\nShared Memory Pages:%ld\nProgram Memory Pages: %ld\nText Memory Pages: %ld\nLib Pages: %ld\nData Pages : %ld\n" , data.pages_replaced - data_old.pages_replaced,
    data_old.context_switches - data.context_switches,
data_old.resident_memory - data.resident_memory,
data_old.shared_memory - data.shared_memory,
data_old.program_memory - data.program_memory,
data_old.text_memory - data.text_memory,
data_old.lib_pages - data.lib_pages,
data_old.data_pages - data.data_pages);

out[0] = data_old.pages_replaced - data.pages_replaced;
out[1] = data_old.context_switches - data.context_switches;
out[2] = data_old.resident_memory - data.resident_memory;
out[3] = data_old.shared_memory - data.shared_memory;
out[4] = data_old.program_memory - data.program_memory;
out[5] = data_old.text_memory - data.text_memory;
out[6] = data_old.lib_pages - data.lib_pages;
out[7] = data_old.data_pages - data.data_pages;

return out;


}

void compare_with_previous_runs() {
    struct memory_data current_data;
    FILE *file = fopen("output.dat", "rb");
    if (file == NULL) {
        fprintf(stderr, "Error opening file for reading.\n");
        return;
    }
    size_t num_elements_read = fread(&current_data, sizeof(struct memory_data), 1, file);
    if (num_elements_read != 1) {
        fprintf(stderr, "Error reading current data from file.\n");
        fclose(file);
        return;
    }
    printf("Successfully read current data from file.\n");
    fseek(file, 0, SEEK_SET); // Rewind file pointer to read from the beginning

    struct memory_data previous_data[MAX_DATA_ENTRIES];
    int num_previous_entries = 0;

    // Read all the previous data entries from the file
    while (fread(&previous_data[num_previous_entries], sizeof(struct memory_data), 1, file) == 1 && num_previous_entries < MAX_DATA_ENTRIES) {
        num_previous_entries++;
    }

    fclose(file);

    printf("Number of previous entries read: %d\n", num_previous_entries);

    // Print the comparison results in table format
    printf("Comparison with previous runs:\n");
    printf("------------------------------------------------------------------------------------------------------------\n");
    printf("| %-5s | %-15s | %-15s | %-15s | %-15s | %-15s | %-15s | %-15s | %-15s | %-15s | %-15s |\n", 
           "Entry", "Pages Replaced", "Context Switches", "Resident Memory", "Shared Memory", "Program Memory", 
           "Text Memory", "Lib Pages", "Data Pages", "Dirty Pages", "Runtime");
    printf("------------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < num_previous_entries; i++) {
        printf("| %-5d | %-15ld | %-15ld | %-15ld | %-15ld | %-15ld | %-15ld | %-15ld | %-15ld | %-15ld | %-15ld |\n", 
               i + 1,
               current_data.pages_replaced - previous_data[i].pages_replaced,
               current_data.context_switches - previous_data[i].context_switches,
               current_data.resident_memory - previous_data[i].resident_memory,
               current_data.shared_memory - previous_data[i].shared_memory,
               current_data.program_memory - previous_data[i].program_memory,
               current_data.text_memory - previous_data[i].text_memory,
               current_data.lib_pages - previous_data[i].lib_pages,
               current_data.data_pages - previous_data[i].data_pages,
               current_data.dirty_pages - previous_data[i].dirty_pages,
               current_data.runtime - previous_data[i].runtime);
        printf("------------------------------------------------------------------------------------------------------------\n");
    }
}



int main(){
    struct pool *pool , *pool2;
    pool = create_pool(100 , 5);
    int *test1;
    double *test2;
    
    test1 = (int*)allocate(&pool);
     printf("RECIEVED: %ld\n" , (struct node*)test1 - pool->free1);
    
    test1[1] = 8;
    test1[3] = 9;
    test2 = (double*)allocate(&pool);
    printf("RECIEVED: %ld\n" , (struct node*)test2 - pool->free1);
    test2[1] = 9.0;
    status(pool);
    // printf("%p - %p\n" , pool , (void*)pool);
    int *ptr;
    status(pool);
    ptr = (int*)pool;
    status(pool);
    // UNCOMMENT THIS LATER// ptr[3] = 10;
    status(pool);
    // printf("%p - %p\n" , pool , ptr);
    printf("element: %d\n" , ptr[3]);;
    // printf("debug: %ld\n" , occuppied - free1);
    // printf("debug: %ld\n" , occuppied->next - free1);
    // printf("debug: %ld\n" , occuppied->next->next - free1);
    // printf("diff: %ld\n" , (struct pool*)test1 - POOL_GLOBAL);
    // printf("diff: %ld\n" , (struct pool*) test2 - POOL_GLOBAL);

    status(pool);
 
    deallocate(&pool , test1);
    status(pool);
    // printf("debug: %ld\n" , occuppied - free1);
    // printf("debug: %ld\n" , occuppied->next - free1);
    // deallocate(&pool, test2);
    status(pool);
    
struct rusage usage;
getrusage(RUSAGE_SELF , &usage);
long start = (usage.ru_stime).tv_usec;
long start2 = (usage.ru_utime).tv_usec;

long baseline = usage.ru_maxrss;



int c = 100000;
int *d , *e, *e2;
// rec(c);
// d = (int*)malloc(5*sizeof(int));
// memset(d, 0 , 5*sizeof(int));
// for(int i = 0 ; i < 1000;  i++){
// e = (int*)malloc(4096*1);
// memset(e , 1 , 4096*1);

// }

getrusage(RUSAGE_SELF , &usage);
printf("memory usage: %ld\n" ,usage.ru_maxrss - baseline);

// e2 = (int*)malloc(4096*310);
// memset(e2 , 1 , 4096*310);



int mem = getrusage(RUSAGE_SELF , &usage);
long final = mem_usage();
printf("IO pages replaced: %ld\n" ,usage.ru_majflt);
printf("pages replaced: %ld\n" ,usage.ru_minflt);
printf("context switches: %ld\n" ,usage.ru_nvcsw);
printf("memory usage: %ld\n" ,usage.ru_maxrss - baseline);
long end = (usage.ru_stime).tv_usec;
long end2 = (usage.ru_utime).tv_usec;
printf("time used: %ld\n" ,(usage.ru_utime).tv_usec);
printf("time used: %ld\n" ,end2 - start2);

long a[100];
for(int i = 0 ; i < 100 ; i++) a[i] = 0;


// save_memory_data();
// retrieve_data();
compare();
compare_with_previous_runs();

freeall(&pool);

struct rusage usage2;
getrusage(RUSAGE_SELF , &usage2);
long startn = (usage2.ru_stime).tv_usec;
long startn2 = (usage2.ru_utime).tv_usec;
int *poi;
int poin[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0};



pool2 = create_pool(sizeof(poin) ,5);
// printf("memory allocated successfully\n");
for(int i = 0 ; i < 5 ; i++){

    for(int i = 0 ; i < 1000000 ; i++){
    poi = (int*)allocate(&pool2);
    *poi = poin;
    //printf("status of huge pool: \n");
    // printf("number: %d\n",poi[10]);
    //status(pool2);
    //for(int j = 0 ; j < 50 ; j++)printf("element: %d\n" , poi[j]);

    
   deallocate(&pool2, poi);
    }
}
freeall(&pool2);

struct rusage usage3;
getrusage(RUSAGE_SELF , &usage3);
long endn = (usage3.ru_stime).tv_usec;
long endn2 = (usage3.ru_utime).tv_usec;
long pooltime1 , pooltime2;
pooltime1 = endn - startn;
pooltime2 = endn2 - startn2;

// save_memory_data();
// retrieve_data();

// printf("pool time: %ld\n" , endn - startn);
// printf("pool time: %ld\n" , endn2 - startn2);

struct rusage usage4;
getrusage(RUSAGE_SELF , &usage4);
startn = (usage4.ru_stime).tv_usec;
startn2 = (usage4.ru_utime).tv_usec;
for(int i = 0 ; i < 5 ; i++){
    for(int i = 0 ; i < 1000000 ; i++){
    poi = (int*)malloc(sizeof(poin));
    *poi = poin;
    //for(int j = 0 ; j < 50 ; j++)printf("element: %d\n" , poi[j]);


   free(poi);
    }
}

struct rusage usage5;
getrusage(RUSAGE_SELF , &usage5);
endn = (usage5.ru_stime).tv_usec;
endn2 = (usage5.ru_utime).tv_usec;
// printf("normal time: %ld\n" , endn - startn);
printf("normal time: %ld\n" , endn2 - startn2);

// save_memory_data();
// retrieve_data();

// printf("pool time: %ld\n" , pooltime1);
printf("pool time: %ld\n" , pooltime2);
  
free(e2);
}