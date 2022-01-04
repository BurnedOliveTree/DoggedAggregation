import io, logging, struct

from server.socketLib.RawSocket import RawSocket


class Socket:
    def __init__(self, host: str, port: str):
        self.socket: RawSocket = None
        self.host: str = host
        self.port: str = port
        self.packet_size = 60000
        self.client_address = None
        self.header_types = '!IHH'

    def read(self):
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
            _, size, amount, number = self.split_read_data(header)
            data = self.socket.receive(size)
            data_map[number] = data
            current_amount += 1
        data = b''.join(val for (_, val) in data_map.items())
        return data, None

    def send(self, binary_stream: io.BytesIO, address: str = None) -> None:
        datagram_number = 0
        data = self.split_send_data(binary_stream.read())
        for datagram in data:
            self.socket.send(datagram)
            logging.debug('Sending datagram #%s: %s', datagram_number, datagram)
            datagram_number += 1


    def split_read_data(self, datagram: bytes):
        header = datagram[:struct.calcsize(self.header_types)]
        size, amount, number = struct.unpack(self.header_types, header)
        return datagram[struct.calcsize(self.header_types):], size, amount, number

    def __create_datagram(self, raw_data: bytes, amount: int, number: int, data_range: tuple):
        datagram: bytearray = bytearray(b'')
        size = (data_range[1] if data_range[1] else len(raw_data)) - data_range[0]
        datagram.extend(struct.pack(self.header_types, size, amount, number))
        datagram.extend(raw_data[data_range[0]:data_range[1]])
        return bytes(datagram)

    def split_send_data(self, raw_data: bytes) -> str:
        data = []
        max_size = min(self.socket.buffer_size + 1, self.packet_size) - struct.calcsize(self.header_types)
        datagram_amount = len(raw_data) // max_size + (1 if len(raw_data) % max_size != 0 else 0)
        if datagram_amount >= self.packet_size:
            raise ValueError("Given data was too big, resulting in too many datagrams")
        for i in range(0, datagram_amount - 1):
            data.append(self.__create_datagram(raw_data, datagram_amount, i, (max_size * i, max_size * (i + 1))))
        data.append(
            self.__create_datagram(raw_data, datagram_amount, datagram_amount - 1, (-(len(raw_data) % max_size), None)))
        return data

    def __enter__(self):
        self.socket.connect()
        return self

    def __exit__(self, exc_type, exc_value, traceback) -> None:
        self.socket.disconnect()

    def connect(self) -> None:
        if not self.socket:
            self.socket = RawSocket("ipv6" if ":" in self.host else "ipv4")
            self.socket.connect(self.host, self.port)
            logging.info('Using TCP socket')

    def disconnect(self):
        self.socket.disconnect()

    def __enter__(self):
        if self.socket:
            self.socket.connect()
        return self

    def __exit__(self, exc_type, exc_value, traceback) -> None:
        if self.socket:
            self.socket.disconnect()