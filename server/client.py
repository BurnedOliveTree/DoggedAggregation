import logging, sys
from socketLib import SocketInterface, Socket, Host, get_project_root


class Client(Host):
    def __init__(self, argv: list):
        super().__init__(argv)

    def connect(self) -> None:
        socket = self.__get_socket()
        data = None
        with SocketInterface(socket) as socket:
            print("Will send data to ", self.host, ":", self.port)
            while data != "QUIT":
                received_data = socket.read()
                print('Received data: ', repr(received_data))
            else:
                socket.send(data)
        print('Client finished')

    def __get_socket(self):
        return Socket(self.host, self.port)


if __name__ == "__main__":
    logging.basicConfig(
        format='%(asctime)s %(levelname)-8s %(message)s',
        datefmt='%Y-%m-%d %H:%M:%S',
        filename=get_project_root() + '/log/client.log',
        level=logging.DEBUG
    )
    Client(sys.argv).connect()
