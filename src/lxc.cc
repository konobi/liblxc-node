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

Handle<Value> Container::New(const Arguments& args) {
	HandleScope scope;
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
		}

		obj->Wrap(args.This());
		return args.This();
	} else {
		NanReturnUndefined();
	}
}

Handle<Value> Container::Defined(const Arguments& args) {
  HandleScope scope;
  Container* obj = ObjectWrap::Unwrap<Container>(args.This()); 
  return obj->con_->is_defined(obj->con_) ? NanTrue() : NanFalse();
}

Handle<Value> Container::State(const Arguments& args) {
  HandleScope scope;
  Container* obj = ObjectWrap::Unwrap<Container>(args.This()); 
  return NanNew(std::string(obj->con_->state(obj->con_)));
}

Handle<Value> Container::Running(const Arguments& args) {
  HandleScope scope;
  Container* obj = ObjectWrap::Unwrap<Container>(args.This()); 
  return obj->con_->is_running(obj->con_) ? NanTrue() : NanFalse();
}

Handle<Value> Container::Freeze(const Arguments& args) {
  HandleScope scope;
  Container* obj = ObjectWrap::Unwrap<Container>(args.This()); 
  return obj->con_->freeze(obj->con_) ? NanTrue() : NanFalse();
}

Handle<Value> Container::UnFreeze(const Arguments& args) {
  HandleScope scope;
  Container* obj = ObjectWrap::Unwrap<Container>(args.This()); 
  return obj->con_->unfreeze(obj->con_) ? NanTrue() : NanFalse();
}

Handle<Value> Container::InitPid(const Arguments& args) {
  HandleScope scope;
  Container* obj = ObjectWrap::Unwrap<Container>(args.This()); 
  NanReturnValue(NanNew<Number>(obj->con_->init_pid(obj->con_)));
}

Handle<Value> Container::WantDaemonize(const Arguments& args) {
  HandleScope scope;
  Container* obj = ObjectWrap::Unwrap<Container>(args.This()); 
  return obj->con_->want_daemonize(obj->con_, args[0]->ToBoolean()->Value()) ? NanTrue() : NanFalse();
}

Handle<Value> Container::WantCloseAllFds(const Arguments& args) {
  HandleScope scope;
  Container* obj = ObjectWrap::Unwrap<Container>(args.This()); 
  return obj->con_->want_close_all_fds(obj->con_, args[0]->ToBoolean()->Value()) ? NanTrue() : NanFalse();
}

Handle<Value> Container::Create(const Arguments& args) {
  HandleScope scope;
  Container* obj = ObjectWrap::Unwrap<Container>(args.This()); 

  static v8::String template_str = args[0].As<String>();
  return obj->con_->create(
    obj->con_,
    template_str,
     // BDEVTYPE
    NULL
    //!!(FLAGS & LXC_CREATE_QUIET)
    // ARGV
  ) ? NanTrue() : NanFalse();
}

Handle<Value> Container::Start(const Arguments& args) {
  HandleScope scope;
  Container* obj = ObjectWrap::Unwrap<Container>(args.This()); 
  return obj->con_->start(
      obj->con_,
      args[0]->ToNumber()->Value(),
      // ARGV
  ) ? NanTrue() : NanFalse();
}



NODE_MODULE(lxc, Container::Init)
