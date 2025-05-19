import socket
import time
import argparse
import threading

def run_server(port, backlog, delay_accept):
    # 创建TCP socket
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    
    # 绑定端口并监听
    server_socket.bind(('0.0.0.0', port))
    server_socket.listen(backlog)
    
    print(f"Server started on port {port} with backlog {backlog}")
    print(f"Delay accept: {delay_accept} seconds")
    
    # 用于统计连接数的变量
    connection_count = 0
    
    def handle_client(client_socket):
        nonlocal connection_count
        connection_count += 1
        print(f"Accepted connection #{connection_count}")
        # 简单处理客户端连接
        client_socket.send(b"Hello from server!")
        client_socket.close()
    
    try:
        while True:
            try:
                # 接受新连接
                client_socket, addr = server_socket.accept()
                print(f"New connection from {addr}")
                
                if delay_accept > 0:
                    # 延迟处理连接，模拟全连接队列堆积
                    print(f"Delaying accept for {delay_accept} seconds...")
                    time.sleep(delay_accept)
                
                # 启动线程处理连接
                threading.Thread(target=handle_client, args=(client_socket,)).start()
                
            except KeyboardInterrupt:
                print("\nServer shutting down...")
                break
            except Exception as e:
                print(f"Error accepting connection: {e}")
                continue
                
    finally:
        server_socket.close()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="TCP Server for queue experiment")
    parser.add_argument("--port", type=int, default=9999, help="Port to listen on")
    parser.add_argument("--backlog", type=int, default=5, help="Backlog queue size")
    parser.add_argument("--delay", type=float, default=5, 
                       help="Delay in seconds before accepting each connection (to fill queue)")
    
    args = parser.parse_args()
    run_server(args.port, args.backlog, args.delay)
