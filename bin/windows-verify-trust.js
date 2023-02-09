#!/usr/bin/env node

'use strict';

const targetFileName = 'wlanapi.dll';

console.log('\ncheck file: %s\n', targetFileName);

const { isLibExist, getLibPath, verifyTrust } = require('../index');

console.log('isLibExist: %s', isLibExist(targetFileName));
console.log('getLibPath: %s', getLibPath(targetFileName));
console.log('verifyTrust: %s', verifyTrust(targetFileName));
