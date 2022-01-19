from io import BytesIO
from socketLib.Socket import Socket
from socketLib.RawSocket import RawSocket
from struct import calcsize
import logging

class SocketTCP(Socket):

    def connect(self) -> None:
        if not self.socket:
            self.socket = RawSocket("ipv6" if ":" in self.host else "ipv4")
            self.socket.setblocking(0)
            try:
                self.socket.connect(self.host, self.port)
            except ConnectionRefusedError:
                self.socket.disconnect()
                return False
            logging.info('Using TCP socket')

    def read(self, socket = None):
        socket = self.socket if socket is None else socket
        amount, current_amount = 1, 0
        data_map: dict = {}
        while current_amount < amount:
            header = socket.receive(calcsize(self.header_types))
            if header is False:
                if len(data_map) > 0:
                    current_amount += 1
                else:
                    amount, current_amount = 1, 0
                continue
            _, size, amount, number = self.split_read_data(header)
            data = socket.receive(size)
            if data is False:
                if len(data_map) > 0:
                    current_amount += 1
                else:
                    amount, current_amount = 1, 0
                continue
            data_map[number] = data
            current_amount += 1
        data = b''.join(val for (_, val) in data_map.items())
        return data, None

    def send(self, binary_stream: BytesIO, address: str = None, socket = None) -> None:
        socket = self.socket if socket is None else socket
        datagram_number = 0
        data = self.split_send_data(binary_stream.read(), socket.buffer_size)
        for datagram in data:
            socket.send(datagram)
            logging.debug('Sending datagram #%s: %s', datagram_number, datagram)
            datagram_number += 1
