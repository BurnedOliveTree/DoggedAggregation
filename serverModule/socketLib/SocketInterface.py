import struct, io


class SocketInterface:
    def __init__(self, socket):
        self.binary_stream = None
        self.socket = socket

    def connect(self) -> None or bool:
        self.binary_stream = io.BytesIO()
        return self.socket.connect()

    def read(self) -> str:
        data = self.socket.read()
        if data:
            if data[0] == 1:
                decoded_data = struct.unpack('!iil', data[1:])  # TODO this needs to not be hardcoded
            else:
                decoded_data = self.decode(data[1:])
            return decoded_data
        raise ValueError('Data not received')

    @staticmethod
    def decode(data: bytes) -> str:
        if data:
            return data.decode("ascii")
        return ""

    def disconnect(self) -> None:
        self.binary_stream.close()
        self.socket.disconnect()

    def __enter__(self):
        succeeded_bind = self.connect()
        if succeeded_bind == False:
            self.disconnect()
            return None
        return self

    def __exit__(self, exc_type, exc_value, traceback) -> None:
        self.disconnect()
