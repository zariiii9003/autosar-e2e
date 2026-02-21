import typing

from _typeshed import WriteableBuffer, ReadableBuffer

E2E_P01_DATAID_BOTH: typing.Final[int]
E2E_P01_DATAID_ALT: typing.Final[int]
E2E_P01_DATAID_LOW: typing.Final[int]
E2E_P01_DATAID_NIBBLE: typing.Final[int]

def e2e_p01_protect(
    data: WriteableBuffer,
    data_id: int,
    *,
    data_id_mode: int = E2E_P01_DATAID_BOTH,
    length: int = 0,
    offset: int = 0,
    increment_counter: bool = True,
) -> None: ...
def e2e_p01_check(
    data: ReadableBuffer,
    data_id: int,
    *,
    data_id_mode: int = E2E_P01_DATAID_BOTH,
    length: int = 0,
    offset: int = 0,
) -> bool: ...
