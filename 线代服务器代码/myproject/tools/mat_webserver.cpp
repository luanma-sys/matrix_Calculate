#include "_public.h"
//  #include "_public.cpp"
// #include <mysql/mysql.h>
#include "_mysql.h"
//  #include "_mysql.cpp"
#include "mymatrix.h"
// #include "mymatrix.cpp"



CLogFile   logfile;    // 服务程序的运行日志。
CTcpServer TcpServer;  // 创建服务端对象。

void EXIT(int sig);    // 进程的退出函数。

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;    // 初始化互斥锁。
pthread_cond_t  cond = PTHREAD_COND_INITIALIZER;     // 初始化条件变量。
vector<int> sockqueue;                              // 客户端socket的队列。

// 线程信息的结构体。
struct st_pthinfo
{
	pthread_t pthid;       // 线程id。
	time_t    atime;       // 最近一次活动的时间。
};

pthread_spinlock_t spin;         // 用于锁定vthid的自旋锁。
vector<struct st_pthinfo> vthid; // 存放全部线程信息的容器。
void* thmain(void* arg);         // 工作线程主函数。

pthread_t checkpthid;
void* checkthmain(void* arg);    // 监控线程主函数。

void thcleanup(void* arg);       // 线程清理函数。

pthread_t checkpoolid;
void* checkpool(void* arg);      // 检查数据库连接池的线程函数。

// 主程序参数的结构体。
struct st_arg
{
	char connstr[101];  // 数据库的连接参数。
	char charset[51];   // 数据库的字符集。
	int  port;          // web服务监听的端口。
} starg;

// 显示程序的帮助
void _help(char* argv[]);

// 把xml解析到参数starg结构中
bool _xmltoarg(char* strxmlbuffer);

// 读取客户端的报文。
int ReadT(const int sockfd, char* buffer, const int size, const int itimeout);

// 从GET请求中获取参数。
bool getvalue(const char* buffer, const char* name, char* value, const int len);

// 判断URL中用户名和密码，如果不正确，返回认证失败的响应报文。
bool Login(connection* conn, const char* buffer, const int sockfd);

// 判断用户是否有调用接口的权限，如果没有，返回没有权限的响应报文。
bool CheckPerm(connection* conn, const char* buffer, const int sockfd);

// 执行接口的sql语句，把数据返回给客户端。
bool ExecSQL(connection* conn, const char* buffer, const int sockfd);

// 执行接口中的矩阵计算
bool ExecMatrix(connection* conn, const char* buffer, const int sockfd);

// 数据库连接池类。
class connpool
{
private:
	struct st_conn
	{
		connection conn;            // 数据库连接。
		pthread_mutex_t mutex;      // 用于数据库连接的互斥锁。
		time_t atime;               // 数据库连接上次使用的时间，如果未连接数据库则取值0。
	}*m_conns;                    // 数据库连接池。

	int  m_maxconns;              // 数据库连接池的最大值。
	int  m_timeout;               // 数据库连接超时时间，单位：秒。
	char m_connstr[101];          // 数据库连接参数：用户名/密码@连接名
	char m_charset[101];          // 数据库的字符集。
public:
	connpool();      // 构造函数。
	~connpool();      // 析构函数。

	 // 初始化数据库连接池，初始化锁，如果数据库连接参数有问题，返回false。
	bool init(const char* connstr, const char* charset, int maxconns, int timeout);
	// 断开数据库连接，销毁锁，释放数据库连接池的内存空间。
	void destroy();

	// 从数据库连接池中获取一个空闲的连接，成功返回数据库连接的地址。
	// 如果连接池已用完或连接数据库失败，返回空。
	connection* get();
	// 归还数据库连接。
	bool free(connection* conn);

	// 检查数据库连接池，断开空闲的连接，在服务程序中，用一个专用的子线程调用此函数。
	void checkpool();
};

connpool mysqlconnpool;  // 声明数据库连接池对象。

