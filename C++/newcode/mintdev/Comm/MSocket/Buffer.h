#ifndef BUFFER_H
#define BUFFER_H

#include <cstdio>

class Buffer
{
  public:
    char* getBuffer();
    void  setBuffer(char*);
    
    int getLen();
    void setLen(int len);
  private:
    char *buffer;
    int len;
};
#endif
