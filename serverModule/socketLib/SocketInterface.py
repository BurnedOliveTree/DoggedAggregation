import struct, io


class SocketInterface:
    def __init__(self, socket):
        self.binary_stream = None
        self.socket = socket
        self.header_types = '!HBB4s'  # TODO Mariannkaaa!

    def connect(self) -> None or bool:
        self.binary_stream = io.BytesIO()
        return self.socket.connect()

    def read(self):
        data = self.socket.receive()
        if data:
            doc_id, doc_type, status_code, hash_hash = self.interpret_header(data)
            content = data[struct.calcsize(self.header_types):]
            return doc_id, doc_type, status_code, hash_hash, content
        raise ValueError('Data not received')

    def disconnect(self) -> None:
        self.binary_stream.close()
        self.socket.disconnect()

    def interpret_header(self, datagram: bytes):
        header = datagram[:struct.calcsize(self.header_types)]
        doc_id, doc_type, status_code, hash_hash = struct.unpack(self.header_types, header)
        return doc_id, doc_type, status_code, hash_hash

    def __enter__(self):
        succeeded_bind = self.connect()
        if succeeded_bind == False:
            self.disconnect()
            return None
        return self

    def __exit__(self, exc_type, exc_value, traceback) -> None:
        self.disconnect()
