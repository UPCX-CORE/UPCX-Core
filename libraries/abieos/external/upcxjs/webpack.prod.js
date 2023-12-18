const path = require("path");

module.exports = {
  entry: {
    upcxjs_api: "./src/upcxjs-api.ts",
    upcxjs_jsonrpc: "./src/rpc-web.ts",
    upcxjs_jssig: "./src/upcxjs-jssig.ts",
    upcxjs_numeric: "./src/upcxjs-numeric.ts",
  },
  mode: "production",
  module: {
    rules: [
      {
        test: /\.tsx?$/,
        use: {
          loader: "ts-loader",
          options: {
            configFile: "tsconfig.web.json",
          },
        },
        exclude: /node_modules/,
      },
    ],
  },
  resolve: {
    extensions: [".tsx", ".ts", ".js"],
  },
  output: {
    filename: (x) => x.chunk.name.replace("_", "-") + ".js",
    library: "[name]",
    path: path.resolve(__dirname, "dist-web"),
  },
};
