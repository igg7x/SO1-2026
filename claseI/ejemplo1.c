#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
int main() {
    //p1
    pid_t pid1 = fork();
    // p1 y p2
    pid_t pid2 = fork();
    // p1 p2 hijop1 hijop2 
    printf("pid1 : %d pid2 :%d\n" ,pid1 , pid2);
    return 0; 
}

//         ID
//       /      \  
//   0  p1       p2 3000
//      /\       /\ 
//  0    3002       hP2  3001 