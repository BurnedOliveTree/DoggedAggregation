import configparser


class Host:
    def __init__(self, argv: list):
        parser = configparser.ConfigParser()
        parser.read(".config")
        host, port, ipv = argv
        self.ip_version = ipv if ipv is not None and ipv in [4, 6] else int(parser.get("host", "ip_version"))
        self.host = host if host is not None else self.get_default_host_address(self.ip_version, parser)
        self.port = port if port is not None else int(parser.get("host", "port"))


    @staticmethod
    def get_default_host_address(ip_version: int, parser) -> str:
        if ip_version == 4:
            return parser.get("host", "address_ipv4")
        elif ip_version == 6:
            return parser.get("host", "address_ipv6")
