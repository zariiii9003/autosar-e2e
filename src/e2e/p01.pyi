import typing

E2E_P01_DATAID_BOTH: typing.Final[int]
E2E_P01_DATAID_ALT: typing.Final[int]
E2E_P01_DATAID_LOW: typing.Final[int]
E2E_P01_DATAID_NIBBLE: typing.Final[int]

def e2e_p01_protect(
    data: bytearray,
    length: int,
    data_id: int,
    *,
    data_id_mode: int = E2E_P01_DATAID_BOTH,
    increment_counter: bool = True,
) -> None: ...
def e2e_p01_check(
    data: bytearray,
    length: int,
    data_id: int,
    *,
    data_id_mode: int = E2E_P01_DATAID_BOTH,
) -> bool: ...
