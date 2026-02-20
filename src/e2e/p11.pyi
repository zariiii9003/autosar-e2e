import typing

E2E_P11_DATAID_BOTH: typing.Final[int]
E2E_P11_DATAID_NIBBLE: typing.Final[int]

def e2e_p11_protect(
    data: bytearray,
    data_id: int,
    *,
    data_id_mode: int = E2E_P11_DATAID_BOTH,
    length: int = 0,
    offset: int = 0,
    increment_counter: bool = True,
) -> None: ...
def e2e_p11_check(
    data: bytearray,
    data_id: int,
    *,
    data_id_mode: int = E2E_P11_DATAID_BOTH,
    length: int = 0,
    offset: int = 0,
) -> bool: ...
