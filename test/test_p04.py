from concurrent.futures import ThreadPoolExecutor
import e2e


def test_e2e_p04_protect():
    # short example
    data = bytearray(b"\x00" * 16)
    data_id = 0x0A0B0C0D

    e2e.p04.e2e_p04_protect(data, data_id, increment_counter=False)
    assert data == bytearray(
        b"\x00\x10\x00\x00\x0a\x0b\x0c\x0d\x86\x2b\x05\x56\x00\x00\x00\x00"
    ), data.hex(sep=" ")

    e2e.p04.e2e_p04_protect(data, data_id, increment_counter=True)
    assert data == bytearray(
        b"\x00\x10\x00\x01\x0a\x0b\x0c\x0d\xa5\x8e\x68\x07\x00\x00\x00\x00"
    ), data.hex(sep=" ")

    # long example (e.g. SOME/IP)
    data = bytearray(b"\x00" * 24)
    data_id = 0x0A0B0C0D
    offset = 8  # bytes

    e2e.p04.e2e_p04_protect(data, data_id, offset=offset, increment_counter=False)
    assert data == bytearray(
        b"\x00\x00\x00\x00\x00\x00\x00\x00"
        b"\x00\x18\x00\x00\x0a\x0b\x0c\x0d"
        b"\x69\xd7\x50\x2e\x00\x00\x00\x00"
    ), data.hex(" ")

    e2e.p04.e2e_p04_protect(data, data_id, offset=offset, increment_counter=True)
    assert data == bytearray(
        b"\x00\x00\x00\x00\x00\x00\x00\x00"
        b"\x00\x18\x00\x01\x0a\x0b\x0c\x0d"
        b"\x4a\x72\x3d\x7f\x00\x00\x00\x00"
    ), data.hex(" ")

    # test length argument
    data = bytearray(b"\x00" * 32)
    e2e.p04.e2e_p04_protect(
        data, data_id, offset=offset, length=24, increment_counter=False
    )
    assert data == bytearray(
        b"\x00\x00\x00\x00\x00\x00\x00\x00"
        b"\x00\x18\x00\x00\x0a\x0b\x0c\x0d"
        b"\x69\xd7\x50\x2e\x00\x00\x00\x00"
        b"\x00\x00\x00\x00\x00\x00\x00\x00"
    ), data.hex(" ")


def test_e2e_p04_check():
    assert (
        e2e.p04.e2e_p04_check(
            b"\x00\x10\x00\x00\x0a\x0b\x0c\x0d\x86\x2b\x05\x56\x00\x00\x00\x00",
            0x0A0B0C0D,
        )
        is True
    )
    assert (
        e2e.p04.e2e_p04_check(
            b"\x00\x10\x00\x01\x0a\x0b\x0c\x0d\x86\x2b\x05\x56\x00\x00\x00\x00",
            0x0A0B0C0D,
        )
        is False
    )

    assert (
        e2e.p04.e2e_p04_check(
            b"\x00\x00\x00\x00\x00\x00\x00\x00"
            b"\x00\x18\x00\x00\x0a\x0b\x0c\x0d"
            b"\x69\xd7\x50\x2e\x00\x00\x00\x00",
            0x0A0B0C0D,
            offset=8,
        )
        is True
    )
    assert (
        e2e.p04.e2e_p04_check(
            b"\x00\x00\x00\x00\x00\x00\x00\x00"
            b"\x00\x18\x00\x01\x0a\x0b\x0c\x0d"
            b"\x69\xd7\x50\x2e\x00\x00\x00\x00",
            0x0A0B0C0D,
            offset=8,
        )
        is False
    )

    # test length argument
    assert (
        e2e.p04.e2e_p04_check(
            b"\x00\x00\x00\x00\x00\x00\x00\x00"
            b"\x00\x18\x00\x00\x0a\x0b\x0c\x0d"
            b"\x69\xd7\x50\x2e\x00\x00\x00\x00"
            b"\x00\x00\x00\x00\x00\x00\x00\x00",
            0x0A0B0C0D,
            length=24,
            offset=8,
        )
        is True
    )


def test_multithreaded():
    tasks = []
    with ThreadPoolExecutor() as pool:
        for _ in range(1000):
            tasks.append(pool.submit(test_e2e_p04_check))
            tasks.append(pool.submit(test_e2e_p04_protect))
        for task in tasks:
            task.result()
