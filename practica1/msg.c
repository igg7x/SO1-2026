#include <stdio.h>
#include <unistd.h>

int main() {
    execl("/usr/bin/whoami", "whoami", NULL);
    return 0;
}