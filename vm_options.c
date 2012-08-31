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
* Menu option #1: Purchase Product
****************************************************************************/
void purchaseProduct(VendingMachineType *vm)
{
	ProductNodeType *product;
	CoinType init, changeCoins[DISTINCT_COINS];
	char input[PRODUCT_NAME_MAX + EXTRA_SPACES];
	double change;
	int addedCoins[DISTINCT_COINS];
	int valid = FALSE, enoughChange = TRUE;
	int i, j, coinVal, remaining, cost, changeTmp;

	assert(vm != NULL);

	/* Initialising. */
	for(i=0; i<PRODUCT_NAME_MAX+EXTRA_SPACES; i++) {
		input[i] = 0;
	}
	for(i=0; i<DISTINCT_COINS; i++) {
		addedCoins[i] = 0;
	}

	init.qty = 0;
	for(i=0; i<DISTINCT_COINS; i++) {
		init.value = vm->coins[i].value;
		changeCoins[i] = init;
	}

	printf("\nPurchase Products\n");
	printf("-----------------\n\n");

	/* Reading product. */
	do {
		printf("Enter a product name (1-40 characters): ");
		fgets(input, PRODUCT_NAME_MAX+EXTRA_SPACES, stdin);

		if(input[0] == '\n') {
			printf("\n");
			return;
		}

		if(strlen(input) < 1 || input[strlen(input)-1] != '\n') {
			readRestOfLine();
			continue;
		}

		valid = TRUE;
		input[strlen(input)-1] = 0;

		/* Searching for product. */
		product = searchProductName(vm, input);

		if(product == NULL) {
			printf("%s product not found.\n\n", input);
			return;
		}
		else if(product->qty == 0) {
			printf("No %s left.\n\n", input);
			return;
		}
	} while(!valid);

	cost = product->price;
	printf("The price is $%.2f.\n\n", (double)cost/100);

	/* Reading in payment. */
	do {
		printf("Enter a coin ($%.2f remaining): ", (double)cost/100);
		coinVal = readInt();

		/* Validating input. */
		if(coinVal == EXIT) {
			printf("\n");
			return;
		}

		/* Validating coin value. */
		if(coinVal == vm->coins[TWO_DOLLAR_COIN].value) {
			addedCoins[TWO_DOLLAR_COIN] += 1;
		}
		else if(coinVal == vm->coins[ONE_DOLLAR_COIN].value) {
			addedCoins[ONE_DOLLAR_COIN] += 1;
		}
		else if(coinVal == vm->coins[FIFTY_CENT_COIN].value) {
			addedCoins[FIFTY_CENT_COIN] += 1;
		}
		else if(coinVal == vm->coins[TWENTY_CENT_COIN].value) {
			addedCoins[TWENTY_CENT_COIN] += 1;
		}
		else if(coinVal == vm->coins[TEN_CENT_COIN].value) {
			addedCoins[TEN_CENT_COIN] += 1;
		}
		else if(coinVal == vm->coins[FIVE_CENT_COIN].value) {
			addedCoins[FIVE_CENT_COIN] += 1;
		}
		else {
			printf("Invalid coin.\n");
			continue;
		}

		/* Subtracts coin value from cost. */
		cost -= coinVal;
	} while(cost > 0);

	/* Determines required change. */
	changeTmp = (-1)*cost;
	change = (double) changeTmp/100;

	/* Exits if no change is required. */
	if(change == 0) {
		printf("\nThank you for purchasing a %s.\n", input);
		printf("No change required.\n\n");
		product->qty--;
		return;
	}

	/* Determining required change. */
	for(i=0; i<DISTINCT_COINS; i++) {
		changeCoins[i].qty = changeTmp/changeCoins[i].value;
		changeTmp -= changeCoins[i].value * changeCoins[i].qty;
	}

	/* Checking there are enough coins in the machine to give change. */
	for(i=0; i<DISTINCT_COINS; i++) {
		remaining = (vm->coins[i].qty) + (addedCoins[i]) - (changeCoins[i].qty);

		if(remaining < 0) {
			enoughChange = FALSE;
		}
	}

	if(!enoughChange) {
		printf("\nError. Insufficient coins to give change. Returning payment.\n\n");
		return;
	}

	/* Editing vending machine coin array. */
	for(i=0; i<DISTINCT_COINS; i++) {
		vm->coins[i].qty += addedCoins[i] - changeCoins[i].qty;
	}
	product->qty--;

	printf("\nThank you for purchasing a %s.\n", input);
	printf("Your change is: $%.2f.\n", (double)change);
	printf("Your change coins are: ");

	/* Printing change coins. */
	for(i=0; i<DISTINCT_COINS; i++) {
		if(i<2 && changeCoins[i].qty != 0) {
			/* Dollars. */
			for(j=0; j<changeCoins[i].qty; j++) {
				printf("$%.0f ", (double) changeCoins[i].value/100);
			}
		}
		else if(changeCoins[i].qty != 0) {
			 /* Cents. */
			for(j=0; j<changeCoins[i].qty; j++) {
				printf("%dc ", changeCoins[i].value);
			}
		}
	}
	printf("\n\n");
}


