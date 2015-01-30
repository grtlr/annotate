annotate
========

Fast annotation of images using superpixels. The image is over segmented into superpixels using the *SEEDS* algorithm.

The project is under development and expected to change frequently.

Requirements
------------

* CMake 2.8.11 or newer
* Qt5 (Qt4 should work with modified ``CMakeLists.txt``)
* OpenCV 3.0.0 **with contrib packages**

Usage
-----

* ``<LeftMouse>`` labels the superpixel under the cursor
* ``<RightMouse>`` labels as background
* ``<Space>`` toggles contours of the superpixels on/off

TODO
----

* dynamically load labels from file
* file management in general
