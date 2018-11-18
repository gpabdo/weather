import setuptools

setuptools.setup(
    name="environment",
    version="0.0.1",
    author="greg",
    author_email="gpabdo@gmail.com",
    description="A small example package",
    packages=setuptools.find_packages(),
    install_requires=[
          'kafka-python==1.3.2',
          'pyserial==3.1.1',
          'requests==2.9.1'
      ],
)
