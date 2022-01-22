#pragma once
#include <unistd.h>
#include <termios.h>
#include <stdio.h>



class Exit{
    char getch(void);

public:
    void exit();
};