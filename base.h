#ifndef BASE_H
#define BASE_H
#include <cstring>

// Input
    #include <stdio.h>
    #include <unistd.h>
    #include <stdlib.h>
    #include <termios.h>
    #include <unistd.h>
    #include <fcntl.h>

inline int kbhit(void)
    {
      struct termios oldt, newt;
      int ch;
      int oldf;
      tcgetattr(STDIN_FILENO, &oldt);
      newt = oldt;
      newt.c_lflag &= ~(ICANON | ECHO);
      tcsetattr(STDIN_FILENO, TCSANOW, &newt);
      oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
      fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
      ch = getchar();
      tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
      fcntl(STDIN_FILENO, F_SETFL, oldf);
      if(ch != EOF)
      {
        //DO whatever you like to do with this charecter ..
        ungetc(ch, stdin);
        return 1;
      }
      return 0;
    }

struct Input {
    bool keys[256];
    void getUserInput() {
        memset(keys, 0, 256);
        if (kbhit()) {
            char buff = getchar();
            keys[(int)buff] = true;
        }
    }
};

//-----------------------------------------------

struct Vec {
    float x, y;
    Vec(float x, float y)
        : x(x), y(y) {}
    
    Vec operator+(const Vec& other) {
        return Vec(x+other.x, y+other.y);
    }

    void set(float x, float y) {
        this->x = x;
        this->y = y;
    }
};

#endif

/*int round(float num) { //numero a redondear
    bool positive = true;   //signo
    if(num < 0){            //si el numero es negativo  
        positive = false;   //guardamos su signo
        num = -num;         //y lo ponemos en positivo
    }

    int ent = (int)num;     //cogemos las cifras enteras del numero 10.5 --> 10
    float buff = num - ent; //restamos el numero con las cifras enteras

    if (buff < 0.5)         //si el resultado es menor que 0.5 significa que no lo redondeamos
        return (positive ? ent:(-ent)); //devolvemos su signo y el valor 
    else 
        return (positive ? (ent+1):(-(ent+1))); //redondeamos el valor y devolvemos su signo
}*/
