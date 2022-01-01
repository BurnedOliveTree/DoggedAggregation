import io, logging, struct

from socketLib.RawSocket import RawSocket

class Socket:
    def __init__(self, host: str, port: str):
        self.socket: RawSocket = None
        self.host: str = host
        self.port: str = port
        self.packet_size = 7
        self.client_adress = None
        self.header_types = '!HBB'  

    def read(self):
        address = None
        amount, current_amount = 1, 0
        data_map: dict = {}
        while current_amount < amount:
            received_data = self.socket.receive_from()
            if received_data is False:
                if len(data_map) > 0:
                    current_amount += 1
                else:
                    amount, current_amount = 1, 0
                continue
            else:
                (datagram, address) = received_data
            if address is not None:
                self.client_adress = address
            data, _, amount, number = self.split_read_data(datagram)
            data_map[number] = data
            current_amount += 1
        data = b''.join(val for (_, val) in data_map.items())
        return data, self.client_adress

    
    def split_read_data(self, datagram: bytes):
        header = datagram[:struct.calcsize(self.header_types)]
        size, amount, number = struct.unpack(self.header_types, header)
        return datagram[struct.calcsize(self.header_types):], size, amount, number

    def send(self, binary_stream: io.BytesIO, address: str = None) -> None:
        datagram_number = 0
        if address is None:
            address = (self.host, self.port)
        data = self.__split_send_data(binary_stream.read(), self.socket.buffer_size)
        for datagram in data:
            self.socket.sendto(datagram, address)
            logging.debug('Sending datagram #%s: %s', datagram_number, datagram)
            datagram_number += 1
    
    def __create_datagram(self, raw_data: bytes, amount: int, number: int, data_range: tuple):
        datagram: bytearray = bytearray(b'')
        size = (data_range[1] if data_range[1] else len(raw_data)) - data_range[0]
        datagram.extend(struct.pack(self.header_types, size, amount, number))
        datagram.extend(raw_data[data_range[0]:data_range[1]])
        return bytes(datagram)
    
    def split_send_data(self, raw_data: bytes, buffer_size: int) -> str:
        data = []
        max_size = min(buffer_size + 1, self.packet_size) - struct.calcsize(self.header_types)
        datagram_amount = len(raw_data) // max_size + (1 if len(raw_data) % max_size != 0 else 0)
        if datagram_amount >= 256 ** struct.calcsize(self.header_types[2]):
            raise ValueError("Given data was too big, resulting in too many datagrams")
        for i in range(0, datagram_amount - 1):
            data.append(self.__create_datagram(raw_data, datagram_amount, i, (max_size * i, max_size * (i + 1))))
        data.append(self.__create_datagram(raw_data, datagram_amount, datagram_amount - 1, ((datagram_amount - 1) * max_size, None)))
        return data
    
    def connect(self):
        raise NotImplementedError("This method is an interface and shouldn't be called directly")
    
    def disconnect(self):
        if self.socket:
            self.socket.disconnect()

    def __enter__(self):
        self.connect()
        return self
    
    def __exit__(self, exc_type, exc_value, traceback) -> None:
        self.disconnect()