int main(int argc, char* argv[])
{
	if (argc != 3) { _help(argv); return -1; }

	CloseIOAndSignal();	signal(SIGINT, EXIT);	signal(SIGTERM, EXIT);
	if (logfile.Open(argv[1], "a+") == false) { printf("logfile.Open(%s) failed.\n", argv[1]); return -1; }

	// 把xml解析到参数starg结构中
	if (_xmltoarg(argv[2]) == false) return -1;

	// 服务端初始化。
	if (TcpServer.InitServer(starg.port) == false)
	{
		logfile.Write("TcpServer.InitServer(%d) failed.\n", starg.port); return -1;
	}

	//初始化数据库连接池
	if (mysqlconnpool.init(starg.connstr, starg.charset, 10, 50) == false)
	{
		logfile.Write("mysqlconnpoll.init() failed\n");	return -1;
	}
	else
	{
		if (pthread_create(&checkpoolid, NULL, checkpool, 0) != 0)
		{
			logfile.Write("pthread_create() failed\n");	return -1;
		}
	}

	//启动十个工作线程
	for (int ii = 0; ii < 10; ii++)
	{
		struct st_pthinfo stpthinfo;
		if (pthread_create(&stpthinfo.pthid, NULL, thmain, (void*)(long)ii) != 0)
		{
			logfile.Write("pthread_create() failed\n");	return -1;
		}

		stpthinfo.atime = time(0);
		vthid.push_back(stpthinfo);
	}

	//创建线程监控器
	if (pthread_create(&checkpthid, NULL, checkthmain, (void*)0) != 0)
	{
		logfile.Write("pthread_create() failed\n");	return -1;
	}

	pthread_spin_init(&spin, 0);

	while (true)
	{
		if (TcpServer.Accept() == false)
		{
			logfile.Write("TcpServer.Accept() failed\n");	return -1;
		}
		logfile.Write("client(%s) connected\n", TcpServer.GetIP());
		pthread_mutex_lock(&mutex);
		sockqueue.push_back(TcpServer.m_connfd);
		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&cond);
	}
}

void* thmain(void* arg)
{
	int pthnum = (int)(long)arg;//线程编号
	pthread_cleanup_push(thcleanup,arg);//线程清理函数
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);	//线程取消方式设置成立即取消
	pthread_detach(pthread_self());	//把线程分离

	int connfd;				//客户端socket
	char strrecvbuf[1024];	//接收客户端请求报文的buffer
	char strsendbuf[1024];	//向客户端发送回应报文的buffer

	while (true)
	{
		pthread_mutex_lock(&mutex);	//给缓存队列加锁

		//如果缓存队列为空，则等待新任务
		while (sockqueue.size() == 0)
		{
			struct timeval now;
			gettimeofday(&now, NULL);	//获取当前时间
			now.tv_sec = now.tv_sec + 20;	//获取20秒后的时间
			pthread_cond_timedwait(&cond, &mutex, (struct timespec*)&now);	//等待条件被触发
			vthid[pthnum].atime = time(0);	//更新当前线程的活动时间
		}
		//从缓存队列中取出第一条记录，然后删除该记录
		connfd = sockqueue[0];
		sockqueue.erase(sockqueue.begin());

		pthread_mutex_unlock(&mutex);	//解锁缓存队列

		//以下是业务处理代码
		logfile.Write("phid=%lu(num=%d),connfd=%d\n", pthread_self(), pthnum, connfd);

		//读取客户端的报文,如果超时或者失败,关闭客户端的socket,继续回到循环
		memset(strrecvbuf, 0, sizeof(strrecvbuf));
		if (ReadT(connfd, strrecvbuf, sizeof(strrecvbuf), 3) <= 0) { close(connfd); continue; }

		//只处理get请求
		if (strncmp(strrecvbuf, "GET", 3) != 0) { close(connfd); continue; }
		
		logfile.Write("%s\n", strrecvbuf);
		connection* conn = mysqlconnpool.get();//获取数据库连接
		if (conn == 0)		//如果获取失败，则向客户端返回错误信息，关闭客户端的socket，进行下一次循环
		{
			memset(strsendbuf, 0, sizeof(strsendbuf));
			sprintf(strsendbuf, \
				"HTTP/1.1 200 OK\r\n"\
				"Server: webserver\r\n"\
				"Content-Type: text/html;charset=utf-8\r\n\r\n"\
				"<retcode>-1</retcode><message>internal error</message>");
			Writen(connfd, strsendbuf, strlen(strsendbuf));
			close(connfd); continue;
		}
		// 判断URL中用户名和密码，如果不正确，返回认证失败的响应报文，关闭客户端的socket，继续回到循环。
		//if (Login(conn, strrecvbuf, connfd) == false) { mysqlconnpool.free(conn); close(connfd); continue; }

		// 判断用户是否有调用接口的权限，如果没有，返回没有权限的响应报文，关闭客户端的socket，继续回到循环。
		//if (CheckPerm(conn, strrecvbuf, connfd) == false) { mysqlconnpool.free(conn); close(connfd); continue; }

		// 先把响应报文头部发送给客户端。
		memset(strsendbuf, 0, sizeof(strsendbuf));
		sprintf(strsendbuf, \
			"HTTP/1.1 200 OK\r\n"\
			"Server: webserver\r\n"\
			"Content-Type: text/html;charset=utf-8\r\n\r\n");
		Writen(connfd, strsendbuf, strlen(strsendbuf));

		//	根据用户的操作决定是进行代数计算还是调用数据库sql
		//-----------------------------------------------------------------------
		char interface[31];
		getvalue(strrecvbuf, "interface", interface, 30);
		if (strcmp(interface, "matrix") == 0)
		{
			ExecMatrix(conn, strrecvbuf, connfd);
		}
		else {
			char strsendbuf[1024];
			memset(strsendbuf, 0, sizeof(strsendbuf));
			sprintf(strsendbuf, \
				"<retcode>-1</retcode><message>no that internal</message>");
			Writen(connfd, strsendbuf, strlen(strsendbuf));
			close(connfd);
		}

		// 再执行接口的sql语句，把数据返回给客户端。
		//if (ExecSQL(conn, strrecvbuf, connfd) == false) { mysqlconnpool.free(conn); close(connfd); continue; }
		//Writen(connfd, "<data>winwin</data>\n", strlen("<data>winwin</data>\n"));
		
		mysqlconnpool.free(conn);;

	}

	pthread_cleanup_pop(1);	//把线程清理函数出栈
}

