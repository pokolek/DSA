//
//  main.c
//  DSA
//
//  Created by Peter Oliver Kolek on 24/02/2020.
//  Copyright © 2020 Peter Oliver Kolek. All rights reserved.
//

#include <stdio.h>
#include <string.h>

typedef struct Memory{      //definovana struktura bloku pamati
    unsigned short size;    //velkost bloku
    char allocated;         //ci uz je allokovany == 1, alebo je free == 0
    struct Memory* next;    //pointer na dalsi blok
}memory;

memory* begin;              //globalna premenna pre zaciatok pamate

void memory_init(void* ptr, unsigned int size)
{
    begin = ptr;            //priradenie zaciatku regionu k nasemu bloku pamate
    begin->size = size;
    begin->next = NULL;
    begin->allocated = 0;
}

void* memory_alloc(unsigned int size)
{
    memory* tmp;        //pomocny blok pamate
    memory* freeBlock = NULL;
    int headder = sizeof(memory);       //velkost hlavicky pamate
    
    tmp = begin;

    while((tmp->next) != NULL) //hladame volny blok do ktoreho mozeme alokovat
    {
        if(((tmp->size) < size + headder) || ((tmp->allocated) == 1))
            tmp = tmp->next;
    }
    
    
    if((tmp->size) > (size + headder)) //1. pripad: ak velkost volneho bloku je dostacujuca nasim potrebam
    {
        freeBlock = ((void*)tmp + size + headder);  //novy freeblock, ktoreho adresu nastavime za blok, ktory alokujeme
        freeBlock->size = (tmp->size) - size - headder;
        freeBlock->next = tmp->next;
        freeBlock->allocated = 0;
                                            // rozdelime blok pamate na 2 bloky, free a ten ktory chceme zapisat
        tmp->size = size + headder;
        tmp->allocated = 1;
        tmp->next = freeBlock;              // vlozime ho pred freeblock
        
        return (void*)tmp + headder;
    }
    
    else if((tmp->size) == size + headder)       //2. pripad: ak sa pozadovana velkost pamate presne rovna volnemu bloku
    {
        tmp->allocated = 1;
        return (void*)tmp + headder;        //vraciame ukazovatel na pozadovane miesto zapisu dat
        
    }
    
    return NULL;                             //3. pripad: nedostatok volnej pamate
}

int memory_check(void* ptr)
{
    memory* tmp = begin;
    int headder = sizeof(memory);
    ptr = (void*)ptr - headder;
    
    while(tmp != NULL)                      //prejdeme pamat po blokoch
    {
        if((tmp == ptr) && (tmp->allocated == 1))  //ak najdeme nas blok a je alokovany vratime 1
            return 1;
        
        tmp = tmp->next;
    }
    
    return 0;                                       //inak vratime 0
}

int memory_free(void* valid_ptr)
{
    if((memory_check(valid_ptr) == 0 )|| (valid_ptr == NULL)) //ak nieje platny blok pamate
        return 1;
    
    memory* tmp = valid_ptr;
    int headder = sizeof(memory);
    
    tmp = (void*)tmp - headder;             //dostaneme sa na zaciatok bloku
    tmp->allocated = 0;
    
    tmp = begin;
    
    while((tmp->next) != NULL)                //ak vzniknu dva volne bloky vedla seba tak ich spojime do jedneho
    {
        if((tmp->allocated == 0) && ((tmp->next)->allocated == 0))
        {
            tmp->size += (tmp->next)->size;
            tmp->next = (tmp->next)->next;
            continue;
        }
        
        tmp = tmp->next;
        
        if(tmp == NULL)
            break;
    }
    
    return 0;
}


void memory_info_print()
{
    memory* tmp = begin;
    int headder = sizeof(memory);
    int i = 1;
    printf("-------------------------------------------------------------\n");
    while (tmp != NULL) {
        
        printf("%d. block:\n", i++);
        printf("\tadress: %p \n", tmp);
        printf("\tdata adress: %p\n", (void*)tmp + headder);
        printf("\tsize: %d \n", tmp->size);
        printf("\tallocated: %d \n", tmp->allocated);
        printf("\tnext: %p\n", tmp->next);
        
        tmp = tmp->next;
    }
    printf("-------------------------------------------------------------\n");
    
}

void test_small()
{
    char *test = memory_alloc(8);
    char *test2 = memory_alloc(8);
    char *test3 = memory_alloc(4);
    
    memory_info_print();
    printf("|test: %p| ", test);
    printf("|test2: %p| ", test2);
    printf("|test3: %p|\n", test3);
    
    
    memory_free(test);
    memory_free(test2);
    memory_info_print();
}

void testBig(void) {
    printf("TEST BIG spusteny\n");
    
    char* pointer = (char*)memory_alloc(4992);                        //alokovany blok o velkosti 4992 bytov
    char* ptr = (char*)memory_alloc(1526);                            //alokovany blok o velkosti 1526 bytov
    char* p = (char*)memory_alloc(2895);                            //alokovany blok o velkosti 2895 bytov

    if (memory_check(pointer)) {
        printf("Ukazovatel pointer je platny\n");
        memset(pointer, 1, 4992);
    }

    memory_free(pointer);
    memory_free(ptr);
    memory_free(p);

}


int main(void){
    
    int size = 5000;
    char region[size];      //pole pamati, s danou velkostou v byte-och
    memory_init(region, size);
    
    
    
    for (int i = 0; i < size; i++)      //vypis adries regionu
        printf("%d: %p\n",i, &region[i]);
    
    //test_small();
    testBig();
    memory_info_print();
    
    return 0;
}
