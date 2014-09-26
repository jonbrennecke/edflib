/**
 *
 * Node JS extension to EDFLIB
 *
 * by @jonbrennecke
 *
 */

(function ( mod ) {

	if ( typeof module === "object" && typeof module.exports === "object" ) { // CommonJS, Node 
		module.exports = mod();
	}
	else
		throw "This module is only supported on Node.js.";

})(function(){

	// load the Native C++ V8 library
	var Native = require( __dirname + '/src/node/build/Release/edflib' );

	// TODO use harmony proxies here, because they're cooler
	// but older versions of NW may not support Harmony
	function EdfProxy ( path ) {
		this.path = path;
		this.edf = new Native.Edf(path);
	}

	EdfProxy.prototype = {
		getHeader : function () {

			var header = this.edf.getHeader();

			// trim whitespace
			Object.keys(header).forEach(function ( key ) {
				if ( typeof header[key] === "string" ) {
					header[key] = header[key].trim();
				}
				else {
					header[key] = header[key].map(function ( val ) {
						return val.trim();
					});
				}

			});

			return header;
		},

		getData : function ( index ) {
			return this.edf.getData(index);
		}
	}

	return {
		"Edf" : EdfProxy
	}


});
