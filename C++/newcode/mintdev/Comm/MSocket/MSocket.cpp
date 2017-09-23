#include <MSocket/MSocket.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstring>

int MSocket::Create(int nSocketPort, int nSocketType, bool reuse)
{
    _evAccept = new event();

    /* Initalize the event library */
    event_base *base = event_init();

    int listenFd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenFd < 0)
    {
      std::cerr << "listen failed";
    }
    int reuseaddr_on = reuse;
    if (setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr_on, sizeof(reuseaddr_on)) == -1)
    {
      std::cerr << "setsockopt failed";
    }
    struct sockaddr_in listenAddr;
    memset(&listenAddr, 0, sizeof(listenAddr));
    listenAddr.sin_family = AF_INET;
    listenAddr.sin_addr.s_addr = INADDR_ANY;
    listenAddr.sin_port = htons(nSocketPort);
    if (bind(listenFd, (struct sockaddr *)&listenAddr, sizeof(listenAddr)) < 0)
    {
      std::cerr << "bind failed";
    }
    if (listen(listenFd, 5) < 0)
    {
      std::cerr <<"listen failed";
    }
    /* Set the socket to non-blocking, this is essential in event based programming with libevent. */
    if (setNonBlock(listenFd) < 0)
    {
      std::cerr <<"failed to set server socket to non-blocking";
    }
    event_set(_evAccept, listenFd, EV_READ|EV_PERSIST, MSocket::onAccept, NULL);//event_self_cbarg());
    struct event *signalInt = evsignal_new(base, SIGINT, MSocket::signalCB, base);
    event_add(signalInt, NULL);
    event_add(_evAccept, NULL);
    event_dispatch();
}

  void MSocket::onAccept(int fd, short ev, void *arg)
  {
    MSocket *obj = static_cast<MSocket*>(arg);
    obj->handleAccept(fd, ev);
  }

  int MSocket::setNonBlock(int fd)
  {
    fcntl(fd, F_SETFL, O_NONBLOCK);
    return 0;
  }

  void MSocket::signalCB(evutil_socket_t fd, short event, void *arg)
  {
    event_base *base = static_cast<event_base*>(arg);
    event_base_loopbreak(base);
  }

  void MSocket::onRead(int fd, short ev, void *arg, Buffer &buffer)
  {
    char buf[2048];
    int len;

    len = read(fd, buf, 2048);
    buffer.setBuffer(buf);
    buffer.setLen(len);
  }


  void MSocket::handleAccept(int fd, short ev)
  {
    int clientFd;
    sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);

    /* Accept the new connection. */
    clientFd = accept(fd, (struct sockaddr *)&clientAddr, &clientLen);
    if (clientFd == -1)
    {
      std::cerr << "[SERVER] accept failed";
      return;
    }

    /* Set the client socket to non-blocking mode. */
    if (setNonBlock(clientFd) < 0)
    {
      std::cerr << "[SERVER] failed to set client socket non-blocking";
    }
  }