// 进程的退出函数。
void EXIT(int sig)
{
	// 以下代码是为了防止信号处理函数在执行的过程中被信号中断。
	signal(SIGINT, SIG_IGN); signal(SIGTERM, SIG_IGN);

	logfile.Write("进程退出，sig=%d。\n", sig);

	TcpServer.CloseListen();    // 关闭监听的socket。

	// 取消全部的线程。
	pthread_spin_lock(&spin);
	for (int ii = 0; ii < vthid.size(); ii++)
	{
		pthread_cancel(vthid[ii].pthid);
	}
	pthread_spin_unlock(&spin);

	sleep(1);        // 让子线程有足够的时间退出。

	pthread_cancel(checkpthid);   // 取消监控线程。
	pthread_cancel(checkpoolid);  // 取消检查数据库连接池的线程。

	pthread_spin_destroy(&spin);
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);

	exit(0);
}


void thcleanup(void* arg)     // 线程清理函数。
{
	pthread_mutex_unlock(&mutex);

	// 把本线程的结构体从存放线程结构体的容器中删除。
	pthread_spin_lock(&spin);
	for (int ii = 0; ii < vthid.size(); ii++)
	{
		if (pthread_equal(pthread_self(), vthid[ii].pthid)) { vthid.erase(vthid.begin() + ii); break; }
	}
	pthread_spin_unlock(&spin);

	logfile.Write("线程%d(%lu)退出。\n", (int)(long)arg, pthread_self());
}

// 显示程序的帮助
void _help(char* argv[])
{
	printf("Using:/project/tools1/bin/webserver logfilename xmlbuffer\n\n");

	printf("Sample:/project/tools1/bin/procctl 10 /project/tools1/bin/webserver /log/idc/webserver.log \"<connstr>127.0.0.1,root,ucp4WaC0tE_s,3306</connstr><charset>utf8</charset><port>8080</port>\"\n\n");

	printf("本程序是数据总线的服务端程序，为数据中心提供http协议的数据访问接口。\n");
	printf("logfilename 本程序运行的日志文件。\n");
	printf("xmlbuffer   本程序运行的参数，用xml表示，具体如下：\n\n");

	printf("connstr     数据库的连接参数，格式：username/password@tnsname。\n");
	printf("charset     数据库的字符集，这个参数要与数据源数据库保持一致，否则会出现中文乱码的情况。\n");
	printf("port        web服务监听的端口。\n\n");
}

