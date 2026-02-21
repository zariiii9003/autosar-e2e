from _typeshed import WriteableBuffer, ReadableBuffer

def e2e_p02_protect(
    data: WriteableBuffer,
    data_id_list: bytes,
    *,
    length: int = 0,
    increment_counter: bool = True,
) -> None: ...
def e2e_p02_check(
    data: ReadableBuffer,
    data_id_list: bytes,
    *,
    length: int = 0,
) -> bool: ...
