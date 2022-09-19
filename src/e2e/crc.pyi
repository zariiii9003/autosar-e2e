import typing

def calculate_crc8(
    data: bytes, start_value: int = 0xFF, first_call: bool = True
) -> int: ...
def calculate_crc8_h2f(
    data: bytes, start_value: int = 0xFF, first_call: bool = True
) -> int: ...
def calculate_crc16(
    data: bytes, start_value: int = 0xFFFF, first_call: bool = True
) -> int: ...
def calculate_crc32(
    data: bytes, start_value: int = 0xFFFFFFFF, first_call: bool = True
) -> int: ...
def calculate_crc32_p4(
    data: bytes, start_value: int = 0xFFFFFFFF, first_call: bool = True
) -> int: ...
def calculate_crc64(
    data: bytes, start_value: int = 0xFFFFFFFFFFFFFFFF, first_call: bool = True
) -> int: ...

CRC8_INITIAL_VALUE: typing.Final[int]
CRC8_XOR_VALUE: typing.Final[int]
CRC8_CHECK: typing.Final[int]
CRC8_MAGIC_CHECK: typing.Final[int]

CRC8H2F_INITIAL_VALUE: typing.Final[int]
CRC8H2F_XOR_VALUE: typing.Final[int]
CRC8H2F_CHECK: typing.Final[int]
CRC8H2F_MAGIC_CHECK: typing.Final[int]

CRC16_INITIAL_VALUE: typing.Final[int]
CRC16_XOR_VALUE: typing.Final[int]
CRC16_CHECK: typing.Final[int]
CRC16_MAGIC_CHECK: typing.Final[int]

CRC32_INITIAL_VALUE: typing.Final[int]
CRC32_XOR_VALUE: typing.Final[int]
CRC32_CHECK: typing.Final[int]
CRC32_MAGIC_CHECK: typing.Final[int]

CRC32P4_INITIAL_VALUE: typing.Final[int]
CRC32P4_XOR_VALUE: typing.Final[int]
CRC32P4_CHECK: typing.Final[int]
CRC32P4_MAGIC_CHECK: typing.Final[int]

CRC64_INITIAL_VALUE: typing.Final[int]
CRC64_XOR_VALUE: typing.Final[int]
CRC64_CHECK: typing.Final[int]
CRC64_MAGIC_CHECK: typing.Final[int]
