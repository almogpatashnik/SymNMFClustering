from setuptools import Extension, setup

module = Extension("symnmfmodule",
                  sources=[
                    'symnmf.c',
                    'symnmfmodule.c',
                    'matricesmethods.c'
                  ])
setup(name='symnmfmodule',
     version='1.0',
     description='Python wrapper for custom C extension',
     ext_modules=[module])