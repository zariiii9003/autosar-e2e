from concurrent.futures import ThreadPoolExecutor

import e2e


def test_e2e_p02_protect():
    data = bytearray(b"\x00" * 8)
    data_id_list = b"\x00" * 16

    e2e.p02.e2e_p02_protect(data, data_id_list)
    assert b"\x45\x01\x00\x00\x00\x00\x00\x00" == bytes(data), data.hex(sep=" ")

    data = bytearray(range(8))
    data_id_list = bytes(range(16))
    e2e.p02.e2e_p02_protect(data, data_id_list)
    assert b"\xbc\x02\x02\x03\x04\x05\x06\x07" == bytes(data), data.hex(sep=" ")

    data = bytearray(range(8))
    data_id_list = bytes(range(16))
    e2e.p02.e2e_p02_protect(data, data_id_list, increment_counter=False)
    assert b"\x61\x01\x02\x03\x04\x05\x06\x07" == bytes(data), data.hex(sep=" ")

    # test length argument
    data = bytearray(b"\x00" * 16)
    data_id_list = b"\x00" * 16
    e2e.p02.e2e_p02_protect(data, data_id_list, length=8)
    assert b"\x45\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" == bytes(
        data
    ), data.hex(sep=" ")


def test_e2e_p02_protect_increment():
    data = bytearray(8)
    data_id_list = bytes(range(1, 17))

    e2e.p02.e2e_p02_protect(data, data_id_list, increment_counter=True)  # counter=01
    assert b"\x1b\x01\x00\x00\x00\x00\x00\x00" == bytes(data), data.hex(sep=" ")
    e2e.p02.e2e_p02_protect(data, data_id_list, increment_counter=True)  # counter=02
    assert b"\x98\x02\x00\x00\x00\x00\x00\x00" == bytes(data), data.hex(sep=" ")
    e2e.p02.e2e_p02_protect(data, data_id_list, increment_counter=True)  # counter=03
    assert b"\x31\x03\x00\x00\x00\x00\x00\x00" == bytes(data), data.hex(sep=" ")
    e2e.p02.e2e_p02_protect(data, data_id_list, increment_counter=True)  # counter=04
    assert b"\x0d\x04\x00\x00\x00\x00\x00\x00" == bytes(data), data.hex(sep=" ")
    e2e.p02.e2e_p02_protect(data, data_id_list, increment_counter=True)  # counter=05
    assert b"\x18\x05\x00\x00\x00\x00\x00\x00" == bytes(data), data.hex(sep=" ")
    e2e.p02.e2e_p02_protect(data, data_id_list, increment_counter=True)  # counter=06
    assert b"\x9b\x06\x00\x00\x00\x00\x00\x00" == bytes(data), data.hex(sep=" ")
    e2e.p02.e2e_p02_protect(data, data_id_list, increment_counter=True)  # counter=07
    assert b"\x65\x07\x00\x00\x00\x00\x00\x00" == bytes(data), data.hex(sep=" ")
    e2e.p02.e2e_p02_protect(data, data_id_list, increment_counter=True)  # counter=08
    assert b"\x08\x08\x00\x00\x00\x00\x00\x00" == bytes(data), data.hex(sep=" ")
    e2e.p02.e2e_p02_protect(data, data_id_list, increment_counter=True)  # counter=09
    assert b"\x1d\x09\x00\x00\x00\x00\x00\x00" == bytes(data), data.hex(sep=" ")
    e2e.p02.e2e_p02_protect(data, data_id_list, increment_counter=True)  # counter=10
    assert b"\x9e\x0a\x00\x00\x00\x00\x00\x00" == bytes(data), data.hex(sep=" ")
    e2e.p02.e2e_p02_protect(data, data_id_list, increment_counter=True)  # counter=11
    assert b"\x37\x0b\x00\x00\x00\x00\x00\x00" == bytes(data), data.hex(sep=" ")
    e2e.p02.e2e_p02_protect(data, data_id_list, increment_counter=True)  # counter=12
    assert b"\x0b\x0c\x00\x00\x00\x00\x00\x00" == bytes(data), data.hex(sep=" ")
    e2e.p02.e2e_p02_protect(data, data_id_list, increment_counter=True)  # counter=13
    assert b"\x1e\x0d\x00\x00\x00\x00\x00\x00" == bytes(data), data.hex(sep=" ")
    e2e.p02.e2e_p02_protect(data, data_id_list, increment_counter=True)  # counter=14
    assert b"\x9d\x0e\x00\x00\x00\x00\x00\x00" == bytes(data), data.hex(sep=" ")
    e2e.p02.e2e_p02_protect(data, data_id_list, increment_counter=True)  # counter=15
    assert b"\xcd\x0f\x00\x00\x00\x00\x00\x00" == bytes(data), data.hex(sep=" ")
    e2e.p02.e2e_p02_protect(data, data_id_list, increment_counter=True)  # counter=00
    assert b"\x0e\x00\x00\x00\x00\x00\x00\x00" == bytes(data), data.hex(sep=" ")


def test_e2e_p02_check():
    data = bytearray(range(8))
    data[1] = 2
    data_id_list = bytes(range(16))
    assert (
        e2e.p02.e2e_p02_check(b"\xbc\x02\x02\x03\x04\x05\x06\x07", data_id_list) is True
    )
    assert (
        e2e.p02.e2e_p02_check(b"\xbc\x01\x02\x03\x04\x05\x06\x07", data_id_list)
        is False
    )

    # test length argument
    data = bytearray(range(16))
    data[1] = 2
    data_id_list = bytes(range(16))
    assert (
        e2e.p02.e2e_p02_check(
            b"\xbc\x02\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0f",
            data_id_list,
            length=8,
        )
        is True
    )


def test_multithreaded():
    tasks = []
    with ThreadPoolExecutor() as pool:
        for _ in range(1000):
            tasks.append(pool.submit(test_e2e_p02_check))
            tasks.append(pool.submit(test_e2e_p02_protect))
        for task in tasks:
            task.result()
