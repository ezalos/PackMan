#include <stdio.h>

void thing(void){
	printf("%s\n", __func__);
}

void other(void){
	printf("%s\n", __func__);
}

int main(void){
	printf("Hello World!\n");
	thing();
	other();
}
