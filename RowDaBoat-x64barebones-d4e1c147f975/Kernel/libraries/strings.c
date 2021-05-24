// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <strings.h>
#include <stdint.h>


int strleng(char * str){
    int i = 0;
    while(str[i]!= 0){
        i++;
    }
    return i;
}

uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base)
{
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}

uint32_t uintToBaseHexa(uint64_t value, char * buffer, uint32_t base, uint32_t length)
{
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= base);

	while (digits < length) {
            *p++ = '0';
            digits++;
    }

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}

int log2(uint32_t n) {
      if (n == 0) {
            return -1;
      }
      int logValue = -1;
      while (n) {
            logValue++;
            n >>= 1;
      }
      return logValue;
}

char * strcopy(char * dest,char* src){
	if(src == 0){
		return 0;
	}
	int i;
	for(i=0 ; i<strleng(src) ; i++){
		dest[i] = src[i];
	}
	dest[i]=0;
	return dest;
}

int strcomp(char * firstString,char* secondString){
	if(firstString == 0 || secondString == 0){
		return 1;
	}
	int i = 0;
	while(firstString[i] != 0 && secondString[i] != 0 && (firstString[i] == secondString[i])){
		i++;
	}
	if(firstString[i]==secondString[i]){
		return 0;
	}
	else if(firstString[i] > secondString[i]){
		return 1;
	}
	return -1;
}

char * strcopycat(char * dest,char* src, char* cat){
	if(src == 0 || cat == 0){
		return 0;
	}
	int i,j;
	for(i=0 ; i<strleng(src) ; i++){
		dest[i] = src[i];
	}
	for(j=0; j<strleng(cat); j++){
		dest[i+j] = cat[j];
	}
	dest[i+j]=0;
	return dest;
}