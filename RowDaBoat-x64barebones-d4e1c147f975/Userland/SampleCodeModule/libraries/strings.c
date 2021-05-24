// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <strings.h>
#include <stdarg.h>
#include <syscall.h>
#include <lib.h>

#define ABS(c) ((c) >= 0 ? (c) : (c) * -1)

char getChar(){
    return syscalls(READ, 0, 0, 0, 0, 0, 0);
}

char waitCharInterruption(){
  return syscalls(CHARINTERRUPT,0,0,0,0,0,0);
}

int strlen(char * str){
    int i = 0;
    while(str[i]!= 0){
        i++;
    }
    return i;
}

int stringcmp(char * str1, char * str2){ 
    int index=0;
    while (str1[index]!=0 && str2[index]!=0 && str1[index]==str2[index])
    {
      index++;
    }
    if(str1[index]==str2[index]){
        return 0;
    }
    return str1[index] > str2[index]? 1 : -1;
}

int scanf(const char* format,...){     //scanf("%d %d %f",&num1, &num2, &num3);
    va_list args;
    int i = 0, j=0, count = 0;
    char *buff={0};
    read(buff);
    char *character;
    va_start(args,format);
    while (format && format[i]) {
      if (format[i] == '%') {
        i++;
        switch (format[i]) {
          case 'c': {
            character = va_arg(args, char*);
            *character = buff[j];
            j++;
            count++;
            break;
          }
          case 'd': {
            buff = strToInt(buff, va_arg(args,int *));
            count++;
            break;
          }
          case 's': {
            character = va_arg(args,char*);
            char temp;
            while( (temp = buff[j]) != '\0'){ 
                *character = buff[j];
                character++;
                j++;
            }
            count++;
            break;
          }
        }
      } else {
        buff[j] =format[i];
        j++;
      }
      i++;
    }
    va_end(args);
    return count;
}

void read(char *buffer){
    int index = 0;
    int c;
    while ((c = getChar()) != '\n') {
        if(c == '\b'){
            if (index != 0) {
                index--;
            }
        }
        else if(c != EOF){
            if (index <= SIZE) {
                buffer[index++] = c;
            }
        }
    }
    buffer[index] = '\0';
}

uint64_t hexastrToInt(char *str){
    uint64_t res = 0;
    char c;

    while ((c = *str++)) {
        char v = ((c & 0xF) + (c >> 6)) | (((c >> 3) & 0x8));
        res = (res << 4) | (uint64_t) v;
    }
    return res;

}

char* strToInt(char* string, int* num){
    *num = 0;
	  int signo = 1;
    char character;
    if(*string == '-'){
      character = *(string + 1);
      if (character >= '0' && character <= '9') {
        signo = -1;
        *num = (character - '0') * signo;
        string+=2;
      } else {
        return string;
      }
	  }
    character = *string;
    while (character >= '0' && character <= '9'){
        *num = (*num)*10+(character-'0')*signo;
        string++;
        character = *string;
    }
    return string;
}

int strtok(char * str, const char delim, char * array[], int arraySize){
	int index = 0;
	if(*str != delim && *str != 0){
		array[index++] = str;
    }
	while( *str != 0 ){
		if(*str == delim){
			*str = 0;
			if( *(str+1) != delim &&  (*(str+1) != 0)){
				if(index >= arraySize){
					return index;
                }
				array[index++] = str + 1;
			}
		}
		str++;
	}
	return index;
}

char* strcat(char* destination, char* source){
  char* ptr = destination + strlen(destination);
    while (*source != '\0') {
        *ptr++ = *source++;
    }
    *ptr = '\0';
    return destination;
}

char * strcopy(char * dest,char* src){
	if(src == 0){
		return 0;
	}
	int i;
	for(i=0 ; i<strlen(src) ; i++){
		dest[i] = src[i];
	}
	dest[i]=0;
	return dest;
}

int atoi(char * s){
  int res = 0;
  int i = 0;
  int sign = 1;
  if(s[i] == '-'){
    sign=-1;
    i++;
  }
  
  for ( ; s[i] != '\0'; ++i){
    res = res * 10 + s[i] - '0';
  }
  return res * sign; 
}


//itoa
// inline function to swap two numbers
static void swap(char *x, char *y) {
      char t = *x;
      *x = *y;
      *y = t;
}

// function to reverse buffer[i..j]
static char *reverse(char *buffer, int i, int j) {
      while (i < j)
            swap(&buffer[i++], &buffer[j--]);

      return buffer;
}

// Iterative function to implement itoa() function in C
char *itoa(int value, char *buffer, int base) {
      // invalid input
      if (base < 2 || base > 32)
            return buffer;

      // consider absolute value of number
      int n = ABS(value);

      int i = 0;
      while (n) {
            int r = n % base;

            if (r >= 10)
                  buffer[i++] = 65 + (r - 10);
            else
                  buffer[i++] = 48 + r;

            n = n / base;
      }

      // if number is 0
      if (i == 0)
            buffer[i++] = '0';

      // If base is 10 and value is negative, the resulting string
      // is preceded with a minus sign (-)
      // With any other base, value is always considered unsigned
      if (value < 0 && base == 10)
            buffer[i++] = '-';

      buffer[i] = '\0';  // null terminate string

      // reverse the string and return it
      return reverse(buffer, 0, i - 1);
}