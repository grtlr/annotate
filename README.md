annotate
========

Fast annotation of images using superpixels. The image is over segmented into superpixels using the *SEEDS* algorithm.

The project is under heavy development.

Requirements
------------

* CMake 2.8.11 or newer
* Qt5 (Qt4 should work with modified ``CMakeLists.txt``)
* OpenCV 3.0.0 **with contrib packages**

Usage
-----

* ``<Space>`` Toggles contours of the superpixels on/off

TODO
----

* right-click should label background
* dynamically load labels from file
* file management in general
