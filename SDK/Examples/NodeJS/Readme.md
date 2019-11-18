## How to run this example

Note that the NodeJS bindings are experimental. The _should_ work with Node v12.13 on Windows.

1. Install node
2. Install Visual Studio 2019
3. Install node-gyp
   
   ```npm install -g node-gyp```

4. In the folder `../../Bindings/NodeJS` run
   
   ```node-gyp rebuild --verbose binding.gyp```

5. modify `readwrite.js` to point to the generated node-addon, e.g. in `../../Bindings/NodeJS/build/Release`, and to the `lib3mf`-binary, e.g. `'../../Bin/lib3mf.dll'`.

6. run the example
   
   ```node readwrite.js```
