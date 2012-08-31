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
* Function readRestOfLine() is used for buffer clearing.
* Please refer to "test_fgets.c" on Blackboard:
* "Course Documents"->"Function Examples"->"Input Validation Examples"
****************************************************************************/
void readRestOfLine()
{
   int c;

   /* Read until the end of the line or end-of-file. */
   while ((c = fgetc(stdin)) != '\n' && c != EOF)
      ;

   /* Clear the error and end-of-file flags. */
   clearerr(stdin);
}


/****************************************************************************
* Initialises the system to a safe empty state.
****************************************************************************/
int systemInit(VendingMachineType *vm)
{
	int i;
	CoinType init;

	assert(vm != NULL);

	if(vm == NULL) {
		printf("Error in vending machine initialization.");
		return FAILURE;
	}

	init.qty = 0;
	init.value = 0;
	for(i=0; i<DISTINCT_COINS; i++) {
		vm->coins[i] = init;
	}

	vm->totalCoins = 0;
	vm->totalProducts = 0;
	vm->headProduct = NULL;

	return SUCCESS;
}


/****************************************************************************
* Loads data.
****************************************************************************/
int loadData(VendingMachineType *vm, char *stockFile, char *coinsFile)
{
	int coinResult, stockResult;
	FILE *coins, *stock;

	/* Opening and checking files */
	coins = fopen(coinsFile, "r");

	if(coins == NULL) {
		printf("Error. Coins file not found.");
		return FAILURE;
	}

	stock = fopen(stockFile, "r");

	if(stock == NULL) {
		printf("Error. Stock file not found.");
		return FAILURE;
	}

	/* Reading in data. */
	coinResult = readCoins(vm, coins);

	if(coinResult == FAILURE) {
		return FAILURE;
	}

	stockResult = readStock(vm, stock);

	if(stockResult == FAILURE) {
		return FAILURE;
	}

	fclose(coins);
	fclose(stock);
	return SUCCESS;
}


/****************************************************************************
* Deallocates all dynamically allocated memory.
****************************************************************************/
void systemFree(VendingMachineType *vm)
{
	ProductNodeType *next, *current;

	assert(vm != NULL);

	current = vm->headProduct;
	while(current != NULL) {
		next = current->nextProduct;
		free(current);
		current = next;
	}
}


/****************************************************************************
* Mine: Reading in an int.
****************************************************************************/
int readInt() {
	char input[BUFSIZ];
	char *ptr;
	int i, opt;
	int valid = FALSE;

	for(i=0; i<BUFSIZ; i++) {
		input[i] = 0;
	}

	fgets(input, BUFSIZ, stdin);

	if(input[0] == '\n') {
		return EXIT;
	}

	opt = (int) strtol(input, &ptr, 10);

	if(isspace(*ptr)) {
		valid = TRUE;
	}

   	if(input[strlen(input)-1] != '\n') {
   		readRestOfLine();
   	}

   	if(valid) {
   		return opt;
   	}
   	else {
   		return -2;
   	}
}


