#include <MSocket/Buffer.h>

char* Buffer::getBuffer()
{
  return buffer;
}

void Buffer::setBuffer(char *buffer_)
{
  buffer = buffer_;
}

int Buffer::getLen()
{
  return len;
}

void Buffer::setLen(int len_)
{
  len = len_;
}