/****************************************************************************
* Menu option #2: Display Product
****************************************************************************/
void displayProducts(VendingMachineType *vm)
{
	ProductNodeType *current;

	assert(vm != NULL);

	printf("\n%-30s %-14s %-5s Qty\n", "Product", "Brand", "Price");
	printf("------------------------------ -------------- ----- ---\n");

	current = vm->headProduct;
	while(current != NULL) {
		printf("%-30s %-14s $%.2f %3d\n", current->name, current->brand, (double)current->price/100, current->qty);
		current = current->nextProduct;
	}
	printf("\n");
}


/****************************************************************************
* Menu option #3: Save and Exit
****************************************************************************/
void saveData(VendingMachineType *vm, char *stockFile, char *coinsFile)
{
	int i;
	FILE *coins, *stock;
	ProductNodeType *current;

	/* Opening and checking files */
	coins = fopen(coinsFile, "w");

	if(coins == NULL) {
		printf("Error. Coins file not found.");
		return;
	}

	stock = fopen(stockFile, "w");

	if(stock == NULL) {
		printf("Error. Stock file not found.");
		return;
	}

	/* Printing coin data. */
	for(i=0; i<DISTINCT_COINS; i++) {
		fprintf(coins, "%d,%d\n", vm->coins[i].value, vm->coins[i].qty);
	}

	/* Printing product data. */
	current = vm->headProduct;
	while(current != NULL) {
		fprintf(stock, "%s,%s,%d,%d\n", current->name, current->brand, current->price, current->qty);
		current = current->nextProduct;
	}

	printf("All data has been saved.\n\n");
	fclose(coins);
	fclose(stock);
}


