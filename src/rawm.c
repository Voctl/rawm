#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <ctype.h>


struct termios orig_termios;

void die(const char *p){
    perror(p);
    exit(1);
}

void disablerawm(){
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);  //we are disabling raw mode with change it to orig_term
}


void enablerawm(){ // this func just change termianl to raw mode

    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disablerawm);

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO);
    raw.c_lflag &= ~(ECHO | ICANON);
    raw.c_oflag &= ~(OPOST);

    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1; // read times
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main(){
    enablerawm();

    char c;

    while (1) {
        c = '\0'; // Reset char buffer 

        int nread = read(STDIN_FILENO, &c, 1);
        if (nread == -1) die("read");


        if (nread == 1) {
            if (iscntrl(c)) {
                // ascii code for ctrl char
                printf("%d\r\n", c); 
            } else {
                // Print ascii and char
                printf("%d ('%c')\r\n", c, c); 
            }

            if (c == 'Q') break;
        }
    }
    return 0;
}


