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

    while((((tmp->size) < size + headder) ||
           ((tmp->allocated) == 1)) && ((tmp->next) != NULL)) //hladame volny blok do ktoreho mozeme alokovat
        tmp = tmp->next;
    
    if((tmp->size) == size + headder)       //1. pripad: ak sa pozadovana velkost pamate presne rovna volnemu bloku
    {
        tmp->allocated = 1;                 //nastavime na 1, velkost nemusime riesit, lebo je rovnaka
        return (void*)tmp + headder;        //vraciame ukazovatel na pozadovane miesto zapisu dat
        
    }
    else if((tmp->size) > (size + headder)) //2. pripad: ak velkost volneho bloku je dostacujuca nasim potrebam
    {
        freeBlock = (void*)((void*)tmp + size + headder);
        freeBlock->size = (tmp->size) - size - headder;
        freeBlock->allocated = 0;
        freeBlock->next = tmp->next;
        tmp->size = size + headder;
        tmp->allocated =1;
        tmp->next = freeBlock;
        
        /*printf("%d - size\n", tmp->size);
        printf("%p - allocated\n", &tmp->allocated);
        printf("%p - next\n", &tmp->next);
        printf("%p - data\n\n", (void*)tmp + headder);*/
        
        return (void*)tmp + headder;
    }
    //printf("\nNULL\n");
    return NULL;                             //3. pripad: nedostatok volnej pamate
}

int memory_free(void* valid_ptr)
{
    memory *tmp = valid_ptr;
    int headder = sizeof(memory);
    
    tmp = (void*)tmp - headder;
    tmp->allocated = 0;
    
    tmp = begin;
    
    while(tmp->next != NULL)
    {
        if((tmp->allocated == 0) && ((tmp->next)->allocated == 0))
        {
            tmp->size += (tmp->next)->size;
            //printf("\nfree size: %d\n", tmp->size);
            tmp->next = (tmp->next)->next;
        }
        tmp = tmp->next;
    }
    
    return 0;
}


int memory_check(void* ptr)
{
    memory* tmp = begin;
    int headder = sizeof(memory);
    ptr = (void*)ptr - headder;
    
    while(tmp != NULL)
    {
        if((tmp == ptr) && (tmp->allocated == 1))
            return 1;
        
        tmp = tmp->next;
    }
    
    return 0;
}


int main(void){
    char region[1024];      //pole pamati, s danou velkostou v byte-och
    memory_init(region, 1024);
    
    for (int i = 0; i < 1024; i++)      //vypis adries regionu
    {
        printf("%d: %p\n",i, &region[i]);
    }
    
    
    char *test = memory_alloc(35);
    char *test2 = memory_alloc(4);
    char *test3 = memory_alloc(4);
    char *test4 = memory_alloc(4);
    
    
    printf("adresa: %p\n", test);
    printf("adresa2: %p\n",test2);
    printf("adresa3: %p\n",test3);
    printf("adresa4: %p\n",test4);
    
    memory_free(test2);
    memory_free(test3);
    
    
    
    //printf("1: %d 2: %d 3: %d 4: %d\n", memory_check(test),memory_check(test2),memory_check(test3),memory_check(test4));
   
    
    return 0;
}
