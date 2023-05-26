Library API
-----------

E2E Profiles
^^^^^^^^^^^^

Profile 01
""""""""""

.. autofunction:: e2e.p01.e2e_p01_protect
.. autofunction:: e2e.p01.e2e_p01_check

.. data:: e2e.p01.E2E_P01_DATAID_BOTH
   :type: typing.Final[int]
   :value: 0x00

.. data:: e2e.p01.E2E_P01_DATAID_ALT
   :type: typing.Final[int]
   :value: 0x01

.. data:: e2e.p01.E2E_P01_DATAID_LOW
   :type: typing.Final[int]
   :value: 0x02

.. data:: e2e.p01.E2E_P01_DATAID_NIBBLE
   :type: typing.Final[int]
   :value: 0x03

Profile 02
""""""""""

.. autofunction:: e2e.p02.e2e_p02_protect
.. autofunction:: e2e.p02.e2e_p02_check

CRC Functions
^^^^^^^^^^^^^

8-bit SAE J1850 CRC Calculation
"""""""""""""""""""""""""""""""

.. autofunction:: e2e.crc.calculate_crc8

.. data:: e2e.crc.CRC8_INITIAL_VALUE
   :type: typing.Final[int]
   :value: 0xFF

.. data:: e2e.crc.CRC8_XOR_VALUE
   :type: typing.Final[int]
   :value: 0xFF

.. data:: e2e.crc.CRC8_CHECK
   :type: typing.Final[int]
   :value: 0x4B

.. data:: e2e.crc.CRC8_MAGIC_CHECK
   :type: typing.Final[int]
   :value: 0xC4

8-bit 0x2F polynomial CRC Calculation
"""""""""""""""""""""""""""""""""""""

.. autofunction:: e2e.crc.calculate_crc8_h2f

.. data:: e2e.crc.CRC8H2F_INITIAL_VALUE
   :type: typing.Final[int]
   :value: 0xFF

.. data:: e2e.crc.CRC8H2F_XOR_VALUE
   :type: typing.Final[int]
   :value: 0xFF

.. data:: e2e.crc.CRC8H2F_CHECK
   :type: typing.Final[int]
   :value: 0xDF

.. data:: e2e.crc.CRC8H2F_MAGIC_CHECK
   :type: typing.Final[int]
   :value: 0x42

16-bit CCITT-FALSE CRC16
""""""""""""""""""""""""

.. autofunction:: e2e.crc.calculate_crc16

.. data:: e2e.crc.CRC16_INITIAL_VALUE
   :type: typing.Final[int]
   :value: 0xFFFF

.. data:: e2e.crc.CRC16_XOR_VALUE
   :type: typing.Final[int]
   :value: 0x0000

.. data:: e2e.crc.CRC16_CHECK
   :type: typing.Final[int]
   :value: 0x29B1

.. data:: e2e.crc.CRC16_MAGIC_CHECK
   :type: typing.Final[int]
   :value: 0x0000

16-bit 0x8005 polynomial CRC calculation
""""""""""""""""""""""""""""""""""""""""

.. autofunction:: e2e.crc.calculate_crc16_arc

.. data:: e2e.crc.CRC16ARC_INITIAL_VALUE
   :type: typing.Final[int]
   :value: 0x0000

.. data:: e2e.crc.CRC16ARC_XOR_VALUE
   :type: typing.Final[int]
   :value: 0x0000

.. data:: e2e.crc.CRC16ARC_CHECK
   :type: typing.Final[int]
   :value: 0xBB3D

.. data:: e2e.crc.CRC16ARC_MAGIC_CHECK
   :type: typing.Final[int]
   :value: 0x0000

32-bit Ethernet CRC Calculation
"""""""""""""""""""""""""""""""

.. autofunction:: e2e.crc.calculate_crc32

.. data:: e2e.crc.CRC32_INITIAL_VALUE
   :type: typing.Final[int]
   :value: 0xFFFFFFFF

.. data:: e2e.crc.CRC32_XOR_VALUE
   :type: typing.Final[int]
   :value: 0xFFFFFFFF

.. data:: e2e.crc.CRC32_CHECK
   :type: typing.Final[int]
   :value: 0xCBF43926

.. data:: e2e.crc.CRC32_MAGIC_CHECK
   :type: typing.Final[int]
   :value: 0xDEBB20E3

32-bit 0xF4ACFB13 polynomial CRC calculation
""""""""""""""""""""""""""""""""""""""""""""

.. autofunction:: e2e.crc.calculate_crc32_p4

.. data:: e2e.crc.CRC32P4_INITIAL_VALUE
   :type: typing.Final[int]
   :value: 0xFFFFFFFF

.. data:: e2e.crc.CRC32P4_XOR_VALUE
   :type: typing.Final[int]
   :value: 0xFFFFFFFF

.. data:: e2e.crc.CRC32P4_CHECK
   :type: typing.Final[int]
   :value: 0x1697D06A

.. data:: e2e.crc.CRC32P4_MAGIC_CHECK
   :type: typing.Final[int]
   :value: 0x904CDDBF

64-bit 0x42F0E1EBA9EA3693 polynomial CRC calculation
""""""""""""""""""""""""""""""""""""""""""""""""""""

.. autofunction:: e2e.crc.calculate_crc64

.. data:: e2e.crc.CRC64_INITIAL_VALUE
   :type: typing.Final[int]
   :value: 0xFFFFFFFFFFFFFFFF

.. data:: e2e.crc.CRC64_XOR_VALUE
   :type: typing.Final[int]
   :value: 0xFFFFFFFFFFFFFFFF

.. data:: e2e.crc.CRC64_CHECK
   :type: typing.Final[int]
   :value: 0x995DC9BBDF1939FA

.. data:: e2e.crc.CRC64_MAGIC_CHECK
   :type: typing.Final[int]
   :value: 0x49958C9ABD7D353F
