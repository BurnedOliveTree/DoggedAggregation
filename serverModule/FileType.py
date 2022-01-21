from enum import Enum


class FileType(Enum):
    jpg = 0
    png = 1
    txt = 2
    doc = 3

    def __str__(self):
        return self.name
