#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-2008 ZeroC, Inc. All rights reserved.
#
# This copy of Ice is licensed to you under the terms described in the
# ICE_LICENSE file included in this distribution.
#
# **********************************************************************

import os, sys, traceback

for toplevel in [".", "..", "../..", "../../..", "../../../.."]:
    toplevel = os.path.normpath(toplevel)
    if os.path.exists(os.path.join(toplevel, "python", "Ice.py")):
        break
else:
    raise "can't find toplevel directory!"

#
# Find Slice directory.
#
slice_dir = os.path.join(os.path.join(toplevel, "..", "slice"))
if not os.path.exists(slice_dir):
    home_dir = os.getenv('ICE_HOME', '')
    if len(home_dir) == 0 or not os.path.exists(os.path.join(home_dir, "slice")):
        print sys.argv[0] + ': Slice directory not found. Define ICE_HOME.'
        sys.exit(1)
    slice_dir = os.path.join(home_dir, "slice")

import Ice
Ice.loadSlice('-I' + slice_dir + ' Test.ice')
import AllTests

def test(b):
    if not b:
        raise RuntimeError('test assertion failed')

def run(args, communicator):
    myClass = AllTests.allTests(communicator, False)

    print "testing server shutdown...",
    myClass.shutdown()
    try:
        myClass.opVoid()
        test(False)
    except Ice.LocalException:
        print "ok"

    return True

try:
    #
    # In this test, we need at least two threads in the
    # client side thread pool for nested AMI.
    #
    initData = Ice.InitializationData()
    initData.properties = Ice.createProperties(sys.argv)
    initData.properties.setProperty('Ice.ThreadPool.Client.Size', '2')
    initData.properties.setProperty('Ice.ThreadPool.Client.SizeWarn', '0')

    #
    # We must set MessageSizeMax to an explicit values, because
    # we run tests to check whether Ice.MemoryLimitException is
    # raised as expected.
    #
    initData.properties.setProperty("Ice.MessageSizeMax", "100")

    communicator = Ice.initialize(sys.argv, initData)
    status = run(sys.argv, communicator)
except:
    traceback.print_exc()
    status = False

if communicator:
    try:
        communicator.destroy()
    except:
        traceback.print_exc()
        status = False

sys.exit(not status)
