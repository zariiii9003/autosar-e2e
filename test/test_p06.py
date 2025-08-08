from concurrent.futures import ThreadPoolExecutor
import e2e


def test_e2e_p06_protect():
    # short example
    data = bytearray(b"\x00" * 8)
    length = len(data)
    data_id = 0x1234

    e2e.p06.e2e_p06_protect(data, length, data_id, increment_counter=False)
    assert data == bytearray(b"\xb1\x55\x00\x08\x00\x00\x00\x00"), data.hex(sep=" ")

    e2e.p06.e2e_p06_protect(data, length, data_id, increment_counter=True)
    assert data == bytearray(b"\xf4\xf5\x00\x08\x01\x00\x00\x00"), data.hex(sep=" ")

    # long example (e.g. SOME/IP)
    data = bytearray(b"\x00" * 16)
    length = len(data)
    data_id = 0x1234
    offset = 8  # bytes

    e2e.p06.e2e_p06_protect(
        data, length, data_id, offset=offset, increment_counter=False
    )
    assert data == bytearray(
        b"\x00\x00\x00\x00\x00\x00\x00\x00\x4e\xb7\x00\x10\x00\x00\x00\x00"
    ), data.hex(sep=" ")

    e2e.p06.e2e_p06_protect(
        data, length, data_id, offset=offset, increment_counter=True
    )
    assert data == bytearray(
        b"\x00\x00\x00\x00\x00\x00\x00\x00\x0b\x17\x00\x10\x01\x00\x00\x00"
    ), data.hex(sep=" ")


def test_e2e_p06_check():
    assert (
        e2e.p06.e2e_p06_check(
            b"\xb1\x55\x00\x08\x00\x00\x00\x00",
            8,
            0x01234,
        )
        is True
    )
    assert (
        e2e.p06.e2e_p06_check(
            b"\xb1\x55\x00\x08\x00\x00\x00\x01",
            8,
            0x01234,
        )
        is False
    )

    assert (
        e2e.p06.e2e_p06_check(
            b"\x00\x00\x00\x00\x00\x00\x00\x00\x4e\xb7\x00\x10\x00\x00\x00\x00",
            16,
            0x1234,
            offset=8,
        )
        is True
    )
    assert (
        e2e.p06.e2e_p06_check(
            b"\x00\x00\x00\x00\x00\x00\x00\x00\x4e\xb7\x00\x10\x00\x00\x00\x01",
            16,
            0x1234,
            offset=8,
        )
        is False
    )


def test_multithreaded():
    tasks = []
    with ThreadPoolExecutor() as pool:
        for _ in range(1000):
            tasks.append(pool.submit(test_e2e_p06_check))
            tasks.append(pool.submit(test_e2e_p06_protect))
        for task in tasks:
            task.result()