// 把xml解析到参数starg结构中
bool _xmltoarg(char* strxmlbuffer)
{
	memset(&starg, 0, sizeof(struct st_arg));

	GetXMLBuffer(strxmlbuffer, "connstr", starg.connstr, 100);
	if (strlen(starg.connstr) == 0) { logfile.Write("connstr is null.\n"); return false; }

	GetXMLBuffer(strxmlbuffer, "charset", starg.charset, 50);
	if (strlen(starg.charset) == 0) { logfile.Write("charset is null.\n"); return false; }

	GetXMLBuffer(strxmlbuffer, "port", &starg.port);
	if (starg.port == 0) { logfile.Write("port is null.\n"); return false; }

	return true;
}

// 读取客户端的报文。
int ReadT(const int sockfd, char* buffer, const int size, const int itimeout)
{
	if (itimeout > 0)
	{
		struct pollfd fds;
		fds.fd = sockfd;
		fds.events = POLLIN;
		int iret;
		if ((iret = poll(&fds, 1, itimeout * 1000)) <= 0) return iret;
	}

	return recv(sockfd, buffer, size, 0);
}

// 判断URL中用户名和密码，如果不正确，返回认证失败的响应报文。
bool Login(connection* conn, const char* buffer, const int sockfd)
{
	char username[31], passwd[31];
	getvalue(buffer, "username", username, 30);
	getvalue(buffer, "passwd", passwd, 30);

	//接下来是登录语句
}

// 从GET请求中获取参数。
bool getvalue(const char* buffer, const char* name, char* value, const int len)
{
	value[0] = 0;

	char* start, * end;
	start = end = 0;

	start = strstr((char*)buffer, (char*)name);
	if (start == 0) return false;

	end = strstr(start, "&");
	if (end == 0) end = strstr(start, " ");

	if (end == 0) return false;

	int ilen = end - (start + strlen(name) + 1);
	if (ilen > len) ilen = len;

	strncpy(value, start + strlen(name) + 1, ilen);

	value[ilen] = 0;

	return true;
}

// 判断用户是否有调用接口的权限，如果没有，返回没有权限的响应报文。 
bool CheckPerm(connection* conn, const char* buffer, const int sockfd);

// 执行接口的sql语句，把数据返回给客户端。
bool ExecSQL(connection* conn, const char* buffer, const int sockfd);

bool ExecMatrix(connection* conn, const char* buffer, const int sockfd)
{
	char intername[64];	memset(intername, 0, sizeof(intername));
	char value[10240];	memset(intername, 0, sizeof(value));
	char eigen_count[64];	memset(intername, 0, sizeof(eigen_count));
	getvalue(buffer, "intername", intername, 64);
	getvalue(buffer, "value", value, 10240);
	getvalue(buffer, "eigen_count",eigen_count ,64);
	if (strlen(eigen_count) == 0)	strcpy(eigen_count, "1");

	matrix re_matrix;
	if (re_matrix.getmatrix(value) == false)
	{
		char strsendbuf[1024];
		memset(strsendbuf, 0, sizeof(strsendbuf));
		sprintf(strsendbuf, \
		"<retcode>-1</retcode><message>matrix error</message>");
		Writen(sockfd, strsendbuf, strlen(strsendbuf));
		close(sockfd);
		logfile.Write("matrix_getmatrix() failed.\n");

		return false;
	}

	const char* temp = "no that function";
	char number[1024];	memset(number, 0, 1024);
	if (strcmp(intername, "matrix_multip") == 0) { temp = re_matrix.matrix_multip(); }
	if (strcmp(intername, "matrix_add") == 0) { temp = re_matrix.matrix_add(); }
	if (strcmp(intername, "matrix_echelon") == 0) { temp = re_matrix.matrix_echelon(); }
	if (strcmp(intername, "matrix_transpose") == 0) { temp = re_matrix.matrix_transpose(); }
	if (strcmp(intername, "matrix_dimensions") == 0) { sprintf(number, "%d", re_matrix.matrix_dimensions()); temp = number; }
	if (strcmp(intername, "matrix_area") == 0) { sprintf(number, "%g", re_matrix.matrix_area()); temp = number; }
	if (strcmp(intername, "matrix_rank_vectors") == 0) { temp = re_matrix.matrix_rank_vectors(); }
	if (strcmp(intername, "matrix_rank") == 0) { temp = re_matrix.matrix_rank(atoi(eigen_count)); }
	if (strcmp(intername, "linearRelation") == 0) { temp = re_matrix.linearRelation(); }


	memset(value, 0, 10240);
	strcpy(value, "<retcode>0</retcode><message>ok</message>");
	Writen(sockfd, value, strlen(value));

	// 向客户端发送xml内容的头部标签<data>。
	Writen(sockfd, "<data>", strlen("<data>"));

	memset(value, 0, 10240);
	sprintf(value, "<value>%s</value>", temp);
	Writen(sockfd, value, strlen(value));

	// 向客户端发送xml内容的尾部标签</data>。
	Writen(sockfd, "</data>", strlen("</data>"));
	logfile.Write("send matrix (%s) \n",value);
	
	close(sockfd);
	return true;
}

