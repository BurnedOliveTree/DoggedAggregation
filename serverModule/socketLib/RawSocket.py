import logging
import socket


class RawSocket:
    def __init__(self, ip_version=None):
        self.buffer_size = 256
        self.socket: socket.socket = None
        if ip_version is not None:
            self.socket = socket.socket(socket.AF_INET if ip_version == "ipv4" else socket.AF_INET6, socket.SOCK_STREAM)

    def connect(self, host, port):
        if self.socket:
            self.socket.connect((host, port))

    def receive(self, size):
        try:
            return self.socket.recv(size)
        except socket.error as err:
            logging.warn(f"Message: {err}")
            return False

    def disconnect(self) -> None:
        if self.socket:
            self.socket.close()
            self.socket = None

    def __enter__(self):
        self.connect()
        return self

    def __exit__(self, exc_type, exc_value, traceback) -> None:
        self.disconnect()
