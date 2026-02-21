from concurrent.futures import ThreadPoolExecutor
import e2e


def test_e2e_p01_protect_both():
    data = bytearray(b"\x00" * 8)

    # do not increment counter
    e2e.p01.e2e_p01_protect(
        data=data,
        data_id=0x123,
        data_id_mode=e2e.p01.E2E_P01_DATAID_BOTH,
        increment_counter=False,
    )
    assert data == bytearray(b"\xcc\x00\x00\x00\x00\x00\x00\x00"), data.hex(sep=" ")

    # increment counter
    e2e.p01.e2e_p01_protect(
        data=data,
        data_id=0x123,
        data_id_mode=e2e.p01.E2E_P01_DATAID_BOTH,
        increment_counter=True,
    )
    assert data == bytearray(b"\x91\x01\x00\x00\x00\x00\x00\x00"), data.hex(sep=" ")


def test_e2e_p01_protect_alt():
    data = bytearray(b"\x00" * 8)

    # do not increment counter
    e2e.p01.e2e_p01_protect(
        data=data,
        data_id=0x123,
        data_id_mode=e2e.p01.E2E_P01_DATAID_ALT,
        increment_counter=False,
    )
    assert data == bytearray(b"\xce\x00\x00\x00\x00\x00\x00\x00"), data.hex(sep=" ")

    # increment counter
    e2e.p01.e2e_p01_protect(
        data=data,
        data_id=0x123,
        data_id_mode=e2e.p01.E2E_P01_DATAID_ALT,
        increment_counter=True,
    )
    assert data == bytearray(b"\x02\x01\x00\x00\x00\x00\x00\x00"), data.hex(sep=" ")


def test_e2e_p01_protect_low():
    data = bytearray(b"\x00" * 8)

    # do not increment counter
    e2e.p01.e2e_p01_protect(
        data=data,
        data_id=0x123,
        data_id_mode=e2e.p01.E2E_P01_DATAID_LOW,
        increment_counter=False,
    )
    assert data == bytearray(b"\xce\x00\x00\x00\x00\x00\x00\x00"), data.hex(sep=" ")

    # increment counter
    e2e.p01.e2e_p01_protect(
        data=data,
        data_id=0x123,
        data_id_mode=e2e.p01.E2E_P01_DATAID_LOW,
        increment_counter=True,
    )
    assert data == bytearray(b"\x93\x01\x00\x00\x00\x00\x00\x00"), data.hex(sep=" ")


def test_e2e_p01_protect_nibble():
    data = bytearray(b"\x00" * 8)

    # do not increment counter
    e2e.p01.e2e_p01_protect(
        data=data,
        data_id=0x123,
        data_id_mode=e2e.p01.E2E_P01_DATAID_NIBBLE,
        increment_counter=False,
    )
    assert data == bytearray(b"\x2a\x10\x00\x00\x00\x00\x00\x00"), data.hex(sep=" ")

    # increment counter
    e2e.p01.e2e_p01_protect(
        data=data,
        data_id=0x123,
        data_id_mode=e2e.p01.E2E_P01_DATAID_NIBBLE,
        increment_counter=True,
    )
    assert data == bytearray(b"\x77\x11\x00\x00\x00\x00\x00\x00"), data.hex(sep=" ")


def test_e2e_p01_check_both():
    assert (
        e2e.p01.e2e_p01_check(
            data=bytearray(b"\xcc\x00\x00\x00\x00\x00\x00\x00"),
            data_id=0x123,
            data_id_mode=e2e.p01.E2E_P01_DATAID_BOTH,
        )
        is True
    )

    assert (
        e2e.p01.e2e_p01_check(
            data=bytearray(b"\xcc\x10\x00\x00\x00\x00\x00\x00"),
            data_id=0x123,
            data_id_mode=e2e.p01.E2E_P01_DATAID_BOTH,
        )
        is False
    )


def test_e2e_p01_check_alt():
    assert (
        e2e.p01.e2e_p01_check(
            data=bytearray(b"\xce\x00\x00\x00\x00\x00\x00\x00"),
            data_id=0x123,
            data_id_mode=e2e.p01.E2E_P01_DATAID_ALT,
        )
        is True
    )

    assert (
        e2e.p01.e2e_p01_check(
            data=bytearray(b"\xce\x10\x00\x00\x00\x00\x00\x00"),
            data_id=0x123,
            data_id_mode=e2e.p01.E2E_P01_DATAID_ALT,
        )
        is False
    )


