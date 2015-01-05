var test = require('tape');

test('basic require and methods', function(t) {
  t.plan(3);

  var lxc;
  t.doesNotThrow(function() {
    lxc = require('../lib/index.js');
  }, true, 'can require library');

  t.equal(typeof lxc.get_container, 'function', 'get_container exists (class)');
  t.equal(typeof lxc.create_container, 'function', 'create_container exists (class)');
});
