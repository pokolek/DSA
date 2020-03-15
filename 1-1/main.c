//
//  main.c
//  DSA
//
//  Created by Peter Oliver Kolek on 17/02/2020.
//  Copyright © 2020 Peter Oliver Kolek. All rights reserved.
//

#include <stdio.h>

int main()
{
    int t,n, kupena = 0, zisk = 0;
    
    scanf("%d",&t);
    
    int pole[10];
    
    for(int i = 0; i < t; i++)
    {
        scanf("%d",&n);
        
        for(int j = 0; j < n; j++)
        {
            scanf("%d",&pole[j]);
        }
        
        for (int k = 0; k < n; k++)
        {
            printf("%d\n",pole[k] );
        }
        
    }
    return 0;
}
