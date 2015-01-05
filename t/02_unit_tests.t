var mocked = require('mocked');
mocked('./t/lib', ['bindings']);


var test = require('tape');
var lxcb;

test('basic addon require', function(t){
  t.plan(2);
  lxcb = require('bindings')('lxc.node');
  t.strictEquals(lxcb._mocks.binding_name, 'lxc.node', "correct binding name");
  t.equals(typeof lxcb, 'function', 'returned binding is a function');
});

var lxc = require('../lib/index');

