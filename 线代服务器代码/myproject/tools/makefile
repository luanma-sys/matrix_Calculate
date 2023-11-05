# 开发框架头文件路径
PUBINCL = -I/myproject/public

# 开发框架cpp文件名，这里直接包含进来，没有采用链接库，是为了方便调试。
PUBCPP = /myproject/public/_public.cpp

# mysql头文件存放路径
MYSQLINCL = -I/myproject/mysql -I/myproject/public/db/mysql

# mysql库文件保存路径
MYSQLLIB = -L/myproject/mysql -L/usr/lib64/mysql/

# mysql链接库
MYSQLLIBS = -lmysqlclient

# 开发框架mysql的cpp文件名，这里直接包含进来，没有采用链接库，是为了方便调试。
MYSQLCPP = /myproject/public/db/mysql/_mysql.cpp

# 线代计算类头文件路径 包括eigen目录
MATINCL = -I/myproject/public -I/myproject/public/eigen3

# 线代计算类cpp文件路径
MATCPP = /myproject/public/mymatrix.cpp 
 
# 编译参数。
CFLAGS = -g -std=c++11

all: procctl mat_webserver sql_webserver

procctl:procctl.cpp
	g++ -o procctl procctl.cpp
	cp procctl ./bin/.

mat_webserver:mat_webserver.cpp
	g++ $(CFLAGS) -o mat_webserver mat_webserver.cpp $(PUBINCL) $(PUBCPP) $(MYSQLINCL) $(MYSQLLIB) $(MYSQLLIBS) $(MYSQLCPP) $(MATINCL) $(MATCPP) -lpthread -lm -lc
	cp mat_webserver ./bin/.

sql_webserver:sql_webserver.cpp
	g++ $(CFLAGS) -o sql_webserver sql_webserver.cpp $(PUBINCL) $(PUBCPP) $(MYSQLINCL) $(MYSQLLIB) $(MYSQLLIBS) $(MYSQLCPP) $(MATINCL) $(MATCPP) -lpthread -lm -lc
	cp sql_webserver ./bin/.
    	
clean: 
	rm -rf procctl mat_webserver sql_webserver

