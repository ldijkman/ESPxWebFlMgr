module.exports = {
  entry: './src/index.js',
  output: {
    filename: './dist/bundle.js'
  },
  devServer : {
      inline: true
  },
  devtool: "cheap-eval-source-map"
}