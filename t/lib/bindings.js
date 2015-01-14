var obj = {};
var _mocks = {};
var lxc = function(){ return this };

module.exports = function(name) {
  _mocks.binding_name = name;
  return lxc;
};

lxc._mocks = _mocks;

