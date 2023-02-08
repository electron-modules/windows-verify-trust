'use strict';

const assert = require('assert');

const { isLibExist, getLibPath, verifyTrust } = require('..');

describe('test', () => {
  it('should be ok', () => {
    assert(winVerifyTrust);
    const targetFileName = 'wlanapi.dll';
    console.log('isLibExist: %b', isLibExist(targetFileName));
    console.log('getLibPath: %s', getLibPath(targetFileName));
    console.log('verifyTrust: %s', verifyTrust(targetFileName));
  });
});
