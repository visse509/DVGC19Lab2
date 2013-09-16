#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>

typedef struct
            {
		    signed long long int p;
		    signed long long int q;
            }pq; 
	   
typedef struct
            {
		    unsigned long long int publicE;
		    unsigned long long int publicN;
		    unsigned long long int privateD;
		    unsigned long long int privateP;
		    unsigned long long int privateQ;
            }keyParts;
	    
int generateKeyPair();
static keyParts myKey;
static keyParts otherKey;
unsigned long long int intPow(unsigned long long int base,unsigned long long int exp){
	int i;
	unsigned long long int x1 = 1,x2 = base;
	for(i = (8*sizeof(exp))-2; i >= 0; i--){
		int nthBit = (exp & ( 1 << i )) >> i;
		if(nthBit == 0){
			x2 = (x1*x2);
			x1 = (x1*x1);
		}else{
			x1 = (x1*x2);
			x2 = (x2*x2);
		}
	}
	return x1;
}
unsigned long long int modPow(unsigned long long int base,unsigned long long int exp,unsigned long long int mod){
	int i;
	unsigned long long int x1 = 1,x2 = base;
	for(i = (8*sizeof(exp))-2; i >= 0; i--){
		int nthBit = (exp & ( 1 << i )) >> i;
		if(nthBit == 0){
			x2 = (x1*x2) % mod; 
			x1 = (x1*x1) % mod;
		}else{
			x1 = (x1*x2) % mod;
			x2 = (x2*x2) % mod;
		}
	}
	return x1;
}

void decimalToBin(unsigned long long int decimal, char resBin[65]){
  unsigned long long int qout = decimal;
  int i = 0;
  while(qout !=0){
      resBin[i++] = (char)(((unsigned long long int)'0')+(qout % 2));
      qout = qout / (unsigned long long int)2;
      
    }
    int j;
    for(j = i; j < 64;j++){
	resBin[j] = '0';
    }
    resBin[64] = '\0';
}
unsigned long long int binToDecimal(char bin[65]){
  int i;
  unsigned long long int res = 0;
  for(i = 0; i < 64; i++){
      if(bin[i] == '1'){
	res += intPow(2.0,((unsigned long long int)i));
      }
  }
  return res;
}

void readFile(FILE *fp,unsigned long long int *res){
  char *line = NULL;
  size_t length = 0;
  int i = 0;
  ssize_t read = 0;
  while (read = (getline(&line, &length, fp)) != -1) {
 //   printf("\n\tline: %s",line);
    res[i] = binToDecimal(line);
   // printf("\tconverted:%llu",res[i]);
    i++;
  }
}

pq divide (pq inPq){
	pq res;
	res.p = (int) (inPq.p/inPq.q);
	res.q = inPq.p % inPq.q;
	//printf("\n\tGCD:\t %llu, \t %llu",res.p,res.q);
	return res;
}
pq gcd(pq inPq){
	pq res;
	res.p = inPq.p;
	res.q = inPq.q;
	pq div;
	if(inPq.q == 0){
		res.p = 1;
		res.q = 0;
		return res; 
	}else{
		pq temp;
		pq div = divide(inPq);
		temp.p = inPq.q;
		temp.q = div.q;
		//printf("\n\tGCD:\t %llu, \t %llu",div.p,div.q);
		pq temp2 = gcd(temp);
		//printf("\n\tGCD:\t %lld, \t %lld",temp2.p,(signed long long int)-3);
		res.p = temp2.q;
		res.q = temp2.p - (div.p * temp2.q);
		
		
	}
	return res;
}
pq getDVal(pq inPq){
	pq res;
	res = gcd(inPq);
	if(res.p < 0){
	  res.p = inPq.q + res.p; 
	}
	return res;
}
unsigned long long int primeCheck(unsigned long long int number){
	int i;
	for(i = 0; i < 20;i++){
		int random = (rand() % number-1) + 1;
		unsigned long long int temp = modPow(random,(number-1),number);
		if((temp != 1) && (random > 0)){
			
			return 0;
		}
	}
	return number;
}
unsigned long long int getRandPrime(){
	unsigned long long int res = (rand() % 10000) + 17;
	while(!primeCheck(res)){
		res = (rand() % 10000) + 17;
	}
	return res;
}

