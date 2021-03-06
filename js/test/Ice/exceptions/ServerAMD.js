// **********************************************************************
//
// Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

(function(module, require, exports)
{
    const Ice = require("ice").Ice;
    const Test = require("Test").Test;
    const TestHelper = require("TestHelper").TestHelper;
    const AMDThrowerI = require("AMDThrowerI").AMDThrowerI;

    class ServerAMD extends TestHelper
    {
        async run(args)
        {
            let communicator;
            let echo;
            try
            {
                const [properties] = this.createTestProperties(args);
                properties.setProperty("Ice.MessageSizeMax", "10");
                properties.setProperty("Ice.Warn.Dispatch", "0");
                properties.setProperty("Ice.Warn.Connections", "0");
                [communicator] = this.initialize(properties);
                echo = await Test.EchoPrx.checkedCast(communicator.stringToProxy("__echo:" + this.getTestEndpoint()));
                const adapter = await communicator.createObjectAdapter("");
                adapter.add(new AMDThrowerI(), Ice.stringToIdentity("thrower"));
                await echo.setConnection();
                const connection = echo.ice_getCachedConnection();
                connection.setCloseCallback(con => {
                    // Re-establish connection if it fails (necessary for MemoryLimitException test)
                    echo.setConnection().then(() => echo.ice_getCachedConnection().setAdapter(adapter));
                });
                connection.setAdapter(adapter);
                echo.ice_getCachedConnection().setAdapter(adapter);
                this.serverReady();
                await communicator.waitForShutdown();
            }
            finally
            {
                if(echo)
                {
                    await echo.shutdown();
                }

                if(communicator)
                {
                    await communicator.destroy();
                }
            }
        }
    }
    exports.ServerAMD = ServerAMD;
}(typeof global !== "undefined" && typeof global.process !== "undefined" ? module : undefined,
  typeof global !== "undefined" && typeof global.process !== "undefined" ? require : this.Ice._require,
  typeof global !== "undefined" && typeof global.process !== "undefined" ? exports : this));
