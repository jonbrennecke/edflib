/**
 *
 * Node JS extension to EDFLIB
 *
 * by @jonbrennecke
 *
 */


// load the V8 library
var Native = require( __dirname + '/src/node/build/Release/edflib' );



module.exports = Native;

