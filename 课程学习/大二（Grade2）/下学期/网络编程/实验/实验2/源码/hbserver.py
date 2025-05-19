import socket
import threading
import time
from datetime import datetime

class TCPServer:
    def __init__(self, host='0.0.0.0', port=12345, heartbeat_threshold=10):
        self.host = host
        self.port = port
        self.heartbeat_threshold = heartbeat_threshold
        self.client_info = {}  # 存储客户端地址和最后心跳时间
        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.running = False

    def start(self):
        self.server_socket.bind((self.host, self.port))
        self.server_socket.listen(5)
        print(f"[{datetime.now().strftime('%Y-%m-%d %H:%M:%S')}] 服务器启动，监听 {self.host}:{self.port}")
        print(f"[{datetime.now().strftime('%Y-%m-%d %H:%M:%S')}] 心跳检测阈值: {self.heartbeat_threshold}秒")
        self.running = True
        
        # 启动心跳检测线程
        heartbeat_thread = threading.Thread(target=self.check_heartbeats)
        heartbeat_thread.daemon = True
        heartbeat_thread.start()
        
        try:
            while self.running:
                try:
                    client_socket, client_address = self.server_socket.accept()
                    print(f"[{datetime.now().strftime('%Y-%m-%d %H:%M:%S')}] 新的客户端连接: {client_address}")
                    
                    # 记录客户端初始连接时间
                    self.client_info[client_address] = {
                        'last_heartbeat': time.time(),
                        'socket': client_socket,
                        'connect_time': datetime.now().strftime('%Y-%m-%d %H:%M:%S')
                    }
                    
                    # 为每个客户端创建接收线程
                    client_thread = threading.Thread(
                        target=self.handle_client,
                        args=(client_socket, client_address)
                    )
                    client_thread.daemon = True
                    client_thread.start()
                except Exception as e:
                    print(f"[{datetime.now().strftime('%Y-%m-%d %H:%M:%S')}] 接受客户端连接时出错: {str(e)}")
                    continue
        except KeyboardInterrupt:
            self.stop()

    def handle_client(self, client_socket, client_address):
        try:
            while self.running:
                try:
                    data = client_socket.recv(1024).decode('utf-8')
                    if not data:
                        break
                    
                    # 如果是心跳包，更新最后心跳时间
                    if data == "HEARTBEAT":
                        self.client_info[client_address]['last_heartbeat'] = time.time()
                        print(f"[{datetime.now().strftime('%Y-%m-%d %H:%M:%S')}] 收到来自 {client_address} 的心跳包")
                    else:
                        print(f"[{datetime.now().strftime('%Y-%m-%d %H:%M:%S')}] 来自 {client_address} 的消息: {data}")
                except ConnectionResetError:
                    print(f"[{datetime.now().strftime('%Y-%m-%d %H:%M:%S')}] 客户端 {client_address} 异常断开")
                    break
                except Exception as e:
                    print(f"[{datetime.now().strftime('%Y-%m-%d %H:%M:%S')}] 处理客户端 {client_address} 时出错: {str(e)}")
                    break
        finally:
            if client_address in self.client_info:
                del self.client_info[client_address]
            try:
                client_socket.close()
            except:
                pass
            print(f"[{datetime.now().strftime('%Y-%m-%d %H:%M:%S')}] 客户端 {client_address} 连接关闭")

    def check_heartbeats(self):
        while self.running:
            try:
                current_time = time.time()
                disconnected_clients = []
                
                # 显示当前连接的客户端状态
                print("\n" + "="*50)
                print(f"[{datetime.now().strftime('%Y-%m-%d %H:%M:%S')}] 当前客户端连接状态:")
                for client_addr, info in self.client_info.items():
                    try:
                        time_since_last_heartbeat = current_time - info['last_heartbeat']
                        status = "正常" if time_since_last_heartbeat <= self.heartbeat_threshold else "超时"
                        print(f"客户端 {client_addr} | 连接时间: {info['connect_time']} | "
                            f"最后心跳: {time_since_last_heartbeat:.1f}秒前 | 状态: {status}")
                        
                        if time_since_last_heartbeat > self.heartbeat_threshold:
                            disconnected_clients.append(client_addr)
                    except Exception as e:
                        print(f"[{datetime.now().strftime('%Y-%m-%d %H:%M:%S')}] 检查客户端 {client_addr} 心跳时出错: {str(e)}")
                        disconnected_clients.append(client_addr)
                
                # 关闭超时的客户端连接
                for client_addr in disconnected_clients:
                    if client_addr in self.client_info:
                        try:
                            self.client_info[client_addr]['socket'].close()
                        except:
                            pass
                        try:
                            del self.client_info[client_addr]
                        except:
                            pass
                        print(f"[{datetime.now().strftime('%Y-%m-%d %H:%M:%S')}] 已断开与 {client_addr} 的连接(心跳超时)")
                
                time.sleep(3)  # 每3秒检查一次心跳并显示状态
            except Exception as e:
                print(f"[{datetime.now().strftime('%Y-%m-%d %H:%M:%S')}] 心跳检测线程出错: {str(e)}")
                continue

    def stop(self):
        self.running = False
        for addr, info in list(self.client_info.items()):
            try:
                info['socket'].close()
            except:
                pass
            try:
                del self.client_info[addr]
            except:
                pass
        try:
            self.server_socket.close()
        except:
            pass
        print(f"[{datetime.now().strftime('%Y-%m-%d %H:%M:%S')}] 服务器已停止")

if __name__ == "__main__":
    server = TCPServer()
    server.start()
