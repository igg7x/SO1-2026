#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>

// v.0.0.2.1
struct sigaction * handler_setter(void *handler){
    //aca deberiamos hacer un par de cosas mas.
    struct sigaction *sa = malloc(sizeof(struct sigaction));
    sa->sa_handler = handler;
    sa->flags = 0;
    return sa;
}
void handler(int s) {
    // Usamos write en lugar de printf porque es "async-signal-safe"
    const char msg[] = "Señal 11 (SIGSEGV) recibida correctamente por el hijo\n";
    write(STDOUT_FILENO, msg, sizeof(msg) - 1);
}
int main() {
   
    struct sigaction *sa = handler_setter(handler);    
//     struct sigaction sa;
//     sa.sa_handler = handler;  // Asignamos el manejador
//    // sigemptyset(&sa.sa_mask); // Inicializamos la máscara de señales vacía
//     sa.sa_flags = 0;  // no usamos flags por el momento  
    pid_t pid = fork();
    if (pid == 0) {
        // Aquí está el cambio clave: usamos sigaction() en lugar de signal()
        if (sigaction(SIGSEGV, sa, NULL) == -1) {
            perror("Error en sigaction");
            exit(1);
        }
        printf("Hijo esperando señal...\n");
        pause(); 
        printf("Hijo terminando ejecución limpia.\n");
    } else if (pid > 0) {
        sleep(1); // Damos tiempo al hijo para configurar su manejador
        printf("Padre enviando señal SIGSEGV al hijo %d\n", pid);
        kill(pid, SIGSEGV);
        wait(NULL); // Esperamos a que el hijo termine
    }
    free(sa);
    return 0;
}


// version 0.0.1
// void handler(int s){
//     printf("Señal recibida por el hijo\n");
// }
// int main(){
//     pid_t pid = fork(); 
//     if (pid == 0)
//     {   
//         signal(11,handler); // registremos el handle de la señal que vamos  a manejar
//         pause(); // pausamos el proceso hasta que recibimos la señal
//     }else if(pid < 0){
//         printf("Err");
//     }else{
//         sleep(1); // esperamos a q se registre el manejador sino mandariamos la señal sin handler
//         kill(pid, 11); // enviamos la señal 11 al prooceso con el pid del hijo
//     }
//     return 0 ; 
// }