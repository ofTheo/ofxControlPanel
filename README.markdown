ofxControlPanel
===============

ofxControlPanel is a simple control panel gui and widget set for OpenFrameworks app runtime tweaks. Settings can be loaded and saved via xml files.

See also src/ofxAutoControlPanel.h to make your life even easier ...

OpenFrameworks is a cross platform open source toolkit for creative coding in C++.

[http://www.openframeworks.cc/](http://www.openframeworks.cc/)

Installation
------------

To use ofxControlPanel, first you need to download and install Open Frameworks. ofxControlPanel is currently developed against the latest version of Open Frameworks on github (007).

To get a copy of the repository you can download the source from [https://github.com/kylemcdonald/ofxControlPanel/zipball/master](https://github.com/kylemcdonald/ofxControlPanel/zipball/master) or, alternatively, you can use git clone:
<pre>
git clone git://github.com/kylemcdonald/ofxControlPanel.git
</pre>

The addon should sit in `openFrameworks/addons/ofxControlPanel/`.

You will also need the following addon dependencies:

* ofxXmlSettings: included with OpenFrameworks

#### Which version to use?

If you are using a stable version (0062, 007, ...) of OpenFrameworks then you want to use a git tag of ofxControlPanel for that version. You can select the tag in the Github menu or clone and check it out using git.

For example, the following commands will clone ofxKinect and switch to the OF 0062 tagged version:
<pre>
git clone git://github.com/kylemcdonald/ofxControlPanel.git
cd ofxControlPanel
git checkout 0062
</pre>

#### Using the latest ofxControlPanel

The master branch of ofxControlPanel will work with the current master of OpenFrameworks and can be considered *relatively* stable.

Running the Example Project
-------------------------------

If you're using OS X, open the Xcode project in ofxControlPanel/example/ and hit "Build and Run".

The addon should work fine in Windows and Linux, although there are no example build files yet.

Adding ofxControlPanel to an Existing Project
---------------------------------------------

If you want to add ofxControlPanel to another project, you need to make sure you include the src folders:

openFrameworks/addons/ofxControlPanel/src

For XCode:

* create a new group "ofxControlPanel"
* drag these directories from ofxControlPanel into this new group: ofxControlPanel/src
* you also need to add the following addon dependencies in a similar manner: ** ofxXmlSettings


DEVELOPING
----------

You can help develop ofxControlPanel on GitHub: [https://github.com/kylemcdonald/ofxControlPanel](https://github.com/kylemcdonald/ofxControlPanel)

Create an account, clone or fork the repo, then request a push/merge.

If you find any bugs or suggestions please log them to GitHub as well.
