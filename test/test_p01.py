import e2e


def test_e2e_p01_protect_both():
    data = bytearray(b"\x00" * 8)

    # do not increment counter
    e2e.p01.e2e_p01_protect(
        data=data,
        data_id=0x123,
        length=7,
        data_id_mode=e2e.p01.E2E_P01_DATAID_BOTH,
        increment_counter=False,
    )
    assert data == bytearray(b"\xcc\x00\x00\x00\x00\x00\x00\x00"), data

    # increment counter
    e2e.p01.e2e_p01_protect(
        data=data,
        data_id=0x123,
        length=7,
        data_id_mode=e2e.p01.E2E_P01_DATAID_BOTH,
        increment_counter=True,
    )
    assert data == bytearray(b"\x91\x01\x00\x00\x00\x00\x00\x00"), data


def test_e2e_p01_protect_alt():
    data = bytearray(b"\x00" * 8)

    # do not increment counter
    e2e.p01.e2e_p01_protect(
        data=data,
        data_id=0x123,
        length=7,
        data_id_mode=e2e.p01.E2E_P01_DATAID_ALT,
        increment_counter=False,
    )
    assert data == bytearray(b"\xce\x00\x00\x00\x00\x00\x00\x00"), data

    # increment counter
    e2e.p01.e2e_p01_protect(
        data=data,
        data_id=0x123,
        length=7,
        data_id_mode=e2e.p01.E2E_P01_DATAID_ALT,
        increment_counter=True,
    )
    assert data == bytearray(b"\x02\x01\x00\x00\x00\x00\x00\x00"), data


def test_e2e_p01_protect_low():
    data = bytearray(b"\x00" * 8)

    # do not increment counter
    e2e.p01.e2e_p01_protect(
        data=data,
        data_id=0x123,
        length=7,
        data_id_mode=e2e.p01.E2E_P01_DATAID_LOW,
        increment_counter=False,
    )
    assert data == bytearray(b"\xce\x00\x00\x00\x00\x00\x00\x00"), data

    # increment counter
    e2e.p01.e2e_p01_protect(
        data=data,
        data_id=0x123,
        length=7,
        data_id_mode=e2e.p01.E2E_P01_DATAID_LOW,
        increment_counter=True,
    )
    assert data == bytearray(b"\x93\x01\x00\x00\x00\x00\x00\x00"), data


def test_e2e_p01_protect_nibble():
    data = bytearray(b"\x00" * 8)

    # do not increment counter
    e2e.p01.e2e_p01_protect(
        data=data,
        data_id=0x123,
        length=7,
        data_id_mode=e2e.p01.E2E_P01_DATAID_NIBBLE,
        increment_counter=False,
    )
    assert data == bytearray(b"\x2a\x10\x00\x00\x00\x00\x00\x00"), data

    # increment counter
    e2e.p01.e2e_p01_protect(
        data=data,
        data_id=0x123,
        length=7,
        data_id_mode=e2e.p01.E2E_P01_DATAID_NIBBLE,
        increment_counter=True,
    )
    assert data == bytearray(b"\x77\x11\x00\x00\x00\x00\x00\x00"), data


def test_e2e_p01_check_both():
    assert (
        e2e.p01.e2e_p01_check(
            data=bytearray(b"\xcc\x00\x00\x00\x00\x00\x00\x00"),
            data_id=0x123,
            length=7,
            data_id_mode=e2e.p01.E2E_P01_DATAID_BOTH,
        )
        is True
    )

    assert (
        e2e.p01.e2e_p01_check(
            data=bytearray(b"\xcc\x10\x00\x00\x00\x00\x00\x00"),
            data_id=0x123,
            length=7,
            data_id_mode=e2e.p01.E2E_P01_DATAID_BOTH,
        )
        is False
    )


def test_e2e_p01_check_alt():
    assert (
        e2e.p01.e2e_p01_check(
            data=bytearray(b"\xce\x00\x00\x00\x00\x00\x00\x00"),
            data_id=0x123,
            length=7,
            data_id_mode=e2e.p01.E2E_P01_DATAID_ALT,
        )
        is True
    )

    assert (
        e2e.p01.e2e_p01_check(
            data=bytearray(b"\xce\x10\x00\x00\x00\x00\x00\x00"),
            data_id=0x123,
            length=7,
            data_id_mode=e2e.p01.E2E_P01_DATAID_ALT,
        )
        is False
    )


def test_e2e_p01_check_low():
    assert (
        e2e.p01.e2e_p01_check(
            data=bytearray(b"\xce\x00\x00\x00\x00\x00\x00\x00"),
            data_id=0x123,
            length=7,
            data_id_mode=e2e.p01.E2E_P01_DATAID_LOW,
        )
        is True
    )

    assert (
        e2e.p01.e2e_p01_check(
            data=bytearray(b"\x93\x11\x00\x00\x00\x00\x00\x00"),
            data_id=0x123,
            length=7,
            data_id_mode=e2e.p01.E2E_P01_DATAID_LOW,
        )
        is False
    )


def test_e2e_p01_check_nibble():
    assert (
        e2e.p01.e2e_p01_check(
            data=bytearray(b"\x2a\x10\x00\x00\x00\x00\x00\x00"),
            data_id=0x123,
            length=7,
            data_id_mode=e2e.p01.E2E_P01_DATAID_NIBBLE,
        )
        is True
    )

    assert (
        e2e.p01.e2e_p01_check(
            data=bytearray(b"\x77\x21\x00\x00\x00\x00\x00\x00"),
            data_id=0x123,
            length=7,
            data_id_mode=e2e.p01.E2E_P01_DATAID_NIBBLE,
        )
        is False
    )
