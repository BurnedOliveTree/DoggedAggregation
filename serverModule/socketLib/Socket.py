import struct

from socketLib.RawSocket import RawSocket


class Socket:
    def __init__(self, host: str, port: str):
        self.socket: RawSocket = None
        self.host: str = host
        self.port: str = port
        self.packet_size = 60000
        self.header_types = '!IIQ'

    def connect(self) -> None:
        if not self.socket:
            self.socket = RawSocket("ipv6" if ":" in self.host else "ipv4")
            self.socket.connect(self.host, self.port)

    def receive(self):
        amount, current_amount = 1, 0
        data_map: dict = {}
        while current_amount < amount:
            header = self.socket.receive(struct.calcsize(self.header_types))
            if header is False:
                if len(data_map) > 0:
                    current_amount += 1
                else:
                    amount, current_amount = 1, 0
                continue
            size, amount, number = self.interpret_header(header)
            data = self.socket.receive(size)
            data_map[number] = data
            current_amount += 1
        data = b''.join(val for (_, val) in data_map.items())
        return data

    def interpret_header(self, datagram: bytes):
        header = datagram[:struct.calcsize(self.header_types)]
        amount, number, size = struct.unpack(self.header_types, header)
        return size, amount, number

    def __create_datagram(self, raw_data: bytes, amount: int, number: int, data_range: tuple):
        datagram: bytearray = bytearray(b'')
        size = (data_range[1] if data_range[1] else len(raw_data)) - data_range[0]
        datagram.extend(struct.pack(self.header_types, size, amount, number))
        datagram.extend(raw_data[data_range[0]:data_range[1]])
        return bytes(datagram)

    def disconnect(self):
        self.socket.disconnect()

    def __enter__(self):
        self.socket.connect()
        return self

    def __exit__(self, exc_type, exc_value, traceback) -> None:
        self.socket.disconnect()
