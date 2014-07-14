var edf = require( __dirname + '/edf' );

var file = edf.read( '../../test2.edf' );

console.log( file.records[1].length );
