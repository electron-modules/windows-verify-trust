'use strict';
const path = require('node:path');
const pkg = require('./package');
const winVerifyTrust = require('bindings')(pkg.name);

const fileExtensions = ['.dll'];

function isValidFilename(filename) {
  const extension = path.extname(filename);
  return fileExtensions.includes(extension);
}

function isLibExist(filename) {
  if (!isValidFilename(filename)) return false;
  return winVerifyTrust.isLibExist(filename);
}

function getLibPath(filename) {
  if (!isValidFilename(filename)) return '';
  return winVerifyTrust.getLibPath(filename);
}

function verifySignature(filename) {
  const result = { trusted: false, message: ''};
  if (!isValidFilename(filename)) {
    result.message = 'File type is not supported';
    return result;
  };
  try {
    const status = winVerifyTrust.verifySignature(filename);
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
  verifySignature
};