connpool::connpool()
{
	m_maxconns = 0;
	m_timeout = 0;
	memset(m_connstr, 0, sizeof(m_connstr));
	memset(m_charset, 0, sizeof(m_charset));
	m_conns = 0;
}

// 初始化数据库连接池，初始化锁，如果数据库连接参数有问题，返回false。
bool connpool::init(const char* connstr, const char* charset, const int maxconns, int timeout)
{
	// 尝试数据库，验证数据库连接参数是否正确。
	connection conn;
	if (conn.connecttodb(connstr, charset) != 0)
	{
		printf("连接数据库失败。\n%s\n", conn.m_cda.message); return false;
	}
	conn.disconnect();

	strncpy(m_connstr, connstr, 100);
	strncpy(m_charset, charset, 100);
	m_maxconns = maxconns;
	m_timeout = timeout;

	// 分配数据库连接池内存空间。
	m_conns = new struct st_conn[m_maxconns];

	for (int ii = 0; ii < m_maxconns; ii++)
	{
		pthread_mutex_init(&m_conns[ii].mutex, 0);    // 初始化锁。
		m_conns[ii].atime = 0;                         // 数据库连接上次使用的时间初初化为0。
	}

	return true;
}

connpool::~connpool()
{
	destroy();
}

// 断开数据库连接，销毁锁，释放连接池。
void connpool::destroy()
{
	for (int ii = 0; ii < m_maxconns; ii++)
	{
		m_conns[ii].conn.disconnect();             // 断开数据库连接。
		pthread_mutex_destroy(&m_conns[ii].mutex); // 断开数据库连接。
	}

	delete[]m_conns;         // 释放数据库连接池的内存空间。
	m_conns = 0;

	memset(m_connstr, 0, sizeof(m_connstr));
	memset(m_charset, 0, sizeof(m_charset));
	m_maxconns = 0;
	m_timeout = 0;
}

