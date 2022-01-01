from io import BytesIO
from lib.Socket import Socket
from struct import pack, unpack

from lib.RawSocket import RawSocket

class SocketInterface:
    def __init__(self, socket: Socket):
        self.binary_stream = None
        self.socket = socket
    
    def read(self, ret_address: bool = False, socket: RawSocket = None) -> str or tuple:
        if not socket:
            socket = self.socket
        data, address = socket.read()
        if data:
            if data[0] == 1:
                is_struct = True
                decoded_data = unpack('!HBB', data[1:])  # TODO this needs to not be hardcoded
            else:
                is_struct = False
                decoded_data = self.decode(data[1:])
            if ret_address:
                return decoded_data, address, is_struct
            else:
                return decoded_data
        raise ValueError('Data not received')

    def send(self, data: str, address: str = None, is_struct = False, socket: RawSocket = None) -> None:
        if not socket:
            socket = self.socket
        if is_struct:
            encoded_data = b'\1' + pack('!HBB', 1, 2, 3)  # TODO this needs to not be hardcoded
        else:
            encoded_data = b'\0' + self.encode(data)
        self.write_to_binary_stream(encoded_data)
        self.socket.send(self.binary_stream, address)
        self.clear_binary_stream()
    
    def clear_binary_stream(self) -> None:
        self.binary_stream.seek(0)
        self.binary_stream.truncate(0)
    
    def write_to_binary_stream(self, data: bytes) -> None:
        self.binary_stream.write(data)
        self.binary_stream.seek(0)

    @staticmethod
    def encode(data: str) -> bytes:
        if data:
            return data.encode("ascii")
        return b""

    @staticmethod
    def decode(data: bytes) -> str:
        if data:
            return data.decode("ascii")
        return ""

    def connect(self) -> None or bool:
        if not self.binary_stream:
            self.binary_stream = BytesIO()
        return self.socket.connect()

    def disconnect(self) -> None:
        self.binary_stream.close()
        self.socket.disconnect()
    
    def __enter__(self):
        if not self.connect():
            self.disconnect()
            return None
        return self
    
    def __exit__(self, exc_type, exc_value, traceback) -> None:
        self.disconnect()

