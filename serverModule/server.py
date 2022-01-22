import configparser
import logging
import os

from FileType import FileType
from socketLib import SocketInterface, Socket, Host
from pynput.keyboard import Key, Listener
import signal

class Server(Host):
    def __init__(self, argv: list):
        super().__init__(argv)
        signal.signal(signal.SIGINT, self.__on_sig_int)
        parser = configparser.ConfigParser()
        parser.read(".config")
        self.server_running = True
        self.data_path = parser.get("server", "data_path")
        file_type_descriptor = int(parser.get("server", "file_type_descriptor"))
        self.file_type = FileType(file_type_descriptor)
        if file_type_descriptor != ((self.port % 1000) +1):
            logging.error("File of wrong format will be received Please check your configuration")

    def connect(self) -> None:
        socket = Socket(self.host, self.port)
        with SocketInterface(socket) as socket:
            logging.info(f"Will receive data from {self.host} : {self.port}")
            while self.server_running:
                file_id, file_type_descriptor, status_code, hash_hash, content = socket.read()
                if status_code != 0:
                    logging.info("Received invalid data")
                elif file_type_descriptor != self.file_type.value:
                    logging.info("File of wrong format received. Please check your configuration")
                else:
                    file_path = f"{self.data_path}{file_id}_{self.file_type.name}.txt"
                    logging.info(f'Received data: {file_path}')
                    with open(f"{file_path}", "wb+") as file:
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