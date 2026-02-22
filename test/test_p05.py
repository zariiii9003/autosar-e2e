from concurrent.futures import ThreadPoolExecutor
import e2e


def test_e2e_p05_protect():
    # short example
    data = bytearray(b"\x00" * 8)
    data_id = 0x1234

    e2e.p05.e2e_p05_protect(data, data_id, increment_counter=False)
    assert data == bytearray(b"\x1c\xca\x00\x00\x00\x00\x00\x00"), data.hex(sep=" ")

    e2e.p05.e2e_p05_protect(data, data_id, increment_counter=True)
    assert data == bytearray(b"\xcf\x8d\x01\x00\x00\x00\x00\x00"), data.hex(sep=" ")

    # long example (e.g. SOME/IP)
    data = bytearray(b"\x00" * 16)
    data_id = 0x1234
    offset = 8  # bytes

    e2e.p05.e2e_p05_protect(data, data_id, offset=offset, increment_counter=False)
    assert data == bytearray(
        b"\x00\x00\x00\x00\x00\x00\x00\x00\x28\x91\x00\x00\x00\x00\x00\x00"
    ), data.hex(sep=" ")

    e2e.p05.e2e_p05_protect(data, data_id, offset=offset, increment_counter=True)
    assert data == bytearray(
        b"\x00\x00\x00\x00\x00\x00\x00\x00\xfb\xd6\x01\x00\x00\x00\x00\x00"
    ), data.hex(sep=" ")

    # test length argument
    data = bytearray(b"\x00" * 10)
    e2e.p05.e2e_p05_protect(data, data_id, length=8, increment_counter=False)
    assert data == bytearray(b"\x1c\xca\x00\x00\x00\x00\x00\x00\x00\x00"), data.hex(
        sep=" "
    )


def test_e2e_p05_check():
    assert (
        e2e.p05.e2e_p05_check(
            b"\x1c\xca\x00\x00\x00\x00\x00\x00",
            0x1234,
        )
        is True
    )
    assert (
        e2e.p05.e2e_p05_check(
            b"\x1c\xca\x01\x00\x00\x00\x00\x00",
            0x1234,
        )
        is False
    )

    assert (
        e2e.p05.e2e_p05_check(
            b"\x00\x00\x00\x00\x00\x00\x00\x00\x28\x91\x00\x00\x00\x00\x00\x00",
            0x1234,
            offset=8,
        )
        is True
    )
    assert (
        e2e.p05.e2e_p05_check(
            b"\x00\x00\x00\x00\x00\x00\x00\x00\x28\x91\x01\x00\x00\x00\x00\x00",
            0x1234,
            offset=8,
        )
        is False
    )

    # test length argument
    assert (
        e2e.p05.e2e_p05_check(
            b"\x1c\xca\x00\x00\x00\x00\x00\x00\xff",
            0x1234,
            length=8,
        )
        is True
    )


def test_multithreaded():
    tasks = []
    with ThreadPoolExecutor() as pool:
        for _ in range(1000):
            tasks.append(pool.submit(test_e2e_p05_check))
            tasks.append(pool.submit(test_e2e_p05_protect))
        for task in tasks:
            task.result()
