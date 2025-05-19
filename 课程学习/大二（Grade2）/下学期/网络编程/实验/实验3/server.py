import socket
import hashlib
import struct

#定义消息类型
COMMAND_ID_REG_REQUEST = 1
COMMAND_ID_REG_RESPONSE = 2
COMMAND_ID_LOGIN_REQUEST = 3
COMMAND_ID_LOGIN_RESPONSE = 4

PORT = 12345 #服务器端口

#存储用户信息
PASSWD_FILE = "passwd"

def handle_registration_request(username, password):
    with open(PASSWD_FILE, "a+") as f:
        f.seek(0)
        for line in f:
            if line.startswith(username + ":"):
                return False, "用户名已存在"
        hashed_password = hashlib.sha256(password.encode()).hexdigest()
        f.write(f"{username}:{hashed_password}\n")
    return True, "注册成功"

def handle_login_request(username, password):
    hashed_password = hashlib.sha256(password.encode()).hexdigest()
    with open(PASSWD_FILE, "r") as f:
        for line in f:
            #比对用户信息
            if line.startswith(username + ":"):
                stored_password = line.split(":")[1].strip()
                if stored_password == hashed_password:
                    return True, "登录成功"
                else:
                    return False, "密码错误"
    return False, "用户不存在"

    
if __name__ == "__main__":
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind(("0.0.0.0", PORT))
    server_socket.listen(5)
    print(f"服务器启动，监听端口 {PORT}")

    while True:
        client_socket, client_address = server_socket.accept()
        print(f"客户端 {client_address} 已连接")

        while True:
            #接收消息
            data = client_socket.recv(2048) #最多接受1024字节数据
            if not data:
                print(f"客户端 {client_address} 断开连接")
                break #没有接收到数据断开

            #解析消息头
            total_length, command_id = struct.unpack("!II", data[:8]) #提取message_length和command_id
            message_body = data[8:] #提取message_body

            if command_id == COMMAND_ID_REG_REQUEST:
                #注册
                username = message_body[:20].decode().strip("\x00")  #解析出用户名
                password = message_body[20:50].decode().strip("\x00")  #解析出密码
                success, description = handle_registration_request(username, password)
                status = 1 if success else 0 #失败0成功1
                response_body = struct.pack("!B64s", status, description.encode())
                response_header = struct.pack("!II", 8 + len(response_body), COMMAND_ID_REG_RESPONSE)
                client_socket.send(response_header + response_body) #返回状态

            elif command_id == COMMAND_ID_LOGIN_REQUEST:
                #登录
                username = message_body[:20].decode().strip("\x00") #解析出用户名
                password = message_body[20:50].decode().strip("\x00") #解析出密码
                success, description = handle_login_request(username, password)
                status = 1 if success else 0 #失败0成功1
                response_body = struct.pack("!B64s", status, description.encode())
                response_header = struct.pack("!II", 8 + len(response_body), COMMAND_ID_LOGIN_RESPONSE)
                client_socket.send(response_header + response_body) #返回状态

        client_socket.close()