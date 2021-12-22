#include <stdio.h>
#include <string.h>

#define MAX_BUF  256

unsigned char getBit(unsigned char, int);
unsigned char setBit(unsigned char, int);
unsigned char clearBit(unsigned char, int);
unsigned char processCounter(unsigned char, unsigned char);
unsigned char incrementValue(unsigned char);
unsigned char processSourceByte(unsigned char, unsigned char);

int main()
{
  	char str[8];
  	int  choice;
  	char message[MAX_BUF];
  	unsigned char keyValue = 0b10110010;
  	unsigned char counterValue = 0b00101000;
  	printf("\nYou may:\n");
  	printf("  (1) Encrypt a message \n");
  	printf("  (2) Decrypt a message \n");
  	printf("\n  what is your selection: ");
  	fgets(str, sizeof(str), stdin);
  	sscanf(str, "%d", &choice);
	unsigned char num;
  	switch (choice) {
		//encrypt a message
    	case 1:
			printf("Enter a message to encrypt: ");
			fgets(message, MAX_BUF, stdin);
			int k=0;
			num = message[k];
			while(k<strlen(message)){
				counterValue = processCounter(counterValue, keyValue);
				counterValue = incrementValue(counterValue);
				keyValue = incrementValue(keyValue);
				num = processSourceByte(num, counterValue);
				printf("%d ", num);
				k++;
				num = message[k];
			}
    		break;
    	//decrypt a message
		case 2:
			printf("Enter encrypted message byte by byte (-1 to finish):\n");
			int input[MAX_BUF];
			int l=0;
			//get bytes from user while input is valid
			scanf("%d", &input[l]);
			l++;
			while(l<MAX_BUF && input[l-1] != -1){
				scanf("%d", &input[l]);
				//check whether entered byte is valid
				if(input[l] >=-1 && input[l]<=255)
					l++;
				else
					printf("Not a byte!\n");
			}
			//size of entered bytes
			int size = l-2;
			l=0;
			while(l<=size){
				counterValue = processCounter(counterValue, keyValue);
				counterValue = incrementValue(counterValue);
				keyValue = incrementValue(keyValue);
				num = processSourceByte(input[l], counterValue);
				printf("%c", num);
				l++;
			}
      		break;
    	default:
			printf("Incorrect command!");
      		break;
  	}
  	return 0;
}



/*
  Function:  getBit
  Purpose:   retrieve value of bit at specified position
       in:   character from which a bit will be returned
       in:   position of bit to be returned
   return:   value of bit n in character c (0 or 1)
*/
unsigned char getBit(unsigned char c, int n)   
{ 
	unsigned char mask =  1 << n;
    unsigned char masked_c = c & mask;
    unsigned char thebit = masked_c >> n;
	return thebit;
}

/*
  Function:  setBit
  Purpose:   set specified bit to 1
       in:   character in which a bit will be set to 1
       in:   position of bit to be set to 1
   return:   new value of character c with bit n set to 1
*/
unsigned char setBit(unsigned char c, int n)   
{ 
	unsigned char mask = 1 << n;
	mask = mask ^ 0b00000000;
	unsigned char byte = c | mask;
	return byte;
}

/*
  Function:  clearBit
  Purpose:   set specified bit to 0
       in:   character in which a bit will be set to 0
       in:   position of bit to be set to 0
   return:   new value of character c with bit n set to 0
*/
unsigned char clearBit(unsigned char c, int n) 
{ 
	unsigned char mask = 1 << n;
	mask = mask ^ 0b11111111;
	unsigned char byte = c & mask;
	return byte;
}

/*
  Function:  processCounter
  Purpose:   process counter value using key value
       in:   counter value to be processed
       in:   key value to process the counter value
   return:   new value of counter value processed by key value
*/
unsigned char processCounter(unsigned char counterV, unsigned char key){
	unsigned char counter = counterV;
	int i, position1, position2;
	unsigned char bit;
	for(i=7; i<=14; i++){
		position1 = i;
		if(getBit(key, i%8) == 1)
			position2 = ((i%8)+1)%8;
		else
			position2 = ((i%8)+2)%8;
		bit = getBit(counter, position1) ^ getBit(counter, position2);
		if(bit == 1)
			counter = setBit(counter, (i%8));
		else
			counter = clearBit(counter, (i%8));
	}
	return counter;
}

/*
  Function:  incrementValue
  Purpose:   increment value of key value or counter value by 1
       in:   counter value or key value
   return:   new value of counter value or key value incremented by 1
*/
unsigned char incrementValue(unsigned char value){
	return value + 1;
}

/*
  Function:  processSourceByte
  Purpose:   process source byte using counter value
       in:   source byte to be processed
       in:   counter value to process the source byte
   return:   new value of source byte processed by counter value
*/
unsigned char processSourceByte(unsigned char sourceByte, unsigned char counter){
	unsigned char source = sourceByte;
	int j, bit;
	int mod = counter % 9;
	if(mod < 3){
		for(j=1; j<=7; j++){
			bit = getBit(sourceByte, j) ^ getBit(counter, j);
			if(bit == 1)
				source = setBit(source, j);
			else
				source = clearBit(source, j);
		}
	}
	else if(mod >= 3 && mod <= 5){
		for(j=2; j<=7; j++){
			bit = getBit(sourceByte, j) ^ getBit(counter, j);
			if(bit == 1)
				source = setBit(source, j);
			else
				source = clearBit(source, j);
		}
	}
	else{
		for(j=0; j<=7; j++){
			bit = getBit(sourceByte, j) ^ getBit(counter, j);
			if(bit == 1)
				source = setBit(source, j);
			else
				source = clearBit(source, j);
		}
	}
	return source;
}
