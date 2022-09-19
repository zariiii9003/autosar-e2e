# SPDX-FileCopyrightText: 2022-present Artur Drogunow <artur.drogunow@zf.com>
#
# SPDX-License-Identifier: MIT

from pathlib import Path
from setuptools import Extension, setup

import_pkg_name = "e2e"
pkg_dir = Path("src") / import_pkg_name

crc_module = Extension(
    f"{import_pkg_name}.crc",
    sources=[str(pkg_dir / "crc.c")],
    include_dirs=[pkg_dir.as_posix()],
)

p02_module = Extension(
    f"{import_pkg_name}.p02",
    sources=[
        str(pkg_dir / "p02.c"),
        str(pkg_dir / "crc.c"),
    ],
    include_dirs=[pkg_dir.as_posix()],
)

setup(
    ext_modules=[
        crc_module,
        p02_module,
    ],
)
