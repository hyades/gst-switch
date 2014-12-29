#! /bin/bash -ex

# Update / upgrade everything
sudo apt-get -y update && sudo apt-get -y upgrade

# gst-switch compile stuff
sudo apt-get -y install build-essential dh-autoreconf
sudo apt-get -y install libgtk-3-dev
sudo apt-get -y install gstreamer1.0.* libgstreamer.*1.0.*

# Python stuff for gst-switch API
sudo apt-get -y install python-software-properties python-pip
sudo apt-get -y install libglib2.0-dev gir1.2-glib-2.0 libgirepository1.0-dev libglib2.0-0 python-gi
sudo apt-get -y install python-scipy
sudo pip install mock --upgrade
sudo pip install pytest-cov --upgrade
sudo pip install pytest-pep8 --upgrade
sudo pip install pylint --upgrade

# Needed for tests
sudo apt-get -y install wget libav-tools

# Needed to get coverage output
if [ $TYPE == 'c' ]; then
	sudo pip install cpp-coveralls
else
	sudo pip install python-coveralls
fi
