libttrssreader
==============

Library for interacting with a TT-RSS session

Build Instructions
-----------------

In root folder

```
mkdir build
cmake ../src
make
```

This will make a shared library named libsession.so and a small test program named CurlTest

Test run instructions
---------------------

For a simple test, run 
```
./CurlTest 
```
after buiding.  It will prompt you for a username, password command (like 'echo password' or a 'gpg2 -pq password.file') and a URL for the TT-RSS installation.
