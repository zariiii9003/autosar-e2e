from _typeshed import WriteableBuffer, ReadableBuffer

def e2e_p07_protect(
    data: WriteableBuffer,
    data_id: int,
    *,
    length: int = 0,
    offset: int = 0,
    increment_counter: bool = True,
) -> None: ...
def e2e_p07_check(
    data: ReadableBuffer,
    data_id: int,
    *,
    length: int = 0,
    offset: int = 0,
) -> bool: ...