/****************************************************************************
* Mine: Reading in stock data.
****************************************************************************/
int readStock(VendingMachineType *vm, FILE *stock) {
	ProductNodeType *new, *previous;
	char readerStock[BUFSIZ];
	char *newName, *newBrand, *ptr, *token;
	int cost, i, quantity;
	int tokenCount = 0, lineCount = 0;

	for(i=0; i<BUFSIZ; i++) {
		readerStock[i] = 0;
	}

	/* Reading stock data. */
	previous = NULL;
	while(fgets(readerStock, BUFSIZ, stock) != NULL) {
		readerStock[strlen(readerStock)-1] = 0;

		/* Tokenising name & brand. */
		newName = strtok(readerStock, ",");
		if(strlen(newName) > PRODUCT_NAME_MAX) {
			printf("Error in stock file. Line %d. Product name.\n", lineCount+1);
			return FAILURE;
		}

		newBrand = strtok(NULL, ",");
		if(strlen(newBrand) > PRODUCT_BRAND_MAX) {
			printf("Error in stock file. Line %d. Brand name.\n", lineCount+1);
			return FAILURE;
		}

		/* Read in product cost. */
		cost = (int)strtol(strtok(NULL, ","), &ptr, 10);
		if(isalpha(*ptr) || cost < 0) {
			printf("Error in stock file. Line %d. Cost.\n", lineCount+1);
			return FAILURE;
		}

		/* Read in product quantity. */
		quantity = (int) strtol(strtok(NULL, ","), &ptr, 10);
		if(isalpha(*ptr) || quantity < 0) {
			printf("Error in stock file. Line %d. Quantity.\n", lineCount+1);
			return FAILURE;
		}

		/* Creating a new node. */
		new = generateNode();
		if(new == NULL) {
			return FAILURE;
		}

		/* Checking number of tokens. */
		tokenCount = 4;
		token = strtok(NULL, ",");
		while(token != NULL) {
			tokenCount++;
			token = strtok(NULL, ",");
		}

		if(tokenCount > 4) {
			printf("Error in stock file. Line %d. Too much info.\n", lineCount+1);
			return FAILURE;
		}

		/* Assigning to new node. */
		for(i=0; i<strlen(newName); i++) {
			new->name[i] = newName[i];
		}

		for(i=0; i<strlen(newBrand); i++) {
			new->brand[i] = newBrand[i];
		}
		new->price = cost;
		new->qty = quantity;

		lineCount++;
		insertNode(vm, new);
	}

	return SUCCESS;
}


/****************************************************************************
* Mine: Reading coin data.
****************************************************************************/
int readCoins(VendingMachineType *vm, FILE *coins) {
	CoinType tmpCoin;
	char readerCoins[BUFSIZ];
	char *token, *ptr;
	int i, tokenCount;
	int val = 0, quantity = 0, lineCount = 0;


	for(i=0; i<BUFSIZ; i++) {
		readerCoins[i] = 0;
	}

	/* Reading in coin data. */
	while(fgets(readerCoins, BUFSIZ, coins) != NULL) {
		readerCoins[strlen(readerCoins)-1] = 0;

		token = strtok(readerCoins,",");
		val = (int) strtol(token,&ptr,10);

		if(isalpha(*ptr)) {
			printf("Error in coin file. Line %d. Coin value.\n", lineCount+1);
			return FAILURE;
		}

		/* Checks that the coins are entered in correct order. */
		if(lineCount == 0 && val != 200) {
			printf("Error in coin file. Line %d.\n", lineCount+1);
			return FAILURE;
		}
		else if(lineCount == 1 && val != 100) {
			printf("Error in coin file. Line %d.\n", lineCount+1);
			return FAILURE;
		}
		else if(lineCount == 2 && val != 50) {
			printf("Error in coin file. Line %d.\n", lineCount+1);
			return FAILURE;
		}
		else if(lineCount == 3 && val != 20) {
			printf("Error in coin file. Line %d.\n", lineCount+1);
			return FAILURE;
		}
		else if(lineCount == 4 && val != 10) {
			printf("Error in coin file. Line %d.\n", lineCount+1);
			return FAILURE;
		}
		else if(lineCount == 5 && val != 5) {
			printf("Error in coin file. Line %d.\n", lineCount+1);
			return FAILURE;
		}
		else if(lineCount > 5) {
			printf("Error in coin file. Incorrect number of lines.");
			return FAILURE;
		}

		token = strtok(NULL,",");
		quantity = (int) strtol(token,&ptr,10);

		if(isalpha(*ptr) || quantity < 0) {
			printf("Error in coin file. Line %d. Quantity.\n", lineCount+1);
			return FAILURE;
		}

		/* Checking number of tokens. */
		tokenCount = 2;
		token = strtok(NULL, ",");
		while(token != NULL) {
			tokenCount++;
			token = strtok(NULL, ",");
		}

		if(tokenCount > 2) {
			printf("Error in coin file. Line %d. Too much info.\n", lineCount+1);
			return FAILURE;
		}

		/* Assigning temporary coin. */
		tmpCoin.value = val;
		tmpCoin.qty = quantity;

		vm->totalCoins += quantity;
		vm->coins[lineCount] = tmpCoin;
		lineCount++;
	}

	/* Checks there were enough entries. */
	if(lineCount < 6) {
		printf("Error in coin file. Incorrect number of lines.");
		return FAILURE;
	}

	return SUCCESS;
}



