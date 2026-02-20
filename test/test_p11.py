from concurrent.futures import ThreadPoolExecutor
import e2e


def test_e2e_p11_protect_both():
    data = bytearray(b"\x00" * 8)
    data_id = 0x123
    data_id_mode = e2e.p11.E2E_P11_DATAID_BOTH

    # do not increment counter
    e2e.p11.e2e_p11_protect(
        data=data,
        data_id=data_id,
        data_id_mode=data_id_mode,
        increment_counter=False,
    )
    assert data == bytearray(b"\xcc\x00\x00\x00\x00\x00\x00\x00"), data

    # increment counter
    e2e.p11.e2e_p11_protect(
        data=data,
        data_id=data_id,
        data_id_mode=data_id_mode,
        increment_counter=True,
    )
    assert data == bytearray(b"\x91\x01\x00\x00\x00\x00\x00\x00"), data


def test_e2e_p11_protect_nibble():
    data = bytearray(b"\x00" * 8)
    data_id = 0x123
    data_id_mode = e2e.p11.E2E_P11_DATAID_NIBBLE

    # do not increment counter
    e2e.p11.e2e_p11_protect(
        data=data,
        data_id=data_id,
        data_id_mode=data_id_mode,
        increment_counter=False,
    )
    assert data == bytearray(b"\x2a\x10\x00\x00\x00\x00\x00\x00"), data

    # increment counter
    e2e.p11.e2e_p11_protect(
        data=data,
        data_id=data_id,
        data_id_mode=data_id_mode,
        increment_counter=True,
    )
    assert data == bytearray(b"\x77\x11\x00\x00\x00\x00\x00\x00"), data

    # long example (e.g. SOME/IP)
    data = bytearray(b"\x00" * 16)
    length = len(data)
    data_id = 0x123
    offset = 8  # bytes

    e2e.p11.e2e_p11_protect(
        data,
        data_id,
        data_id_mode=data_id_mode,
        length=length,
        offset=offset,
        increment_counter=False,
    )
    assert data == bytearray(
        b"\x00\x00\x00\x00\x00\x00\x00\x00\x7d\x10\x00\x00\x00\x00\x00\x00"
    ), data.hex(sep=" ")

    e2e.p11.e2e_p11_protect(
        data, data_id, offset=offset, length=length, increment_counter=True
    )
    assert data == bytearray(
        b"\x00\x00\x00\x00\x00\x00\x00\x00\xa5\x11\x00\x00\x00\x00\x00\x00"
    ), data.hex(sep=" ")


def test_e2e_p11_check_both():
    data_id = 0x123
    data_id_mode = e2e.p11.E2E_P11_DATAID_BOTH

    data = bytearray(b"\xcc\x00\x00\x00\x00\x00\x00\x00")
    assert (
        e2e.p11.e2e_p11_check(
            data=data,
            data_id=data_id,
            data_id_mode=data_id_mode,
        )
        is True
    )

    data = bytearray(b"\xcc\x10\x00\x00\x00\x00\x00\x00")
    assert (
        e2e.p11.e2e_p11_check(
            data=data,
            data_id=data_id,
            data_id_mode=data_id_mode,
        )
        is False
    )


def test_e2e_p11_check_nibble():
    data_id = 0x123
    data_id_mode = e2e.p11.E2E_P11_DATAID_NIBBLE

    data = bytearray(b"\x2a\x10\x00\x00\x00\x00\x00\x00")
    assert (
        e2e.p11.e2e_p11_check(
            data=data,
            data_id=data_id,
            data_id_mode=data_id_mode,
        )
        is True
    )

    data = bytearray(b"\x77\x21\x00\x00\x00\x00\x00\x00")
    assert (
        e2e.p11.e2e_p11_check(
            data=data,
            data_id=data_id,
            data_id_mode=data_id_mode,
        )
        is False
    )

    # long example (e.g. SOME/IP)
    data_id = 0x123
    offset = 8  # bytes

    data = bytearray(
        b"\x00\x00\x00\x00\x00\x00\x00\x00\x7d\x10\x00\x00\x00\x00\x00\x00"
    )
    length = len(data)
    assert (
        e2e.p11.e2e_p11_check(
            data=data,
            data_id=data_id,
            length=length,
            data_id_mode=data_id_mode,
            offset=offset,
        )
        is True
    )

    data = bytearray(
        b"\x00\x00\x00\x00\x00\x00\x00\x00\x7d\x11\x00\x00\x00\x00\x00\x00"
    )
    length = len(data)
    assert (
        e2e.p11.e2e_p11_check(
            data=data,
            data_id=data_id,
            length=length,
            data_id_mode=data_id_mode,
            offset=offset,
        )
        is False
    )


def test_multithreaded():
    tasks = []
    with ThreadPoolExecutor() as pool:
        for _ in range(1000):
            tasks.append(pool.submit(test_e2e_p11_check_both))
            tasks.append(pool.submit(test_e2e_p11_check_nibble))
            tasks.append(pool.submit(test_e2e_p11_protect_both))
            tasks.append(pool.submit(test_e2e_p11_protect_nibble))
        for task in tasks:
            task.result()
