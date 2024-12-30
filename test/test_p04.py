import e2e


def test_e2e_p04_protect():
    # short example
    data = bytearray(b"\x00" * 16)
    length = len(data)
    data_id = 0x0A0B0C0D

    e2e.p04.e2e_p04_protect(data, length, data_id, increment_counter=False)
    assert data == bytearray(
        b"\x00\x10\x00\x00\x0a\x0b\x0c\x0d\x86\x2b\x05\x56\x00\x00\x00\x00"
    ), data.hex(sep=" ")

    e2e.p04.e2e_p04_protect(data, length, data_id, increment_counter=True)
    assert data == bytearray(
        b"\x00\x10\x00\x01\x0a\x0b\x0c\x0d\xa5\x8e\x68\x07\x00\x00\x00\x00"
    ), data.hex(sep=" ")

    # long example (e.g. SOME/IP)
    data = bytearray(b"\x00" * 24)
    length = len(data)
    data_id = 0x0A0B0C0D
    offset = 8  # bytes

    e2e.p04.e2e_p04_protect(
        data, length, data_id, offset=offset, increment_counter=False
    )
    assert data == bytearray(
        b"\x00\x00\x00\x00\x00\x00\x00\x00"
        b"\x00\x18\x00\x00\x0a\x0b\x0c\x0d"
        b"\x69\xd7\x50\x2e\x00\x00\x00\x00"
    ), data.hex(" ")

    e2e.p04.e2e_p04_protect(
        data, length, data_id, offset=offset, increment_counter=True
    )
    assert data == bytearray(
        b"\x00\x00\x00\x00\x00\x00\x00\x00"
        b"\x00\x18\x00\x01\x0a\x0b\x0c\x0d"
        b"\x4a\x72\x3d\x7f\x00\x00\x00\x00"
    ), data.hex(" ")


def test_e2e_p04_check():
    assert (
        e2e.p04.e2e_p04_check(
            b"\x00\x10\x00\x00\x0a\x0b\x0c\x0d\x86\x2b\x05\x56\x00\x00\x00\x00",
            16,
            0x0A0B0C0D,
        )
        is True
    )
    assert (
        e2e.p04.e2e_p04_check(
            b"\x00\x10\x00\x01\x0a\x0b\x0c\x0d\x86\x2b\x05\x56\x00\x00\x00\x00",
            16,
            0x0A0B0C0D,
        )
        is False
    )

    assert (
        e2e.p04.e2e_p04_check(
            b"\x00\x00\x00\x00\x00\x00\x00\x00"
            b"\x00\x18\x00\x00\x0a\x0b\x0c\x0d"
            b"\x69\xd7\x50\x2e\x00\x00\x00\x00",
            24,
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
            24,
            0x0A0B0C0D,
            offset=8,
        )
        is False
    )
