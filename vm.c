/*****************************************************************************
* COSC1283/1284 - Programming Techniques
* Semester 2 2011 Assignment #2 - Vending Machine
* Full Name        : Alyssa Biasi
* Course Code      : COSC1284
* Program Code     : BP215
******************************************************************************/

#include "vm.h"
#include "vm_options.h"
#include "vm_utility.h"

int main(int argc, char *argv[])
{
   VendingMachineType vm;
   int finished = FALSE;
   int opt = 0;
   int initFlag, dataFlag;

   /* Checking arguments. */
   if(argc != 3) {
	   printf("Incorrect number of arguments.\n\n");
	   exit(EXIT_FAILURE);
   }

  /* Initialise the vending machine to a safe empty state. */
   initFlag = systemInit(&vm);

   dataFlag = loadData(&vm, argv[1], argv[2]);

   /* Testing to see if both systemInit(.) and loadData(.) are ok */
   if (initFlag == FAILURE || dataFlag == FAILURE) {
	   exit(EXIT_FAILURE);
   }

   printf("\n");
   do {
	   opt = 0;

	   printf("Main Menu:\n");
	   printf("1) Purchase Product\n");
	   printf("2) Display Products\n");
	   printf("3) Save and Exit\n");
	   printf("Supplier-Only Menu:\n");
	   printf("4) Add Product\n");
	   printf("5) Remove Product\n");
	   printf("6) Display Coins\n");
	   printf("7) Re-stock Products\n");
	   printf("8) Re-stock Coins\n");
	   printf("9) Abort\n");

	   do {
		   printf("\nSelect your option (1-9): ");

		   opt = readInt();

		   switch(opt) {
			   case 1:
				   purchaseProduct(&vm);
				   break;
			   case 2:
				   displayProducts(&vm);
				   break;
			   case 3:
				   saveData(&vm, argv[1], argv[2]);
				   finished = TRUE;
				   break;
			   case 4:
				   addProduct(&vm);
				   break;
			   case 5:
				   removeProduct(&vm);
				   break;
			   case 6:
				   displayCoins(&vm);
				   break;
			   case 7:
				   restockProducts(&vm);
				   break;
			   case 8:
				   restockCoins(&vm);
				   break;
			   case 9:
				   printf("All new data has been lost.\n\n");
				   finished = TRUE;
				   break;
			   default:
				   break;
		   }
	   }while(opt < 1 || opt > 9);
   } while(!finished);

   /* Deallocate all dynamically allocated memory. */
   systemFree(&vm);

   exit(EXIT_SUCCESS);
}

