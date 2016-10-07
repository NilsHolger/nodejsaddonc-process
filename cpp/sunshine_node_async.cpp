#include <node.h>
#include <uv.h>
#include <iostream>
#include <windows.h>
#include "sunshine.h"

using namespace std;

namespace loginAsyncAddon {
  using v8::Function;
  using v8::FunctionCallbackInfo;
  using v8::Isolate;
  using v8::Local;
  using v8::Object;
  using v8::String;
  using v8::Value;
  using v8::Persistent;

  struct Login {
    uv_work_t  request;
    Persistent<Function> callback;
    string username;
    string password;
    string result;
  };
  
  static void LoginAsync(uv_work_t *req) {
    Login *login = static_cast<Login *>(req->data);
    
    //emulate a long going worker thread here
    Sleep(2000);
    string result = "Not authorized";
    double res = is_user_authorized(login->username, login->password);
    if (res){
        result = "Login success";
    }
    login->result = result;
  }
  
  static void LoginAsyncComplete(uv_work_t *req,int status)
  {
    Isolate * isolate = Isolate::GetCurrent();

    v8::HandleScope handleScope(isolate);

    Login *login = static_cast<Login *>(req->data);

    const char *result = login->result.c_str();
    Local<Value> argv[1] = { String::NewFromUtf8(isolate, result) };
    
    Local<Function>::New(isolate, login->callback)->Call(isolate->GetCurrentContext()->Global(), 1, argv);
    
    login->callback.Reset();
    delete login;
  }


  //there is no exception handling in this sexy code
  void LoginAsync(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    v8::String::Utf8Value param1(args[0]->ToString());
    std::string user = std::string(*param1);
    v8::String::Utf8Value param2(args[1]->ToString());
    std::string pwd = std::string(*param2); 
     
    Login * login = new Login();
    login->request.data = login;
    login->username = user;
    login->password = pwd;
    
    Local<Function> callback = Local<Function>::Cast(args[2]);
    login->callback.Reset(isolate, callback);

    //magic happens here the libuv library at work
    uv_queue_work(uv_default_loop(), &login->request, LoginAsync, LoginAsyncComplete);
    
    args.GetReturnValue().Set(Undefined(isolate));  
  }
  
  void init(Local<Object> exports) {
    NODE_SET_METHOD(exports, "logInAsync", LoginAsync);
  }

  NODE_MODULE(sunshineasync, init)

} 