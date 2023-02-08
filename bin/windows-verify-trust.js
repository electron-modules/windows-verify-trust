#!/usr/bin/env node

'use strict';

const { argv } = process;

const targetFileName = argv.pop();

console.log('\ncheck file: %s\n', targetFileName);

const { isLibExist, getLibPath, verifyTrust } = require('../index');

console.log('isLibExist: %b', isLibExist(targetFileName));
console.log('getLibPath: %s', getLibPath(targetFileName));
console.log('verifyTrust: %s', verifyTrust(targetFileName));
