

////////////////////////////////////////////////////////////
// build script //////////////////////////////////////////
////////////////////////////////////////////////////////////

var spawn = require('child_process').spawn;

// install edflib

process.chdir( __dirname+'/src/node/' );

var npm = spawn( 'nw-gyp', [ 'rebuild', '--target=0.8.6' ]);

npm.stdout.pipe(process.stdout); // TODO disable in production
npm.stderr.pipe(process.stderr); // TODO disable in production

