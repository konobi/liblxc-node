var lxc_b = require('bindings')('lxc.node');

console.dir(lxc_b);

var con = new lxc_b.Container("bob");
console.log("bob is defined: " + con.is_defined());
console.log("bob state is: " + con.state());

function create () {
  var args = Array.prototype.slice.call(arguments);
  var cb = args.shift();
  var template = args.shift();
  var bdevtype = args.shift();
  var flags = args.shift();
  var extra = args;
  
  return lxc_b.create(template, bdevtype, flags, extras, cb);
}
