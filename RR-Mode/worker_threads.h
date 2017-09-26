/*
 * worker_threads.h
 *
 *  Created on: Mar 4, 2013
 *      Author: yaowei
 */

#ifndef WORK_THREAD_H_
#define WORK_THREAD_H_

#include <vector>
#include <boost/thread.hpp>
#include "defines.h"
#include "threadSafe_container.h"

#define DATA_BUFFER_SIZE 1920*1080

typedef struct {
	int sfd;
}CONN_INFO;

typedef struct {
    pthread_t 			thread_id;        	/* unique ID of this thread */
    struct event_base 	*base;    			/* libevent handle this thread uses */
    struct event 		notify_event;  		/* listen event for notify pipe */
    int 				notify_receive_fd;  /* receiving end of notify pipe */
    int 				notify_send_fd;     /* sending end of notify pipe */
    CThreadSafeList<CONN_INFO> 	list_conn;	/* queue of new connections to handle */
} LIBEVENT_THREAD;

//文件头结构体
typedef struct SImageHeader
{
	int width;          //图片的宽度
	int height;         //图片的高度
	int serverType;     //请求的服务类型
	int dataOffset;     //图片数据头的位置
	int dataSize;       //图片大小
	int headerCheckCode;    //标志位
} SImageHeader;

typedef struct{
    int    sfd;
    char*  rBuf;
    int    rlen;
    char*  wBuf;
    int    wlen;
    bool   isVerify;
    SImageHeader header;
    LIBEVENT_THREAD *thread; 				/* Pointer to the thread object serving this connection */
}CONN;

class CWorkerThread
{
public:
	CWorkerThread();
	virtual ~CWorkerThread();

public:

	bool InitThreads(struct event_base* main_base);

	void DispatchSfdToWorker(int sfd);

private:

	bool SetupThread(LIBEVENT_THREAD* me);

	static void RegisterThreadInitialized(void);
	static void WaitForThreadRegistration(int nthreads);

	static void ReadPipeCb(int fd, short event, void* arg);
	static CONN*InitNewConn(const CONN_INFO& conn_info, LIBEVENT_THREAD* libevent_thread_ptr);

	static void CreateWorker(void *(*func)(void *), void *arg);
	static void *WorkerLibevent(void *arg);

	static void ClientTcpReadCb(struct bufferevent *bev, void *arg);
	static void ClientTcpErrorCb(struct bufferevent *bev, short event, void *arg);

	/* 为了重复利用连接内存资源 */
	static void InitFreeConns();
	static CONN *GetConnFromFreelist();
	static bool AddConnToFreelist(CONN *conn);
	static void FreeConn(CONN *conn);
	static void CloseConn(CONN *conn, struct bufferevent *bev);

private:

	std::vector<LIBEVENT_THREAD*> vec_libevent_thread_;
	int last_thread_;

	static int init_count_;
	static pthread_mutex_t 	init_lock_;
	static pthread_cond_t 	init_cond_;

	static boost::mutex mutex_;
	static std::vector<CONN*> vec_freeconn_;
	static int freetotal_;
	static int freecurr_;

private:
    static int CheckHeader(int recv_size, CONN* conn);
    static int CheckData(CONN* conn);
};

#endif /* CTHREAD_H_ */