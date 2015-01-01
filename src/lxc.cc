#include <node.h>
#include <nan.h>
using namespace v8;

extern "C" {
#include <lxc/lxccontainer.h>
}
#include "lxc.h"

Persistent<Function> Container::constructor;
struct lxc_container Container::*con_;

Container::Container() { }
Container::~Container() { }

void Container::Init(Handle<Object> target) {
  NanScope();

  Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
  tpl->SetClassName(NanNew("Container"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  NODE_SET_PROTOTYPE_METHOD(tpl, "is_defined", Defined);
  NODE_SET_PROTOTYPE_METHOD(tpl, "state", State);
  NODE_SET_PROTOTYPE_METHOD(tpl, "is_running", Running);
  NODE_SET_PROTOTYPE_METHOD(tpl, "freeze", Freeze);
  NODE_SET_PROTOTYPE_METHOD(tpl, "unfreeze", UnFreeze);
  NODE_SET_PROTOTYPE_METHOD(tpl, "init_pid", InitPid);
  NODE_SET_PROTOTYPE_METHOD(tpl, "want_daemonize", WantDaemonize);
  NODE_SET_PROTOTYPE_METHOD(tpl, "want_close_all_fds", WantCloseAllFds);

  NanAssignPersistent(constructor, tpl->GetFunction());
  target->Set(NanNew("Container"), tpl->GetFunction());
}

NAN_METHOD(Container::New) {
	NanScope();
	static v8::String *cname;
	static v8::String *configpath;

	if (args.IsConstructCall()) {
		cname = *args[0].As<String>();
		configpath = *args[1].As<String>();
		Container* obj = new Container();

		obj->con_ = lxc_container_new(
			(const char*)cname,
			(const char*)configpath
		);
		
		if(obj->con_ == NULL) {
  		  NanThrowError("Unable to get container");
		  NanReturnUndefined();
		}

		obj->Wrap(args.This());
		return args.This();
	} else {
		NanReturnUndefined();
	}
}

NAN_METHOD(Container::Defined) {
  NanScope();
  Container* obj = ObjectWrap::Unwrap<Container>(args.This()); 
  return obj->con_->is_defined(obj->con_) ? NanTrue() : NanFalse();
}

NAN_METHOD(Container::State) {
  NanScope();
  Container* obj = ObjectWrap::Unwrap<Container>(args.This()); 
  return NanNew(std::string(obj->con_->state(obj->con_)));
}

NAN_METHOD(Container::Running) {
  NanScope();
  Container* obj = ObjectWrap::Unwrap<Container>(args.This()); 
  return obj->con_->is_running(obj->con_) ? NanTrue() : NanFalse();
}

NAN_METHOD(Container::Freeze) {
  NanScope();
  Container* obj = ObjectWrap::Unwrap<Container>(args.This()); 
  return obj->con_->freeze(obj->con_) ? NanTrue() : NanFalse();
}

NAN_METHOD(Container::UnFreeze) {
  NanScope();
  Container* obj = ObjectWrap::Unwrap<Container>(args.This()); 
  return obj->con_->unfreeze(obj->con_) ? NanTrue() : NanFalse();
}

NAN_METHOD(Container::InitPid) {
  NanScope();
  Container* obj = ObjectWrap::Unwrap<Container>(args.This()); 
  NanReturnValue(NanNew<Number>(obj->con_->init_pid(obj->con_)));
}

NAN_METHOD(Container::WantDaemonize) {
  NanScope();
  Container* obj = ObjectWrap::Unwrap<Container>(args.This()); 
  return obj->con_->want_daemonize(obj->con_, args[0]->ToBoolean()->Value()) ? NanTrue() : NanFalse();
}

NAN_METHOD(Container::WantCloseAllFds) {
  NanScope();
  Container* obj = ObjectWrap::Unwrap<Container>(args.This()); 
  return obj->con_->want_close_all_fds(obj->con_, args[0]->ToBoolean()->Value()) ? NanTrue() : NanFalse();
}

NAN_METHOD(Container::Create) {
  NanScope();
  Container* obj = ObjectWrap::Unwrap<Container>(args.This()); 

  Handle<Value> target = args[0];
  Handle<Value> bdevtype = args[1];
  Handle<Value> flags = args[2];

  Handle<String> template_str = target.As<String>();
  Handle<String> bdevtype_str = bdevtype.As<String>();
  Handle<Integer> flags_num = flags.As<Integer>();

  return obj->con_->create(
    obj->con_,
    template_str,
    bdevtype_str,
    NULL,
    !!((int64_t)*flags_num & LXC_CREATE_QUIET),
    ARGV
  ) ? NanTrue() : NanFalse();
}

NAN_METHOD(Container::Start) {
  NanScope();
  Container* obj = ObjectWrap::Unwrap<Container>(args.This()); 
  return obj->con_->start(
      obj->con_,
      args[0]->ToNumber()->Value(),
      // ARGV
  ) ? NanTrue() : NanFalse();
}



NODE_MODULE(lxc, Container::Init)
