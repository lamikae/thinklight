# -*- encoding: utf-8 -*-
from distutils.core import setup, Extension

libthinklight = Extension("thinklight/libthinklight",
    sources = ["libthinklight/thinklight.c"],
    libraries=["usb"],
    extra_compile_args=["-fPIC"]
    )

setup(
    name="thinklight",
    scripts=['bin/thinklight'],
    version="1.0.0",
    license="GPL-3",
    maintainer="lamikae",
    maintainer_email="mikael.lammentausta+github@gmail.com",
    description="Novelty gadget controller script",
    packages=["thinklight"],
    ext_modules=[libthinklight]
    )