def test_e2e_p01_check_low():
    assert (
        e2e.p01.e2e_p01_check(
            data=bytearray(b"\xce\x00\x00\x00\x00\x00\x00\x00"),
            data_id=0x123,
            data_id_mode=e2e.p01.E2E_P01_DATAID_LOW,
        )
        is True
    )

    assert (
        e2e.p01.e2e_p01_check(
            data=bytearray(b"\x93\x11\x00\x00\x00\x00\x00\x00"),
            data_id=0x123,
            data_id_mode=e2e.p01.E2E_P01_DATAID_LOW,
        )
        is False
    )


def test_e2e_p01_check_nibble():
    assert (
        e2e.p01.e2e_p01_check(
            data=bytearray(b"\x2a\x10\x00\x00\x00\x00\x00\x00"),
            data_id=0x123,
            data_id_mode=e2e.p01.E2E_P01_DATAID_NIBBLE,
        )
        is True
    )

    assert (
        e2e.p01.e2e_p01_check(
            data=bytearray(b"\x77\x21\x00\x00\x00\x00\x00\x00"),
            data_id=0x123,
            data_id_mode=e2e.p01.E2E_P01_DATAID_NIBBLE,
        )
        is False
    )


def test_multithreaded():
    tasks = []
    with ThreadPoolExecutor() as pool:
        for _ in range(1000):
            tasks.append(pool.submit(test_e2e_p01_check_both))
            tasks.append(pool.submit(test_e2e_p01_check_alt))
            tasks.append(pool.submit(test_e2e_p01_check_low))
            tasks.append(pool.submit(test_e2e_p01_check_nibble))
            tasks.append(pool.submit(test_e2e_p01_protect_both))
            tasks.append(pool.submit(test_e2e_p01_protect_alt))
            tasks.append(pool.submit(test_e2e_p01_protect_low))
            tasks.append(pool.submit(test_e2e_p01_protect_nibble))
        for task in tasks:
            task.result()


def test_e2e_p01_protect_length_offset():
    data_id = 0x123
    data_id_mode = e2e.p01.E2E_P01_DATAID_NIBBLE

    data = bytearray(b"\x00" * 18)
    length = 16
    offset = 8  # bytes

    # do not increment counter
    e2e.p01.e2e_p01_protect(
        data=data,
        data_id=data_id,
        data_id_mode=data_id_mode,
        length=length,
        offset=offset,
        increment_counter=False,
    )
    assert data == bytearray(
        b"\x00\x00\x00\x00\x00\x00\x00\x00\x7d\x10\x00\x00\x00\x00\x00\x00\x00\x00"
    ), data.hex(sep=" ")

    # increment counter
    e2e.p01.e2e_p01_protect(
        data=data,
        data_id=data_id,
        data_id_mode=data_id_mode,
        length=length,
        offset=offset,
        increment_counter=True,
    )
    assert data == bytearray(
        b"\x00\x00\x00\x00\x00\x00\x00\x00\x20\x11\x00\x00\x00\x00\x00\x00\x00\x00"
    ), data.hex(sep=" ")


def test_e2e_p01_check_length_offset():
    data_id = 0x123
    data_id_mode = e2e.p01.E2E_P01_DATAID_NIBBLE
    length = 16
    offset = 8  # bytes

    assert (
        e2e.p01.e2e_p01_check(
            data=bytearray(
                b"\x00\x00\x00\x00\x00\x00\x00\x00\x7d\x10\x00\x00\x00\x00\x00\x00\xff\xff"
            ),
            data_id=data_id,
            data_id_mode=data_id_mode,
            length=length,
            offset=offset,
        )
        is True
    )

    assert (
        e2e.p01.e2e_p01_check(
            data=bytearray(
                b"\x01\x00\x00\x00\x00\x00\x00\x00\x7d\x10\x00\x00\x00\x00\x00\x00\xff\xff"
            ),
            data_id=data_id,
            data_id_mode=data_id_mode,
            length=length,
            offset=offset,
        )
        is False
    )
