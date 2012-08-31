/*****************************************************************************
* COSC1283/1284 - Programming Techniques
* Semester 2 2011 Assignment #2 - Vending Machine
* Full Name        : Alyssa Biasi
* Course Code      : COSC1284
* Program Code     : BP215
******************************************************************************/

#include "vm.h"
#include "vm_utility.h"
#include "vm_options.h"


/****************************************************************************
* ProductNodeType: Creates a new initialised node.
*
* 		-- Returns: Blank node or NULL.
****************************************************************************/
ProductNodeType* generateNode() {
	ProductNodeType *new;
	int i;

	new = malloc(sizeof(ProductNodeType));
	if(new == NULL) {
		printf("Product node memory allocation failed.");
		return NULL;
	}

	for(i=0; i<PRODUCT_NAME_MAX+1; i++) {
		new->name[i] = 0;
	}

	for(i=0; i<PRODUCT_BRAND_MAX+1; i++) {
		new->brand[i] = 0;
	}

	new->price = 0;
	new->qty = 0;
	new->nextProduct = NULL;
	return new;
}


/****************************************************************************
* ProductNodeType: Inserts new node into lined list.
****************************************************************************/
void insertNode(VendingMachineType *vm, ProductNodeType *new) {
	ProductNodeType *current, *previous;

	/* Organized alphabetically. */
	/* Searching through list for correct spot. */
	previous = NULL;
	current = vm->headProduct;
	while(current != NULL && strcmp(current->name, new->name) < 0) {
		previous = current;
		current = current->nextProduct;
	}

	/* Inserting new node. */
	if(previous == NULL) {
		new->nextProduct = current;
		vm->headProduct = new;
	}
	else {
		previous->nextProduct = new;
		new->nextProduct = current;
	}

	vm->totalProducts++;
}


/****************************************************************************
* ProductNodeType: Deletes a node from the linked list.
****************************************************************************/
void deleteNode(VendingMachineType *vm, char *input) {
	ProductNodeType *current, *previous;
	int found = FALSE;

	/* Searching list for matching product name. */
	previous = NULL;
	current = vm->headProduct;
	while(current != NULL) {
		if(strcmp(current->name, input) == 0) {
			found = TRUE;
			break;
		}

		previous = current;
		current = current->nextProduct;
	}

	/* Checks whether a match was found. */
	if(!found) {
		printf("%s product not found.\n\n", input);
		return;
	}

	/* Re-links the list. */
	if(previous == NULL) {
		vm->headProduct = current->nextProduct;
	}
	else {
		previous->nextProduct = current->nextProduct;
		current->nextProduct = NULL;
	}

	vm->totalProducts--;
	free(current);
	printf("%s has been removed from the vending machine.\n\n", input);
}


/****************************************************************************
* ProductNodeType: Searches the linked list for a product
* 					node with a matching product name.
*
* 		-- Returns: Matching product node or NULL.
****************************************************************************/
ProductNodeType* searchProductName(VendingMachineType *vm, char *name) {
	ProductNodeType *current;

	current = vm->headProduct;
	while(current != NULL) {
		if(strcmp(current->name, name) == 0) {
			break;
		}
		current = current->nextProduct;
	}

	return current;
}


