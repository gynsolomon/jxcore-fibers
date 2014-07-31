var fs = require('fs'), path = require('path');

// Seed random numbers [gh-82]
Math.random();

// Look for binary for this platform
var v8 = 'v8-'+ /[0-9]+\.[0-9]+/.exec(process.versions.v8)[0];
var platform = process.platform;
if(platform == 'mac'){
    platform = 'darwin';
}
var modPath = path.join(__dirname, 'bin', platform+ '-'+ process.arch+ '-'+ v8, 'fibers');
try {
	fs.statSync(modPath+ '.node');
} catch (ex) {
	// No binary!
	throw new Error('`'+ modPath+ '.node` is missing. Try reinstalling `node-fibers`?');
}

// Pull in fibers implementation
module.exports = require(modPath).Fiber;
