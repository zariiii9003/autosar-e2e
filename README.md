# autosar-e2e

[![PyPI - Version](https://img.shields.io/pypi/v/autosar-e2e.svg)](https://pypi.org/project/autosar-e2e)
[![PyPI - Python Version](https://img.shields.io/pypi/pyversions/autosar-e2e.svg)](https://pypi.org/project/autosar-e2e)

-----

**Table of Contents**

- [Description](#description)
- [Installation](#installation)
- [Usage](#usage)
- [Test](#test)
- [Build](#build)
- [License](#license)

## Description

This library provides fast C implementations of the E2E CRC algorithms and E2E profiles. 

Currently all relevant CRC algorithms are available in module `e2e.crc`
but only E2E profile 2 is available. If you provide example data for the other profiles i would try to implement them, too.

## Installation

```console
pip install autosar-e2e
```

## Usage

### CRC example
```python3
import e2e
crc: int = e2e.crc.calculate_crc8_h2f(b"\x00\x00\x00\x00")
```

### E2E Profile 2
```python3
import e2e
# create data
data = bytearray(b"\x00" *  8)
length = len(data) - 1
data_id_list = b"\x00" * 16
# increment counter and calculate CRC inplace
e2e.p02.e2e_p02_protect(data, length, data_id_list, increment_counter=True)
# check CRC
crc_correct: bool = e2e.p02.e2e_p02_check(data, length, data_id_list)
```

## Test

```console
pip install tox
tox
```

## Build

```console
pip install build
python -m build .
```

## License

`autosar-e2e` is distributed under the terms of the [MIT](https://spdx.org/licenses/MIT.html) license.