int encryptMessage(){
	char file[256];
	printf("\n\tPlease enter filename of your file:");
	scanf("%s", file);
	printf("\n\t%s",file);
}
int generateKeyPair(){
	srand(2);
	printf("\tGenerating key-pair... \n");
	myKey.privateP = getRandPrime(); //random prim
	myKey.privateQ = getRandPrime(); //random prim
	myKey.publicN = myKey.privateP*myKey.privateQ;
	unsigned long long int nTheta = (myKey.privateP-1) * (myKey.privateQ-1);
	myKey.publicE = 17;
	pq inPq;
	inPq.p = myKey.publicE;
	inPq.q = nTheta;
	pq res = getDVal(inPq);
	myKey.privateD = res.p;
	printf("\tgenerated prime1 is: %llu \n\t prime2 is: %llu \n\t n is: %llu \n\t nTheta is: %llu \n\t d is: %lld \n\t e is: %llu ",myKey.privateP,myKey.privateQ,myKey.publicN,nTheta,myKey.privateD,myKey.publicE);
	//printf("\n\t\%llu", myKey.publicD);
	printf("\n\tdone");
	
	/* test */
	//myKey.privateP = 61; //random prim
	//myKey.privateQ = 53; //random prim
	//myKey.publicN = 3233;
	//myKey.privateD = 2753;
	//myKey.publicE = 17;
	return 0;
}
void decrypt(unsigned long long int inText[8],char text[8]){
  int i;
  int messageSize = 3;
  int charSize = sizeof(char);
  unsigned long long int messageParts[messageSize];
  for(i = 0; i < messageSize; i++){
    //unsigned long long int temp = inText[i];
    messageParts[i] = modPow(inText[i],myKey.privateD,myKey.publicN);
    printf("\n\tDebug:\t%llu,\t%llu",myKey.privateD,myKey.publicN);
    printf("\n\tDecrypted part:\t%llu, original:\t%llu",messageParts[i],inText[i]);
  }

  for(i = 0; i < messageSize;i++){
    text[i] = messageParts[i];
  }
}
void encrypt(char text[8],unsigned long long int resText[8]){
  int i;
  int messageSize = 8;
  int charSize = sizeof(char);
  unsigned long long int messageParts[messageSize];
  for(i = 0; i < messageSize; i++){
    unsigned long long int temp = text[i];
    if(text[i] == 0){
      messageParts[i] = 0;
      break;
    }else{
      messageParts[i] = modPow((unsigned long long int)temp,myKey.publicE,myKey.publicN);
       printf("\n\tEncrypted part:\t%llu, original:\t%llu",messageParts[i],temp);
    }
  }
  for(i = 0; i < messageSize;i++){
    resText[i] = messageParts[i];
  }
}


int encryptText(){
  char inText[8];
  unsigned long long int resText[8];
  FILE *fp = fopen("output","wr");
  printf("\n\tEnter text to encrypt: ");
  scanf("%s", inText,resText);
  encrypt(inText,resText);
  int i;
  for(i = 0; i < 8; i++){
    if(resText[i] == 0){
      break;
    }
    char resBin[65];
    decimalToBin(resText[i],resBin);
    fprintf(fp,"%s\n",resBin);
  }
  
  fclose(fp);
  printf("\n\tEncrypted text:\t%s\n",inText);
}
int decryptText(){
  char resText[8];
  unsigned long long int inText[8];
  FILE *fp = fopen("output","r");
  readFile(fp,inText);
  decrypt(inText,resText);
  printf("\n\tDecrypted text:\t%s\n",resText);
}
int readPublicKey(){
 // unsigned long long int inN,inE;
  printf("\n\tEnter key n-val: ");
  scanf("%llu", &(otherKey.publicN));
  printf("\n\tEnter key e-val: ");
  scanf("%llu", &(otherKey.publicE));
}
int quit() {
 printf("\n\tBye bye\n\n");
 return 0;
}

int printMeny() {
 printf("\n\tmake your choice:\n\n");
 printf("\t0: Quit\n");
 printf("\t1: Generate key-pair\n");
 printf("\t2: gcd test\n");
 printf("\t3: modPow test\n");
 printf("\t4: generate Random prime number test\n");
 printf("\t5: encrypt message\n");
 printf("\t6: print my public key\n");
 printf("\t7: read other public key\n");
 printf("\t8: encrypt text\n");
 printf("\t9: decrypt text\n");
 return 0;
}

int main() {
 int val;

 do {
  printMeny();
  scanf("%d", &val);
  if (val == 0)
	quit();
	else if (val == 1)
		generateKeyPair();
	else if (val == 2){
		pq test;
		test.p = 17;
		test.q = 300; 
		pq res = getDVal(test);
		printf("\n\tres:");
		printf("\tP: %lld Q: %lld \n",res.p,res.q);
	}
	else if (val == 3){
		printf("\n\t res: \t %llu",modPow(2,2,100000));
	}
	else if(val == 4){
		printf("\n\t res: \t %llu",getRandPrime());
	}
	else if(val == 5){
		encryptMessage();
	}
	else if(val == 6){
		printf("\n\t Public keys: \n\t n-key:\t%llu \n\t e-key:\t%llu",myKey.publicN,myKey.publicE);
	}
	else if(val == 7){
		readPublicKey();
	}
	else if(val == 8){
		encryptText();
	}
	else if(val == 9){
		decryptText();
	}
  else
   printf("\t\tOgiltgt val!\n\n");
 } while (val != 0);
 return 0;
}
