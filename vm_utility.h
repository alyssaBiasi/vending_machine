/*****************************************************************************
* COSC1283/1284 - Programming Techniques
* Semester 2 2011 Assignment #2 - Vending Machine
* Full Name        : Alyssa Biasi
* Course Code      : COSC1284
* Program Code     : BP215
******************************************************************************/

#ifndef _VM_UTILITY_H
#define _VM_UTILITY_H

/* Function prototypes. */
void readRestOfLine();
int systemInit(VendingMachineType*);
int loadData(VendingMachineType*, char*, char*);
void systemFree(VendingMachineType*);

/* Mine */
int readInt();
int readCoins(VendingMachineType*, FILE*);
int readStock(VendingMachineType*, FILE*);
int checkTokens(char[]);
ProductNodeType *generateNode();
ProductNodeType *searchProductName(VendingMachineType*, char*);
void insertNode(VendingMachineType*, ProductNodeType*);
void deleteNode(VendingMachineType*, char*);

#endif
