autosar-e2e
===========

.. only:: html

   .. image:: https://img.shields.io/pypi/v/autosar-e2e.svg
       :target: https://pypi.org/project/autosar-e2e
       :alt: PyPI - Version
   .. image:: https://img.shields.io/pypi/pyversions/autosar-e2e.svg
       :target: https://pypi.org/project/autosar-e2e
       :alt: PyPI - Python Version
   .. image:: https://readthedocs.org/projects/autosar-e2e/badge/?version=latest
       :target: https://autosar-e2e.readthedocs.io/en/latest/?badge=latest
       :alt: Documentation Status
   .. image:: https://github.com/zariiii9003/autosar-e2e/actions/workflows/test.yml/badge.svg
       :target: https://github.com/zariiii9003/autosar-e2e/actions
       :alt: Github Actions

.. toctree::
   :maxdepth: 3

   self
   api

Description
-----------

This library provides fast C implementations of the E2E CRC algorithms and E2E profiles.

Currently all relevant CRC algorithms are available in module `e2e.crc`
but only E2E profile 2 is available. If you provide example data for other profiles i would try to implement them, too.

Installation
------------
You can install ``autosar-e2e`` from `PyPI`_::

   python -m pip install autosar-e2e

Usage
-----

.. code-block::
    :caption: CRC example

    import e2e

    crc: int = e2e.crc.calculate_crc8_h2f(b"\x00\x00\x00\x00")

.. code-block::
    :caption: E2E P02 example

    import e2e

    # create data
    data = bytearray(b"\x00" *  8)
    length = len(data) - 1
    data_id_list = b"\x00" * 16

    # increment counter and calculate CRC inplace
    e2e.p02.e2e_p02_protect(data, length, data_id_list, increment_counter=True)

    # check CRC
    crc_correct: bool = e2e.p02.e2e_p02_check(data, length, data_id_list)


Test
----

Run the tests with::

   pip install pipx
   pipx run tox


Build
-----

Build ``autosar-e2e`` with::

    pip install pipx
    pipx run build
    pipx run twine check dist/*


License
-------

``autosar-e2e`` is distributed under the terms of the `MIT`_ license.


.. _PyPI: https://pypi.org/project/autosar-e2e/
.. _MIT: https://spdx.org/licenses/MIT.html
