# nodejsaddonc-process
nodejs addon javascript c++ sync async v8 libuv

in cpp folder node-gyp configure build
in root node sunshine

simulates a login process from javascript to c++ asynchronously

node's event loop thread returns immediately to the javascript code
libuv's worker thread executes in the background
the javascript callback function is invoked by libuv upon completion returning the data from the worker thread

this allows the node application to continue on doing its business while c++ processing is happening in the background

for example computational intensive operations and so forth

observe there is no exception handling

to use the two addons
in binding.gyp sources property specify the file names to include
the target name must match the name of the addon provided in the v8 class
