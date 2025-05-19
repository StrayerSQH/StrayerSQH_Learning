import socket
import threading
import time

class TCPClient:
    def __init__(self, host='127.0.0.1', port=12345):
        self.host = host
        self.port = port
        self.client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.running = False
        self.heartbeat_count = 0  # 心跳计数器
        self.initial_interval = 5  # 前3次心跳间隔(秒)
        self.final_interval = 100  # 3次后心跳间隔(秒)
        self.max_initial_heartbeats = 3  # 初始阶段心跳次数

    def connect(self):
        try:
            self.client_socket.connect((self.host, self.port))
            print(f"已连接到服务器 {self.host}:{self.port}")
            self.running = True
            
            # 启动心跳线程
            heartbeat_thread = threading.Thread(target=self.send_heartbeats)
            heartbeat_thread.daemon = True
            heartbeat_thread.start()
            
            # 启动接收线程
            receive_thread = threading.Thread(target=self.receive_messages)
            receive_thread.daemon = True
            receive_thread.start()
            
            # 主线程处理用户输入
            while self.running:
                message = input("请输入要发送的消息(输入'exit'退出): ")
                if message.lower() == 'exit':
                    self.disconnect()
                    break
                try:
                    self.client_socket.sendall(message.encode('utf-8'))
                except:
                    print("发送消息失败")
                    self.disconnect()
                    break
        except ConnectionRefusedError:
            print(f"无法连接到服务器 {self.host}:{self.port}")

    def send_heartbeats(self):
        while self.running:
            try:
                # 发送心跳包
                self.client_socket.sendall("HEARTBEAT".encode('utf-8'))
                self.heartbeat_count += 1
                print(f"发送第 {self.heartbeat_count} 次心跳包")
                
                # 根据心跳次数决定下次心跳间隔
                if self.heartbeat_count < self.max_initial_heartbeats:
                    interval = self.initial_interval
                else:
                    interval = self.final_interval
                
                time.sleep(interval)
            except:
                print("发送心跳失败")
                self.disconnect()
                break

    def receive_messages(self):
        while self.running:
            try:
                data = self.client_socket.recv(1024).decode('utf-8')
                if not data:
                    print("服务器断开连接")
                    self.disconnect()
                    break
                if data != "HEARTBEAT": 
                    print(f"来自服务器的消息: {data}")
            except:
                print("接收消息失败")
                self.disconnect()
                break

    def disconnect(self):
        if self.running:
            self.running = False
            try:
                self.client_socket.close()
            except:
                pass
            print("已断开与服务器的连接")

if __name__ == "__main__":
    client = TCPClient()
    client.connect()