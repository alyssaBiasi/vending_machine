/*****************************************************************************
* COSC1283/1284 - Programming Techniques
* Semester 2 2011 Assignment #2 - Vending Machine
* Full Name        : Alyssa Biasi
* Course Code      : COSC1284
* Program Code     : BP215
******************************************************************************/

#ifndef _VM_H
#define _VM_H

/* System-wide header files. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

/* Constants. */
#define COIN_DEFAULT 5
#define STOCK_DEFAULT 5
#define PRODUCT_NAME_MAX 40
#define PRODUCT_BRAND_MAX 20
#define DISTINCT_COINS 6

#define SUCCESS 1
#define FAILURE 0

/* Mine */
#define TRUE 1
#define FALSE 0
#define EXIT -1
#define EXTRA_SPACES 2
#define TWO_DOLLAR_COIN 0
#define ONE_DOLLAR_COIN 1
#define FIFTY_CENT_COIN 2
#define TWENTY_CENT_COIN 3
#define TEN_CENT_COIN 4
#define FIVE_CENT_COIN 5

/* Structure definitions. */
typedef struct coin
{
   unsigned qty;
   unsigned value;  /* Stored in cents (not dollars). */
} CoinType;

typedef struct productNode
{
   char name[PRODUCT_NAME_MAX + 1];
   char brand[PRODUCT_BRAND_MAX + 1];
   unsigned price;  /* Stored in cents (not dollars). */
   unsigned qty;
   struct productNode *nextProduct;
} ProductNodeType;

typedef struct vendingMachine
{
   CoinType coins[DISTINCT_COINS];
   unsigned totalCoins;
   ProductNodeType *headProduct;
   unsigned totalProducts;
} VendingMachineType;

#endif
