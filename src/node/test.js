var edf = require( __dirname + '/build/Release/edflib' );

var file = edf.read( '../../BA1216 05_07_2012.edf' );

console.log(file);