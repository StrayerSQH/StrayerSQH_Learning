import socket
import struct

#定义消息类型
COMMAND_ID_REG_REQUEST = 1
COMMAND_ID_REG_RESPONSE = 2
COMMAND_ID_LOGIN_REQUEST = 3
COMMAND_ID_LOGIN_RESPONSE = 4

SERVER_ADDRESS = "127.0.0.1"
SERVER_PORT = 12345

def send_registration_request(username, password):
    #注册
    message_body = struct.pack("!20s30s", username.encode(), password.encode())
    message_header = struct.pack("!II", 8 + len(message_body), COMMAND_ID_REG_REQUEST)
    return message_header + message_body

def send_login_request(username, password):
    #登录
    message_body = struct.pack("!20s30s", username.encode(), password.encode())
    message_header = struct.pack("!II", 8 + len(message_body), COMMAND_ID_LOGIN_REQUEST)
    return message_header + message_body

    

if __name__ == "__main__":
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect((SERVER_ADDRESS, SERVER_PORT))

    while True:
        choice = input("请输入操作（1：注册，2：登录）：")
        if choice == "1":
            username = input("请输入用户名：")
            password = input("请输入密码：")
            request_message = send_registration_request(username, password)
        elif choice == "2":
            username = input("请输入用户名：")
            password = input("请输入密码：")
            request_message = send_login_request(username, password)
        else:
            print("无效输入")
            continue

        client_socket.send(request_message)
        
        # 接收响应
        response_header = client_socket.recv(8)  # 接收消息头
        if not response_header:
            print("未收到响应")
            break

        total_length, command_id = struct.unpack("!II", response_header)
        remaining_length = total_length - 8  # 消息体长度

        response_body = b""
        while remaining_length > 0:
            chunk = client_socket.recv(remaining_length)
            if not chunk:
                print("连接中断")
                break
            response_body += chunk
            remaining_length -= len(chunk)

        if remaining_length > 0:
            print("未收到完整的响应")
            break

        status, description = struct.unpack("!B64s", response_body)
        description = description.decode().strip("\x00")
        print(f"服务器响应：状态码={status}，描述={description}")

    client_socket.close()
