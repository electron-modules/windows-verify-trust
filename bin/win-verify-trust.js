#!/usr/bin/env node

'use strict';
const pkg = require('../build/Release/win-verify-trust.node');

function verify(name) {
  const status = pkg.verifySignature(name);
  const result = {};
  if (status === 0) {
    result.trusted = true;
    result.message = "The library is signed, the signature was verified";
  } else {
    result.trusted = false;
    result.message = `error: ${status}`;
  }
  return result;
}

console.log('C++ addon', verify("ntdll.dll"));
