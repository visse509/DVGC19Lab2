#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>

typedef struct
            {
                        int p;
                        int q;
            }pq;  

int generateKeyPair();
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
pq divide (pq inPq){
	pq res;
	res.p = (int) (inPq.p/inPq.q);
	res.q = inPq.p % inPq.q;
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
		pq temp2 = gcd(temp);
		
		res.p = temp2.q;
		res.q = temp2.p - (div.p * temp2.q);
	}
	return res;
}
pq getEVal(pq inPq){
	pq res;
	res = gcd(inPq);
	return res;
}
unsigned long long int primeCheck(unsigned long long int number){
	int i;
	fflush(0);
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
	unsigned long long int res = rand() % 30 + 5;
	while(!primeCheck(res)){
		res = rand() % 30 + 5;
	}
	return res;
}

int encryptMessage(){
	char file[256];
	printf("\n\tPlease enter filename of your file:");
	scanf("%s", &file);
	printf("\n\t%s",file);
}
int generateKeyPair(){
	srand(2);
	printf("\tGenerating key-pair... \n");
	unsigned long long int prim1 = getRandPrime(); //random prim
	unsigned long long int prim2 = getRandPrime(); //random prim
	unsigned long long int nVal = prim1*prim2;
	unsigned long long int nTheta = (prim1-1) * (prim2-1);
	unsigned long eVal = 17;
	pq inPq;
	inPq.p = prim1;
	inPq.q = prim2;
	pq res = getEVal(inPq);
	
	printf("\tgenerated prime1 is: %llu \n\t prime2 is: %llu \n\t n is: %llu \n\t nTheta is: %llu \n\t d is: %d \n\t e is: %lu ",prim1,prim2,nVal,nTheta,inPq.p,eVal);
	printf("\n\t\%d", res.p);
	printf("\n\tdone");
	return 0;
}

int squareroot() {
 double r;
 printf("\tType a number: ");
 scanf("%lf", &r);
 printf("\n\t\tThe square root of %16.8f = %f\n", r, sqrt(r));
 return 0;
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
		test.q = 5; 
		pq res = getEVal(test);
		printf("\n\tres:");
		printf("\tP: %d Q: %d \n",res.p,res.q);
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
  else
   printf("\t\tOgiltgt val!\n\n");
 } while (val != 0);
 return 0;
}
