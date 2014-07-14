var edf = require( __dirname + '/edf' );

var file = edf.read( '../../test.edf' );

console.log( file.records[1].slice(0,20) );
