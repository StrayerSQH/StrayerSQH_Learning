import socket
import time
import argparse
from concurrent.futures import ThreadPoolExecutor

def test_connection(server_ip, server_port, timeout=2):
    try:
        start_time = time.time()
        # 创建TCP socket
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.settimeout(timeout)
        
        # 尝试连接
        sock.connect((server_ip, server_port))
        
        # 接收服务器响应
        data = sock.recv(1024)
        sock.close()
        
        end_time = time.time()
        latency = end_time - start_time
        
        return (True, latency)
    except Exception as e:
        return (False, str(e))

def run_client(server_ip, server_port, num_connections, max_workers):
    success_count = 0
    failure_count = 0
    latencies = []
    
    def worker(_):
        nonlocal success_count, failure_count
        success, result = test_connection(server_ip, server_port)
        if success:
            success_count += 1
            latencies.append(result)
        else:
            failure_count += 1
    
    start_time = time.time()
    
    # 使用线程池快速创建大量连接
    with ThreadPoolExecutor(max_workers=max_workers) as executor:
        executor.map(worker, range(num_connections))
    
    end_time = time.time()
    total_time = end_time - start_time
    
    # 打印结果
    print("\nTest Results:")
    print(f"Total connections attempted: {num_connections}")
    print(f"Successful connections: {success_count}")
    print(f"Failed connections: {failure_count}")
    print(f"Success rate: {success_count/num_connections*100:.2f}%")
    print(f"Total test time: {total_time:.2f} seconds")
    
    if latencies:
        avg_latency = sum(latencies) / len(latencies)
        print(f"Average latency for successful connections: {avg_latency*1000:.2f} ms")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="TCP Client for queue experiment")
    parser.add_argument("--server", required=True, help="Server IP address")
    parser.add_argument("--port", type=int, default=9999, help="Server port")
    parser.add_argument("--num", type=int, default=10, help="Number of connections to attempt")
    parser.add_argument("--workers", type=int, default=10, 
                       help="Maximum number of concurrent workers")
    
    args = parser.parse_args()
    run_client(args.server, args.port, args.num, args.workers)