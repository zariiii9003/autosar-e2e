import e2e


def test_e2e_p02_protect():
    data = bytearray(b"\x00" * 8)
    length = len(data) - 1
    data_id_list = b"\x00" * 16

    e2e.p02.e2e_p02_protect(data, length, data_id_list)
    assert b"\x45\x01\x00\x00\x00\x00\x00\x00" == bytes(data)

    data = bytearray(range(8))
    length = len(data) - 1
    data_id_list = bytes(range(16))
    e2e.p02.e2e_p02_protect(data, length, data_id_list)
    assert b"\xbc\x02\x02\x03\x04\x05\x06\x07" == bytes(data)


def test_e2e_p02_check():
    data = bytearray(range(8))
    data[1] = 2
    length = len(data) - 1
    data_id_list = bytes(range(16))
    assert (
        e2e.p02.e2e_p02_check(b"\xbc\x02\x02\x03\x04\x05\x06\x07", length, data_id_list)
        is True
    )
    assert (
        e2e.p02.e2e_p02_check(b"\xbc\x01\x02\x03\x04\x05\x06\x07", length, data_id_list)
        is False
    )