/****************************************************************************
* Menu option #4: Add Product
****************************************************************************/
void addProduct(VendingMachineType *vm)
{
	ProductNodeType *new, *search;
	char newName[PRODUCT_NAME_MAX+EXTRA_SPACES], newBrand[PRODUCT_BRAND_MAX+EXTRA_SPACES];
	int valid = FALSE, duplicate = FALSE;
	int i, cost;

	assert(vm != NULL);

	/* Initializing. */
	for(i=0; i<PRODUCT_NAME_MAX+EXTRA_SPACES; i++) {
		newName[i] = 0;
	}
	for(i=0; i<PRODUCT_BRAND_MAX+EXTRA_SPACES; i++) {
		newBrand[i] = 0;
	}

	printf("\nAdd Product\n");
	printf("-----------\n\n");

	/* Reading in the product name. */
	do {
		duplicate = FALSE;
		printf("Name (1-40 characters): ");
		fgets(newName, PRODUCT_NAME_MAX+EXTRA_SPACES, stdin);

		if(newName[0] == '\n') {
			printf("\n");
			return;
		}

		/* Checking for valid input. */
		if(strlen(newName) < 1 || newName[strlen(newName)-1] != '\n') {
			readRestOfLine();
			continue;
		}

		valid = TRUE;
		newName[strlen(newName)-1] = 0;

		/* Searching for duplicate. */
		search = searchProductName(vm, newName);

		if(search != NULL) {
			printf("Product name already in use.\n\n");
			return;
		}
	} while(!valid);

	/* Reading in product brand. */
	valid = FALSE;
	do {
		printf("Brand (1-20 characters): ");
		fgets(newBrand, PRODUCT_BRAND_MAX+EXTRA_SPACES, stdin);

		if(newBrand[0] == '\n') {
			printf("\n");
			return;
		}

		if(strlen(newBrand) >= 1 && newBrand[strlen(newBrand)-1] == '\n') {
			newBrand[strlen(newBrand)-1] = 0;
			valid = TRUE;
		}
		else {
			readRestOfLine();
		}
	} while(!valid);

	/* Read in product price. */
	valid = FALSE;
	do {
		printf("Price (in cents): ");
		cost = readInt();

		if(cost >= 0){
			valid = TRUE;
		}
		else if(cost == EXIT) {
			printf("\n");
			return;
		}
	} while(!valid);

	/* Assigning to new node. */
	new = generateNode();
	if(new == NULL) {
		exit(EXIT_FAILURE);
	}

	for(i=0; i<strlen(newName); i++) {
		new->name[i] = newName[i];
	}

	for(i=0; i<strlen(newBrand); i++) {
		new->brand[i] = newBrand[i];
	}

	new->price = cost;
	new->qty = STOCK_DEFAULT;

	/* Adding the node. */
	insertNode(vm, new);

	printf("\nNew product added to vending machine.\n");
	printf("%s - %s - %d - %d\n\n", new->name, new->brand, new->price, new->qty);
}


/****************************************************************************
* Menu option #5: Remove Product
****************************************************************************/
void removeProduct(VendingMachineType *vm)
{
	char input[40+EXTRA_SPACES];
	int valid = FALSE;

	assert(vm != NULL);

	printf("\nDelete Product\n");
	printf("--------------\n");

	/* Reading input. */
	do {
		printf("Name (1-40 characters): ");
		fgets(input, 40+EXTRA_SPACES, stdin);

		if(input[0] == '\n') {
			printf("\n");
			return;
		}

		if(input[strlen(input)-1] != '\n') {
			readRestOfLine();
		}

		input[strlen(input) -1] = 0;

		if(strlen(input) >= 1 && strlen(input) <= 40) {
			valid = TRUE;
		}
	} while(!valid);

	deleteNode(vm, input);
}


/****************************************************************************
* Menu option #6: Display Coins
****************************************************************************/
void displayCoins(VendingMachineType *vm)
{
	char string[20];
	int i, ret;
	double coin, val;
	double total = 0;

	assert(vm != NULL);

	printf("\nCoin  Quantity Value\n");
	printf("----- -------- --------\n");

	for(i=0; i<DISTINCT_COINS; i++) {
		coin = (double)vm->coins[i].value/100;
		val = coin * vm->coins[i].qty;
		total += val;

		ret = sprintf(string, "$%.2f", val);
		printf("$%.2f %8d %8s\n", coin, vm->coins[i].qty, string);
	}

	printf("%23s\n","--------");

	ret = sprintf(string, "$%.2f", total);
	printf("Total Value: %10s\n\n",string);
}


/****************************************************************************
* Menu option #7: Re-stock Product
****************************************************************************/
void restockProducts(VendingMachineType* vm)
{
	ProductNodeType *current;

	assert(vm != NULL);

	printf("\nRestock Products\n");
	printf("----------------\n");

	current = vm->headProduct;

	while(current != NULL) {
		current->qty = STOCK_DEFAULT;
		current = current->nextProduct;
	}

	printf("\nAll products have been re-stocked to the default level.\n\n");
}


/****************************************************************************
* Menu option #8: Re-stock Coins
****************************************************************************/
void restockCoins(VendingMachineType *vm)
{
	int i;

	assert(vm != NULL);

	printf("\nRestock Coins\n");
	printf("-------------\n\n");

	vm->totalCoins = 0;

	for(i=0; i<DISTINCT_COINS; i++) {
		vm->coins[i].qty = COIN_DEFAULT;
		vm->totalCoins += COIN_DEFAULT;
	}

	printf("All coins have been re-stocked to the default level.\n\n");
}


