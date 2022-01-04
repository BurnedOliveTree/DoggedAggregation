import logging
import sys
from socketLib import SocketInterface, Socket, Host, get_project_root
from pynput.keyboard import Key, Listener
from threading import Thread
import signal


class Server(Host):
    def __init__(self, argv: list):
        super().__init__(argv)
        signal.signal(signal.SIGINT, self.__on_sig_int)
        self.server_running = True

    def connect(self) -> None:
        socket = Socket(self.host, self.port)
        with SocketInterface(socket) as socket:
            print("Will receive data from ", self.host, ":", self.port)
            while self.server_running:
                filename, content = socket.read()
                print(f'Received data: {filename}')
                with open(filename, "wb") as file:
                    file.write(content)

    def __on_release(self, key):
        if key == Key.esc:
            self.server_running = False
        return self.server_running

    def __on_sig_int(self, signum, frame):
        signal.signal(signum, signal.SIG_IGN)
        self.server_running = False

    def get_admin_command(self):
        with Listener(on_release=self.__on_release) as listener:
            listener.join()


if __name__ == "__main__":
    logging.basicConfig(
        format='%(asctime)s %(levelname)-8s %(message)s',
        datefmt='%Y-%m-%d %H:%M:%S',
        filename=get_project_root() + '/log/client.log',
        level=logging.DEBUG
    )
    server = Server(sys.argv)
    server_listening = Thread(target=server.connect, daemon=True)
    server_listening.start()
    server.get_admin_command()
