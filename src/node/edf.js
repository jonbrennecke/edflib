/**
 *
 * Node JS extension to EDFLIB
 *
 * by @jonbrennecke
 *
 */


// load the V8 library
var edflib = require( __dirname + '/build/Release/edflib' ),
	rtrim = /\s+$/;


module.exports = {
	read : function ( filename ) {
		var edf = edflib.read( filename );

		// trim trailing whitespace from the header fields
		// ---
		// since javascript's regex engine is so blazingly fast, we'll 
		// just use JS for stripping characters instead of C++
		for ( var key in edf.header ) {

			if ( Object.prototype.toString.call(edf.header[key]) != '[object Array]' ) {
				edf.header[key] = edf.header[key].replace(rtrim,'');
				continue;
			}

			edf.header[key] = edf.header[key].map( function ( val ) {
				return val.replace(rtrim,'');
			});
		}

		return edf; 
	}
}
