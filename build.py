from setuptools import Extension
from setuptools.command.build_ext import build_ext

extensions = [
    Extension("fgshun_bst._bst", sources=["fgshun_bst/_bst.c"]),
]

def build(setup_kwargs):
    setup_kwargs.update({"ext_modules": extensions, "cmdclass": {"build_ext": build_ext}})
