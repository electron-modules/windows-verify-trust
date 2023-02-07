#!/usr/bin/env node

'use strict';

const { isLibExist, getLibPath, verifySignature } = require('../index');

console.log('isLibExist', isLibExist("ntdll.dll"));
console.log('getLibPath', getLibPath("ntdll.dll"));
console.log('verifySignature', verifySignature("ntdll.dll"));

