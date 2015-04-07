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
Before starting the problem, we need to create a file that will contain the labels. The contents of the file could look like this:

    car 0 128 255
    bike 0 255 128
    pedestrian 0 255 255

The first entry specifies the name, that will be shown in the GUI. The 3 numbers represent the RGB colors of each label. **Note:** A *background* class and a *missing* class are automatically added. Their respective colors are ``(0,0,0)`` and ``(128,128,128)``, so you might want to have different colors for your own classes.

The tool can be started via the command line:

    ./annotate labels.txt <indir> <outdir>

There is an option (``-n``) that controls the number of superpixels:

    ./annotate -n 600 labels.txt <indir> <outdir>

If no option is set, the default number of superpixels is ``400``.

For more information you can also view the help:

    ./annotate --help

You can use the following actions while labeling:

* ``<LeftMouse>`` labels the superpixel under the cursor
* ``<RightMouse>`` labels as background
* ``<Space>`` toggles contours of the superpixels on/off

TODO
----
- [ ] fix TravisCI build
- [x] dynamically load labels from file
- [ ] improve file management in general
