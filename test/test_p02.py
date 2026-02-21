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
