def e2e_p06_protect(
    data: bytearray,
    length: int,
    data_id: int,
    *,
    offset: int = 0,
    increment_counter: bool = True,
) -> None: ...
def e2e_p06_check(
    data: bytes, length: int, data_id: int, *, offset: int = 0
) -> bool: ...
