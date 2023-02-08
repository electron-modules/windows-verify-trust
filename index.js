'use strict';

const path = require('path');
const pkg = require('./package');

const winVerifyTrust = require('bindings')(pkg.name);

function isLibExist(filename) {
  return winVerifyTrust.isLibExist(filename);
}

function getLibPath(filename) {
  return winVerifyTrust.getLibPath(filename);
}

function verifyTrust(filename) {
  const result = { trusted: false, message: ''};
  try {
    const status = winVerifyTrust.verifyTrust(filename);
    if (status === 0) {
      result.trusted = true;
      result.message = 'The library is signed, the signature was verified';
    } else {
      result.message = `Error code: ${status}`;
    }
  } catch (e) {
    result.message = e.message;
  }
  return result;
}

module.exports = {
  isLibExist,
  getLibPath,
  verifyTrust
};
