import socket, logging

class RawSocket:
    def __init__(self, ip_version = None, created_socket = None):
        self.buffer_size = 65535 
        self.socket: socket.socket = None
        self.default_timeout = 10
        if created_socket is not None: 
            self.socket = created_socket
        elif ip_version is not None:
            self.create_socket(ip_version)
        self.enable_timeout()
    
    def create_socket(self, ip_version):
        if ip_version == 'ipv4':
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        elif ip_version == 'ipv6':
            self.socket = socket.socket(socket.AF_INET6, socket.SOCK_STREAM)
    
    def enable_timeout(self):
        self.socket.settimeout(self.default_timeout)
        
    def bind(self, host, port):
        try:
            self.socket.bind((host, port))
        except socket.error as message:
            logging.critical(f"Bind failed. Message {message}")
            return False
    
    def listen(self, num_workers):
        return self.socket.listen(num_workers)
    
    def accept(self):
        try:
            return self.socket.accept()
        except socket.error as message:
            logging.critical(f"Accept failed. Message {message}")
            return False
    
    def receive(self, size):
        try:
            return self.socket.recv(size)
        except socket.error as err:
            logging.warn(f"Message: {err}")
            return False
    
    def receive_from(self):
        try:
            self.socket.recvfrom(self.buffer_size)
        except socket.error as err:
            logging.warn(f"Socket timed out, data is probably corrupted. Message: {err}")
            return False
    
    def send(self, datagram):
        bytes_sent = 0
        while bytes_sent < len(datagram):
            bytes_sent += self.socket.send(datagram)
    
    def sendto(self, datagram, address):
        bytes_sent = 0
        while bytes_sent < len(datagram):
            bytes_sent += self.socket.sendto(datagram, address)
    
    def sendall(self, data):
        self.socket.sendall(data)
        
    def connect(self, host, port):
        if self.socket:
            self.socket.connect((host, port))

    def disconnect(self) -> None:
        if self.socket:
            self.socket.close()
            self.socket = None
    
    def __enter__(self):
        self.connect()
        return self
    
    def __exit__(self, exc_type, exc_value, traceback) -> None:
        self.disconnect()