import e2e


def test_e2e_p07_protect():
    # short example
    data = bytearray(b"\x00" * 24)
    length = len(data)
    data_id = 0x0a0b0c0d

    e2e.p07.e2e_p07_protect(data, length, data_id, increment_counter=False)
    assert data == bytearray(
        b"\x1f\xb2\xe7\x37\xfc\xed\xbc\xd9"
        b"\x00\x00\x00\x18\x00\x00\x00\x00"
        b"\x0a\x0b\x0c\x0d\x00\x00\x00\x00"
    ), data.hex(sep=" ")

    e2e.p07.e2e_p07_protect(data, length, data_id, increment_counter=True)
    assert data == bytearray(
        b"\x7b\xde\x72\x68\xb8\xe9\xbc\x27"
        b"\x00\x00\x00\x18\x00\x00\x00\x01"
        b"\x0a\x0b\x0c\x0d\x00\x00\x00\x00"
    ), data.hex(sep=" ")

    # long example (e.g. SOME/IP)
    data = bytearray(b"\x00" * 32)
    length = len(data)
    data_id = 0x0a0b0c0d
    offset = 8  # bytes

    e2e.p07.e2e_p07_protect(
        data, length, data_id, offset=offset, increment_counter=False
    )
    assert data == bytearray(
        b"\x00\x00\x00\x00\x00\x00\x00\x00"
        b"\x17\xf7\xc8\x17\x32\x38\x65\xa8"
        b"\x00\x00\x00\x20\x00\x00\x00\x00"
        b"\x0a\x0b\x0c\x0d\x00\x00\x00\x00"
    ), data.hex(sep=" ")

    e2e.p07.e2e_p07_protect(
        data, length, data_id, offset=offset, increment_counter=True
    )
    assert data == bytearray(
        b"\x00\x00\x00\x00\x00\x00\x00\x00"
        b"\x73\x9b\x5d\x48\x76\x3c\x65\x56"
        b"\x00\x00\x00\x20\x00\x00\x00\x01"
        b"\x0a\x0b\x0c\x0d\x00\x00\x00\x00"
    ), data.hex(sep=" ")


def test_e2e_p07_check():
    assert (
        e2e.p07.e2e_p07_check(
            b"\x1f\xb2\xe7\x37\xfc\xed\xbc\xd9"
            b"\x00\x00\x00\x18\x00\x00\x00\x00"
            b"\x0a\x0b\x0c\x0d\x00\x00\x00\x00",
            24,
            0x0a0b0c0d,
        )
        is True
    )
    assert (
        e2e.p07.e2e_p07_check(
            b"\x1f\xb2\xe7\x37\xfc\xed\xbc\xd9"
            b"\x00\x00\x00\x18\x00\x00\x00\x00"
            b"\x0a\x0b\x0c\x0d\x00\x00\x00\x01",
            24,
            0x0a0b0c0d,
        )
        is False
    )

    assert (
        e2e.p07.e2e_p07_check(
            b"\x00\x00\x00\x00\x00\x00\x00\x00"
            b"\x17\xf7\xc8\x17\x32\x38\x65\xa8"
            b"\x00\x00\x00\x20\x00\x00\x00\x00"
            b"\x0a\x0b\x0c\x0d\x00\x00\x00\x00",
            32,
            0x0a0b0c0d,
            offset=8,
        )
        is True
    )
    assert (
        e2e.p07.e2e_p07_check(
            b"\x00\x00\x00\x00\x00\x00\x00\x00"
            b"\x17\xf7\xc8\x17\x32\x38\x65\xa8"
            b"\x00\x00\x00\x20\x00\x00\x00\x00"
            b"\x0a\x0b\x0c\x0d\x00\x00\x00\x01",
            32,
            0x0a0b0c0d,
            offset=8,
        )
        is False
    )
