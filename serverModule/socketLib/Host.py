<<<<<<< HEAD:server/socketLib/Host.py
from argparse import ArgumentParser
from subprocess import check_output


class Host:
    def __init__(self, argv: list):
        host, port, ipv = self.__parse_args(argv)
        self.ip_version = ipv if ipv is not None and ipv in [4, 6] else 4
        self.host = host if host is not None else self.get_default_host_address(self.ip_version)
        self.port = port if port is not None else 8000
    
    @staticmethod
    def __parse_args(argv):
        parser = ArgumentParser(description=f'Run a socket-using server/client in python')
        parser.add_argument('-a', '--address', type=str, help='the ip address of server')
        parser.add_argument('-p', '--port', type=int, help='the port of server')
        parser.add_argument('-v', '--ipv', type=int, help='the ip version used in communication')
        args = parser.parse_args(argv[1:])
        return args.address, args.port, args.ipv
    
    @staticmethod
    def get_default_host_address(ip_version: int) -> str:
        if ip_version == 4:
            return "127.0.0.1"
        elif ip_version == 6:
            return "::1"

def get_project_root() -> str:
    return check_output(['git', 'rev-parse', '--show-toplevel']).decode('ascii').strip()
=======
import configparser
from argparse import ArgumentParser


class Host:
    def __init__(self, argv: list):
        parser = configparser.ConfigParser()
        parser.read(".config")
        host, port, ipv = self.__parse_args(argv)
        self.ip_version = ipv if ipv is not None and ipv in [4, 6] else int(parser.get("host", "ip_version"))
        self.host = host if host is not None else self.get_default_host_address(self.ip_version, parser)
        self.port = port if port is not None else int(parser.get("host", "port"))

    @staticmethod
    def __parse_args(argv):
        parser = ArgumentParser(description=f'Run a socket-using serverModule/client in python')
        parser.add_argument('-a', '--address', type=str, help='the ip address of serverModule')
        parser.add_argument('-p', '--port', type=int, help='the port of serverModule')
        parser.add_argument('-v', '--ipv', type=int, help='the ip version used in communication')
        args = parser.parse_args(argv[1:])
        return args.address, args.port, args.ipv

    @staticmethod
    def get_default_host_address(ip_version: int, parser) -> str:
        if ip_version == 4:
            return parser.get("host", "address_ipv4")
        elif ip_version == 6:
            return parser.get("host", "address_ipv6")
>>>>>>> master:serverModule/socketLib/Host.py
