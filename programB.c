#include<stdio.h>



int main() {
    printf("\n[Program B]: Hello world\n");
    fflush(stdout);
    char str[100000];
    while(fgets(str, sizeof(str), stdin) != NULL) {
        // printf("\n[Program B]: Enter text: ");
        // fflush(stdout);
        // Read a line from stdin        
        printf("[Program B got input]: %s", str);
        fflush(stdout);
    }
}