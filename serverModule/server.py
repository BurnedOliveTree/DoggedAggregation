import logging
import sys
import os
from socketLib import SocketInterface, Socket, Host, get_project_root
from pynput.keyboard import Key, Listener
from threading import Thread
import signal


class Server(Host):
    def __init__(self, argv: list):
        super().__init__(argv)
        signal.signal(signal.SIGINT, self.__on_sig_int)
        self.server_running = True
        self.types = {
            0: "jpg",
            1: "png",
            2: "txt",
            3: "doc"
        }

    def connect(self) -> None:
        socket = Socket(self.host, self.port)
        with SocketInterface(socket) as socket:
            logging.info(f"Will receive data from {self.host} : {self.port}")
            while self.server_running:
                doc_id, doc_type, status_code, hash_hash, content = socket.read()
                if status_code != 0:
                    logging.info("Received invalid data")
                elif doc_type not in self.types.keys() and doc_type != (self.port % 1000):
                    logging.info("File of wrong format received. Please check your configuration")
                else:
                    filename = f"{doc_id}.{self.types[doc_type]}"
                    logging.info(f'Received data: {filename}')
                    with open(f"data/{filename}", "wb") as file:
                        file.write(content)
                        file.flush()
                        os.fsync(file.fileno())

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
        filename=get_project_root() + '/serverModule/log/server.log',
        level=logging.DEBUG
    )
    server = Server(sys.argv)
    server_listening = Thread(target=server.connect, daemon=True)
    server_listening.start()
    server.get_admin_command()
