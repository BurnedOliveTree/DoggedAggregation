import socket, logging


class RawSocket:
    def __init__(self, ip_version=None):
        self.buffer_size = 65535
        self.socket: socket.socket = None
        if ip_version is not None:
            self.create_socket(ip_version)

    def receive_from(self):
        try:
            self.socket.recvfrom(self.buffer_size)
        except socket.error as err:
            logging.warn(f"Socket error, data is probably corrupted. Message: {err}")
            return False

    def disconnect(self) -> None:
        if self.socket:
            self.socket.close()
            self.socket = None

    def connect(self, host, port):
        if self.socket:
            self.socket.connect((host, port))

    def create_socket(self, ip_version):
        if ip_version == 'ipv4':
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        elif ip_version == 'ipv6':
            self.socket = socket.socket(socket.AF_INET6, socket.SOCK_STREAM)

    def receive(self, size):
        try:
            return self.socket.recv(size)
        except socket.error as err:
            logging.warn(f"Message: {err}")
            return False

    def __enter__(self):
        self.connect()
        return self

    def __exit__(self, exc_type, exc_value, traceback) -> None:
        self.disconnect()