// 1)从数据库连接池中寻找一个空闲的、已连接好的connection，如果找到了，返回它的地址。
// 2)如果没有找到，在连接池中找一个未连接数据库的connection，连接数据库，如果成功，返回connection的地址。
// 3)如果第2)步找到了未连接数据库的connection，但是，连接数据库失败，返回空。
// 4)如果第2)步没有找到未连接数据库的connection，表示数据库连接池已用完，也返回空。
connection* connpool::get()
{
	int pos = -1;       // 用于记录第一个未连接数据库的数组位置。

	for (int ii = 0; ii < m_maxconns; ii++)
	{
		if (pthread_mutex_trylock(&m_conns[ii].mutex) == 0)       // 尝试加锁。
		{
			if (m_conns[ii].atime > 0)       // 如果数据库连接是已连接的状态。
			{
				printf("取到连接%d。\n", ii);
				m_conns[ii].atime = time(0);   // 把数据库连接的使用时间设置为当前时间。
				return &m_conns[ii].conn;    // 返回数据库连接的地址。
			}

			if (pos == -1) pos = ii;          // 记录第一个未连接数据库的数组位置。
			else pthread_mutex_unlock(&m_conns[ii].mutex);     // 释放锁。
		}
	}

	if (pos == -1)   // 如果连接池已用完，返回空。
	{
		printf("连接池已用完。\n"); return NULL;
	}

	// 连接池没有用完，让m_conns[pos].conn连上数据库。
	printf("新连接%d。\n", pos);

	// 连接数据库。
	if (m_conns[pos].conn.connecttodb(m_connstr, m_charset) != 0)
	{
		// 如果连接数据库失败，释放锁，返回空。
		printf("连接数据库失败。\n");
		pthread_mutex_unlock(&m_conns[pos].mutex);  // 释放锁。
		return NULL;
	}

	m_conns[pos].atime = time(0);      // 把数据库连接的使用时间设置为当前时间。

	return &m_conns[pos].conn;
}

// 归还数据库连接。 
bool connpool::free(connection* conn)
{
	for (int ii = 0; ii < m_maxconns; ii++)
	{
		if (&m_conns[ii].conn == conn)
		{
			printf("归还%d\n", ii);
			m_conns[ii].atime = time(0);      // 把数据库连接的使用时间设置为当前时间。
			pthread_mutex_unlock(&m_conns[ii].mutex);        // 释放锁。
			return true;
		}
	}

	return false;
}

// 检查连接池，断开空闲的连接。
void connpool::checkpool()
{
	for (int ii = 0; ii < m_maxconns; ii++)
	{
		if (pthread_mutex_trylock(&m_conns[ii].mutex) == 0)  // 尝试加锁。
		{
			if (m_conns[ii].atime > 0)    // 如果是一个可用的连接。
			{
				// 判断连接是否超时。
				if ((time(0) - m_conns[ii].atime) > m_timeout)
				{
					printf("连接%d已超时。\n", ii);
					m_conns[ii].conn.disconnect();     // 断开数据库连接。
					m_conns[ii].atime = 0;               // 重置数据库连接的使用时间。
				}
				else
				{
					// 如果没有超时，执行一次sql，验证连接是否有效，如果无效，断开它。
					// 如果网络断开了，或者数据库重启了，那么就需要重连数据库，在这里，只需要断开连接就行了，
					// 重连的操作交给get()函数。
					if (m_conns[ii].conn.execute("select * from dual") != 0)
					{
						printf("连接%d已故障。\n", ii);
						m_conns[ii].conn.disconnect();     // 断开数据库连接。
						m_conns[ii].atime = 0;               // 重置数据库连接的使用时间。
					}
				}
			}

			pthread_mutex_unlock(&m_conns[ii].mutex);   // 释放锁。
		}

		// 如果尝试加锁失败，表示数据库连接正在使用中，不必检查。
	}
}


void* checkpool(void* arg)    // 检查数据库连接池的线程函数。
{
	while (true)
	{
		mysqlconnpool.checkpool();  sleep(30);
	}
}

void* checkthmain(void* arg)    // 监控线程主函数。
{
	while (true)
	{
		// 遍历工作线程结构体的容器，检查每个工作线程是否超时。
		for (int ii = 0; ii < vthid.size(); ii++)
		{
			// 工作线程超时间为20秒，这里用25秒判断超时，足够。
			if ((time(0) - vthid[ii].atime) > 25)
			{
				// 已超时。
				logfile.Write("thread %d(%lu) timeout(%d).\n", ii, vthid[ii].pthid, time(0) - vthid[ii].atime);

				// 取消已超时的工作线程。
				pthread_cancel(vthid[ii].pthid);

				// 重新创建工作线程。
				if (pthread_create(&vthid[ii].pthid, NULL, thmain, (void*)(long)ii) != 0)
				{
					logfile.Write("pthread_create() failed.\n"); EXIT(-1);
				}

				vthid[ii].atime = time(0);      // 设置工作线程的活动时间。
			}
		}

		sleep(3);
	}
}
