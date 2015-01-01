var lxc_b = require('bindings')('lxc.node');

console.dir(lxc_b);

var con = new lxc_b("bob");
console.log("bob is defined: " + con.is_defined());
console.log("bob state is: " + con.state());


