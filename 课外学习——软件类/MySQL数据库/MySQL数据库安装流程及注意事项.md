# MySQL数据库安装流程及注意事项

1.删除MySQL注册表建议参考链接： https://www.cnblogs.com/lwqbk/p/16943191.html 。

2.安装MySQL建议参考CSDN链接：https://blog.csdn.net/m0_52559040/article/details/121843945 进行安装。安装时注意不要选择最新版本。

3.在安装到Windows Service Name这一步时，可以输入自己起的名字，但是在后来使用`net start xxxxx`命令时一定要注意改为自己起的名字!!!例如如果将Windows Service Name写为MySQLStrayer后，在使用启动命令时应该使用`net start mysqlstrayer`才能正确启动.
