import configparser
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
        parser = configparser.ConfigParser()
        parser.read(get_project_root() + "/serverModule/.config")
        self.server_running = True
        self.data_path = parser.get("server", "data_path")
        self.file_type = parser.get("server", "file_type")
        self.file_type_descriptor = parser.get("server", "file_type_descriptor")
        if self.file_type_descriptor != (self.port % 1000):
            logging.error("File of wrong format received. Please check your configuration")

    def connect(self) -> None:
        socket = Socket(self.host, self.port)
        with SocketInterface(socket) as socket:
            logging.info(f"Will receive data from {self.host} : {self.port}")
            while self.server_running:
                file_id, file_type_descriptor, status_code, hash_hash, content = socket.read()
                if status_code != 0:
                    logging.info("Received invalid data")
                elif file_type_descriptor != self.file_type_descriptor:
                    logging.info("File of wrong format received. Please check your configuration")
                else:
                    file_path = f"{self.data_path}{file_id}.{self.file_type}"
                    logging.info(f'Received data: {file_path}')
                    with open(f"data/{file_path}", "wb") as file:
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
