# 启动计算服务器
/myproject/tools/bin/procctl 10 /myproject/tools/bin/mat_webserver /log/idc/mat_webserver.log "<connstr>127.0.0.1,root,ucp4WaC0tE_s,3306</connstr><charset>utf8</charset><port>8080</port>"

# 启动sql服务器
/myproject/tools/bin/procctl 10 /myproject/tools/bin/sql_webserver /log/idc/sql_webserver.log "<connstr>127.0.0.1,root,ucp4WaC0tE_s,mat-storage,3306</connstr><charset>utf8</charset><port>8081</port>"
