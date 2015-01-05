var lxc_b = require('bindings')('lxc.node');

//var con = new lxc_b("bob");
//console.log("bob is defined: " + con.is_defined());
//console.log("bob state is: " + con.state());

function LXC () {
  return this;
}

LXC.create_container = function create_container (name, configpath) {

};

LXC.get_container = function get_container (name) {

};

module.exports = LXC;
