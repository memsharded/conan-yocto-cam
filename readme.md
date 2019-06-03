Image Filter application
========================

Application that receives images over ZMQ and applies a set of filters and displays all of them in a window canvas.


py-sender
---------

Python mock of a sender application that reads images (*.jpg* & *.png*) from the *image* folder and sends them over ZMQ.

**Installation**

```
$ cd py-sender
$ pip install -r requirements.txt
```

**Running**

```
$ cd py-sender
$ python sender.py [proxy (*)] [port (5559)]
```

image-filter
------------

C++ application that receives images from ZMQ.

**Installation & compilation**

```
$ cd image-filter
$ mkdir build && cd build
$ conan install ..
$ conan build ..
```

**Running**

First, copy the resources files from the *res* folder to the application folder.

```
$ cd image-filter
$ cp /res/cars.xml build/app/bin/cars.xml
```

Then, execute the application.

```
$ cd build/app/bin
$ ./app [broker (localhost)] [port (5559)]
```

**Packaging**

```
$ cd image-filter
$ conan create . danimtb/testing
```

**Deploying & running**

```
$ mkdir deploy && cd deploy
$ conan install image-filter/0.0.1@danimtb/testing
$ cd bin
$ ./app [broker (localhost)] [port (5559)]
```