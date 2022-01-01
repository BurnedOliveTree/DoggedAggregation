import logging, sys
from socketLib import SocketInterface, SocketTCP, Host, get_project_root


class Client(Host):
    def __init__(self, argv: list):
        super().__init__(argv)
        self.send_type_is_struct = False

    def connect(self) -> None:
        self.__get_input_type()
        socket = SocketTCP(self.host, self.port)
        with SocketInterface(socket) as socket:
            if socket is not None:
                print("Will send data to ", self.host, ":", self.port)
                data = ''  # TODO
                while data != "QUIT":
                    received_data = socket.read()
                    print('Received data: ', repr(received_data))
                else:
                    socket.send(data)
            else:
                print('Failed to connect, exiting now...')
        print('Client finished')

    def __get_input_type(self):
        input_type = input("Input types of wanted to receive data separated with semicolon\n Your choice: ")
        # TODO
        try:
            option = int(input_type)
            if option == 2:
                self.send_type_is_struct = True
        except ValueError:
            print("Wrong input! Sending default input type - string")


if __name__ == "__main__":
    logging.basicConfig(
        format='%(asctime)s %(levelname)-8s %(message)s',
        datefmt='%Y-%m-%d %H:%M:%S',
        filename=get_project_root() + '/server/log/server.log',
        level=logging.DEBUG
    )
    Client(sys.argv).connect()
