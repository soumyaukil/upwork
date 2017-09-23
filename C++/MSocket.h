#ifndef MSOCKET_H
#define MSOCKET_H

#include <vector>
#include <stdint.h>
#include <atomic>
#include <mutex>
#include <MSocket/Buffer.h>
#include <event.h>
#include <event2/event_compat.h>
#include <event2/event.h>
#include <event2/event_struct.h>

enum enuMOVState{
        stVoid=0,
        stAccepting=1,
        stReading=2,
        stWritting=3,
        stConnecting=4
};


class MSocket
{
public:
        static long mMaxThreads;
        static long mCurrThreads;
        static std::vector<int> mSocklst;
        static long mMaintain;
        static bool mMaintainLoop;

private:
        static void onAccept(int fd, short ev, void *arg);
        static void signalCB(evutil_socket_t fd, short event, void *arg);
        void handleAccept(int fd, short ev);
        static void onRead(int fd, short ev, void *arg, Buffer &buffer);
        int setNonBlock(int fd);

        static int WorkerProc(void* pParam );
        static int MaintainProc(void* pParam);
        static int ConnectingThread(void* pParam);

        long mstate;
        event* _evAccept;
        long mIssuedIO; // how many io we already issued.
        long mPrcessThreadNum; //this is the number that how many thread runs at the same time.
        long mAsyncflag;       //which will check the flag;
        void * mpAcceptBuff;
        std::atomic_flag locked = ATOMIC_FLAG_INIT;
        long mReference;// how many reference to this client made.

        uint32_t mLastCloseTicker;
        long mIsConnecting; //if this number is not zero, means it is in connecting status;

public:
        inline long RefInc() {return ++mReference;}
        inline long RefDec() {return --mReference;};
        inline long GetIssuedIONum() { return mIssuedIO;};
        inline long GetReferenceNum() { return mReference;};
        inline long GetState() { return mstate;};
        inline uint32_t GetPeerAddr() {};

protected:
        Buffer rov; //read ovl
        Buffer wov; //write ovl
        uint16_t mLastReadTick;
        uint16_t mLastWriteTick;
        int mInAddlen;
        int mOutAddlen;
        bool m_bNoBlock;


protected:
        std::mutex mCritical;
protected:
        int Create(int nSocketPort, int nSocketType, bool reuse);
        MSocket();
        virtual ~MSocket();

};

#endif
