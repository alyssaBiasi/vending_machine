/*****************************************************************************
* COSC1283/1284 - Programming Techniques
* Semester 2 2011 Assignment #2 - Vending Machine
* Full Name        : Alyssa Biasi
* Course Code      : COSC1284
* Program Code     : BP215
******************************************************************************/

#ifndef _VM_OPTIONS_H
#define _VM_OPTIONS_H

void purchaseProduct(VendingMachineType*);
void displayProducts(VendingMachineType*);
void saveData(VendingMachineType*, char*, char*);
void addProduct(VendingMachineType*);
void removeProduct(VendingMachineType*);
void displayCoins(VendingMachineType*);
void restockProducts(VendingMachineType*);
void restockCoins(VendingMachineType*);

#endif
