#!/bin/bash
#cd /store/open/gstreamer/stage

#gdb --args \
./tools/gst-switch-srv -v \
    --gst-debug-no-color \
    --gst-debug="switch:5" \
    --gst-debug="convbin:5" \
    --gst-debug="tcpmixsrc:5" \
    --port="3000" \
